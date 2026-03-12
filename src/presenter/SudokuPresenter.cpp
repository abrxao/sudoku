#include "SudokuPresenter.h"

SudokuPresenter::SudokuPresenter(SudokuModel *model, MainWindow *view, QObject *parent)
    : QObject(parent), m_model(model), m_view(view)
{

  connect(m_model, &SudokuModel::gridLoaded, this, &SudokuPresenter::onGridLoaded);
  connect(m_model, &SudokuModel::cellUpdated, this, &SudokuPresenter::onCellUpdated);
  connect(m_view, &MainWindow::cellSelected, this, &SudokuPresenter::onCellSelected);
  connect(m_view, &MainWindow::cellInput, this, &SudokuPresenter::onCellInput);
  connect(m_view, &MainWindow::newGameRequested, this, &SudokuPresenter::onNewGameRequested);
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
  checkStuckCells();
}

void SudokuPresenter::onCellUpdated(int row, int col, int value)
{

  m_view->setCellValue(row, col, value, false);
}

void SudokuPresenter::onCellSelected(int row, int col)
{
  if (m_model->getValue(row, col) != 0)
  {
    m_view->clearHelper();
    return;
  }

  QSet<int> possibilities = m_model->getPossibilities(row, col);
  m_view->showHelper(row, col, possibilities);
}

void SudokuPresenter::onCellInput(int row, int col, int value)
{

  if (!m_model->isValidMove(row, col, value))
  {

    int safeOldValue = m_model->getValue(row, col);
    m_view->setCellValue(row, col, safeOldValue, false);

    m_view->showError(QString("The number <b>%1</b> is already at same line, column or quadrant.").arg(value));
    return;
  }

  m_model->setValue(row, col, value);

  onCellSelected(row, col);

  checkStuckCells();

  if (m_model->isGameWon())
  {
    m_view->showVictoryMessage();
  }
}
void SudokuPresenter::onNewGameRequested(int diffIndex)
{
  Difficulty selectedDiff = static_cast<Difficulty>(diffIndex);

  m_view->clearBoard();
  m_view->clearHelper();

  m_model->loadRandomGrid(selectedDiff);
}

void SudokuPresenter::checkStuckCells()
{
  for (int r = 0; r < 9; ++r)
  {
    for (int c = 0; c < 9; ++c)
    {
      if (m_model->getValue(r, c) == 0)
      {

        bool isStuck = m_model->getPossibilities(r, c).isEmpty();
        m_view->setCellStuck(r, c, isStuck);
      }
    }
  }
}