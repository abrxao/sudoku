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

  int cellSize = 50;
  for (int i = 0; i < 9; ++i)
  {
    m_table->setColumnWidth(i, cellSize);
    m_table->setRowHeight(i, cellSize);
  }

  m_table->setFixedSize(cellSize * 9 + 2, cellSize * 9 + 2);

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

  if (value == 0)
  {
    item->setText("");
    item->setBackground(Qt::white);
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

      item->setBackground(Qt::white);
    }
  }
}