#ifndef SUDOKUPRESENTER_H
#define SUDOKUPRESENTER_H

#include <QObject>
#include "../model/SudokuModel.h"
#include "../view/MainWindow.h"

class SudokuPresenter : public QObject
{
  Q_OBJECT

public:
  SudokuPresenter(SudokuModel *model, MainWindow *view, QObject *parent = nullptr);

  void startGame(Difficulty diff = Difficulty::Easy);

private slots:

  void onGridLoaded();
  void onCellUpdated(int row, int col, int value);
  void onCellSelected(int row, int col);
  void onCellInput(int row, int col, int value);
  void onNewGameRequested(int diffIndex);

private:
  SudokuModel *m_model;
  MainWindow *m_view;
  void checkStuckCells();
  void syncViewWithModel();
};

#endif // SUDOKUPRESENTER_H