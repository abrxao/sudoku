#include "MainWindow.h"
#include <QHeaderView>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_isUpdating(false)
{
  setupUI();
}

void MainWindow::setupUI()
{
  this->setWindowTitle(tr("Sudoku Assistant - Pro Version"));
  this->setFixedSize(750, 550);

  QWidget *topBar = new QWidget(this);
  QHBoxLayout *topLayout = new QHBoxLayout(topBar);
  topLayout->setContentsMargins(0, 0, 0, 0);

  m_langCombo = new QComboBox(this);
  m_langCombo->addItems({"English", "Français"});
  m_langCombo->setStyleSheet("font-size: 14pt; padding: 5px;");

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
                qDebug() << "[I18N ERROR] Erro to load sudoku_fr.qm. Verify CMake file.";
              }
            }
            else
            { 
              qApp->removeTranslator(&m_translator);
            }
            retranslateUI(); });

  m_diffCombo = new QComboBox(this);
  m_diffCombo->addItems({"Easy", "Medium", "Hard", "Insane"});
  m_diffCombo->setStyleSheet("font-size: 14pt; padding: 5px;");

  m_newGameBtn = new QPushButton(tr("New Game"), this);
  m_newGameBtn->setStyleSheet("font-size: 14pt; padding: 5px; background-color: #2196f3; color: white; font-weight: bold; border-radius: 4px;");

  topLayout->addStretch();
  topLayout->addWidget(new QLabel(tr("Level:"), this));
  topLayout->addWidget(m_diffCombo);
  topLayout->addWidget(m_newGameBtn);

  connect(m_newGameBtn, &QPushButton::clicked, this, [this]()
          { emit newGameRequested(m_diffCombo->currentIndex()); });

  m_table = new QTableWidget(9, 9, this);
  m_table->horizontalHeader()->setVisible(false);
  m_table->verticalHeader()->setVisible(false);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->setFocusPolicy(Qt::StrongFocus);

  int cellSize = 50;
  for (int i = 0; i < 9; ++i)
  {
    m_table->setColumnWidth(i, cellSize);
    m_table->setRowHeight(i, cellSize);
  }
  m_table->setFixedSize(cellSize * 9 + 2, cellSize * 9 + 2);
  m_table->setStyleSheet(
      "QTableWidget { gridline-color: #d0d0d0; font-size: 16pt; }"
      "QTableWidget::item:selected { background-color: #bbdefb; color: black; }");

  m_helperLabel = new QLabel(tr("Select an empty cell\nto see hints."), this);
  m_helperLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  m_helperLabel->setWordWrap(true);
  m_helperLabel->setStyleSheet("QLabel { font-size: 14pt; padding: 20px; background-color: #f5f5f5; border: 1px solid #ccc; border-radius: 5px; }");
  m_helperLabel->setFixedWidth(250);

  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

  QHBoxLayout *gameLayout = new QHBoxLayout();
  gameLayout->addWidget(m_table);
  gameLayout->addWidget(m_helperLabel);

  mainLayout->addWidget(topBar);
  mainLayout->addLayout(gameLayout);

  setCentralWidget(centralWidget);

  connect(m_table, &QTableWidget::currentCellChanged, this, &MainWindow::onCurrentCellChanged);
  connect(m_table, &QTableWidget::cellChanged, this, &MainWindow::onCellChanged);

  m_table->installEventFilter(this);
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
  QColor quadrantColor = isDarkQuadrant ? QColor("#f4f6f8") : QColor("#ffffff");

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
      font.setPointSize(14);
      item->setFont(font);
      item->setBackground(QColor("#e0e0e0"));
      item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
    else
    {
      item->setBackground(quadrantColor);
      item->setForeground(QBrush(Qt::blue));
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
      item->setBackground(QColor("#ffcdd2"));
    }
    else
    {
      bool isDarkQuadrant = ((row / 3) + (col / 3)) % 2 != 0;
      QColor quadrantColor = isDarkQuadrant ? QColor("#f4f6f8") : QColor("#ffffff");
      item->setBackground(quadrantColor);
    }
  }

  m_isUpdating = false;
}

void MainWindow::showError(const QString &message)
{
  m_helperLabel->setText(QString("<span style='color: #d32f2f; font-weight: bold;'>⚠️ %1</span><br><br>%2")
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
  m_levelLabel->setText(tr("Level:"));
  m_newGameBtn->setText(tr("New Game"));

  m_diffCombo->setItemText(0, tr("Easy"));
  m_diffCombo->setItemText(1, tr("Medium"));
  m_diffCombo->setItemText(2, tr("Hard"));
  m_diffCombo->setItemText(3, tr("Insane"));

  clearHelper();

  m_newGameBtn->setToolTip(tr("Start a new game with the selected difficulty"));
  m_diffCombo->setToolTip(tr("Select the difficulty level"));
  m_langCombo->setToolTip(tr("Change the application language"));
  m_table->setToolTip(tr("Use arrow keys to navigate, numbers 1-9 to input, and Backspace to clear"));
}