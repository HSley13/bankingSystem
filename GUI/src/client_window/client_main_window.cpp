#include "client_main_window.h"
#include <option_main_window.h>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

client_main_window::client_main_window(sql::Connection *db_connection, QWidget *parent)
    : QMainWindow(parent), connection(db_connection)
{
    window_stack = new QStackedWidget();
    window_stack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCentralWidget(window_stack);
    setStyleSheet("color: beige;"
                  "font-family: Arial Black;"
                  "font-size: 20;"
                  "font: bold italic 14px;"
                  "background-color: black;");
    setWindowTitle("Client");
    resize(500, 500);

    QWidget *central_widget = new QWidget(this);

    QPushButton *create_account = new QPushButton("1. New to our Bank and Would like to Create an Account", this);
    connect(create_account, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(1); });

    QPushButton *account_inquiry = new QPushButton("2. Already possess an Account and would like to process some inquiries relative to it", this);
    connect(account_inquiry, &QPushButton::clicked, this, &client_main_window::account_inquiry_func);

    QPushButton *bank_info = new QPushButton("3. Information on our Bank", this);
    connect(bank_info, &QPushButton::clicked, this, [=]()
            { window_stack->setCurrentIndex(2); });

    QLabel *image_label = new QLabel(this);
    QPixmap image("/Users/test/Documents/banking_system/GUI/src/ressources/client.jpeg");
    image_label->setPixmap(image.scaled(600, 600, Qt::KeepAspectRatio));
    image_label->setScaledContents(true);

    vbox = new QVBoxLayout(central_widget);
    vbox->addWidget(image_label, 2, Qt::AlignCenter);
    vbox->addWidget(create_account, Qt::AlignCenter);
    vbox->addWidget(account_inquiry, Qt::AlignCenter);
    vbox->addWidget(bank_info, Qt::AlignCenter);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

    QWidget *create_account_widget = new QWidget();
    create_account_widget->setWindowTitle("Create Account");

    QLabel *message = new QLabel("Please Provide Us with the following Information in order to create your account. Make sure You enter the Correct Information", this);

    QLabel *national_ID = new QLabel("National ID with at least a letter within it: ", this);
    insert_national_ID = new QLineEdit(this);
    QHBoxLayout *hbox1 = new QHBoxLayout();
    hbox1->addWidget(national_ID, Qt::AlignCenter);
    hbox1->addWidget(insert_national_ID, Qt::AlignCenter);

    QLabel *first_name = new QLabel("First Name: ", this);
    insert_first_name = new QLineEdit(this);
    QHBoxLayout *hbox2 = new QHBoxLayout();
    hbox2->addWidget(first_name, Qt::AlignCenter);
    hbox2->addWidget(insert_first_name, Qt::AlignCenter);

    QLabel *last_name = new QLabel("Last Name: ", this);
    insert_last_name = new QLineEdit(this);
    QHBoxLayout *hbox3 = new QHBoxLayout();
    hbox3->addWidget(last_name, Qt::AlignCenter);
    hbox3->addWidget(insert_last_name, Qt::AlignCenter);

    QLabel *date_birth = new QLabel("Date of Birth: ", this);
    calendar = new QDateEdit(this);
    calendar->setCalendarPopup(true);
    calendar->setDate(QDate::currentDate());
    connect(calendar, &QDateEdit::dateChanged, this, [=]()
            { selected_date = calendar->date(); });

    QHBoxLayout *hbox4 = new QHBoxLayout();
    hbox4->addWidget(date_birth, Qt::AlignCenter);
    hbox4->addWidget(calendar, Qt::AlignCenter);

    QLabel *phone_number = new QLabel("Phone Number: ", this);
    insert_phone_number = new QLineEdit(this);
    QHBoxLayout *hbox5 = new QHBoxLayout();
    hbox5->addWidget(phone_number, Qt::AlignCenter);
    hbox5->addWidget(insert_phone_number, Qt::AlignCenter);

    QLabel *email = new QLabel("Email: ", this);
    insert_email = new QLineEdit(this);
    QHBoxLayout *hbox6 = new QHBoxLayout();
    hbox6->addWidget(email, Qt::AlignCenter);
    hbox6->addWidget(insert_email, Qt::AlignCenter);

    QLabel *address = new QLabel("Address ( Taiwan-Taipei_City-Datong_District-Zhongshan_Road-001 ): ", this);
    insert_address = new QLineEdit(this);
    QHBoxLayout *hbox7 = new QHBoxLayout();
    hbox7->addWidget(address, Qt::AlignCenter);
    hbox7->addWidget(insert_address, Qt::AlignCenter);

    QString info_text = "Your Account should have at least $100 when creating it, so please enter those $100 and not less.\n\n"
                        "Interest Rate Scale according to your First Deposit, which can't be changed.\n\n"
                        "1. Balance = $100 ---> Interest Rate = 0%\n"
                        "2. $500 > Balance > $100 ---> Interest Rate = 2%\n"
                        "3. $1000 > Balance >= $500 ---> Interest Rate = 5%\n"
                        "4. Balance > $1000 ---> Interest Rate = 7%";

    QTextEdit *interest_rate_info = new QTextEdit(this);
    interest_rate_info->setPlainText(info_text);
    interest_rate_info->setReadOnly(true);

    insert_balance = new QLineEdit(this);

    QHBoxLayout *hbox8 = new QHBoxLayout();
    hbox8->addWidget(interest_rate_info, Qt::AlignCenter);
    hbox8->addWidget(insert_balance, Qt::AlignCenter);
    QGroupBox *grp_balance = new QGroupBox();
    grp_balance->setLayout(hbox8);

    QLabel *password = new QLabel("Enter Password: ", this);
    insert_password = new QLineEdit(this);
    QHBoxLayout *hbox9 = new QHBoxLayout();
    hbox9->addWidget(password, Qt::AlignCenter);
    hbox9->addWidget(insert_password, Qt::AlignCenter);

    QLabel *password_confirmation = new QLabel("Enter Password Confirmation: ", this);
    insert_password_confirmation = new QLineEdit(this);
    insert_password_confirmation->setEchoMode(QLineEdit::Password);
    QHBoxLayout *hbox10 = new QHBoxLayout();
    hbox10->addWidget(password_confirmation, Qt::AlignCenter);
    hbox10->addWidget(insert_password_confirmation, Qt::AlignCenter);

    QLabel *question = new QLabel("Ask Yourself question which is gonna be used for Password Recovery", this);
    insert_question = new QLineEdit(this);
    QHBoxLayout *hbox11 = new QHBoxLayout();
    hbox11->addWidget(question, Qt::AlignCenter);
    hbox11->addWidget(insert_question, Qt::AlignCenter);

    QLabel *answer = new QLabel("Enter the Answer", this);
    insert_answer = new QLineEdit(this);
    QHBoxLayout *hbox12 = new QHBoxLayout();
    hbox12->addWidget(answer, Qt::AlignCenter);
    hbox12->addWidget(insert_answer, Qt::AlignCenter);

    QLabel *confirm_answer = new QLabel("Confirm the Answer", this);
    insert_confirm_answer = new QLineEdit(this);
    QHBoxLayout *hbox13 = new QHBoxLayout();
    hbox13->addWidget(confirm_answer, Qt::AlignCenter);
    hbox13->addWidget(insert_confirm_answer, Qt::AlignCenter);

    confirm_button = new QPushButton("Confirm", this);
    confirm_button->setStyleSheet("color: black;"
                                  "background-color: beige;");
    connect(confirm_button, &QPushButton::clicked, this, &client_main_window::confirm_button_func);

    back_button = new QPushButton("Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &client_main_window::back_button_func);

    QVBoxLayout *VBOX = new QVBoxLayout(create_account_widget);
    VBOX->addWidget(message, Qt::AlignCenter);
    VBOX->addLayout(hbox1, Qt::AlignCenter);
    VBOX->addLayout(hbox2, Qt::AlignCenter);
    VBOX->addLayout(hbox3, Qt::AlignCenter);
    VBOX->addLayout(hbox4, Qt::AlignCenter);
    VBOX->addLayout(hbox5, Qt::AlignCenter);
    VBOX->addLayout(hbox6, Qt::AlignCenter);
    VBOX->addLayout(hbox7, Qt::AlignCenter);
    VBOX->addWidget(grp_balance, Qt::AlignCenter);
    VBOX->addLayout(hbox9, Qt::AlignCenter);
    VBOX->addLayout(hbox10, Qt::AlignCenter);
    VBOX->addLayout(hbox11, Qt::AlignCenter);
    VBOX->addLayout(hbox12, Qt::AlignCenter);
    VBOX->addLayout(hbox13, Qt::AlignCenter);
    VBOX->addWidget(confirm_button, Qt::AlignCenter);
    VBOX->addWidget(back_button, Qt::AlignCenter);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

    QWidget *bank_info_widget = new QWidget();
    bank_info_widget->setWindowTitle("Bank Info");

    QTextEdit *information = new QTextEdit(this);
    information->setPlainText(
        "Title: The CROSS_CONTINENTAL TREASURY BANK: A Compendium of Financial Excellence\n\n"
        "CHAPTER 1: INTRODUCTION TO THE CROSS-CONTINENTAL TREASURY BANK\n"
        "SECTION 1.1: Our Vision and Mission\n"
        "\n"
        "The CROSS-CONTINENTAL TREASURY BANK envisions a world where financial services transcend borders, empower individuals, and foster economic growth.\n"
        "Our mission is to provide cutting-edge banking solutions with a commitment to integrity, innovation, and client satisfaction.\n"
        "SECTION 1.2: Core Values\n"
        "\n"
        "Integrity: Upholding the highest ethical standards in all interactions.\n"
        "Innovation: Pioneering financial solutions that adapt to the dynamic global landscape.\n"
        "Client Satisfaction: Placing our clients at the center and ensuring their success is our success.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 2: THE PILLARS OF FINANCIAL SECURITY\n"
        "\n"
        "SECTION 2.1: Robust Security Protocols\n"
        "\n"
        "Implementing state-of-the-art encryption and cybersecurity measures to safeguard client information.\n"
        "Regular audits and assessments to fortify our defenses against emerging threats.\n"
        "SECTION 2.2: Compliance and Regulatory Adherence\n"
        "\n"
        "Strict adherence to international banking regulations and compliance standards.\n"
        "Continuous training and development for staff to stay abreast of regulatory changes.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 3: SERVICES TAILORED TO YOUR AMBITIONS\n"
        "\n"
        "SECTION 3.1: Personalized Banking Solutions\n"
        "\n"
        "Tailoring financial services to meet the unique needs and aspirations of our diverse clientele.\n"
        "Offering a wide array of accounts, investment options, and lending services.\n"
        "SECTION 3.2: Global Accessibility\n"
        "\n"
        "A network of branches and ATMs spanning continents, ensuring clients can access their funds whenever and wherever they need.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 4: EMPOWERING FINANCIAL LITERACY\n"
        "\n"
        "SECTION 4.1: Educational Initiatives\n"
        "\n"
        "The CROSS-CONTINENTAL TREASURY BANK commits to promoting financial literacy through workshops, seminars, and accessible online resources.\n"
        "Encouraging informed decision-making and responsible financial behavior.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 5: ENVIRONMENTAL AND SOCIAL RESPONSIBILITY\n"
        "\n"
        "SECTION 5.1: Sustainable Banking Practices\n"
        "\n"
        "Integrating environmental and social responsibility into our business model.\n"
        "Investing in eco-friendly initiatives and supporting community development projects.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 6: THE FUTURE OF BANKING\n"
        "\n"
        "SECTION 6.1: Technological Advancements\n"
        "\n"
        "Embracing cutting-edge technology to enhance efficiency, security, and user experience.\n"
        "Exploring blockchain, artificial intelligence, and other innovations to shape the future of banking.\n"
        "SECTION 6.2: Continuous Improvement\n"
        "\n"
        "A commitment to continuous improvement through client feedback, market analysis, and strategic partnerships.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 7: CLIENT-CENTRIC APPROACH\n"
        "\n"
        "SECTION 7.1: Personalized Customer Service\n"
        "\n"
        "A dedicated customer support team ensuring a seamless and delightful banking experience.\n"
        "Proactive resolution of issues and a commitment to exceeding client expectations.\n"
        "\n"
        "\n"
        "\n"
        "CHAPTER 8: THANKS FOR CHOOSING OUR BANK\n"
        "\n"
        "In the ever-evolving world of finance, The CROSS-CONTINENTAL TREASURY BANK stands as a beacon of reliability, innovation, and commitment.\n"
        " This compendium serves as a testament to our dedication to providing unparalleled financial services on a global scale. \n"
        " Join us in the pursuit of financial excellence, where your aspirations find a home in The CROSS-CONTINENTAL TREASURY BANK.\n");

    information->setReadOnly(true);
    information->setLineWrapMode(QTextEdit::WidgetWidth);
    information->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    information->setStyleSheet("color: white;");

    back_button = new QPushButton("Previous Menu", this);
    connect(back_button, &QPushButton::clicked, this, &client_main_window::back_button_func);

    vbox = new QVBoxLayout(bank_info_widget);
    vbox->addWidget(information, Qt::AlignCenter);
    vbox->addWidget(back_button, Qt::AlignCenter);
    vbox->setAlignment(Qt::AlignCenter);

    /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

    window_stack->addWidget(central_widget);
    window_stack->addWidget(create_account_widget);
    window_stack->addWidget(bank_info_widget);
}

void client_main_window::back_button_func()
{
    int current_index = window_stack->currentIndex();

    if (current_index)
        window_stack->setCurrentIndex(0);
}

void client_main_window::confirm_button_func()
{
    std::string national_ID1 = insert_national_ID->text().toStdString();
    std::string first_name1 = insert_first_name->text().toStdString();
    std::string last_name1 = insert_last_name->text().toStdString();
    int phone_number1 = insert_phone_number->text().toInt();
    QString date_birth1 = selected_date.toString(Qt::ISODate);
    std::string email1 = insert_email->text().toStdString();
    std::string address1 = insert_address->text().toStdString();
    double balance = insert_balance->text().toDouble();
    std::string question = insert_question->text().toStdString();
    std::string answer = insert_answer->text().toStdString();
    std::string confirm_answer = insert_confirm_answer->text().toStdString();
    int account_number;

    std::string password1 = insert_password->text().toStdString();
    std::string password_confirmation1 = insert_password_confirmation->text().toStdString();

    if (balance < 100 || password1 != password_confirmation1)
    {
        if (balance < 100)
        {
            insert_balance->setStyleSheet("border: 1px solid red");
            QMessageBox::warning(this, "Balance XXX", "Insufficient Balance, Enter an amount >= 100");

            return;
        }
        else
        {
            insert_password_confirmation->setStyleSheet("border: 1px solid red");
            QMessageBox::warning(this, "Password XXX", "Password Confirmation Incorrect, check and try again");

            return;
        }
    }

    insert_balance->setStyleSheet("border: 1px solid gray");
    insert_password_confirmation->setStyleSheet("border: 1px solid gray");

    double interest_rate = 0;
    if (balance > 100 && balance < 500)
        interest_rate = 0.02;
    else if (balance < 1000 && balance >= 500)
        interest_rate = 0.05;
    else if (balance >= 1000)
        interest_rate = 0.07;

    if (question.empty())
    {
        QMessageBox::warning(this, "void!!!", "Question left empty!!!");
        insert_question->setStyleSheet("border: 1px solid red;");

        return;
    }

    insert_question->setStyleSheet("border: 1px solid gray;");

    if (answer.empty())
    {
        QMessageBox::warning(this, "void!!!", "Answer left empty!!!");
        insert_answer->setStyleSheet("border: 1px solid red;");

        return;
    }

    insert_answer->setStyleSheet("border: 1px solid gray;");

    if (answer.compare(confirm_answer))
    {
        QMessageBox::warning(this, "Confirmation Incorrect!!", "The Answer's Confirmation is Incorrect!!");
        insert_confirm_answer->setStyleSheet("border: 1px solid red;");

        return;
    }

    insert_confirm_answer->setStyleSheet("border: 1px solid gray;");

    bool OK;

    QString info = "National ID: " + QString::fromStdString(national_ID1) + "\n"
                                                                            "First Name: " +
                   QString::fromStdString(first_name1) + "\n"
                                                         "Last Name: " +
                   QString::fromStdString(last_name1) + "\n"
                                                        "Date of Birth: " +
                   date_birth1 + "\n"
                                 "Phone Number: " +
                   QString::number(phone_number1) + "\n"
                                                    "Email: " +
                   QString::fromStdString(email1) + "\n"
                                                    "Address: " +
                   QString::fromStdString(address1) + "\n"
                                                      "Balance: " +
                   QString::number(balance) + "\n"
                                              "Question: " +
                   QString::fromStdString(question) + "\n"
                                                      "Answer: " +
                   QString::fromStdString(answer) + "\n\n"
                                                    "Type YES to confirm and NO to cancel";

    QString input = QInputDialog::getText(this, "Confirmation", info, QLineEdit::Normal, "", &OK);

    if (input.isEmpty())
    {
        QMessageBox::warning(this, "void", "Input Empty");

        return;
    }

    if (OK && !input.isEmpty())
    {
        if (input != "YES")
        {
            QMessageBox::warning(this, "Unsufficient amount", "CANCELLING...");

            return;
        }
    }

    std::string hash_password = BANK::hashing_password(password1);

    Account::Qt_create_account(connection, account_number, national_ID1, first_name1, last_name1, date_birth1.toStdString(), phone_number1, email1, address1, balance, interest_rate, hash_password, question, answer);

    national_ID1.clear();
    last_name1.clear();
    first_name1.clear();
    email1.clear();
    address1.clear();
    password1.clear();
    password_confirmation1.clear();
    question.clear();
    answer.clear();
    confirm_answer.clear();

    insert_national_ID->clear();
    insert_last_name->clear();
    insert_first_name->clear();
    insert_email->clear();
    insert_address->clear();
    insert_phone_number->clear();
    insert_balance->clear();
    insert_password->clear();
    insert_password_confirmation->clear();
    insert_question->clear();
    insert_answer->clear();
    insert_confirm_answer->clear();
}

void client_main_window::account_inquiry_func()
{
    QMessageBox::information(this, "Redirecting...", "You are about to be redirected to the Client's Official Page");

    option_main_window *new_window = new option_main_window(connection);

    new_window->show();
}
