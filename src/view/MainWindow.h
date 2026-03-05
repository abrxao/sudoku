#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "../model/SudokuModel.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

  void displayGrid(SudokuModel *model);

private:
  QTableWidget *m_table;
  void setupUI();
};

#endif // MAINWINDOW_H