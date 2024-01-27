#include "adm_main_window.h"
#include <iostream>
#include <QApplication>
#include <QWidget>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    adm_main_window *Main_window = new adm_main_window;
    Main_window->show();

    app.exec();
}