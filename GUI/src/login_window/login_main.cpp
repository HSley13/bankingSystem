#include "login_window.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    sql::Connection *db_connection = nullptr;

    login_window Main_window(db_connection);
    Main_window.show();

    app.exec();
}