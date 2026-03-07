#include "MainWindow.h"
#include <QHeaderView>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  setupUI();
}

void MainWindow::setupUI()
{
  this->setWindowTitle("Sudoku Assistant");
  this->setFixedSize(600, 600);

  m_table = new QTableWidget(9, 9, this);
  m_table->horizontalHeader()->setVisible(false);
  m_table->verticalHeader()->setVisible(false);

  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->setFocusPolicy(Qt::NoFocus);

  m_table->setStyleSheet(
      "QTableWidget { gridline-color: #d0d0d0; }"
      "QTableWidget::item:selected { background-color: #aed581; color: black; }");

  int cellSize = 50;
  for (int i = 0; i < 9; ++i)
  {
    m_table->setColumnWidth(i, cellSize);
    m_table->setRowHeight(i, cellSize);
  }
  m_table->setFixedSize(cellSize * 9 + 2, cellSize * 9 + 2);

  connect(m_table, &QTableWidget::cellClicked, this, &MainWindow::cellClicked);

  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(centralWidget);
  layout->addWidget(m_table);
  layout->setAlignment(Qt::AlignCenter);
  setCentralWidget(centralWidget);
}

void MainWindow::clearBoard()
{
  m_table->clearContents();
}

void MainWindow::setCellValue(int row, int col, int value, bool isFixed)
{
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
}
