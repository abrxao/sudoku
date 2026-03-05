#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <QString>
#include <QVector>
#include <QSet>

enum class Difficulty
{
  Easy,
  Medium,
  Hard,
  Insane
};

class SudokuModel
{
public:
  SudokuModel();

  bool loadRandomGrid(Difficulty difficulty);
  int getValue(int row, int col) const;
  bool loadFromString(const QString &gridData);
  QSet<int> getPossibilities(int row, int col) const;

private:
  int m_grid[9][9];
  void clearGrid();
  QString getFilePathForDifficulty(Difficulty diff) const;
};

#endif // SUDOKUMODEL_H