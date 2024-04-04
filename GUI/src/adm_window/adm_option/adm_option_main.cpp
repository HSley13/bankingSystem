#include "adm_option_main_window.h"
#include <database.h>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    connection_details ID;

    sql::Connection *db_connection = connection_setup(&ID);

    adm_option_main_window *Adm_option_main_window = new adm_option_main_window(db_connection);
    Adm_option_main_window->show();

    app.exec();
}