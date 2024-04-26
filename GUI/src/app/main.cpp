#include "main_window.h"
#include <database.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    if (argc < 5)
    {
        std::cerr << " ERROR !!!!The Information required to connect to the Databse should be entered as command line arguments : Server-- > argv[1], Port-- > argv[2], UserName-- > argv[3], Schema-- > argv[4], Password-- > argv[5] " << std::endl;

        return 1;
    }

    connection_details ID;
    ID.server = argv[1];
    ID.port = std::stoi(argv[2]);
    ID.user = argv[3];
    ID.schema = argv[4];
    ID.password = argv[5];

    sql::Connection *db_connection = connection_setup(&ID);

    main_window Main_window(db_connection);
    Main_window.show();

    return app.exec();
}