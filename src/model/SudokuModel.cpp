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
    qWarning() << "Error trying to open file:" << filePath;
    return false;
  }

  QTextStream in(&file);
  QString firstLine = in.readLine();

  QString gridLine = in.readLine();
  if (gridLine.length() < 81)
  {
    qWarning() << "Invalid Grid. Size:" << gridLine.length();
    return false;
  }

  int index = 0;
  for (int row = 0; row < 9; ++row)
  {
    for (int col = 0; col < 9; ++col)
    {
      m_grid[row][col] = gridLine[index].digitValue();
      index++;
    }
  }

  return true;
}