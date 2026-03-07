#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

  void setCellValue(int row, int col, int value, bool isFixed = false);
  void clearBoard();

signals:

  void cellClicked(int row, int col);

private:
  QTableWidget *m_table;
  void setupUI();
};

#endif // MAINWINDOW_H