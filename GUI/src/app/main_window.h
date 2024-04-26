#pragma once

#include <database.h>
#include <iostream>

#include <QtCore>
#include <QtWidgets>

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(sql::Connection *db_connection, QWidget *parent = nullptr);

private:
    sql::Connection *connection;

private slots:
    void adm_button_clicked();
    void client_button_clicked();
};
