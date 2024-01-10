#include <iostream>
#include </usr/local/mysql/include/mysql.h>
#include <string>
#include <vector>
using namespace std;

//  g++ -std=c++11 -g -o main main.cpp -L/usr/local/mysql-8.2.0-macos13-arm64/lib -lmysqlclient -rpath /usr/local/mysql-8.2.0-macos13-arm64/lib

// TO do
// Some options are still needed to be coded
// Passoword Hahing to secure them
// Create appropriate triggers for the queries
// Finish the Bank Class which will be responsible to manage the create accounts
// check for memory leaks
// review the code and make it the simplest way it can be
// 

struct connection_details
{
    string server, user, password, database;
};

MYSQL *mysql_connection_setup(connection_details *mysql_ID)
{
    MYSQL *connection = mysql_init(NULL);
    if (!connection)
    {
        cerr << "Database Initialiation Error: " << mysql_error(connection) << endl;
        exit(1);
    }

    if (!mysql_real_connect(connection, mysql_ID->server.c_str(), mysql_ID->user.c_str(), mysql_ID->password.c_str(), mysql_ID->database.c_str(), 0, NULL, 0))
    {
        cerr << "Error Connection to the database: " << mysql_error(connection) << endl;
        exit(1);
    }

    return connection;
}

MYSQL_RES *mysql_perform_query(MYSQL *connection, const char *sql_query)
{
    if (mysql_query(connection, sql_query))
    {
        cerr << "Query Error: " << mysql_error(connection) << endl;
        exit(1);
    }

    return mysql_use_result(connection);
}

class Bank
{
    public :

    // on hold


};

class Account
{
    public:

    string first_name, last_name, date_birth, email, password, national_ID, address;
    int phone_number, account_number;

    void create_account(int account_number, string first_name, string last_name, string result, string date_birth, string email, string password, string national_ID, string address, int phone_number, MYSQL *connection);

    void check_balance(const Account &obj, MYSQL *connection);

    void remove_accounts(const Account &obj, MYSQL *connection);
};

void Account :: create_account(int account_number, string first_name, string last_name, string results, string date_birth, string email, string password, string national_ID, string address, int phone_number, MYSQL *connection)
{
    string account_number_str = to_string(account_number);

    string create_account = " CREATE TABLE ' "+account_number_str+" ' (account_number INT PRIMARY KEY, first_name VARCHAR(100), last_name VARCHAR(100), balance DECIMAL(100, 6) DEFAULT 100, transactions_history VARCHAR(100), transactions_date DATETIME DEFAULT NOW()  ); ";

    MYSQL_RES *result = mysql_perform_query(connection, create_account.c_str());

    string insert = "INSERT INTO ' "+account_number_str+" ' (account_number, first_name, last_name) VALUES ( ' "+account_number_str+" ', ' "+first_name +" ', ' "+last_name+" ',  ); ";

    result = mysql_perform_query(connection, insert.c_str());

    cout << "This is Your Account Number, remember it cause you will need it to gain access to everything you want to do in the future:  ";
    // cout << ;

    create_account = "SELECT account_number WHERE national_ID = ' "+national_ID+" '; ";

    result = mysql_perform_query(connection, create_account.c_str());
    if (result)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        cout << row;
    }
}

void Account :: check_balance(const Account &obj, MYSQL *connection)
{
    string account_number_str = to_string(obj.account_number);

    string check_balance1 = "SELECT balance FROM ' "+account_number_str+" '; ";

    MYSQL_RES *result = mysql_perform_query(connection, check_balance1.c_str());
    if (result)
    {
        MYSQL_ROW row = mysql_fetch_row(result);

        cout << "Your Current Balance is: ";

        cout << row[mysql_num_fields(result) - 1];

        mysql_free_result(result);
    }
}

void Account :: remove_accounts(const Account &obj, MYSQL *connection)
{
    string account_number_str = to_string(obj.account_number);

    string remove_account0 = " DROP TABLE ' "+account_number_str+" '; ";
}

class Transactions : public Account
{

    public:

    void log_transactions();

    void retrieve_transaction_history(const Account &obj, MYSQL *connection);

    void money_withdrawal(const Account &obj, MYSQL *connection, int sum_to_withdraw);

    void transfer_money(const Account &obj1, const Account &obj2, MYSQL *connection, int amount_to_send);
};

void Transactions :: money_withdrawal(const Account &obj, MYSQL *connection, int sum_to_withdraw)
{
    MYSQL_RES *result;
    string account_number_str = to_string(obj.account_number);
    string sum_to_withdraw_str = to_string(sum_to_withdraw);

    string money_withdrawal0 = "UPDATE TABLE ' "+account_number_str+" ' SET balance = balance - ' "+sum_to_withdraw_str+" ';";
    result = mysql_perform_query(connection, money_withdrawal0.c_str());

    cout << "You have withdrawed " << sum_to_withdraw << " dollars and your new Balance is: ";

    string new_balance = "SELECT balance FROM ' "+account_number_str+" ';";
    result = mysql_perform_query(connection, new_balance.c_str());
    if (result)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        cout << row << endl;

        mysql_free_result(result);
    }
}

void Transactions :: retrieve_transaction_history(const Account &obj, MYSQL *connection)
{
    MYSQL_RES *result;
    string account_number_str = to_string(obj.account_number);

    string retrieve_transaction_history0 = "SELECT transactions_history FROM ' "+account_number_str+" ';";
    
    result = mysql_perform_query(connection, retrieve_transaction_history0.c_str());
    if (result)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        cout << row << endl;

        mysql_free_result(result);
    }
}

void Transactions :: transfer_money(const Account &obj1, const Account &obj2, MYSQL *connection, int amount_to_send)
{
    MYSQL_RES *result;
    string account_number1_str = to_string(obj1.account_number);
    string account_number2_str = to_string(obj2.account_number);
    string amount_to_send_str = to_string(amount_to_send);

    string transfer0 = "UPTADE TABLE ' "+account_number1_str+" ' SET balance = balance - ' "+amount_to_send_str+" ';";
    result = mysql_perform_query(connection, transfer0.c_str());

    string transfer1 = "UPTADE TABLE ' "+account_number2_str+" ' SET balance = balance + ' "+amount_to_send_str+" ';";
    result = mysql_perform_query(connection, transfer1.c_str());

    cout << "You have send " << amount_to_send << " dollars and your new Balance is: ";

    string new_balance = "SELECT balance FROM ' "+account_number1_str+" ';";
    result = mysql_perform_query(connection, new_balance.c_str());
    if (result)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        cout << row << endl;

        mysql_free_result(result);
    }
}


int main(void)
{
    MYSQL *connection;
    MYSQL_RES *result;

    connection_details mysql_ID;
    mysql_ID.server = "localhost";
    mysql_ID.user = "root";
    mysql_ID.password = "sleyHortes1312";
    mysql_ID.database = "bankingSystemDatabase";

    connection = mysql_connection_setup(&mysql_ID);

    cout << "---------- ********** ---------- ********** ---------- ********** WELCOME TO THE CROSS-CONTINENTAL TREASURY BANK ********** ---------- ********** ---------- ********** ----------" << endl;

    cout << endl;
    cout << endl;
    cout << endl;

    cout << "                                                 *********** Among the List below, choose what best suits your Status **********" << endl;
    cout << endl;

    cout << "1. You are New to our Bank and Would like to Create an Account" << endl;

    cout << "2. You already possess an Account and would like to process some inquiries relative to it" << endl;

    cout << "3. Information on our Bank" << endl;
    
    cout << endl;

    int options, options2, options3, options4, options5;
   

    string first_name, last_name, date_of_birth, email, password, password_confirmation;
    int national_ID, phone_number, account_number;

    string create_account, transfer, check_balance, account_history, money_withdrawal;

    vector <Account> accounts;
    accounts.reserve(1000);

    cin >> options;
    switch(options)
    {
        case 1:

            cout << "Please Provide Us with the following Information in order to create your account, Make sure that you enter the correct Information: " << endl;
            cout << endl;

            cout << "First Name: ";
            cin >> first_name;

            cout << "Last Name: ";
            cin >> last_name;

            cout << "Date of Birth: ";
            cin >> date_of_birth;

            cout << "National ID: ";
            cin >> national_ID;

            cout << "Email: ";
            cin >> email;

            cout << "Phone Number: ";
            cin >> phone_number;

            cout << "Password: ";
            cin >> password;

            first_name.append(last_name);

            do
            {
                cout << "Passord Confirmation: ";
                cin >> password_confirmation;

            } while (password.compare(password_confirmation));
            
            // accounts[1].create_account(1000, first_name, last_name, results, date_birth, email, password, national_ID, address, phone_number, connection); this is commented because I don't want to execute the query yet

            // there will be much more account vectors to hold the created ones;

        break;

        case 2:

        cout << "Choose among the options below, what best suits your requirements" << endl;
        cout << endl;

        cout << "1. Checking your Balance" << endl;

        cout << "2. Deposit" << endl;

        cout << "3. Money Withdrawal " << endl;

        cout << "4. Money Transfer" << endl;

        cout << "5. Edit Account Information" << endl;

        
        cin >> options2;
        switch(options2)
        {
            case 1:
                cout << "What is yout Account Number: " << endl;
                cin >> account_number;

                cout << "What is your Password: " << endl;
                cin >> password;

                // on hold


            break;

            case 2:
            // on hold

            break;

            case 3:
            // on hold

            break;

            case 4:
            // on hold

            break;

            case 5:

            cout << "Choose among the options below, what best suits your requirements" << endl;
            cout << endl;

            cout << "1. Edit Personal Information" << endl;

            cout << "2. Change Password" << endl;

            cin >> options3;
            switch(options3)
            {
                case 1:
                cout << "Choose among the options below, what best suits your requirements: PS: The Date of Birth and National ID number can't be edited" << endl;
                cout << endl;

                cout << "1. Edit Name" << endl;

                cout << "2. Edit email" << endl;

                cout << "3. Edit Phone Number" << endl;

                cin >> options4;
                switch(options4)
                {
                    case 1:
                    // on hold

                    break;

                    case 2:
                    // on hold

                    break;

                    case 3:
                    // on hold
                    break;
                }

                break;

                case 2:
                    // on hold

                break;
            }
            break;

        }


        break;

        case 3:
            cout <<  "Title The CROSS_CONTINENTAL TREASURY BANK: A Compendium of Financial Excellence"<< endl;
    
    cout << "CHAPTER 1: INTRODUCTION TO THE CROSS-CONTINENTAL TREASURY BANK"<< endl;
    cout << "SECTION 1.1: Our Vision and Mission"<< endl;
    cout << endl;
    cout << "The CROSS-CONTINENTAL TREASURY BANK envisions a world where financial services transcend borders, empower individuals, and foster economic growth."<< endl;
    cout << "Our mission is to provide cutting-edge banking solutions with a commitment to integrity, innovation, and client satisfaction."<< endl;
    cout << "SECTION 1.2: Core Values"<< endl;
    cout << endl;
    cout << "Integrity: Upholding the highest ethical standards in all interactions."<< endl;
    cout << "Innovation: Pioneering financial solutions that adapt to the dynamic global landscape."<< endl;
    cout << "Client Satisfaction: Placing our clients at the center and ensuring their success is our success."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 2: THE PILLARS OF FINANCIAL SECURITY" << endl;
    cout << endl;
    cout << "SECTION 2.1: Robust Security Protocols"<< endl;
    cout << endl;
    cout << "Implementing state-of-the-art encryption and cybersecurity measures to safeguard client information."<< endl;
    cout << "Regular audits and assessments to fortify our defenses against emerging threats."<< endl;
    cout << "SECTION 2.2: Compliance and Regulatory Adherence"<< endl;
    cout << endl;
    cout << "Strict adherence to international banking regulations and compliance standards."<< endl;
    cout << "Continuous training and development for staff to stay abreast of regulatory changes."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 3: SERVICES TAILORED TO YOUR AMBITIONS"<< endl;
    cout << endl;
    cout << "SECTION 3.1: Personalized Banking Solutions"<< endl;
    cout << endl;
    cout << "Tailoring financial services to meet the unique needs and aspirations of our diverse clientele."<< endl;
    cout << "Offering a wide array of accounts, investment options, and lending services."<< endl;
    cout << "SECTION 3.2: Global Accessibility"<< endl;
    cout << endl;
    cout << "A network of branches and ATMs spanning continents, ensuring clients can access their funds whenever and wherever they need."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 4: EMPOWERING FINANCIAL LITERACY"<< endl;
    cout << endl;
    cout << "SECTION 4.1: Educational Initiatives"<< endl;
    cout << endl;
    cout << "The CROSS-CONTINENTAL TREASURY BANK commits to promoting financial literacy through workshops, seminars, and accessible online resources."<< endl;
    cout << "Encouraging informed decision-making and responsible financial behavior."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 5: ENVIRONMENTAL AND SOCIAL RESPONSABILITY"<< endl;
    cout << endl;
    cout << "SECTION 5.1: Sustainable Banking Practices"<< endl;
    cout << endl;
    cout << "Integrating environmental and social responsibility into our business model."<< endl;
    cout << "Investing in eco-friendly initiatives and supporting community development projects."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 6: THE FUTURE OF BANKING"<< endl;
    cout << endl;
    cout << "SECTION 6.1: Technological Advancements"<< endl;
    cout << endl;
    cout << "Embracing cutting-edge technology to enhance efficiency, security, and user experience."<< endl;
    cout << "Exploring blockchain, artificial intelligence, and other innovations to shape the future of banking."<< endl;
    cout << "SECTION 6.2: Continuous Improvement"<< endl;
    cout << endl;
    cout << "A commitment to continuous improvement through client feedback, market analysis, and strategic partnerships."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 7: CLIENT-CENTRIC APPROACH"<< endl;
    cout << endl;
    cout << "SECTION 7.1: Personalized Customer Service"<< endl;
    cout << endl;
    cout << "A dedicated customer support team ensuring a seamless and delightful banking experience."<< endl;
    cout << "Proactive resolution of issues and a commitment to exceeding client expectations."<< endl;
cout << endl;
cout << endl;
cout << endl;

    cout << "CHAPTER 8: THANKS FOR CHOOSING OUR BANK"<< endl;
    cout <<  endl;
    cout << "In the ever-evolving world of finance, The CROSS-CONTINENTAL TREASURY BANK stands as a beacon of reliability, innovation, and commitment." << endl;
    cout << " This compendium serves as a testament to our dedication to providing unparalleled financial services on a global scale. " << endl;
    cout << " Join us in the pursuit of financial excellence, where your aspirations find a home in The CROSS-CONTINENTAL TREASURY BANK." << endl; 
    break;
    }
    
    

}


