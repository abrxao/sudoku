#include "MainWindow.h"
#include "SudokuCellDelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_isUpdating(false)
{
  setupUI();
}

void MainWindow::setupUI()
{
  this->setWindowTitle(tr("Sudoku Assistant - Pro Version"));
  this->setFixedSize(760, 560);

  this->setStyleSheet("QMainWindow { background-color: #F4F6F8; }");

  QWidget *topBar = new QWidget(this);
  QHBoxLayout *topLayout = new QHBoxLayout(topBar);
  topLayout->setContentsMargins(10, 10, 10, 10);

  m_langCombo = new QComboBox(this);
  m_langCombo->addItems({"English", "Français"});
  QString comboStyle = "QComboBox { font-size: 13pt; padding: 6px 12px; border: 2px solid #B2DFDB; border-radius: 6px; background-color: white; color: #263238; font-weight: bold; }"
                       "QComboBox::drop-down { border: none; }";
  m_langCombo->setStyleSheet(comboStyle);

  m_hintsCheckbox = new QCheckBox(this);
  m_hintsCheckbox->setChecked(true);
  m_hintsCheckbox->setStyleSheet("font-size: 13pt; color: #00796B; font-weight: bold; spacing: 8px; margin-left: 15px;");

  connect(m_hintsCheckbox, &QCheckBox::toggled, this, &MainWindow::hintsToggled);

  topLayout->addWidget(m_langCombo);
  topLayout->addWidget(m_hintsCheckbox);
  topLayout->addStretch();
  topLayout->addWidget(m_hintsCheckbox);
  topLayout->addSpacing(20);
  topLayout->addWidget(m_langCombo);

  m_table = new QTableWidget(9, 9, this);
  m_table->horizontalHeader()->setVisible(false);
  m_table->verticalHeader()->setVisible(false);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

  m_table->setStyleSheet(
      "QTableWidget { gridline-color: #80CBC4; font-size: 18pt; background-color: white; border: 2px solid #00796B; border-radius: 8px; }"
      "QTableWidget::item:selected { background-color: #E0F2F1; color: #00796B; border: 2px solid #009688; }");

  QMenuBar *menuBar = this->menuBar();
  menuBar->setStyleSheet("QMenuBar { background-color: #F4F6F8; color: #263238; font-size: 12pt; }"
                         "QMenuBar::item:selected { background-color: #E0F2F1; }");

  m_menuFile = menuBar->addMenu("");
  m_menuNewGame = m_menuFile->addMenu("");
  m_actionEasy = m_menuNewGame->addAction("");
  m_actionMedium = m_menuNewGame->addAction("");
  m_actionHard = m_menuNewGame->addAction("");
  m_actionInsane = m_menuNewGame->addAction("");

  m_menuFile->addSeparator();
  m_actionSave = m_menuFile->addAction("");
  m_actionLoad = m_menuFile->addAction("");

  connect(m_actionEasy, &QAction::triggered, this, [this]()
          { emit newGameRequested(0); });
  connect(m_actionMedium, &QAction::triggered, this, [this]()
          { emit newGameRequested(1); });
  connect(m_actionHard, &QAction::triggered, this, [this]()
          { emit newGameRequested(2); });
  connect(m_actionInsane, &QAction::triggered, this, [this]()
          { emit newGameRequested(3); });

  connect(m_actionSave, &QAction::triggered, this, &MainWindow::saveGameRequested);
  connect(m_actionLoad, &QAction::triggered, this, &MainWindow::loadGameRequested);

  int cellSize = 52;
  for (int i = 0; i < 9; ++i)
  {
    m_table->setColumnWidth(i, cellSize);
    m_table->setRowHeight(i, cellSize);
  }
  m_table->setFixedSize(cellSize * 9 + 4, cellSize * 9 + 4);

  m_helperLabel = new QLabel(this);
  m_helperLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  m_helperLabel->setWordWrap(true);
  m_helperLabel->setStyleSheet(
      "QLabel { font-size: 14pt; padding: 20px; background-color: white; border: 2px solid #B2DFDB; border-radius: 8px; color: #263238; }");
  m_helperLabel->setFixedWidth(250);

  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
  mainLayout->setContentsMargins(20, 20, 20, 20);
  mainLayout->setSpacing(15);

  QHBoxLayout *gameLayout = new QHBoxLayout();
  gameLayout->setSpacing(15);
  gameLayout->addWidget(m_table);
  gameLayout->addWidget(m_helperLabel);

  mainLayout->addWidget(topBar);
  mainLayout->addLayout(gameLayout);

  setCentralWidget(centralWidget);

  connect(m_table, &QTableWidget::currentCellChanged, this, &MainWindow::onCurrentCellChanged);
  m_table->installEventFilter(this);
  m_table->setItemDelegate(new SudokuCellDelegate(m_table));

  connect(m_langCombo, &QComboBox::currentIndexChanged, this, [this](int index)
          {
            if (index == 1)
            { 
              if (m_translator.load(":/i18n/sudoku_fr.qm"))
              {
                qApp->installTranslator(&m_translator);
              }
              else
              {
                qDebug() << "[I18N ERROR] Falha ao carregar sudoku_fr.qm.";
              }
            }
            else
            { 
              qApp->removeTranslator(&m_translator);
            }
            retranslateUI(); });

  retranslateUI();
}
void MainWindow::clearBoard()
{
  m_table->clearContents();
}

void MainWindow::setCellValue(int row, int col, int value, bool isFixed)
{
  m_isUpdating = true;

  QTableWidgetItem *item = m_table->item(row, col);
  if (!item)
  {
    item = new QTableWidgetItem();
    m_table->setItem(row, col, item);
  }

  bool isDarkQuadrant = ((row / 3) + (col / 3)) % 2 != 0;
  QColor quadrantColor = isDarkQuadrant ? QColor("#F4F9F9") : QColor("#FFFFFF");

  if (value == 0)
  {
    item->setText("");
    item->setBackground(quadrantColor);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
  }
  else
  {
    item->setText(QString::number(value));
    item->setTextAlignment(Qt::AlignCenter);

    if (isFixed)
    {
      QFont font = item->font();
      font.setBold(true);
      font.setPointSize(16);
      item->setFont(font);
      item->setBackground(QColor("#ECEFF1"));
      item->setForeground(QBrush(QColor("#263238")));
      item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
    else
    {

      QFont font = item->font();
      font.setBold(true);
      font.setPointSize(18);
      item->setFont(font);
      item->setBackground(quadrantColor);
      item->setForeground(QBrush(QColor("#FF9800")));
    }
  }

  m_isUpdating = false;
}
void MainWindow::onCellChanged(int row, int col)
{
  if (m_isUpdating)
    return;

  QTableWidgetItem *item = m_table->item(row, col);
  if (!item)
    return;

  QString text = item->text();
  int value = 0;

  if (!text.isEmpty())
  {
    bool ok;
    value = text.toInt(&ok);
    if (!ok || value < 1 || value > 9)
    {

      m_isUpdating = true;
      item->setText("");
      m_isUpdating = false;
      return;
    }
  }
  emit cellInput(row, col, value);
}

void MainWindow::showHelper(int row, int col, const QSet<int> &possibilities)
{
  if (possibilities.isEmpty())
  {
    m_helperLabel->setText(tr("Cell (%1, %2)\n\nNo valid possibilities or already filled.").arg(row + 1).arg(col + 1));
    return;
  }

  QList<int> list(possibilities.begin(), possibilities.end());
  std::sort(list.begin(), list.end());

  QString helperText = tr("Cell (%1, %2)\n\nPossibilities:\n").arg(row + 1).arg(col + 1);
  for (int p : list)
  {
    helperText += QString::number(p) + "  ";
  }

  if (list.size() == 1)
  {
    helperText += tr("\n\n⭐ Only one option!");
  }

  m_helperLabel->setText(helperText);
}

void MainWindow::clearHelper()
{
  m_helperLabel->setText(tr("Select an empty cell\nto see hints."));
}

void MainWindow::showVictoryMessage()
{
  QMessageBox::information(this,
                           tr("Sudoku Completed"),
                           tr("Congratulations! You solved the Sudoku correctly.\nChoose a new level to continue playing."));
}

void MainWindow::setCellStuck(int row, int col, bool isStuck)
{
  m_isUpdating = true;

  QTableWidgetItem *item = m_table->item(row, col);
  if (item)
  {
    if (isStuck)
    {
      item->setBackground(QColor("#FFCC80"));
    }
    else
    {
      bool isDarkQuadrant = ((row / 3) + (col / 3)) % 2 != 0;
      QColor quadrantColor = isDarkQuadrant ? QColor("#F4F9F9") : QColor("#FFFFFF");
      item->setBackground(quadrantColor);
    }
  }

  m_isUpdating = false;
}

void MainWindow::showError(const QString &message)
{
  m_helperLabel->setText(QString("<span style='color: #F57C00; font-weight: bold; font-size: 16pt;'>⚠️ %1</span><br><br><span style='color: #263238;'>%2</span>")
                             .arg(tr("Invalid Action"))
                             .arg(message));
}

void MainWindow::onCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
  if (currentRow >= 0 && currentColumn >= 0)
  {
    emit cellSelected(currentRow, currentColumn);
  }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (obj == m_table && event->type() == QEvent::KeyPress)
  {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    int row = m_table->currentRow();
    int col = m_table->currentColumn();

    if (row < 0 || col < 0)
      return false;

    int key = keyEvent->key();

    if (key == Qt::Key_Up || key == Qt::Key_Down || key == Qt::Key_Left || key == Qt::Key_Right)
    {
      return false;
    }

    QTableWidgetItem *item = m_table->item(row, col);
    if (item && !(item->flags() & Qt::ItemIsEditable))
    {
      return true;
    }

    if ((key >= Qt::Key_1 && key <= Qt::Key_9))
    {
      int value = key - Qt::Key_0;
      emit cellInput(row, col, value);
      return true;
    }

    if (key == Qt::Key_Backspace || key == Qt::Key_Delete)
    {
      emit cellInput(row, col, 0);
      return true;
    }
  }

  return QMainWindow::eventFilter(obj, event);
}

void MainWindow::retranslateUI()
{
  this->setWindowTitle(tr("Sudoku Assistant - Pro Version"));

  // Menus
  m_menuFile->setTitle(tr("File"));
  m_menuNewGame->setTitle(tr("New Game"));
  m_actionEasy->setText(tr("Easy"));
  m_actionMedium->setText(tr("Medium"));
  m_actionHard->setText(tr("Hard"));
  m_actionInsane->setText(tr("Insane"));
  m_actionSave->setText(tr("Save Game"));
  m_actionLoad->setText(tr("Load Game"));

  // Checkbox
  m_hintsCheckbox->setText(tr("Show Hints"));
  m_hintsCheckbox->setToolTip(tr("Toggle the visibility of candidate numbers inside empty cells"));

  m_langCombo->setToolTip(tr("Change the application language"));
  m_table->setToolTip(tr("Use arrow keys to navigate, numbers 1-9 to input, and Backspace to clear"));

  clearHelper();
}

void MainWindow::setCellPossibilities(int row, int col, const QSet<int> &possibilities)
{
  QTableWidgetItem *item = m_table->item(row, col);
  if (item)
  {
    int mask = 0;

    for (int p : possibilities)
    {
      mask |= (1 << (p - 1));
    }
    item->setData(Qt::UserRole, mask);
  }
}

QString MainWindow::promptSaveFilePath()
{
  QString path = QFileDialog::getSaveFileName(
      this,
      tr("Save Game"),
      "",
      tr("Sudoku Files (*.sudoku);;All Files (*)"));

  if (!path.isEmpty() && !path.endsWith(".sudoku", Qt::CaseInsensitive))
  {
    path += ".sudoku";
  }

  return path;
}

QString MainWindow::promptLoadFilePath()
{
  return QFileDialog::getOpenFileName(
      this,
      tr("Load Game"),
      "",
      tr("Sudoku Files (*.sudoku);;All Files (*)"));
}