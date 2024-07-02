#include "adm_option_main_window.h"

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

adm_option_main_window::adm_option_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack, QWidget *parent)
    : QMainWindow(parent), connection(db_connection), _previous_stack(previous_stack)
{
        stack = new QStackedWidget(this);
        setCentralWidget(stack);
        setStyleSheet("font-family: Arial Black;"
                      "font-size: 16px;"
                      "font: bold italic;"
                      "color: beige;"
                      "background-color: black;");
        resize(200, 200);

        QWidget *adm_option_widget = new QWidget();

        QLabel *image_label = new QLabel(this);
        QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/adm_option.jpeg");
        image_label->setPixmap(image.scaled(80, 80, Qt::KeepAspectRatio));
        image_label->setScaledContents(true);

        QPushButton *button1 = new QPushButton("1. Create Administrator", this);
        connect(button1, &QPushButton::clicked, this, [=]()
                { stack->setCurrentIndex(1); });

        QPushButton *button2 = new QPushButton("2. Accounts Table", this);
        connect(button2, &QPushButton::clicked, this, [=]()
                { stack->setCurrentIndex(2); });

        QPushButton *button3 = new QPushButton("3. Specific Account", this);
        connect(button3, &QPushButton::clicked, this, [=]()
                { stack->setCurrentIndex(3); });

        QPushButton *button4 = new QPushButton("4. People in Debt", this);
        connect(button4, &QPushButton::clicked, this, [=]()
                { stack->setCurrentIndex(4); });

        QPushButton *button5 = new QPushButton("5. Specific Account in Debt", this);
        connect(button5, &QPushButton::clicked, this, [=]()
                { stack->setCurrentIndex(5); });

        QPushButton *button6 = new QPushButton("6. Account Transaction History", this);
        connect(button6, &QPushButton::clicked, this, [=]()
                { stack->setCurrentIndex(6); });

        QPushButton *button7 = new QPushButton("7. Transaction History according a Date", this);
        connect(button7, &QPushButton::clicked, this, [=]()
                { stack->setCurrentIndex(7); });

        QPushButton *button8 = new QPushButton("8. Remove Account", this);
        connect(button8, &QPushButton::clicked, this, [=]()
                { stack->setCurrentIndex(8); });

        QPushButton *back_button = new QPushButton("Previous Menu", this);
        back_button->setStyleSheet("color: beige;"
                                   "font-family: Arial Black;"
                                   "font-size: 20;"
                                   "font: bold italic 14px;"
                                   "background-color: black;");
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QFormLayout *formLayout1 = new QFormLayout();
        formLayout1->addRow(button1);
        formLayout1->addRow(button2);
        formLayout1->addRow(button3);
        formLayout1->addRow(button4);

        QFormLayout *formLayout2 = new QFormLayout();
        formLayout2->addRow(button5);
        formLayout2->addRow(button6);
        formLayout2->addRow(button7);
        formLayout2->addRow(button8);

        QGridLayout *gridLayout = new QGridLayout(adm_option_widget);
        gridLayout->setSpacing(2);
        gridLayout->addWidget(image_label, 0, 0, 1, 2, Qt::AlignCenter);
        gridLayout->addLayout(formLayout1, 1, 0);
        gridLayout->addLayout(formLayout2, 1, 1);
        gridLayout->addWidget(back_button, 2, 0, 1, 2, Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *wid_1 = new QWidget();

        new_adm_account_number = new QLineEdit(this);
        new_adm_password = new QLineEdit(this);

        new_adm_password_confirmation = new QLineEdit(this);
        new_adm_password_confirmation->setEchoMode(QLineEdit::Password);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::wid_1_config);

        back_button = new QPushButton("Previous Menu");
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QFormLayout *wid_1_layout = new QFormLayout();
        wid_1_layout->addRow("Enter Desired Account Number", new_adm_account_number);
        wid_1_layout->addRow("Enter Desired Password", new_adm_password);
        wid_1_layout->addRow("Confirmed Password", new_adm_password_confirmation);
        wid_1_layout->addWidget(confirm_button);
        wid_1_layout->addWidget(back_button);

        QGroupBox *box_1 = new QGroupBox();
        box_1->setLayout(wid_1_layout);
        box_1->setFixedSize(500, 300);

        QHBoxLayout *HBOX_1 = new QHBoxLayout(wid_1);
        HBOX_1->addWidget(box_1);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_2 = new QWidget();

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_accounts_table);

        back_button = new QPushButton("Previous Menu");
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QLabel *message_2 = new QLabel("Are sure to display The Accounts Table?", this);

        QVBoxLayout *wid_2_vbox = new QVBoxLayout();
        wid_2_vbox->addWidget(message_2);
        wid_2_vbox->addWidget(confirm_button);
        wid_2_vbox->addWidget(back_button);

        QGroupBox *box_2 = new QGroupBox();
        box_2->setLayout(wid_2_vbox);
        box_2->setFixedSize(500, 300);

        QHBoxLayout *HBOX_2 = new QHBoxLayout(wid_2);
        HBOX_2->addWidget(box_2);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_3 = new QWidget();

        wid_3_account_number = new QLineEdit(this);

        QFormLayout *wid_3_layout = new QFormLayout();
        wid_3_layout->addRow("DISPLAY SPECIFIC ACCOUNT INFO", wid_3_account_number);

        QLabel *message_3 = new QLabel("Are sure to display the entered Account?", this);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_specific_accounts_table);

        back_button = new QPushButton("Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QVBoxLayout *wid_3_vbox = new QVBoxLayout();
        wid_3_vbox->addLayout(wid_3_layout);
        wid_3_vbox->addWidget(message_3);
        wid_3_vbox->addWidget(confirm_button);
        wid_3_vbox->addWidget(back_button);

        QGroupBox *box_3 = new QGroupBox();
        box_3->setLayout(wid_3_vbox);

        QHBoxLayout *HBOX_3 = new QHBoxLayout(wid_3);
        HBOX_3->addWidget(box_3);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_4 = new QWidget();

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_people_in_debt);

        back_button = new QPushButton("Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QLabel *message_4 = new QLabel("Are sure to display all People in debt??", this);

        QVBoxLayout *wid_4_vbox = new QVBoxLayout();
        wid_4_vbox->addWidget(message_4, Qt::AlignCenter);
        wid_4_vbox->addWidget(confirm_button, Qt::AlignCenter);
        wid_4_vbox->addWidget(back_button, Qt::AlignCenter | Qt::AlignBottom);

        QGroupBox *box_4 = new QGroupBox();
        box_4->setLayout(wid_4_vbox);

        QHBoxLayout *HBOX_4 = new QHBoxLayout(wid_4);
        HBOX_4->addWidget(box_4);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_5 = new QWidget();

        wid_5_account_number = new QLineEdit(this);

        QFormLayout *wid_5_layout = new QFormLayout();
        wid_5_layout->addRow("DISPLAY SPECIFIC ACCOUNT IN DEBT", wid_5_account_number);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_specific_accounts_in_debt);

        back_button = new QPushButton("Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QLabel *message_5 = new QLabel("Are sure to display the entered Account?", this);

        QVBoxLayout *wid_5_vbox = new QVBoxLayout();
        wid_5_vbox->addLayout(wid_5_layout, Qt::AlignCenter);
        wid_5_vbox->addWidget(message_5, Qt::AlignCenter);
        wid_5_vbox->addWidget(confirm_button, Qt::AlignCenter);
        wid_5_vbox->addWidget(back_button, Qt::AlignCenter | Qt::AlignBottom);

        QGroupBox *box_5 = new QGroupBox();
        box_5->setLayout(wid_5_vbox);

        QHBoxLayout *HBOX_5 = new QHBoxLayout(wid_5);
        HBOX_5->addWidget(box_5);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_6 = new QWidget();

        wid_6_account_number = new QLineEdit(this);

        QFormLayout *wid_6_layout = new QFormLayout();
        wid_6_layout->addRow("DISPLAY TRANSACTION HISTORY", wid_6_account_number);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_transactions_history);

        back_button = new QPushButton("Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QLabel *message_6 = new QLabel("Are sure to display the Transaction History?", this);

        QVBoxLayout *wid_6_vbox = new QVBoxLayout();
        wid_6_vbox->addLayout(wid_6_layout, Qt::AlignCenter);
        wid_6_vbox->addWidget(message_6, Qt::AlignCenter);
        wid_6_vbox->addWidget(confirm_button, Qt::AlignCenter);
        wid_6_vbox->addWidget(back_button, Qt::AlignCenter | Qt::AlignBottom);

        QGroupBox *box_6 = new QGroupBox();
        box_6->setLayout(wid_6_vbox);

        QHBoxLayout *HBOX_6 = new QHBoxLayout(wid_6);
        HBOX_6->addWidget(box_6);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_7 = new QWidget();

        specific_account_number_transac = new QLineEdit(this);

        calendar = new QDateEdit(this);
        calendar->setCalendarPopup(true);
        calendar->setDate(QDate::currentDate());
        connect(calendar, &QDateEdit::dateChanged, this, [=]()
                { selected_date = calendar->date(); });

        QComboBox *choice = new QComboBox(this);
        choice->addItem("Display All Transactions BEFORE the selected Date");
        choice->addItem("Display All Transactions AFTER the selected Date");
        choice->addItem("Display All Transactions ONLY on the selected Date");
        choice->setStyleSheet("color: red;");

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::display_relative_transactions_history);

        back_button = new QPushButton("Return to the Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QFormLayout *wid_7_layout = new QFormLayout();
        wid_7_layout->addRow("Enter Account Number", specific_account_number_transac);
        wid_7_layout->addWidget(calendar);
        wid_7_layout->addWidget(choice);
        wid_7_layout->addWidget(confirm_button);
        wid_7_layout->addWidget(back_button);

        QGroupBox *box_7 = new QGroupBox();
        box_7->setLayout(wid_7_layout);

        QHBoxLayout *HBOX_7 = new QHBoxLayout(wid_7);
        HBOX_7->addWidget(box_7);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_8 = new QWidget();

        QLineEdit *wid_8_account_number = new QLineEdit(this);

        confirm_button = new QPushButton("Confirm", this);
        confirm_button->setStyleSheet("color: black;"
                                      "background-color: beige;");
        connect(confirm_button, &QPushButton::clicked, this, &adm_option_main_window::delete_accounts);

        back_button = new QPushButton("Previous Menu", this);
        connect(back_button, &QPushButton::clicked, this, &adm_option_main_window::back_button_func);

        QLabel *message_8 = new QLabel("Are sure you want to delete the entered Account?", this);

        QFormLayout *wid_8_layout = new QFormLayout();
        wid_8_layout->addRow("DELETE ACCOUNT", wid_8_account_number);
        wid_8_layout->addWidget(confirm_button);
        wid_8_layout->addWidget(back_button);

        QGroupBox *box_8 = new QGroupBox();
        box_8->setLayout(wid_8_layout);

        QHBoxLayout *HBOX_8 = new QHBoxLayout(wid_8);
        HBOX_8->addWidget(box_8);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        stack->addWidget(adm_option_widget);
        stack->addWidget(wid_1);
        stack->addWidget(wid_2);
        stack->addWidget(wid_3);
        stack->addWidget(wid_4);
        stack->addWidget(wid_5);
        stack->addWidget(wid_6);
        stack->addWidget(wid_7);
        stack->addWidget(wid_8);
}

void adm_option_main_window::back_button_func()
{
        int current_index = stack->currentIndex();

        (current_index != 0) ? stack->setCurrentIndex(0) : _previous_stack->setCurrentIndex(0);
}

void adm_option_main_window::wid_1_config()
{
        int account_number = new_adm_account_number->text().toInt();
        std::string password = new_adm_password->text().toStdString();
        std::string password_confirmation = new_adm_password_confirmation->text().toStdString();

        if (password.compare(password_confirmation))
        {
                new_adm_password_confirmation->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(this, "Warning!", "Password Confirmation Incorrect, try again");

                return;
        }

        new_adm_password_confirmation->setStyleSheet("border: 1px solid gray");

        std::string hash_password = BANK::hashing_password(password);

        BANK::create_adm(connection, account_number, hash_password);

        QMessageBox::information(this, "Password Confirmation Correct, Account created successfully", "Password Confirmation Correct, Account created successfully");

        new_adm_account_number->clear();
        new_adm_password->clear();
        new_adm_password_confirmation->clear();

        hash_password.clear();
        password.clear();
        password_confirmation.clear();
}

void adm_option_main_window::display_accounts_table()
{
        BANK::Qt_display_accounts_table(connection);
}

void adm_option_main_window::display_specific_accounts_table()
{
        int account_number = wid_3_account_number->text().toInt();

        BANK::Qt_display_specific_accounts(connection, account_number);

        wid_3_account_number->clear();
}

void adm_option_main_window::display_people_in_debt()
{
        BANK::Qt_display_people_in_debt(connection);
}

void adm_option_main_window::display_specific_accounts_in_debt()
{
        int account_number = wid_5_account_number->text().toInt();

        BANK::Qt_display_specific_accounts_in_debt(connection, account_number);
}

void adm_option_main_window::display_transactions_history()
{
        int account_number = wid_6_account_number->text().toInt();

        Transactions::Qt_display_transactions_history(connection, account_number);

        wid_6_account_number->clear();
}

void adm_option_main_window::display_relative_transactions_history()
{
        int account_number = specific_account_number_transac->text().toInt();
        QString date = selected_date.toString(Qt::ISODate);
        int selected_choice = choice->currentIndex();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);
        if (hashed_password == "")
        {
                specific_account_number_transac->setStyleSheet("border: 1px solid red");

                return;
        }

        specific_account_number_transac->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_display_specific_transactions_history(connection, account_number, date.toStdString(), selected_choice);

        specific_account_number_transac->clear();
}

void adm_option_main_window::delete_accounts()
{
        int account_number = wid_7_account_number->text().toInt();

        Account::Qt_remove_accounts(connection, account_number);

        wid_7_account_number->clear();
}