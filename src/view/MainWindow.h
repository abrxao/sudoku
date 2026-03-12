#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  void setCellValue(int row, int col, int value, bool isFixed = false);
  void clearBoard();
  void showHelper(int row, int col, const QSet<int> &possibilities);
  void clearHelper();
  void showVictoryMessage();
  void setCellStuck(int row, int col, bool isStuck);
  void showError(const QString &message);

signals:
  void cellClicked(int row, int col);
  void cellInput(int row, int col, int value);
  void newGameRequested(int difficultyIndex);

private slots:
  void onCellChanged(int row, int col);

private:
  QTableWidget *m_table;
  QLabel *m_helperLabel;
  QComboBox *m_diffCombo;
  QPushButton *m_newGameBtn;
  bool m_isUpdating;
  void setupUI();
};

#endif // MAINWINDOW_H