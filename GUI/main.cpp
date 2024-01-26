#include <QApplication>
#include "mainwindow.h"
#include "database.h"
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
    std ::unique_ptr<MainWindow> mainWindow = std ::make_unique<MainWindow>();

    mainWindow->show();
    return app.exec();
}