#include "SudokuModel.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

SudokuModel::SudokuModel()
{
  clearGrid();
}

void SudokuModel::clearGrid()
{
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      m_grid[i][j] = 0;
}

int SudokuModel::getValue(int row, int col) const
{
  if (row < 0 || row >= 9 || col < 0 || col >= 9)
    return 0;
  return m_grid[row][col];
}

bool SudokuModel::loadFirstGridFromFile(const QString &filePath)
{
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return false;
  }

  QTextStream in(&file);
  QString firstLine = in.readLine();

  QString gridLine = in.readLine();

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
      m_grid[row][col] = c.digitValue();
    }
  }
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