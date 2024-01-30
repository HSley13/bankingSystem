#include "database.h"
#include "mainwindow.h"
#include <iostream>
#include <QWidget>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <argon2.h>

sql::Connection *DatabaseManager::connectionSetup(connection_details *ID)
{
    try
    {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *connection;

        driver = sql::mysql::get_driver_instance();
        connection = driver->connect(ID->server, ID->user, ID->password);

        connection->setSchema("myDB");

        return connection;
    }
    catch (const sql::SQLException &e)
    {
        std::cerr << "SQL ERROR: " << e.what() << std::endl;
        return nullptr;
    }
}

void DatabaseManager::closeConnection(sql::Connection *connection)
{
    if (connection)
        connection->close();
}
