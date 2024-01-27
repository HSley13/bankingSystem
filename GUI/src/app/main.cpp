#include "main_window.h"
#include "database.h"
#include <adm_main_window.h>
#include <info_main_window.h>
#include <QWidget>
#include <QApplication>
#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <argon2.h>

int main(int argc, char **argv)
{
    connection_details ID;
    ID.server = "localhost";
    ID.user = "root";
    ID.password = "sleyHortes1312";

    sql ::Connection *connection = DatabaseManager::connectionSetup(&ID);
    if (!connection)
    {
        std ::cerr << "Failed to establish the Database connection." << std ::endl;
        return 1;
    }

    QApplication app(argc, argv);

    main_window *Main_window = new main_window;
    Main_window->show();

    return app.exec();
}