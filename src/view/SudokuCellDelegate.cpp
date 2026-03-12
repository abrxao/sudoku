#include "SudokuCellDelegate.h"
#include <QPainter>

SudokuCellDelegate::SudokuCellDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void SudokuCellDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QStyledItemDelegate::paint(painter, option, index);

    int value = index.data(Qt::DisplayRole).toInt();

    if (value == 0)
    {

        int mask = index.data(Qt::UserRole).toInt();

        if (mask > 0)
        {
            painter->save();

            QFont tinyFont = option.font;
            tinyFont.setPointSize(8);
            painter->setFont(tinyFont);
            painter->setPen(QColor("#9E9E9E"));

            QRect r = option.rect;
            int cellW = r.width() / 3;
            int cellH = r.height() / 3;

            for (int i = 1; i <= 9; ++i)
            {
                if (mask & (1 << (i - 1)))
                {
                    int col = (i - 1) % 3;
                    int row = (i - 1) / 3;

                    QRect numRect(r.x() + col * cellW, r.y() + row * cellH, cellW, cellH);

                    painter->drawText(numRect, Qt::AlignCenter, QString::number(i));
                }
            }
            painter->restore();
        }
    }
}