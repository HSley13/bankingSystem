#pragma once

#include <database.h>
class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(sql::Connection *db_connection, QWidget *parent = nullptr);

private:
    sql::Connection *connection;

    QStackedWidget *_stack;

private slots:
    void adm_button_clicked();
    void client_button_clicked();
};
