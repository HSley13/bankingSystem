#include "option_main_window.h"
#include <database.h>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    connection_details ID;

    sql::Connection *db_connection = connection_setup(&ID);

    option_main_window *Option_main_window = new option_main_window(db_connection);
    Option_main_window->show();

    app.exec();
}