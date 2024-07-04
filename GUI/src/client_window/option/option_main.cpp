#include "option_main_window.h"
#include <database.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    sql::Connection *db_connection = nullptr;

    option_main_window Option_main_window(db_connection, nullptr);
    Option_main_window.show();

    app.exec();
}