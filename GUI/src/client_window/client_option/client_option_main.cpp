#include "client_option_main_window.h"
#include <iostream>
#include <QApplication>
#include <QWidget>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    client_option_main_window *Client_option_main_window = new client_option_main_window;
    Client_option_main_window->show();

    app.exec();
}