#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <vector>

#include </usr/local/mysql-connector-c++-8.2.0/include/jdbc/mysql_driver.h>
#include </usr/local/mysql-connector-c++-8.2.0/include/jdbc/mysql_connection.h>
#include </usr/local/mysql-connector-c++-8.2.0/include/jdbc/cppconn/prepared_statement.h>
#include </opt/homebrew/include/argon2.h>

using namespace std;

// g++ -std=c++11 -o main main.cpp -L/usr/local/mysql-connector-c++-8.2.0/lib64 -lmysqlcppconn -lssl -lcrypto -rpath /usr/local/mysql-connector-c++-8.2.0/lib64  -L/opt/homebrew/lib -largon2 -rpath /opt/homebrew/lib

// BELOW ARE ALL THE REQUIREMENTS AND PREPARATIONS FOR THE DATABSE IN ORDER TO USE THIS PROJECT'S CODE. MAKE SURE THAT YOU HAVE MYSQL INSTALLED ON YOUR PC. YOU CAN USE EITHER VSCODE OR MYSQL WORKBENCH TO RUN THESE FOLLOWING QUERIES

/*
        *************** CREATE accounts TABLE *************** 
        CREATE TABLE accounts 
        (
            account_number INT PRIMARY KEY AUTO_INCREMENT,
            national_ID VARCHAR(255) NOT NULL UNIQUE,
            first_name VARCHAR(255),
            last_name VARCHAR(1000),
            date_birth VARCHAR(255),
            phone_number INT,
            email VARCHAR(255),
            address VARCHAR(255),
            balance DECIMAL(20,5),
            interest_rate DECIMAL(5,2),
            initial_timestamp DATETIME DEFAULT NOW()
        )AUTO_INCREMENT = 1000000000;


       *************** CREATE transactions TABLE *************** 
        CREATE TABLE transactions
        (
            account_number INT PRIMARY KEY AUTO_INCREMENT,
            deposit DECIMAL(20,5) DEFAULT 0,
            withdrawal DECIMAL(20,5) DEFAULT 0,
            transfer DECIMAL(20,5) DEFAULT 0,
            receive DECIMAL(20,5) DEFAULT 0
        )AUTO_INCREMENT = 1000000000;


       *************** CREATE hashed_password TABLE *************** 
        CREATE TABLE hashed_password
        (
            account_number INT PRIMARY KEY,
            hashed_password VARBINARY(500)
        );


        *************** ALL THE TRIGGERS  ***************

        ------- insert account
        CREATE TRIGGER insert_account AFTER INSERT ON accounts FOR EACH ROW 
            BEGIN
                INSERT INTO transactions (deposit, withdrawal, transfer, receive) 
                VALUES (0, 0, 0, 0);
            END;

        ------- new_deposit
        CREATE TRIGGER new_deposit AFTER UPDATE ON transactions FOR EACH ROW
            BEGIN
                IF OLD.deposit <> NEW.deposit THEN
                    UPDATE accounts SET balance = balance + NEW.deposit, initial_timestamp = NOW()
                    WHERE account_number = NEW.account_number;
                END IF;
            END;

        ------- new_withdrawal
        CREATE TRIGGER new_withdrawal AFTER UPDATE ON transactions FOR EACH ROW
            BEGIN
                IF OLD.withdrawal <> NEW.withdrawal THEN
                    UPDATE accounts SET balance = balance - NEW.withdrawal, initial_timestamp = NOW()
                    WHERE account_number = NEW.account_number;
                END IF;
            END;

        ------- new_transfer
        CREATE TRIGGER new_transfer AFTER UPDATE ON transactions FOR EACH ROW
            BEGIN
                IF OLD.transfer <> NEW.transfer THEN
                    UPDATE accounts SET balance = balance - NEW.transfer, initial_timestamp = NOW()
                    WHERE account_number = NEW.account_number;
                END IF;
            END;

        ------- new_receive
        CREATE TRIGGER new_receive AFTER UPDATE ON transactions FOR EACH ROW
            BEGIN
                IF OLD.receive <> NEW.receive THEN
                    UPDATE accounts SET balance = balance + NEW.receive, initial_timestamp = NOW()
                    WHERE account_number = NEW.account_number;
                END IF;
            END;


        *************** ALL THE PROCEDURES *************** 
        ------- update_and_log_name
        CREATE PROCEDURE update_and_log_name (IN account_number1 INT, IN new_first_name VARCHAR(255))
            BEGIN
                DECLARE old_first_name VARCHAR(255);

                SELECT first_name INTO old_first_name FROM accounts WHERE account_number = account_number1;

                IF old_first_name <> new_first_name THEN
                    UPDATE accounts SET first_name = new_first_name
                    WHERE account_number = account_number1;

                    SET @table_name = CONCAT('NO', account_number1);
                    SET @transaction_details = CONCAT('Name changed to ', new_first_name);
                    SET @sql_statement = CONCAT('INSERT INTO ', @table_name, ' VALUES (?, NOW());');

                    PREPARE stmt FROM @sql_statement;
                    EXECUTE stmt USING @transaction_details;
                    DEALLOCATE PREPARE stmt;
                END IF;
            END;

        ------- update_and_log_email
        CREATE PROCEDURE update_and_log_email (IN account_number1 INT, IN new_email VARCHAR(255))
            BEGIN
                DECLARE old_email VARCHAR(255);

                SELECT email INTO old_email FROM accounts WHERE account_number = account_number1;

                IF old_email <> new_email THEN
                    UPDATE accounts SET email = new_email
                    WHERE account_number = account_number1;

                    SET @table_name = CONCAT('NO', account_number1);
                    SET @transaction_details = CONCAT('Email changed to ', new_email);
                    SET @sql_statement = CONCAT('INSERT INTO ', @table_name, ' VALUES (?, NOW());');

                    PREPARE stmt FROM @sql_statement;
                    EXECUTE stmt USING @transaction_details;
                    DEALLOCATE PREPARE stmt;
                END IF;
            END;

        ------- update_and_log_address 
        CREATE PROCEDURE update_and_log_address (IN account_number1, IN new_address VARCHAR(255))
            BEGIN
                DECLARE old_address VARCHAR(255);

                SELECT address INTO old_address FROM accounts WHERE account_number = account_number1;

                IF old_address <> new_address THEN
                    UPDATE accounts SET address = new_address
                    WHERE account_number = account_number1;

                    SET @table_name = CONCAT('NO', account_number1);
                    SET @transaction_details = CONCAT('Address changed to ', new_address);
                    SET @sql_statement = CONCAT('INSERT INTO ', @table_name, ' VALUES (?, NOW());');

                    PREPARE stmt FROM @sql_statement;
                    EXECUTE stmt USING @transaction_details;
                    DEALLOCATE PREPARE stmt;
                END IF;
            END;

        ------- update_and_log_phone_number
        CREATE PROCEDURE update_and_log_phone_number (IN account_number1, IN new_phone_number VARCHAR(255))
            BEGIN
                DECLARE old_phone_number INT;

                SELECT phone_number INTO old_phone_number FROM accounts WHERE account_number = account_number1;

                IF old_phone_number <> new_phone_number THEN
                    UPDATE accounts SET phone_number = new_phone_number
                    WHERE account_number = account_number1;

                    SET @table_name = CONCAT('NO', account_number1);
                    SET @transaction_details = CONCAT('Phone Number changed to ', new_phone_number);
                    SET @sql_statement = CONCAT('INSERT INTO ', @table_name, ' VALUES (?, NOW());');

                    PREPARE stmt FROM @sql_statement;
                    EXECUTE stmt USING @transaction_details;
                    DEALLOCATE PREPARE stmt;
                END IF;
            END;

        ------- insert_or_update_hashed_password
        CREATE PROCEDURE insert_or_update_hashed_password (IN account_number INT, IN hash_password VARCHAR(1000))
            BEGIN
            INSERT INTO password_security (account_number, hashed_password)
            VALUES (account_number1, hashed_password1)
            ON DUPLICATE KEY UPDATE hashed_password = hashed_password1;
        END;
*/

// TODO LIST
// In case someone forgets his/she password, use his/her national ID to change the password to a new one. Create a Function.
// Finish the Bank Class which will be responsible to manage the createD accounts
// Check and resolve the memory leaks problem
// Reduce code redundancy into functions
// Review the code and make it the simplest way it can be
// Borrow / Lend Class which will increase the borrowed / lent money to pay the Bank according the duration
// Create the GUI for a more user friendly

class connection_details
{
    public :
    string server, user, password, database;
};

sql :: Connection *connection_setup(connection_details *ID)
{
    try
    {
        sql :: mysql :: MySQL_Driver *driver;
        sql :: Connection *connection;

        driver = sql :: mysql :: get_driver_instance();
        connection = driver->connect(ID->server, ID->user, ID->password);

        connection->setSchema("bankingSystemDatabase");

        return connection;
    }

    catch (sql :: SQLException *e)
    {
        cerr << "Connection Error: " << e->what() << "Code causing Error: " << e->getErrorCode() << endl;
        return NULL;
    }
}

double check_balance(sql :: Connection *connection, int account_number)
{
    double balance;

    sql :: PreparedStatement *prep_statement = connection->prepareStatement("SELECT balance FROM accounts WHERE account_number = ?;");
    prep_statement->setInt(1, account_number);

    sql :: ResultSet *result = prep_statement->executeQuery();

    if (result->next()) balance = result->getDouble("balance");

    delete prep_statement;
    delete result;

    return balance;
}

class Transactions
{
    public:

    void deposit(sql :: Connection *connection, double amount_to_deposit, int account_number);

    void withdrawal(sql :: Connection *connection, double sum_to_withdraw, int account_number);

    void transfer(sql :: Connection *connection, double amount_to_send, int account_number1, int account_number2);

    void transactions_history(sql :: Connection *connection, int account_number); 
};

void Transactions :: deposit (sql :: Connection *connection, double amount_to_deposit, int account_number)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("UPDATE transactions SET deposit = ? WHERE account_number = ?;");
    prep_statement->setDouble(1, amount_to_deposit);
    prep_statement->setInt(2, account_number);

    prep_statement->executeQuery();

    cout << "You have deposit " << amount_to_deposit << " dollars and your new Balance is: " << check_balance(connection, account_number) << endl;

    string table_name = "NO";
    table_name.append(to_string(account_number));

    prep_statement = connection->prepareStatement("INSERT INTO "+table_name+" VALUES ( CONCAT(?, ?), NOW() );");
    prep_statement->setString(1, "New deposit of ");
    prep_statement->setDouble(2, amount_to_deposit);

    prep_statement->executeUpdate();
    
    delete prep_statement;
}

void Transactions :: withdrawal(sql :: Connection *connection, double amount_to_withdraw, int account_number)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("UPDATE transactions SET withdrawal = ? WHERE account_number = ?;");
    prep_statement->setDouble(1, amount_to_withdraw);
    prep_statement->setInt(2, account_number);

    prep_statement->executeQuery();

    cout << "You have withdrawn " << amount_to_withdraw << " dollars and your new Balance is: " << check_balance(connection, account_number) << endl;

    string table_name = "NO";
    table_name.append(to_string(account_number));

    prep_statement = connection->prepareStatement("INSERT INTO "+table_name+" VALUES ( CONCAT(?, ?), NOW() );");
    prep_statement->setString(1, "New Withdrawal of ");
    prep_statement->setDouble(2, amount_to_withdraw);

    prep_statement->executeUpdate();
    
    delete prep_statement;
}

void Transactions :: transfer(sql :: Connection *connection, double amount_to_transfer,int account_number1, int account_number2)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("UPDATE transactions SET transfer = ? WHERE account_number = ?;");
    prep_statement->setDouble(1, amount_to_transfer);
    prep_statement->setInt(2, account_number1);

    prep_statement->executeUpdate();

    cout << "You have sent " << amount_to_transfer << " dollars to the account_number :" << account_number2 << " and Your new balance is: " << check_balance(connection, account_number1) << endl;

    prep_statement = connection->prepareStatement("UPDATE transactions SET receive = ? WHERE account_number = ?;");
    prep_statement->setDouble(1, amount_to_transfer);
    prep_statement->setInt(2, account_number2);

    prep_statement->executeUpdate();

    string table_name1 = "NO";
    table_name1.append(to_string(account_number1));

    prep_statement = connection->prepareStatement("INSERT INTO "+table_name1+" VALUES ( CONCAT(?, ?), NOW() );");
    prep_statement->setString(1, "New Transfer of ");
    prep_statement->setDouble(2, amount_to_transfer);

    prep_statement->executeUpdate();

    string table_name2 = "NO";
    table_name2.append(to_string(account_number2));

    prep_statement = connection->prepareStatement("INSERT INTO "+table_name2+" VALUES ( CONCAT(?, ?), NOW() );");
    prep_statement->setString(1, "New Receive of ");
    prep_statement->setDouble(2, amount_to_transfer);

    prep_statement->executeUpdate();

    delete prep_statement;
}

void Transactions :: transactions_history(sql :: Connection *connection, int account_number)
{
    string table_name = "NO";
    table_name.append(to_string(account_number));

    sql :: PreparedStatement *prep_statement = connection->prepareStatement("SELECT * FROM "+table_name+";");

    sql :: ResultSet *result = prep_statement->executeQuery();

    while (result->next())
    {
        cout << "Transaction_details: " << result->getString("transaction_details") << "  Date & Time: " << result->getString("date_time") << endl;
    }
    
   delete prep_statement;
   delete result;
}

string generate_random_salt(size_t len)
{
    const string valid_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    random_device rd;
    mt19937 generator(rd());

    uniform_int_distribution<> distribution(0, valid_chars.size() - 1);

    string salt;
    for (size_t i = 0; i < valid_chars.size(); i++) 
    {
        salt.push_back(valid_chars[distribution(generator)]);
    }

    return salt;
}

string hashing_password(const string &password)
{
    const size_t SALT_LENGTH = 32;

    string salt = generate_random_salt(SALT_LENGTH);

    const uint32_t t_cost = 2;
    const uint32_t m_cost = 32;
    const uint32_t parallelism = 1;
    const uint32_t hash_length = 32;

    string hash;
    hash.resize(hash_length);

    int result = argon2_hash(t_cost, m_cost, parallelism, password.c_str(), password.length(), salt.c_str(), salt.length(), &hash[0], hash.length(), NULL, 0, Argon2_id, ARGON2_VERSION_NUMBER);

    if (result != ARGON2_OK) 
    {
        cout << "Error Hashing Password" << endl;
        exit(1);
    }

    string hashed_password = salt;
    hashed_password.append(hash);

    return hashed_password;
}

bool verifying_password(const string &password, const string &hashed_password)
{
    const uint32_t t_cost = 2;
    const uint32_t m_cost = 32;
    const uint32_t parallelism = 1;
    const uint32_t hash_length = 32;

    const size_t SALT_LENGH = hashed_password.length() - hash_length;

    string hash;
    hash.resize(hash_length);

    int result = argon2_hash(t_cost, m_cost, parallelism, password.c_str(), password.length(), hashed_password.c_str(), SALT_LENGH, &hash[0], hash.length(), NULL, 0, Argon2_id, ARGON2_VERSION_NUMBER);

    if (result != ARGON2_OK) 
    {
        cout << "Error Verifying Password" << endl;
        exit(1);
    }

    return !hashed_password.substr(SALT_LENGH, hash_length).compare(hash);
}

string retrieve_hashed_password(int account_number, sql :: Connection *connection)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("SELECT hashed_password FROM password_security WHERE account_number = ?");
    prep_statement->setInt(1, account_number);

    sql :: ResultSet *result = prep_statement->executeQuery();

    string hashed_password;

    if(result->next()) hashed_password = result->getString("hashed_password");

    delete prep_statement;
    delete result;

    return hashed_password;
}

class Account : public Transactions
{
    public:

    void create_account(int account_number, string national_ID, string first_name, string last_name, string date_birth, int phone_number, string email, string address, double balance, double interest_rate, string password, string hash_password, sql :: Connection *connection);

    void remove_accounts(sql :: Connection *connection, int account_number);
};

void Account :: create_account(int account_number, string national_ID, string first_name, string last_name, string date_birth, int phone_number, string email, string address, double balance, double interest_rate, string password, string hash_password, sql :: Connection *connection)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("INSERT INTO accounts (national_ID, first_name, last_name, date_birth, phone_number, email, address, balance, interest_rate) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);");
    prep_statement->setString(1, national_ID);
    prep_statement->setString(2, first_name);
    prep_statement->setString(3, last_name);
    prep_statement->setString(4, date_birth);
    prep_statement->setInt(5, phone_number);
    prep_statement->setString(6, email);
    prep_statement->setString(7, address);
    prep_statement->setDouble(8, balance);
    prep_statement->setDouble(9, interest_rate);

    prep_statement->executeUpdate();

    cout << "This is Your Account Number, remember it cause you will need it to gain access to everything you want to do in the future:  ";

    prep_statement = connection->prepareStatement("SELECT account_number FROM accounts WHERE national_ID = ?");
    prep_statement->setString(1, national_ID);

    sql :: ResultSet *result = prep_statement->executeQuery();

    if(result->next())
    {
        account_number = result->getInt("account_number");

        cout << account_number << endl;

        string table_name = "NO";
        table_name.append(to_string(account_number));

        prep_statement = connection->prepareStatement("CREATE TABLE "+table_name+" (transaction_details VARCHAR(100), date_time DATETIME DEFAULT NOW() );");

        prep_statement->executeUpdate();

        prep_statement = connection->prepareStatement("INSERT INTO "+table_name+" (transaction_details) VALUES (?);");
        prep_statement->setString(1, "Account Created");

        prep_statement->executeUpdate();
    }

    prep_statement = connection->prepareStatement("CALL insert_or_update_hashed_password(?, ?);");
    prep_statement->setInt(1, account_number);
    prep_statement->setString(2, hash_password);

    prep_statement->executeUpdate();

    delete prep_statement;
    delete result;
}

void Account :: remove_accounts(sql :: Connection *connection, int account_number)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("DELETE FROM accounts WHERE account_number = ?;");
    prep_statement->setInt(1, account_number);

    prep_statement->executeUpdate();

    prep_statement = connection->prepareStatement("DELETE FROM password_security WHERE account_number = ?;");
    prep_statement->setInt(1, account_number);

    prep_statement->executeUpdate();

    prep_statement = connection->prepareStatement("DELETE FROM transactions WHERE account_number = ?;");
    prep_statement->setInt(1, account_number);

    prep_statement->executeUpdate();

    string table_name = "NO";
    table_name.append(to_string(account_number));

    prep_statement = connection->prepareStatement("DROP TABLE "+table_name+";");

    prep_statement->executeUpdate();

    delete prep_statement;
}

sql :: SQLString retrieve_interest_rate_initial_timestamp(sql :: Connection *connection, int account_number)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("SELECT initial_timestamp FROM accounts WHERE account_number = ?;");
    prep_statement->setInt(1, account_number);

    sql :: ResultSet *result = prep_statement->executeQuery();
    
    sql :: SQLString initial_timestamp;

    if (result->next())  initial_timestamp = result->getString("initial_timestamp");

    return initial_timestamp;
}

int calculate_interest_rate_time_elapsed(sql :: Connection *connection, sql :: SQLString initial_timestamp)
{
    sql :: PreparedStatement *prep_statement = connection->prepareStatement("SELECT NOW() AS time_now;");
    
    sql :: ResultSet *result = prep_statement->executeQuery();

    sql :: SQLString current_time;
    if (result->next()) current_time = result->getString("time_now");

    prep_statement = connection->prepareStatement("SELECT TIMESTAMPDIFF(MINUTE, '" + initial_timestamp + "', '" +current_time+ "') AS time_elapsed;");

    result = prep_statement->executeQuery();
    
    int time_elapsed = 0;

    if (result->next()) time_elapsed = result->getInt("time_elapsed");

    return time_elapsed;
}

void apply_interest_rate_to_balance (sql :: Connection *connection, int account_number)
{
    double interest_rate;

    sql :: PreparedStatement *prep_statement = connection->prepareStatement("SELECT interest_rate FROM accounts WHERE account_number = ?;");
    prep_statement->setInt(1, account_number);

    sql :: ResultSet *result = prep_statement->executeQuery();

    if(result->next()) interest_rate = result->getDouble("interest_rate");

    int time_elapsed = calculate_interest_rate_time_elapsed(connection, retrieve_interest_rate_initial_timestamp(connection, account_number));

    prep_statement = connection->prepareStatement("SELECT balance FROM accounts WHERE account_number = ?;");
    prep_statement->setInt(1, account_number);

    result = prep_statement->executeQuery();
    
    double balance;

    if (result->next()) balance = result->getDouble("balance");

    double interest_rate_plus_balance;

    interest_rate_plus_balance = (interest_rate * balance * time_elapsed) + balance;

    prep_statement = connection->prepareStatement("UPDATE accounts SET balance = ? WHERE account_number = ?;");
    prep_statement->setDouble(1, interest_rate_plus_balance);
    prep_statement->setInt(2, account_number);

    prep_statement->executeUpdate();

    delete prep_statement;
    delete result;
}

class BANK : public Account
{   
    public :

    vector <Account> created_accounts;

};

void retrieve_borrowal_initial_timestamp (sql :: Connection *connection, int account_number)
{

    // on hold

}

void calculate_borrowal_time_elapsed (sql :: Connection *connection, sql :: SQLString initial_timestamp)
{

    // on hold  

}

void apply_interest_rate_to_borrowed_money(sql :: Connection *connection, int account_number)
{

    // on hold  

}

int main(int argc, const char* argv[])
{
    try
    {
        if (argc < 2)
        {
            cerr << "You should Enter the Password as the Second Argument in order to connect to the Database" << endl;
            return 1;
        }

        connection_details ID;
        ID.server= "localhost";
        ID.user = "root";
        ID.password = argv[1];

        sql :: Connection *connection = connection_setup(&ID);
    
        sql :: PreparedStatement *prep_statement;
        sql :: ResultSet *result;

        int options, options2, options3, options4;
    
        string first_name, last_name, new_first_name, date_birth, email, new_email, national_ID, address, new_address, password, password_confirmation, new_password, new_password_confirmation, hash_password, new_hash_password;

        int phone_number, new_phone_number, account_number, account_number1, account_number2;

        double balance, amount_to_deposit, amount_to_withdraw, amount_to_transfer, interest_rate;

        stack <int> main_menu;

        stack <int> sub_menu;

        Account accounts;

        do 
        {
            cout << "---------- ********** ---------- ********** ---------- ********** WELCOME TO THE CROSS-CONTINENTAL TREASURY BANK ********** ---------- ********** ---------- ********** ----------" << endl;

            cout << endl;
            cout << endl;
            cout << endl;

            cout << "                                                 *********** Among the List below, choose what best suits your Status **********"                                                     << endl;
            cout << endl;

            cout << "1. You are New to our Bank and Would like to Create an Account" << endl;

            cout << "2. You already possess an Account and would like to process some inquiries relative to it" << endl;

            cout << "3. Information on our Bank" << endl;

            cout << "0. Quit" << endl;

            cout << endl;

            cin >> options;

            if (options != 0) main_menu.push(options);

            switch(options)
            {
                case 1: // Create the Account and the table History along the way

                    cout << "Please Provide Us with the following Information in order to create your account, Make sure that you enter the correct Information: " << endl;
                    cout << endl;

                    cout << "National ID with at least a letter: ";
                    cin >> national_ID;

                    cout << "First Name: ";
                    cin >> first_name;

                    cout << "Last Name: ";
                    cin >> last_name;

                    cout << "Date of Birth with format 2024-01-31 : ";
                    cin >> date_birth;

                    cout << "Phone Number: ";
                    cin >> phone_number;

                    cout << "Email: ";
                    cin >> email;

                    cout << "Address with format Taiwan-Taipei_City-Datong_District-Zhongshan_Road-001: ";
                    cin >> address;

                    do
                    {
                        cout << "Your Account should have at least 100 when creating it, so Please enter those 100 dollars and not less: " << endl;

                        cout << "Interest Rate Scale according to your First Deposit which can't be changed: " << endl;

                        cout << "Balance = 100 ---> Interest Rate = 0" << endl;

                        cout << "500 > Balance > 100 ---> Interest Rate = 5%" << endl;

                        cout << "1000 > Balance >= 500 ---> Interest Rate = 7%" << endl;

                        cout << "Balance > 1000 ---> Interest Rate = 10%" << endl;

                        cin >> balance;

                    } while (balance < 100);

                    if (balance == 100) interest_rate = 0;

                    else if (balance > 100 && balance < 500) interest_rate = 0.05;

                    else if (balance < 1000 && balance >= 500) interest_rate = 0.07;

                    else interest_rate = 0.1;

                    cout << "Password: ";
                    cin >> password;

                    do
                    {
                        cout << "Passord Confirmation: ";
                        cin >> password_confirmation;

                    }while (password.compare(password_confirmation));

                    hash_password = hashing_password(password);

                    accounts.create_account(account_number, national_ID, first_name, last_name, date_birth, phone_number, email, address, balance, interest_rate, password, hash_password, connection);

                    password.clear();
                    password_confirmation.clear();

                break;

                case 2: // Perform Inquiry Relative to an Existing Account 
                    do
                    {
                        cout << "Choose among the options below, what best suits your requirements" << endl;
                        cout << endl;

                        cout << "1. Check your Balance" << endl;

                        cout << "2. Deposit" << endl;

                        cout << "3. Money Withdrawal " << endl;

                        cout << "4. Money Transfer" << endl;

                        cout << "5. Edit Account Information" << endl;

                        cout << "6. Delete Your Account" << endl;

                        cout << "0. Back to the Previous Menu" << endl;

                        cout << endl;

                        cin >> options2;

                        if (options2 != 0) main_menu.push(options2);

                        if (options2 == 0 && !sub_menu.empty())
                        {
                            options2 = sub_menu.top();
                            sub_menu.pop();

                            break;
                        }

                        switch(options2)
                        {
                            case 1: // Balance Check
                                cout << "What is your Account Number: ";
                                cin >> account_number;

                                cout << "What is your Password: ";
                                cin >> password;

                                hash_password = retrieve_hashed_password(account_number, connection);

                                if (verifying_password(password, hash_password)) 
                                {
                                    apply_interest_rate_to_balance(connection, account_number);

                                    cout << "Your Current Balance is: " << check_balance(connection, account_number) << endl;

                                    password.clear();
                                }

                                else cout << "Your Password is Incorrect" << endl;

                            break;

                            case 2: // Deposit
                                cout << "What is your Account Number: ";
                                cin >> account_number;

                                cout << "What is the Amount you would like to Deposit: ";
                                cin >> amount_to_deposit;

                                cout << "What is your Password: ";
                                cin >> password;

                                hash_password = retrieve_hashed_password(account_number, connection);

                                if (verifying_password(password, hash_password)) 
                                {
                                    apply_interest_rate_to_balance(connection, account_number);

                                    accounts.deposit(connection, amount_to_deposit, account_number);

                                    password.clear();
                                }

                                else cout << "Your Password is Incorrect" << endl;

                            break;

                            case 3: // Withdraw
                                cout << "What is your Account Number: ";
                                cin >> account_number;

                                cout << "What is the Amount you would like to Withdraw: ";
                                cin >> amount_to_withdraw;

                                cout << "What is your Password: ";
                                cin >> password;

                                hash_password = retrieve_hashed_password(account_number, connection);

                                if (verifying_password(password, hash_password)) 
                                {
                                    apply_interest_rate_to_balance(connection, account_number);

                                    balance = check_balance(connection, account_number);

                                    while (amount_to_withdraw > balance) 
                                    {
                                        cout << "Your balance is: " << balance << " which is less than the amount you want Withdraw" << endl;

                                        cout << "So Please enter a reasonnable amount: ";
                                        cin >> amount_to_withdraw;
                                    }

                                    accounts.withdrawal(connection, amount_to_withdraw, account_number);

                                    password.clear();
                                }

                                else cout << "Your Password is Incorrect" << endl;

                            break;

                            case 4: // Transfer
                                cout << "What is your Account Number: ";
                                cin >> account_number1;

                                cout << "What is the Amount you would like to Transfer: ";
                                cin >> amount_to_deposit;

                                cout << "What is the Account Number to receive the Money: ";
                                cin >> account_number2;

                                cout << "What is your Password: ";
                                cin >> password;

                                hash_password = retrieve_hashed_password(account_number1, connection);

                                if (verifying_password(password, hash_password)) 
                                {
                                    apply_interest_rate_to_balance(connection, account_number1);

                                    balance = check_balance(connection, account_number1);

                                    while (amount_to_transfer > balance) 
                                    {
                                        cout << "Your balance is: " << balance << " which is less than the amount you want Transfer" << endl;

                                        cout << "So Please enter a reasonnable amount: ";
                                        cin >> amount_to_transfer;
                                    }

                                    apply_interest_rate_to_balance(connection, account_number2);

                                    accounts.transfer(connection, amount_to_deposit, account_number1, account_number2);    

                                    password.clear();                                                         
                                }

                                else cout << "Your Password is Incorrect" << endl;

                            break;

                            case 5: // Edit Account Information
                                do
                                {
                                    cout << "Choose among the options below, what best suits your requirements" << endl;
                                    cout << endl;

                                    cout << "1. Edit Personal Information" << endl;

                                    cout << "2. Change Password" << endl;

                                    cout << "0. Back to the Previous Menu" << endl;

                                    cout << endl;

                                    cin >> options3;

                                    if (options3 != 0) main_menu.push(options3);

                                    if (options3 == 0 && !sub_menu.empty())
                                    {
                                        options3 = sub_menu.top();
                                        sub_menu.pop();

                                        break;
                                    }

                                    switch(options3) 
                                    {
                                        case 1: // Edit personal Information
                                            do
                                            {
                                                cout << "Choose among the options below, what best suits your requirements: PS: The Date of Birth and National ID number can't be edited" << endl;
                                                cout << endl;

                                                cout << "1. Edit Name" << endl;

                                                cout << "2. Edit email" << endl;

                                                cout << "3. Edit address" << endl;

                                                cout << "4. Edit Phone Number" << endl;

                                                cout << "0. Back to the Previous Menu" << endl;

                                                cout << endl;

                                                cin >> options4;

                                                if (options4 != 0) main_menu.push(options4);

                                                if (options4 == 0 && !sub_menu.empty())
                                                {
                                                    options4 = sub_menu.top();
                                                    sub_menu.pop();

                                                    break;
                                                }

                                                switch(options4)
                                                {
                                                    case 1: // Edit Name
                                                        cout << "Enter Your Account Number: ";
                                                        cin >> account_number;

                                                        cout << "What is your Password: ";
                                                        cin >> password;

                                                        hash_password = retrieve_hashed_password(account_number, connection);

                                                        if (verifying_password(password, hash_password))
                                                        {
                                                            cout << "Enter the New First Name. PS: You can't change your Last Name: ";
                                                            cin >> new_first_name;

                                                            prep_statement = connection->prepareStatement("CALL update_and_log_name(?,?);");
                                                            prep_statement->setInt(1, account_number);
                                                            prep_statement->setString(2, new_first_name);

                                                            prep_statement->executeUpdate();

                                                            password.clear();
                                                        }

                                                        else cout << "Your Password is Incorrect" << endl;

                                                    break;

                                                    case 2: // Edit Email
                                                        cout << "Enter Your Account Number: ";
                                                        cin >> account_number;

                                                        cout << "What is your Password: ";
                                                        cin >> password;

                                                        hash_password = retrieve_hashed_password(account_number, connection);

                                                        if (verifying_password(password, hash_password))
                                                        {
                                                            cout << "Enter the New Mail: ";
                                                            cin >> new_email;

                                                            prep_statement = connection->prepareStatement("CALL update_and_log_email(?,?);");
                                                            prep_statement->setInt(1, account_number);
                                                            prep_statement->setString(2, new_email);

                                                            prep_statement->executeUpdate();

                                                            password.clear();
                                                        }

                                                        else cout << "Your Password is Incorrect" << endl;

                                                    break;

                                                    case 3: // Edit address
                                                        cout << "Enter Your Account Number: ";
                                                        cin >> account_number;

                                                        cout << "What is your Password: ";
                                                        cin >> password;

                                                        hash_password = retrieve_hashed_password(account_number, connection);

                                                        if (verifying_password(password, hash_password))
                                                        {
                                                            cout << "Enter the New Address: ";
                                                            cin >> new_address;

                                                            prep_statement = connection->prepareStatement("CALL update_and_log_address(?,?);");
                                                            prep_statement->setInt(1, account_number);
                                                            prep_statement->setString(2, new_address);

                                                            prep_statement->executeUpdate();

                                                            password.clear();
                                                        }

                                                        else cout << "Your Password is Incorrect" << endl;

                                                    break;

                                                    case 4: // Edit Phone Number
                                                        cout << "Enter Your Account Number: ";
                                                        cin >> account_number;

                                                        cout << "What is your Password: ";
                                                        cin >> password;

                                                        hash_password = retrieve_hashed_password(account_number, connection);

                                                        if (verifying_password(password, hash_password))
                                                        {
                                                            cout << "Enter the New Phone Number: ";
                                                            cin >> new_phone_number;

                                                            prep_statement = connection->prepareStatement("CALL update_and_log_phone_number(?,?);");
                                                            prep_statement->setInt(1, account_number);
                                                            prep_statement->setInt(2, new_phone_number);

                                                            prep_statement->executeUpdate();

                                                            password.clear();
                                                        }

                                                        else cout << "Your Password is Incorrect" << endl;   

                                                    break;
                                                }

                                            }while (options4 != 0);

                                        break;

                                        case 2: // Change Password
                                            cout << "Enter Your Account Number: ";
                                            cin >> account_number;

                                            cout << "What is your Password: ";
                                            cin >> password;

                                            hash_password = retrieve_hashed_password(account_number, connection);

                                            if (verifying_password(password, hash_password))
                                            {
                                                cout << "What is the New Password: ";
                                                cin >> new_password;

                                                do
                                                {
                                                    cout << "New Password Confirmation: ";
                                                    cin >> new_password_confirmation;

                                                }while (password.compare(new_password_confirmation));

                                                new_hash_password = hashing_password(new_password);

                                                prep_statement = connection->prepareStatement("CALL insert_or_update_hash_password(?, ?);");
                                                prep_statement->setInt(1, account_number);
                                                prep_statement->setString(2, new_hash_password);

                                                prep_statement->executeUpdate();

                                                password.clear();
                                                new_password.clear();
                                            }   

                                            else cout << "Your Password is Incorrect" << endl;

                                        break;

                                    }

                                }while (options3 != 0);

                            break;

                            case 6: // Delete an Account
                                cout << "Enter Your Account Number: ";
                                cin >> account_number;

                                cout << "What is your Password: ";
                                cin >> password;

                                hash_password = retrieve_hashed_password(account_number, connection);

                                if (verifying_password(password, hash_password)) 
                                {
                                    accounts.remove_accounts(connection, account_number);
                                    password.clear();
                                }

                                else cout << "Your Password is Incorrect" << endl;

                            break;

                        }

                    }while (options2 != 0);

                break;

                case 3: // Information on the Bank
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

                case 0:
                    cout << "Thanks for having choose CROSS-CONTINENTAL TREASURY BANK, Have a Good Day" << endl;
                    exit(1);

                break;

            }

            connection->close();
            delete connection;
            delete prep_statement;
            delete result;

        }while (options != 0);

    }

    catch (sql :: SQLException *e)
    {
        cerr << "Connection Error: " << e->what() << "Code causing Error: " << e->getErrorCode() << endl;
    }

    catch (const std :: exception *e)
    {
        cout << "Error: " << e->what() << endl;
    }
}