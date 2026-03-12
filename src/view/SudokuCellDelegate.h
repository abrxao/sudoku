#ifndef SUDOKUCELLDELEGATE_H
#define SUDOKUCELLDELEGATE_H

#include <QStyledItemDelegate>

class SudokuCellDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  explicit SudokuCellDelegate(QObject *parent = nullptr);

  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // SUDOKUCELLDELEGATE_H