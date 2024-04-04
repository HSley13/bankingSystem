#include "adm_main_window.h"
#include <database.h>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    connection_details ID;

    sql::Connection *db_connection = connection_setup(&ID);

    adm_main_window *Main_window = new adm_main_window(db_connection);
    Main_window->show();

    app.exec();
}