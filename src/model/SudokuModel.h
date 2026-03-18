#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <QObject>
#include <QString>
#include <QSet>

enum class Difficulty
{
  Easy,
  Medium,
  Hard,
  Insane
};

class SudokuModel : public QObject
{
  Q_OBJECT

public:
  explicit SudokuModel(QObject *parent = nullptr);
  bool loadFromString(const QString &gridData);
  bool loadRandomGrid(Difficulty difficulty);
  int getValue(int row, int col) const;
  void setValue(int row, int col, int value);
  QSet<int> getPossibilities(int row, int col) const;
  bool isGameWon() const;
  bool isValidMove(int row, int col, int value) const;
  bool saveToFile(const QString &filePath) const;
  bool loadFromFile(const QString &filePath);
  bool isFixed(int row, int col) const;

signals:

  void gridLoaded();
  void cellUpdated(int row, int col, int value);

private:
  int m_grid[9][9];
  int m_originalGrid[9][9];
  void clearGrid();
  QString getFilePathForDifficulty(Difficulty diff) const;
};

#endif