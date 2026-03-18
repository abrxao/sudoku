#include "MainWindow.h"
#include "SudokuCellDelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>

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

  m_levelLabel = new QLabel(this);
  m_levelLabel->setStyleSheet("font-size: 14pt; color: #00796B; font-weight: bold;");

  m_diffCombo = new QComboBox(this);
  m_diffCombo->addItems({"Easy", "Medium", "Hard", "Insane"});
  m_diffCombo->setStyleSheet(comboStyle);

  m_hintsCheckbox = new QCheckBox(this);
  m_hintsCheckbox->setChecked(true);
  m_hintsCheckbox->setStyleSheet("font-size: 13pt; color: #00796B; font-weight: bold; spacing: 8px; margin-left: 15px;");

  connect(m_hintsCheckbox, &QCheckBox::toggled, this, &MainWindow::hintsToggled);

  m_newGameBtn = new QPushButton(this);
  m_newGameBtn->setStyleSheet(
      "QPushButton { font-size: 14pt; padding: 8px 16px; background-color: #FF9800; color: white; font-weight: bold; border-radius: 6px; border: none; }"
      "QPushButton:hover { background-color: #F57C00; }"
      "QPushButton:pressed { background-color: #E65100; }");

  topLayout->addWidget(m_langCombo);
  topLayout->addWidget(m_hintsCheckbox);
  topLayout->addStretch();
  topLayout->addWidget(m_levelLabel);
  topLayout->addWidget(m_diffCombo);
  topLayout->addWidget(m_newGameBtn);

  m_table = new QTableWidget(9, 9, this);
  m_table->horizontalHeader()->setVisible(false);
  m_table->verticalHeader()->setVisible(false);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

  m_table->setStyleSheet(
      "QTableWidget { gridline-color: #80CBC4; font-size: 18pt; background-color: white; border: 2px solid #00796B; border-radius: 8px; }"
      "QTableWidget::item:selected { background-color: #E0F2F1; color: #00796B; border: 2px solid #009688; }");

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
  mainLayout->setContentsMargins(15, 0, 15, 15);

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

  connect(m_newGameBtn, &QPushButton::clicked, this, [this]()
          { emit newGameRequested(m_diffCombo->currentIndex()); });

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
  m_hintsCheckbox->setText(tr("Show Hints"));
  m_hintsCheckbox->setToolTip(tr("Toggle the visibility of candidate numbers inside empty cells"));
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