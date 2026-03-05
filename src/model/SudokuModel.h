#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <QString>
#include <QVector>
#include <QSet>

class SudokuModel
{
public:
  SudokuModel();

  bool loadFirstGridFromFile(const QString &filePath);
  int getValue(int row, int col) const;
  bool loadFromString(const QString &gridData);
  QSet<int> getPossibilities(int row, int col) const;

private:
  int m_grid[9][9];
  void clearGrid();
};

#endif // SUDOKUMODEL_H