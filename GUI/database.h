#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <argon2.h>

class connection_details
{
public:
    std ::string server, user, password;
};

class DatabaseManager
{

private:
    std::string server, user, password;

public:
    static sql::Connection *connectionSetup(connection_details *ID);
    static void closeConnection(sql::Connection *connection);
};

#endif
