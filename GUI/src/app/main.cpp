#include "main_window.h"
#include <QWidget>
#include <QApplication>
#include <iostream>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    main_window *Main_window = new main_window;
    Main_window->show();

    return app.exec();
}