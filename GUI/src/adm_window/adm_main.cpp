#include "adm_main_window.h"
#include <database.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    sql::Connection *db_connection;

    adm_main_window Main_window(db_connection);
    Main_window.show();

    app.exec();
}