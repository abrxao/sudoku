#include <QDebug>
#include "SudokuPresenter.h"

SudokuPresenter::SudokuPresenter(SudokuModel *model, MainWindow *view, QObject *parent)
    : QObject(parent), m_model(model), m_view(view)
{

  connect(m_model, &SudokuModel::gridLoaded, this, &SudokuPresenter::onGridLoaded);
  connect(m_model, &SudokuModel::cellUpdated, this, &SudokuPresenter::onCellUpdated);

  connect(m_view, &MainWindow::cellClicked, this, &SudokuPresenter::onCellClicked);
}

void SudokuPresenter::startGame(Difficulty diff)
{

  m_model->loadRandomGrid(diff);
}

void SudokuPresenter::onGridLoaded()
{
  syncViewWithModel();
}

void SudokuPresenter::syncViewWithModel()
{
  m_view->clearBoard();
  for (int row = 0; row < 9; ++row)
  {
    for (int col = 0; col < 9; ++col)
    {
      int val = m_model->getValue(row, col);

      m_view->setCellValue(row, col, val, val != 0);
    }
  }
}

void SudokuPresenter::onCellUpdated(int row, int col, int value)
{

  m_view->setCellValue(row, col, value, false);
}

void SudokuPresenter::onCellClicked(int row, int col)
{
  QSet<int> possibilities = m_model->getPossibilities(row, col);

  if (possibilities.isEmpty())
  {
    qDebug() << "Cell (" << row << "," << col << ") clicked. Any possibles values or already filled.";
  }
  else
  {
    QList<int> list(possibilities.begin(), possibilities.end());
    std::sort(list.begin(), list.end());

    qDebug() << "Cell (" << row << "," << col << ") clicked. Valids values:" << list;
  }
}