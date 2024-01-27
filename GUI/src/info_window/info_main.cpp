#include "info_main_window.h"
#include <QApplication>
#include <QMainWindow>
#include <iostream>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    info_main_window *Info_main_window = new info_main_window;
    Info_main_window->show();

    app.exec();
}