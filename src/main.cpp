#include <QApplication>
#include "model/SudokuModel.h"
#include "view/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SudokuModel model;

    if (!model.loadFirstGridFromFile(":/Easy.txt"))
    {
        return -1;
    }

    MainWindow window;
    window.displayGrid(&model);
    window.show();

    return app.exec();
}