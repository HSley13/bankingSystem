#include "client_main_window.h"
#include <database.h>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    sql::Connection *db_connection;

    client_main_window *Client_main_window = new client_main_window(db_connection);
    Client_main_window->show();

    app.exec();
}