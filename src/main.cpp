#include <QApplication>
#include "model/SudokuModel.h"
#include "view/MainWindow.h"
#include "presenter/SudokuPresenter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SudokuModel model;
    MainWindow view;

    SudokuPresenter presenter(&model, &view);

    presenter.startGame(Difficulty::Easy);
    view.show();

    return app.exec();
}