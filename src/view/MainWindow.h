#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QEvent>
#include <QKeyEvent>
#include <QTranslator>
#include <QCoreApplication>
#include <QCheckBox>

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
  bool eventFilter(QObject *obj, QEvent *event) override;
  void retranslateUI();
  void setCellPossibilities(int row, int col, const QSet<int> &possibilities);

signals:
  void cellSelected(int row, int col);
  void cellInput(int row, int col, int value);
  void newGameRequested(int difficultyIndex);
  void hintsToggled(bool enabled);

private slots:
  void onCellChanged(int row, int col);
  void onCurrentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
  QTableWidget *m_table;
  QLabel *m_helperLabel;
  QComboBox *m_diffCombo;
  QPushButton *m_newGameBtn;
  QComboBox *m_langCombo;
  QLabel *m_levelLabel;
  QTranslator m_translator;
  QCheckBox *m_hintsCheckbox;
  bool m_isUpdating;
  void setupUI();
};

#endif // MAINWINDOW_H