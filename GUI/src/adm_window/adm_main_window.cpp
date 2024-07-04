#include "adm_main_window.h"

adm_main_window::adm_main_window(sql::Connection *db_connection, QStackedWidget *previous_stack, QWidget *parent)
    : QMainWindow(parent), connection(db_connection), _previous_stack(previous_stack)
{
        _stack = new QStackedWidget(this);
        setCentralWidget(_stack);
        setStyleSheet("font-family: Arial Black;"
                      "font-size: 16px;"
                      "font: bold italic;"
                      "color: beige;"
                      "background-color: black;");
        resize(200, 200);

        QWidget *adm_option_widget = new QWidget(this);

        QLabel *image_label = new QLabel(this);
        QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/adm_option.jpeg");
        image_label->setPixmap(image.scaled(80, 80, Qt::KeepAspectRatio));
        image_label->setScaledContents(true);

        QPushButton *button1 = new QPushButton("1. Create Administrator", this);
        connect(button1, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(1); });

        QPushButton *button2 = new QPushButton("2. Accounts Table", this);
        connect(button2, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(2); });

        QPushButton *button3 = new QPushButton("3. Specific Account", this);
        connect(button3, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(3); });

        QPushButton *button4 = new QPushButton("4. People in Debt", this);
        connect(button4, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(4); });

        QPushButton *button5 = new QPushButton("5. Specific Account in Debt", this);
        connect(button5, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(5); });

        QPushButton *button6 = new QPushButton("6. Account Transaction History", this);
        connect(button6, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(6); });

        QPushButton *button7 = new QPushButton("7. Transaction History according a Date", this);
        connect(button7, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(7); });

        QPushButton *button8 = new QPushButton("8. Remove Account", this);
        connect(button8, &QPushButton::clicked, this, [=]()
                { _stack->setCurrentIndex(8); });

        _back_button = new QPushButton("Previous Menu", this);
        _back_button->setStyleSheet("color: beige;"
                                    "font-family: Arial Black;"
                                    "font-size: 20;"
                                    "font: bold italic 14px;"
                                    "background-color: black;");
        connect(_back_button, &QPushButton::clicked, this, &adm_main_window::back_button_func);

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
        gridLayout->addWidget(_back_button, 2, 0, 1, 2, Qt::AlignCenter);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
        QWidget *wid_1 = new QWidget(this);

        _new_adm_account_number = new QLineEdit(this);
        _new_adm_password = new QLineEdit(this);

        _new_adm_password_confirmation = new QLineEdit(this);
        _new_adm_password_confirmation->setEchoMode(QLineEdit::Password);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &adm_main_window::wid_1_config);

        _back_button = new QPushButton("Previous Menu");
        connect(_back_button, &QPushButton::clicked, this, &adm_main_window::back_button_func);

        QFormLayout *wid_1_layout = new QFormLayout();
        wid_1_layout->addRow("Enter Desired Account Number", _new_adm_account_number);
        wid_1_layout->addRow("Enter Desired Password", _new_adm_password);
        wid_1_layout->addRow("Confirmed Password", _new_adm_password_confirmation);
        wid_1_layout->addWidget(_confirm_button);
        wid_1_layout->addWidget(_back_button);

        QGroupBox *box_1 = new QGroupBox();
        box_1->setLayout(wid_1_layout);
        box_1->setFixedSize(500, 300);

        QHBoxLayout *HBOX_1 = new QHBoxLayout(wid_1);
        HBOX_1->addWidget(box_1);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_2 = new QWidget(this);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &adm_main_window::display_accounts_table);

        _back_button = new QPushButton("Previous Menu");
        connect(_back_button, &QPushButton::clicked, this, &adm_main_window::back_button_func);

        QLabel *message_2 = new QLabel("Are sure to display The Accounts Table?", this);

        QVBoxLayout *wid_2_vbox = new QVBoxLayout();
        wid_2_vbox->addWidget(message_2);
        wid_2_vbox->addWidget(_confirm_button);
        wid_2_vbox->addWidget(_back_button);

        QGroupBox *box_2 = new QGroupBox();
        box_2->setLayout(wid_2_vbox);
        box_2->setFixedSize(500, 300);

        QHBoxLayout *HBOX_2 = new QHBoxLayout(wid_2);
        HBOX_2->addWidget(box_2);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_3 = new QWidget(this);

        wid_3_account_number = new QLineEdit(this);

        QFormLayout *wid_3_layout = new QFormLayout();
        wid_3_layout->addRow("DISPLAY SPECIFIC ACCOUNT INFO", wid_3_account_number);

        QLabel *message_3 = new QLabel("Are sure to display the entered Account?", this);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &adm_main_window::display_specific_accounts_table);

        _back_button = new QPushButton("Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &adm_main_window::back_button_func);

        QVBoxLayout *wid_3_vbox = new QVBoxLayout();
        wid_3_vbox->addLayout(wid_3_layout);
        wid_3_vbox->addWidget(message_3);
        wid_3_vbox->addWidget(_confirm_button);
        wid_3_vbox->addWidget(_back_button);

        QGroupBox *box_3 = new QGroupBox();
        box_3->setLayout(wid_3_vbox);

        QHBoxLayout *HBOX_3 = new QHBoxLayout(wid_3);
        HBOX_3->addWidget(box_3);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_4 = new QWidget(this);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &adm_main_window::display_people_in_debt);

        _back_button = new QPushButton("Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &adm_main_window::back_button_func);

        QLabel *message_4 = new QLabel("Are sure to display all People in debt??", this);

        QVBoxLayout *wid_4_vbox = new QVBoxLayout();
        wid_4_vbox->addWidget(message_4, Qt::AlignCenter);
        wid_4_vbox->addWidget(_confirm_button, Qt::AlignCenter);
        wid_4_vbox->addWidget(_back_button, Qt::AlignCenter | Qt::AlignBottom);

        QGroupBox *box_4 = new QGroupBox();
        box_4->setLayout(wid_4_vbox);

        QHBoxLayout *HBOX_4 = new QHBoxLayout(wid_4);
        HBOX_4->addWidget(box_4);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_5 = new QWidget(this);

        wid_5_account_number = new QLineEdit(this);

        QFormLayout *wid_5_layout = new QFormLayout();
        wid_5_layout->addRow("DISPLAY SPECIFIC ACCOUNT IN DEBT", wid_5_account_number);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &adm_main_window::display_specific_accounts_in_debt);

        _back_button = new QPushButton("Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &adm_main_window::back_button_func);

        QLabel *message_5 = new QLabel("Are sure to display the entered Account?", this);

        QVBoxLayout *wid_5_vbox = new QVBoxLayout();
        wid_5_vbox->addLayout(wid_5_layout, Qt::AlignCenter);
        wid_5_vbox->addWidget(message_5, Qt::AlignCenter);
        wid_5_vbox->addWidget(_confirm_button, Qt::AlignCenter);
        wid_5_vbox->addWidget(_back_button, Qt::AlignCenter | Qt::AlignBottom);

        QGroupBox *box_5 = new QGroupBox();
        box_5->setLayout(wid_5_vbox);

        QHBoxLayout *HBOX_5 = new QHBoxLayout(wid_5);
        HBOX_5->addWidget(box_5);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_6 = new QWidget(this);

        wid_6_account_number = new QLineEdit(this);

        QFormLayout *wid_6_layout = new QFormLayout();
        wid_6_layout->addRow("DISPLAY TRANSACTION HISTORY", wid_6_account_number);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &adm_main_window::display_transactions_history);

        _back_button = new QPushButton("Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &adm_main_window::back_button_func);

        QLabel *message_6 = new QLabel("Are sure to display the Transaction History?", this);

        QVBoxLayout *wid_6_vbox = new QVBoxLayout();
        wid_6_vbox->addLayout(wid_6_layout, Qt::AlignCenter);
        wid_6_vbox->addWidget(message_6, Qt::AlignCenter);
        wid_6_vbox->addWidget(_confirm_button, Qt::AlignCenter);
        wid_6_vbox->addWidget(_back_button, Qt::AlignCenter | Qt::AlignBottom);

        QGroupBox *box_6 = new QGroupBox();
        box_6->setLayout(wid_6_vbox);

        QHBoxLayout *HBOX_6 = new QHBoxLayout(wid_6);
        HBOX_6->addWidget(box_6);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_7 = new QWidget(this);

        _specific_account_number_transac = new QLineEdit(this);

        _calendar = new QDateEdit(this);
        _calendar->setCalendarPopup(true);
        _calendar->setDate(QDate::currentDate());
        connect(_calendar, &QDateEdit::dateChanged, this, [=]()
                { _selected_date = _calendar->date(); });

        _choice = new QComboBox(this);
        _choice->addItem("Display All Transactions BEFORE the selected Date");
        _choice->addItem("Display All Transactions AFTER the selected Date");
        _choice->addItem("Display All Transactions ONLY on the selected Date");
        _choice->setStyleSheet("color: red;");

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &adm_main_window::display_relative_transactions_history);

        _back_button = new QPushButton("Return to the Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &adm_main_window::back_button_func);

        QFormLayout *wid_7_layout = new QFormLayout();
        wid_7_layout->addRow("Enter Account Number", _specific_account_number_transac);
        wid_7_layout->addWidget(_calendar);
        wid_7_layout->addWidget(_choice);
        wid_7_layout->addWidget(_confirm_button);
        wid_7_layout->addWidget(_back_button);

        QGroupBox *box_7 = new QGroupBox();
        box_7->setLayout(wid_7_layout);

        QHBoxLayout *HBOX_7 = new QHBoxLayout(wid_7);
        HBOX_7->addWidget(box_7);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        QWidget *wid_8 = new QWidget(this);

        QLineEdit *wid_8_account_number = new QLineEdit(this);

        _confirm_button = new QPushButton("Confirm", this);
        _confirm_button->setStyleSheet("color: black;"
                                       "background-color: beige;");
        connect(_confirm_button, &QPushButton::clicked, this, &adm_main_window::delete_accounts);

        _back_button = new QPushButton("Previous Menu", this);
        connect(_back_button, &QPushButton::clicked, this, &adm_main_window::back_button_func);

        QLabel *message_8 = new QLabel("Are sure you want to delete the entered Account?", this);

        QFormLayout *wid_8_layout = new QFormLayout();
        wid_8_layout->addRow("DELETE ACCOUNT", wid_8_account_number);
        wid_8_layout->addWidget(_confirm_button);
        wid_8_layout->addWidget(_back_button);

        QGroupBox *box_8 = new QGroupBox();
        box_8->setLayout(wid_8_layout);

        QHBoxLayout *HBOX_8 = new QHBoxLayout(wid_8);
        HBOX_8->addWidget(box_8);

        /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

        _stack->addWidget(adm_option_widget);
        _stack->addWidget(wid_1);
        _stack->addWidget(wid_2);
        _stack->addWidget(wid_3);
        _stack->addWidget(wid_4);
        _stack->addWidget(wid_5);
        _stack->addWidget(wid_6);
        _stack->addWidget(wid_7);
        _stack->addWidget(wid_8);
}

void adm_main_window::back_button_func()
{
        int current_index = _stack->currentIndex();

        (current_index != 0) ? _stack->setCurrentIndex(0) : _previous_stack->setCurrentIndex(0);
}

void adm_main_window::wid_1_config()
{
        int account_number = _new_adm_account_number->text().toInt();
        std::string password = _new_adm_password->text().toStdString();
        std::string password_confirmation = _new_adm_password_confirmation->text().toStdString();

        if (password.compare(password_confirmation))
        {
                _new_adm_password_confirmation->setStyleSheet("border: 1px solid red");

                QMessageBox::warning(this, "Warning!", "Password Confirmation Incorrect, try again");

                return;
        }

        _new_adm_password_confirmation->setStyleSheet("border: 1px solid gray");

        std::string hash_password = BANK::hashing_password(password);

        BANK::create_adm(connection, account_number, hash_password);

        QMessageBox::information(this, "Password Confirmation Correct, Account created successfully", "Password Confirmation Correct, Account created successfully");

        _new_adm_account_number->clear();
        _new_adm_password->clear();
        _new_adm_password_confirmation->clear();

        hash_password.clear();
        password.clear();
        password_confirmation.clear();
}

void adm_main_window::display_accounts_table()
{
        BANK::Qt_display_accounts_table(connection);
}

void adm_main_window::display_specific_accounts_table()
{
        int account_number = wid_3_account_number->text().toInt();

        BANK::Qt_display_specific_accounts(connection, account_number);

        wid_3_account_number->clear();
}

void adm_main_window::display_people_in_debt()
{
        BANK::Qt_display_people_in_debt(connection);
}

void adm_main_window::display_specific_accounts_in_debt()
{
        int account_number = wid_5_account_number->text().toInt();

        BANK::Qt_display_specific_accounts_in_debt(connection, account_number);
}

void adm_main_window::display_transactions_history()
{
        int account_number = wid_6_account_number->text().toInt();

        Transactions::Qt_display_transactions_history(connection, account_number);

        wid_6_account_number->clear();
}

void adm_main_window::display_relative_transactions_history()
{
        int account_number = _specific_account_number_transac->text().toInt();
        QString date = _selected_date.toString(Qt::ISODate);
        int selected_choice = _choice->currentIndex();

        std::string hashed_password = BANK::Qt_retrieve_hashed_password(connection, account_number);
        if (hashed_password.empty())
        {
                _specific_account_number_transac->setStyleSheet("border: 1px solid red");

                return;
        }

        _specific_account_number_transac->setStyleSheet("border: 1px solid gray");

        Transactions::Qt_display_specific_transactions_history(connection, account_number, date.toStdString(), selected_choice);

        _specific_account_number_transac->clear();
}

void adm_main_window::delete_accounts()
{
        int account_number = wid_7_account_number->text().toInt();

        Account::Qt_remove_accounts(connection, account_number);

        wid_7_account_number->clear();
}