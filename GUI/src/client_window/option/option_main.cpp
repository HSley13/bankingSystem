#include "option_main_window.h"
#include <iostream>
#include <QApplication>
#include <QWidget>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    option_main_window *Option_main_window = new option_main_window;
    Option_main_window->show();

    app.exec();
}