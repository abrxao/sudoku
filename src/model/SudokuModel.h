#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <QString>
#include <QVector>

class SudokuModel
{
public:
  SudokuModel();

  bool loadFirstGridFromFile(const QString &filePath);

  int getValue(int row, int col) const;

private:
  int m_grid[9][9];
  void clearGrid();
};

#endif // SUDOKUMODEL_H