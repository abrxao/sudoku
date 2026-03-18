#include "SudokuPresenter.h"

SudokuPresenter::SudokuPresenter(SudokuModel *model, MainWindow *view, QObject *parent)
    : QObject(parent), m_model(model), m_view(view)
{

  connect(m_model, &SudokuModel::gridLoaded, this, &SudokuPresenter::onGridLoaded);
  connect(m_model, &SudokuModel::cellUpdated, this, &SudokuPresenter::onCellUpdated);
  connect(m_view, &MainWindow::cellSelected, this, &SudokuPresenter::onCellSelected);
  connect(m_view, &MainWindow::cellInput, this, &SudokuPresenter::onCellInput);
  connect(m_view, &MainWindow::newGameRequested, this, &SudokuPresenter::onNewGameRequested);
  connect(m_view, &MainWindow::hintsToggled, this, &SudokuPresenter::onHintsToggled);
  connect(m_view, &MainWindow::saveGameRequested, this, &SudokuPresenter::onSaveGameRequested);
  connect(m_view, &MainWindow::loadGameRequested, this, &SudokuPresenter::onLoadGameRequested);
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

      bool isFixed = m_model->isFixed(row, col);

      m_view->setCellValue(row, col, val, isFixed);
    }
  }
  updateGridState();
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

  updateGridState();

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

void SudokuPresenter::updateGridState()
{
  for (int r = 0; r < 9; ++r)
  {
    for (int c = 0; c < 9; ++c)
    {
      if (m_model->getValue(r, c) == 0)
      {
        QSet<int> poss = m_model->getPossibilities(r, c);

        if (m_showHints)
        {
          m_view->setCellPossibilities(r, c, poss);
        }
        else
        {
          m_view->setCellPossibilities(r, c, QSet<int>());
        }

        m_view->setCellStuck(r, c, poss.isEmpty());
      }
      else
      {
        m_view->setCellPossibilities(r, c, QSet<int>());
      }
    }
  }
}

void SudokuPresenter::onHintsToggled(bool enabled)
{
  m_showHints = enabled;
  updateGridState();
}

void SudokuPresenter::onSaveGameRequested()
{
  QString path = m_view->promptSaveFilePath();

  if (!path.isEmpty())
  {
    if (!m_model->saveToFile(path))
    {
      m_view->showError(tr("Failed to save the game. Check file permissions."));
    }
  }
}

void SudokuPresenter::onLoadGameRequested()
{
  QString path = m_view->promptLoadFilePath();

  if (!path.isEmpty())
  {
    if (!m_model->loadFromFile(path))
    {
      m_view->showError(tr("Failed to load the game. The file might be corrupted."));
    }
  }
}