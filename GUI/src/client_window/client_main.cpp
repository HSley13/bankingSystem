#include "client_main_window.h"
#include <iostream>
#include <QApplication>
#include <QWidget>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    client_main_window *Client_main_window = new client_main_window;
    Client_main_window->show();

    app.exec();
}