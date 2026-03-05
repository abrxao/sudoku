#include <QtTest>
#include <QSet>
#include "../..src/model/SudokuModel.h"

class TestSudokuModel : public QObject
{
  Q_OBJECT

private slots:
  void testPossibilitiesCalculation()
  {
    SudokuModel model;
    QString firstGrid = "000021076000079002207080004724100005060795001010002708002803060983260017450000800";

    QVERIFY(model.loadFromString(firstGrid));

    QSet<int> expected = {3, 5, 8};
    QSet<int> actual = model.getPossibilities(0, 0);

    QCOMPARE(actual, expected);
    QVERIFY(model.getPossibilities(0, 4).isEmpty());
  }
};

QTEST_APPLESS_MAIN(TestSudokuModel)
#include "tst_SudokuModel.moc"