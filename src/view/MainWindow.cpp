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

void MainWindow::displayGrid(SudokuModel *model)
{
  if (!model)
    return;

  for (int row = 0; row < 9; ++row)
  {
    for (int col = 0; col < 9; ++col)
    {
      int val = model->getValue(row, col);
      QTableWidgetItem *item = new QTableWidgetItem();

      if (val != 0)
      {
        item->setText(QString::number(val));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        QFont font = item->font();
        font.setBold(true);
        font.setPointSize(14);
        item->setFont(font);
        item->setBackground(QColor("#e0e0e0"));
      }
      m_table->setItem(row, col, item);
    }
  }
}