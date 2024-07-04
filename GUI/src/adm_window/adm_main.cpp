#include "adm_main_window.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    sql::Connection *db_connection;

    adm_main_window Adm_option_main_window(db_connection);
    Adm_option_main_window.show();

    app.exec();
}