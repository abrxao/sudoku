#include "SudokuModel.h"
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>

SudokuModel::SudokuModel(QObject *parent) : QObject(parent)
{
  clearGrid();
}

void SudokuModel::clearGrid()
{
  for (int i = 0; i < 9; ++i)
  {
    for (int j = 0; j < 9; ++j)
    {
      m_grid[i][j] = 0;
      m_originalGrid[i][j] = 0;
    }
  }
}

int SudokuModel::getValue(int row, int col) const
{
  if (row < 0 || row >= 9 || col < 0 || col >= 9)
    return 0;
  return m_grid[row][col];
}

QString SudokuModel::getFilePathForDifficulty(Difficulty diff) const
{
  switch (diff)
  {
  case Difficulty::Easy:
    return ":/Easy.txt";
  case Difficulty::Medium:
    return ":/Medium.txt";
  case Difficulty::Hard:
    return ":/Hard.txt";
  case Difficulty::Insane:
    return ":/Insane.txt";
  }
  return ":/Easy.txt";
}

bool SudokuModel::loadRandomGrid(Difficulty difficulty)
{
  QString filePath = getFilePathForDifficulty(difficulty);
  QFile file(filePath);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return false;
  }

  QTextStream in(&file);
  QString firstLine = in.readLine();

  bool ok;
  int gridCount = firstLine.toInt(&ok);

  if (!ok || gridCount <= 0)
  {
    return false;
  }

  int randomIdx = QRandomGenerator::global()->bounded(gridCount);

  QString gridLine;
  for (int i = 0; i <= randomIdx; ++i)
  {
    gridLine = in.readLine();
    if (gridLine.isNull())
    {
      return false;
    }
  }

  return loadFromString(gridLine);
}

bool SudokuModel::loadFromString(const QString &gridData)
{
  if (gridData.length() != 81)
  {
    return false;
  }

  int index = 0;
  for (int row = 0; row < 9; ++row)
  {
    for (int col = 0; col < 9; ++col)
    {
      QChar c = gridData[index++];
      if (!c.isDigit())
      {
        clearGrid();
        return false;
      }
      int val = c.digitValue();
      m_grid[row][col] = val;
      m_originalGrid[row][col] = val;
    }
  }
  emit gridLoaded();
  return true;
}

QSet<int> SudokuModel::getPossibilities(int row, int col) const
{
  if (m_grid[row][col] != 0)
  {
    return QSet<int>();
  }

  QSet<int> possibilities = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (int i = 0; i < 9; ++i)
  {
    possibilities.remove(m_grid[row][i]);
    possibilities.remove(m_grid[i][col]);
  }

  int startRow = (row / 3) * 3;
  int startCol = (col / 3) * 3;

  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      possibilities.remove(m_grid[startRow + i][startCol + j]);
    }
  }

  return possibilities;
}

void SudokuModel::setValue(int row, int col, int value)
{
  if (row >= 0 && row < 9 && col >= 0 && col < 9)
  {
    m_grid[row][col] = value;
    emit cellUpdated(row, col, value);
  }
}

bool SudokuModel::isGameWon() const
{

  for (int r = 0; r < 9; ++r)
  {
    for (int c = 0; c < 9; ++c)
    {
      if (m_grid[r][c] == 0)
      {
        return false;
      }
    }
  }

  for (int i = 0; i < 9; ++i)
  {
    int rowMask = 0, colMask = 0, blockMask = 0;

    for (int j = 0; j < 9; ++j)
    {
      int rVal = m_grid[i][j];
      if ((rowMask & (1 << rVal)))
      {
        return false;
      }
      rowMask |= (1 << rVal);

      int cVal = m_grid[j][i];
      if ((colMask & (1 << cVal)))
      {
        return false;
      }
      colMask |= (1 << cVal);

      int bRow = (i / 3) * 3 + (j / 3);
      int bCol = (i % 3) * 3 + (j % 3);
      int bVal = m_grid[bRow][bCol];
      if ((blockMask & (1 << bVal)))
      {

        return false;
      }
      blockMask |= (1 << bVal);
    }
  }

  return true;
}
bool SudokuModel::isValidMove(int row, int col, int value) const
{
  if (value == 0)
    return true;

  for (int i = 0; i < 9; ++i)
  {
    if (i != col && m_grid[row][i] == value)
      return false;
    if (i != row && m_grid[i][col] == value)
      return false;
  }

  int startRow = (row / 3) * 3;
  int startCol = (col / 3) * 3;
  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      int r = startRow + i;
      int c = startCol + j;
      if ((r != row || c != col) && m_grid[r][c] == value)
        return false;
    }
  }

  return true;
}

bool SudokuModel::isFixed(int row, int col) const
{
  if (row < 0 || row >= 9 || col < 0 || col >= 9)
    return false;
  return m_originalGrid[row][col] != 0;
}

bool SudokuModel::saveToFile(const QString &filePath) const
{
  QFile file(filePath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    return false;
  }

  QTextStream out(&file);
  for (int r = 0; r < 9; ++r)
  {
    for (int c = 0; c < 9; ++c)
    {
      out << m_originalGrid[r][c];
    }
  }
  out << "\n";

  for (int r = 0; r < 9; ++r)
  {
    for (int c = 0; c < 9; ++c)
    {
      out << m_grid[r][c];
    }
  }

  return true;
}

bool SudokuModel::loadFromFile(const QString &filePath)
{
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return false;
  }

  QTextStream in(&file);
  QString origData = in.readLine().trimmed();
  QString currentData = in.readLine().trimmed();

  if (origData.length() != 81 || currentData.length() != 81)
  {
    return false;
  }

  int idx = 0;
  for (int r = 0; r < 9; ++r)
  {
    for (int c = 0; c < 9; ++c)
    {
      m_originalGrid[r][c] = origData[idx].digitValue();
      m_grid[r][c] = currentData[idx].digitValue();
      idx++;
    }
  }

  emit gridLoaded();
  return true;
}