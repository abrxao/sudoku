#include "SudokuModel.h"
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
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
    qWarning() << "Error trying to open assets file:" << filePath;
    return false;
  }

  QTextStream in(&file);
  QString firstLine = in.readLine();

  bool ok;
  int gridCount = firstLine.toInt(&ok);

  if (!ok || gridCount <= 0)
  {
    qWarning() << "File error. Invalid grid count at:" << filePath;
    return false;
  }

  int randomIdx = QRandomGenerator::global()->bounded(gridCount);

  QString gridLine;
  for (int i = 0; i <= randomIdx; ++i)
  {
    gridLine = in.readLine();
    if (gridLine.isNull())
    {
      qWarning() << "File ending before get sorted grid.";
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