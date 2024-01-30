#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <vector>
#include <database.h> // library that I have created which is used for both the Banking System main.cpp and the GUI Implementation. THIS LIBRARY IS LOCATED IN GUI/src/database

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

using namespace std;

// BELOW ARE ALL THE REQUIREMENTS AND PREPARATIONS FOR THE DATABASE IN ORDER TO USE THIS PROJECT'S CODE. MAKE SURE THAT YOU HAVE MYSQL INSTALLED ON YOUR PC. YOU CAN USE EITHER VSCODE OR MYSQL WORKBENCH TO RUN THESE FOLLOWING QUERIES

/*
        ----- I use CMake to link all the executables and libraries. The Database_library is also link there. Check all the CMake files to understand how the System is built
        ----- Download Argon 2 in order to use its library for the password hashing process
        ----- Change the Include Directories Path to find all the preinstalled libraries required to run this project on your PC
        ----- When Running Your Program, Pass your Database's Password as the Second Argument
        ----- The Interest Rate works in a way that it updates the Balance everytime there is a change in the latter; By change I mean New Deposit, New Withdrawal, New Transfer and New Money Recceived from another Account
        ----- I have also set the Interest rate according to how many days have passed since the last modification in your balance.
        ----- If You want to apply the Interest Rate Daily or Monthly for all Accounts, I will leave an SQL Event which will help you doing so. Event Name----> apply_interest_rate_to_balance
        ----- By appliying the Interest Rate Event to your database remember deleting or commenting the //BANK :: apply_interest_rate_to_balance()// function, everywhere I have used it throughout the program
        -----
        -----

        *************** ALL THE TABLES ***************
        ------- accounts
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
            initial_timestamp DATETIME,
            borrowed_money DECIMAL(20,5) DEFAULT 0,
            initial_borrowed_money_timestamp DATETIME,
        )AUTO_INCREMENT = 1000000000;

        ------- transactions
        CREATE TABLE transactions
        (
            account_number INT PRIMARY KEY AUTO_INCREMENT,
            deposit DECIMAL(20,5) DEFAULT 0,
            withdrawal DECIMAL(20,5) DEFAULT 0,
            transfer DECIMAL(20,5) DEFAULT 0,
            receive DECIMAL(20,5) DEFAULT 0
        )AUTO_INCREMENT = 1000000000;

       ------- hashed_password
        CREATE TABLE hashed_password
        (
            account_number INT PRIMARY KEY,
            hashed_password VARBINARY(500)
        );

        ------- borrowal_record
        CREATE TABLE borrowal_record
        (
           account_number INT PRIMARY KEY,
           borrowed_amount DECIMAL(20, 5),
           interest_rate DECIMAL(5,3),
           initial_timestamp TIMESTAMP DEFAULT NULL,
           paid_timestamp TIMESTAMP DEFAULT NULL
        );

        ------- event_schedule
        CREATE TABLE event_schedule
        (
            account_number INT PRIMARY KEY,
            scheduled_time TIMESTAMP,
            triggered INT DEFAULT 0
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

        ------- new_receive
        CREATE TRIGGER new_receive AFTER UPDATE ON transactions FOR EACH ROW
            BEGIN
                IF OLD.receive <> NEW.receive THEN
                    UPDATE accounts SET balance = balance + NEW.receive, initial_timestamp = NOW()
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
        CREATE PROCEDURE insert_or_update_hashed_password (IN account_number1 INT, IN hashed_password1 VARBINARY(1000))
            BEGIN
            INSERT INTO password_security
            VALUES (account_number1, hashed_password1)
            ON DUPLICATE KEY UPDATE hashed_password = hashed_password1;
        END;

        ------- update_borrowed_money
        CREATE PROCEDURE update_borrowed_money(IN account_number1 INT)
        BEGIN
            DECLARE borrowed_money_interest_rate DECIMAL (20,5);
            DECLARE borrowed_money DECIMAL (20,5);
            DECLARE time_elapsed INT;

            SELECT interest_rate INTO borrowed_money_interest_rate FROM borrowal_record
            WHERE account_number = account_number1;

            SET time_elapsed = borrowed_money_interest_rate(retrieve_borrowal_money_initial_timestamp(account_number1));

            SELECT borrowed_amount INTO borrowed_money FROM borrowal_record
            WHERE account_number = account_number1;

            SET borrowed_money = (borrowed_money_interest_rate * borrowed_money * time_elapsed) + borrowed_money;

            UPDATE borrowal_record
            SET borrowed_amount = borrowed_money
            WHERE account_number = account_number1;
        END;

        ------- deduce_borrowed_money
        CREATE PROCEDURE deduce_borrowed_money(IN account_number1 INT)
        BEGIN
            DECLARE updated_borrowed_money DECIMAL (20,5);

            SELECT borrowed_amount INTO updated_borrowed_money FROM borrowal_record
            WHERE account_number = account_number1;

            UPDATE accounts SET balance = balance - updated_borrowed_money;
        END;


        *************** ALL THE FUNCTIONS ***************
        ------- retrieve_borrowal_money_initial_timestamp (USED IN update_borrowed_money PROCEDURE)
        CREATE FUNCTION retrieve_borrowal_money_initial_timestamp (account_number1 INT) RETURNS TIMESTAMP
        READS SQL DATA
            BEGIN
                DECLARE init_timestamp TIMESTAMP;

                SELECT initial_timestamp INTO init_timestamp FROM borrowal_record
                WHERE account_number = account_number1;

                RETURN init_timestamp;
            END;

        ------- borrowed_money_interest_rate (USED IN update_borrowed_money PROCEDURE)
        CREATE FUNCTION borrowed_money_interest_rate(initial_timestamp TIMESTAMP) RETURNS INT
        NO SQL
            BEGIN
                DECLARE time_elapsed INT;

                SET time_elapsed = TIMESTAMPDIFF(HOUR, initial_timestamp, NOW());

                RETURN time_elapsed;
            END;


        *************** ALL THE EVENTS  ***************
        ------- check_events
        CREATE EVENT check_events
        ON SCHEDULE EVERY 1 HOUR
        STARTS CURRENT_TIMESTAMP + INTERVAL 5 HOUR
        DO
            BEGIN
                DECLARE account_number1 INT;
                DECLARE invalid_case INT DEFAULT FALSE;

                DECLARE cursor_acc CURSOR FOR
                SELECT account_number FROM event_schedule
                WHERE scheduled_time <= CURRENT_TIMESTAMP AND triggered = 0;

                DECLARE CONTINUE HANDLER FOR NOT FOUND SET invalid_case = TRUE;

                OPEN cursor_acc;
                    read_loop : LOOP

                        FETCH cursor_acc INTO account_number1;

                        IF invalid_case THEN
                            LEAVE read_loop;
                        END IF;

                        UPDATE event_schedule_table SET triggered = 1
                        WHERE account_number = account_number1;

                        CALL update_borrowed_money(account_number1);
                        CALL deduce_borrowed_money(account_number1);

                    END LOOP read_loop;
                CLOSE cursor_acc;
            END;

        ------- apply_interest_rate_to_balance
        CREATE EVENT apply_interest_rate_to_balance
        ON EVERY 1 DAY//MONTH
        STARTS CURRENT_TIMESTAMP
        DO
        BEGIN
            UPDATE accounts SET balance = (balance * interest_rate) + balance;
        END;
*/

// TODO LIST
// Check and resolve the memory leaks issues
// Reduce code redundancy into functions
// Create the GUI for a more user friendly

int main(int argc, const char **argv)
{
    try
    {
        if (argc < 2)
        {
            cerr << "You should Enter the Password as the Second Argument in order to connect to the Database" << endl;
            return 1;
        }

        connection_details ID;
        ID.server = "localhost";
        ID.user = "root";
        ID.password = (argc >= 2) ? argv[1] : "";

        sql ::Connection *connection = connection_setup(&ID);
        if (!connection)
        {
            cerr << "Failed to establish the Database connection." << endl;
            return 1;
        }

        int adm_options, options, options1, options2, options3, options4;

        string first_name, last_name, new_first_name, date_birth, email, new_email, national_ID, address, new_address, password, password_confirmation, new_password, new_password_confirmation, hash_password, new_hash_password, initial_timestamp;

        int phone_number, new_phone_number, account_number, account_number1, account_number2, k = 3;

        double balance, amount_to_deposit, amount_to_withdraw, amount_to_transfer, interest_rate, amount_to_borrow, amount_to_return, borrowal_interest_rate;

        stack<int> main_menu;

        stack<int> sub_menu;

        do
        {
            cout << "---------- ********** ---------- ********** ---------- ********** WELCOME TO THE CROSS-CONTINENTAL TREASURY BANK ********** ---------- ********** ---------- ********** ----------" << endl;

            cout << endl;
            cout << endl;
            cout << endl;

            cout << "                                                 *********** Among the List below, choose what best suits You **********" << endl;
            cout << endl;

            cout << "1. Administrator" << endl;

            cout << "2. Regular Client" << endl;

            cout << "0. QUIT" << endl;

            cout << endl;

            cin >> options;
            cout << endl;

            if (options)
                sub_menu.push(options);

            switch (options)
            {
            case 1: // Administrator
                cout << "Enter Account Number: ";
                cin >> account_number;
                cout << endl;

                hash_password = BANK ::retrieve_adm_hashed_password(connection, account_number);

                cout << "What is your Password: " << endl;

                cout << "You have 3 Chances" << endl;

                do
                {
                    cin >> password;
                    cout << endl;

                    if (BANK ::verifying_password(password, hash_password))
                    {
                        do
                        {
                            cout << "1. Create an Administrator" << endl;

                            cout << "2. Display The Accounts Table" << endl;

                            cout << "3. Display Specific Account through the Account_number" << endl;

                            cout << "4. Display People in Debt " << endl;

                            cout << "5. Display Specific Person in Debt through the Account_number" << endl;

                            cout << "6. Display all Transactions History for an Account_number" << endl;

                            cout << "7. Remove Accounts through the Account_number" << endl;

                            cout << "0. Return to the Previous Menu" << endl;
                            cout << endl;

                            cin >> adm_options;
                            cout << endl;

                            if (adm_options)
                                sub_menu.push(adm_options);

                            if (!adm_options && sub_menu.empty())
                            {
                                adm_options = sub_menu.top();
                                sub_menu.pop();

                                break;
                            }

                            switch (adm_options)
                            {
                            case 1: // Create an Administrator
                                cout << "Enter your Desired Account Number: ";
                                cin >> account_number;
                                cout << endl;

                                cout << "Enter Your Password: ";
                                cin >> password;
                                cout << endl;

                                do
                                {
                                    cout << "Password Confirmation: ";
                                    cin >> password_confirmation;
                                    cout << endl;

                                } while (password.compare(password_confirmation));

                                hash_password = BANK ::hashing_password(password);

                                BANK ::create_adm(connection, account_number, hash_password);

                                password.clear();
                                password_confirmation.clear();

                                break;

                            case 2: // Display Accounts Table

                                BANK ::display_accounts_table(connection);

                                break;

                            case 3: // Display Specific Account
                                cout << "Enter The Account Number To be displayed" << endl;
                                cin >> account_number;
                                cout << endl;

                                BANK ::display_specific_accounts(connection, account_number);

                                break;

                            case 4: // Display People in Debt

                                BANK ::display_people_in_debt(connection);

                                break;

                            case 5: // Display Specific Accounts in Debt
                                cout << "Enter The Account Number's in Debt To be displayed" << endl;
                                cin >> account_number;
                                cout << endl;

                                BANK ::display_specific_accounts_in_debt(connection, account_number);

                                break;

                            case 6: // Display Transactions History
                                cout << "Enter The Account Number's Transactions History To be displayed" << endl;
                                cin >> account_number;
                                cout << endl;

                                BANK ::display_transactions_history(connection, account_number);

                                break;

                            case 7: // Remove Account
                                cout << "Enter The Account Number To be removed" << endl;
                                cin >> account_number;
                                cout << endl;

                                BANK ::remove_accounts(connection, account_number);
                                break;
                            }

                        } while (adm_options);

                        break;
                    }

                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                    k--;

                } while (k);

                k = 3;

                break;

            case 2: // Regular client
                do
                {
                    cout << "1. New to our Bank and Would like to Create an Account" << endl;

                    cout << "2. Already possess an Account and would like to process some inquiries relative to it" << endl;

                    cout << "3. Information on our Bank" << endl;

                    cout << "0. Return to the Previous Menu" << endl;

                    cout << endl;

                    cin >> options1;

                    if (options1)
                        sub_menu.push(options1);

                    if (!options1 && !sub_menu.empty())
                    {
                        options1 = sub_menu.top();
                        sub_menu.pop();

                        break;
                    }

                    switch (options1)
                    {
                    case 1: // Create the Account and the table History along the way

                        cout << "Please Provide Us with the following Information in order to create your account. Make sure You enter the Correct Information: " << endl;
                        cout << endl;

                        cout << "National ID with at least a letter within it: ";
                        cin >> national_ID;
                        cout << endl;

                        cout << "First Name: ";
                        cin >> first_name;
                        cout << endl;

                        cout << "Last Name: ";
                        cin >> last_name;
                        cout << endl;

                        cout << "Date of Birth ( 2024-01-31 ) : ";
                        cin >> date_birth;
                        cout << endl;

                        cout << "Phone Number: ";
                        cin >> phone_number;
                        cout << endl;

                        cout << "Email: ";
                        cin >> email;
                        cout << endl;

                        cout << "Address ( Taiwan-Taipei_City-Datong_District-Zhongshan_Road-001 ): ";
                        cin >> address;
                        cout << endl;

                        do
                        {
                            cout << "Your Account should have at least 100 when creating it, so Please enter those 100 dollars and not less: " << endl;
                            cout << endl;

                            cout << "Interest Rate Scale according to your First Deposit which can't be changed: " << endl;
                            cout << endl;

                            cout << "1. Balance = 100 ---> Interest Rate = 0" << endl;

                            cout << "2. 500 > Balance > 100 ---> Interest Rate = 2%" << endl;

                            cout << "3. 1000 > Balance >= 500 ---> Interest Rate = 5%" << endl;

                            cout << "4. Balance > 1000 ---> Interest Rate = 7%" << endl;

                            cin >> balance;

                        } while (balance < 100);

                        if (balance == 100)
                            interest_rate = 0;

                        else if (balance > 100 && balance < 500)
                            interest_rate = 0.02;

                        else if (balance < 1000 && balance >= 500)
                            interest_rate = 0.05;

                        else
                            interest_rate = 0.07;

                        cout << "Password: ";
                        cin >> password;
                        cout << endl;

                        do
                        {
                            cout << "Passord Confirmation: ";
                            cin >> password_confirmation;
                            cout << endl;

                        } while (password.compare(password_confirmation));

                        hash_password = BANK ::hashing_password(password);

                        Account ::create_account(connection, account_number, national_ID, first_name, last_name, date_birth, phone_number, email, address, balance, interest_rate, password, hash_password);

                        password.clear();
                        password_confirmation.clear();

                        break;

                    case 2: // Perform Inquiry Relative to an Existing Account
                        do
                        {
                            cout << "Choose among the Options below, what best suits You" << endl;
                            cout << endl;

                            cout << "1. Check your Balance" << endl;

                            cout << "2. Deposit" << endl;

                            cout << "3. Money Withdrawal " << endl;

                            cout << "4. Money Transfer" << endl;

                            cout << "5. Borrow Money" << endl;

                            cout << "6. Return Borrowed Money" << endl;

                            cout << "7. Edit Account Information / Forget Password" << endl;

                            cout << "8. Transaction History" << endl;

                            cout << "9. Delete Account" << endl;

                            cout << "0. Return to the Previous Menu" << endl;

                            cout << endl;

                            cin >> options2;
                            cout << endl;

                            if (options2)
                                main_menu.push(options2);

                            if (!options2 && !sub_menu.empty())
                            {
                                options2 = sub_menu.top();
                                sub_menu.pop();

                                break;
                            }

                            switch (options2)
                            {
                            case 1: // Balance Check
                                BANK ::authentification_message(connection, account_number, hash_password);

                                do
                                {
                                    cin >> password;
                                    cout << endl;

                                    if (BANK ::verifying_password(password, hash_password))
                                    {
                                        BANK ::apply_interest_rate_to_balance(connection, account_number);

                                        cout << "Your Current Balance is: " << check_balance(connection, account_number) << endl;
                                        cout << endl;

                                        password.clear();

                                        break;
                                    }

                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 2: // Deposit
                                cout << "Enter Amount to Deposit: ";
                                cin >> amount_to_deposit;
                                cout << endl;

                                BANK ::authentification_message(connection, account_number, hash_password);

                                do
                                {
                                    cin >> password;
                                    cout << endl;

                                    if (BANK ::verifying_password(password, hash_password))
                                    {
                                        BANK ::apply_interest_rate_to_balance(connection, account_number);

                                        Transactions ::deposit(connection, amount_to_deposit, account_number);

                                        password.clear();

                                        break;
                                    }

                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 3: // Withdraw
                                cout << "Enter Amount to Withdraw: ";
                                cin >> amount_to_withdraw;
                                cout << endl;

                                BANK ::authentification_message(connection, account_number, hash_password);

                                do
                                {
                                    cin >> password;
                                    cout << endl;

                                    if (BANK ::verifying_password(password, hash_password))
                                    {
                                        BANK ::apply_interest_rate_to_balance(connection, account_number);

                                        balance = check_balance(connection, account_number);

                                        while (amount_to_withdraw > balance)
                                        {
                                            cout << "Balance is: " << balance << " which is less than the Amount to Withdraw" << endl;

                                            cout << "So Please Enter a reasonnable Amount: ";
                                            cin >> amount_to_withdraw;
                                            cout << endl;
                                        }

                                        Transactions ::withdrawal(connection, amount_to_withdraw, account_number);

                                        password.clear();

                                        break;
                                    }

                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 4: // Transfer
                                cout << "Enter Amount to Transfer: ";
                                cin >> amount_to_transfer;
                                cout << endl;

                                cout << "Enter the Account Number to receive the Money: ";
                                cin >> account_number2;
                                cout << endl;

                                BANK ::authentification_message(connection, account_number1, hash_password);

                                do
                                {
                                    cin >> password;
                                    cout << endl;

                                    if (BANK ::verifying_password(password, hash_password))
                                    {
                                        BANK ::apply_interest_rate_to_balance(connection, account_number1);

                                        balance = check_balance(connection, account_number1);

                                        while (amount_to_transfer > balance)
                                        {
                                            cout << "Your balance is: " << balance << " which is less than the Amount to Transfer" << endl;

                                            cout << "So Please enter a reasonnable Amount: ";
                                            cin >> amount_to_transfer;
                                            cout << endl;
                                        }

                                        BANK ::apply_interest_rate_to_balance(connection, account_number2);

                                        Transactions ::transfer(connection, amount_to_transfer, account_number1, account_number2);

                                        password.clear();

                                        break;
                                    }

                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 5: // Borrow Money
                                cout << "No one is allowed to borrow Money if currently owes the Bank any amount otherwise She/He will be logged out of the System Completely. Please go to the Previous Menu and Pay what is owed before asking for any New Borrowal" << endl;
                                cout << endl;

                                cout << "Enter Amount to Borrow: " << endl;
                                cout << "Interest Rate Scale on Borrowed Amount " << endl;
                                cout << endl;

                                cout << "1. Borrowed Amount = 100 ---> Interest Rate = 0.1%. PS: TO BE RETURN WITHIN 1 DAY OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE" << endl;

                                cout << "2. 500 > Borrowed Amount > 100 ---> Interest Rate = 5% PS: TO BE RETURN WITHIN 2 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE" << endl;

                                cout << "3. 1000 > Borrowed Amount >= 500 ---> Interest Rate = 7% PS: TO BE RETURN WITHIN 3 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE" << endl;

                                cout << "4. Borrowed Amount > 1000 ---> Interest Rate = 10% PS: TO BE RETURN WITHIN 4 DAYS OR IT'LL BE DEDUCED FROM YOUR ACCOUNT WITH A 0.01 MORE ON THE INTEREST RATE" << endl;

                                cin >> amount_to_borrow;
                                cout << endl;

                                BANK ::authentification_message(connection, account_number, hash_password);

                                do
                                {
                                    cin >> password;
                                    cout << endl;

                                    if (BANK ::verifying_password(password, hash_password))
                                    {
                                        if (amount_to_borrow == 100)
                                        {
                                            borrowal_interest_rate = 0.001;

                                            Transactions ::insert_borrowal(connection, account_number, amount_to_borrow, borrowal_interest_rate);

                                            unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO event_schedule (account_number, scheduled_time) VALUES (?, CURRENT_TIMESTAMP + INTERVAL 24 HOUR);"));
                                            prep_statement->setInt(1, account_number);

                                            prep_statement->executeUpdate();

                                            Transactions ::borrow(connection, amount_to_borrow, account_number);
                                        }

                                        else if (amount_to_borrow > 100 && amount_to_borrow < 500)
                                        {
                                            borrowal_interest_rate = 0.05;

                                            Transactions ::insert_borrowal(connection, account_number, amount_to_borrow, borrowal_interest_rate);

                                            unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO event_schedule (account_number, scheduled_time) VALUES (?, CURRENT_TIMESTAMP + INTERVAL 48 HOUR);"));
                                            prep_statement->setInt(1, account_number);

                                            prep_statement->executeUpdate();

                                            Transactions ::borrow(connection, amount_to_borrow, account_number);
                                        }

                                        else if (amount_to_borrow < 1000 && amount_to_borrow >= 500)
                                        {
                                            borrowal_interest_rate = 0.07;

                                            Transactions ::insert_borrowal(connection, account_number, amount_to_borrow, borrowal_interest_rate);

                                            unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO event_schedule (account_number, scheduled_time) VALUES (?, CURRENT_TIMESTAMP + INTERVAL 72 HOUR);"));
                                            prep_statement->setInt(1, account_number);

                                            prep_statement->executeUpdate();

                                            Transactions ::borrow(connection, amount_to_borrow, account_number);
                                        }

                                        else
                                        {
                                            borrowal_interest_rate = 0.1;

                                            Transactions ::insert_borrowal(connection, account_number, amount_to_borrow, borrowal_interest_rate);

                                            unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("INSERT INTO event_schedule (account_number, scheduled_time) VALUES (?, CURRENT_TIMESTAMP + INTERVAL 96 HOUR);"));
                                            prep_statement->setInt(1, account_number);

                                            prep_statement->executeUpdate();

                                            Transactions ::borrow(connection, amount_to_borrow, account_number);
                                        }

                                        break;
                                    }

                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 6: // Return Borrowed Money
                                BANK ::authentification_message(connection, account_number, hash_password);

                                do
                                {
                                    cin >> password;
                                    cout << endl;

                                    if (BANK ::verifying_password(password, hash_password))
                                    {
                                        unique_ptr<sql ::PreparedStatement> prep_statement_call_update(connection->prepareStatement("CALL update_borrowed_money(?);"));
                                        prep_statement_call_update->setInt(1, account_number);

                                        prep_statement_call_update->executeUpdate();

                                        cout << "The Amount ought to be returned is: ";
                                        unique_ptr<sql ::PreparedStatement> prep_statement_select_borrowal(connection->prepareStatement("SELECT borrowed_amount FROM borrowal_record WHERE account_number = ?;"));
                                        prep_statement_select_borrowal->setInt(1, account_number);

                                        unique_ptr<sql ::ResultSet> result(prep_statement_select_borrowal->executeQuery());

                                        double due_returned;
                                        if (result->next())
                                        {
                                            due_returned = result->getDouble("borrowed_amount");
                                            cout << due_returned << endl;
                                        }

                                        cout << "Enter Exact Amount to be returned and not less" << endl;
                                        cin >> amount_to_return;
                                        cout << endl;

                                        while (due_returned != amount_to_return)
                                        {
                                            cout << "Entered Amount is less than the one to be returned, Enter it again" << endl;
                                            cin >> amount_to_return;
                                            cout << endl;
                                        }

                                        cout << "Thanks, You have officially paid your debt and are now allowed to make another one" << endl;
                                        cout << endl;

                                        unique_ptr<sql ::PreparedStatement> prep_statement_delete_borrowal(connection->prepareStatement("DELETE FROM borrowal_record WHERE account_number = ?;"));
                                        prep_statement_delete_borrowal->setInt(1, account_number);

                                        prep_statement_delete_borrowal->executeUpdate();

                                        unique_ptr<sql ::PreparedStatement> prep_statement_delete_event(connection->prepareStatement("DELETE FROM event_schedule WHERE account_number = ?;"));
                                        prep_statement_delete_event->setInt(1, account_number);

                                        prep_statement_delete_event->executeUpdate();

                                        Transactions ::insert_transactions(connection, account_number, "New Money Returned, Sum of ", amount_to_return);

                                        break;
                                    }

                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 7: // Edit Account Information
                                do
                                {
                                    cout << "Choose among the options below, what best suits your requirements" << endl;
                                    cout << endl;

                                    cout << "1. Edit Personal Information" << endl;

                                    cout << "2. Change Password" << endl;

                                    cout << "3. Forget Password" << endl;

                                    cout << "0. Return to the Previous Menu" << endl;

                                    cout << endl;

                                    cin >> options3;
                                    cout << endl;

                                    if (options3)
                                        main_menu.push(options3);

                                    if (!options3 && !sub_menu.empty())
                                    {
                                        options3 = sub_menu.top();
                                        sub_menu.pop();

                                        break;
                                    }

                                    switch (options3)
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

                                            cout << "0. Return to the Previous Menu" << endl;

                                            cout << endl;

                                            cin >> options4;
                                            cout << endl;

                                            if (options4)
                                                main_menu.push(options4);

                                            if (!options4 && !sub_menu.empty())
                                            {
                                                options4 = sub_menu.top();
                                                sub_menu.pop();

                                                break;
                                            }

                                            switch (options4)
                                            {
                                            case 1: // Edit Name
                                                BANK ::authentification_message(connection, account_number, hash_password);

                                                do
                                                {
                                                    cin >> password;
                                                    cout << endl;

                                                    if (BANK ::verifying_password(password, hash_password))
                                                    {
                                                        cout << "Enter the New First Name. PS: Last Name can't be changed: ";
                                                        cin >> new_first_name;
                                                        cout << endl;

                                                        unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_name(?,?);"));
                                                        prep_statement->setInt(1, account_number);
                                                        prep_statement->setString(2, new_first_name);

                                                        prep_statement->executeUpdate();

                                                        password.clear();

                                                        break;
                                                    }

                                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                                    k--;

                                                } while (k);

                                                k = 3;

                                                break;

                                            case 2: // Edit Email
                                                BANK ::authentification_message(connection, account_number, hash_password);

                                                do
                                                {
                                                    cin >> password;
                                                    cout << endl;

                                                    if (BANK ::verifying_password(password, hash_password))
                                                    {
                                                        cout << "Enter the New Mail: ";
                                                        cin >> new_email;

                                                        unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_email(?,?);"));
                                                        prep_statement->setInt(1, account_number);
                                                        prep_statement->setString(2, new_email);

                                                        prep_statement->executeUpdate();

                                                        password.clear();

                                                        break;
                                                    }

                                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                                    k--;

                                                } while (k);

                                                k = 3;

                                                break;

                                            case 3: // Edit address
                                                BANK ::authentification_message(connection, account_number, hash_password);

                                                do
                                                {
                                                    cin >> password;
                                                    cout << endl;

                                                    if (BANK ::verifying_password(password, hash_password))
                                                    {
                                                        cout << "Enter the New Address: ";
                                                        cin >> new_address;
                                                        cout << endl;

                                                        unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_address(?,?);"));
                                                        prep_statement->setInt(1, account_number);
                                                        prep_statement->setString(2, new_address);

                                                        prep_statement->executeUpdate();

                                                        password.clear();

                                                        break;
                                                    }

                                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                                    k--;

                                                } while (k);

                                                k = 3;

                                                break;

                                            case 4: // Edit Phone Number
                                                BANK ::authentification_message(connection, account_number, hash_password);

                                                do
                                                {
                                                    cin >> password;
                                                    cout << endl;

                                                    if (BANK ::verifying_password(password, hash_password))
                                                    {
                                                        cout << "Enter the New Phone Number: ";
                                                        cin >> new_phone_number;
                                                        cout << endl;

                                                        unique_ptr<sql ::PreparedStatement> prep_statement(connection->prepareStatement("CALL update_and_log_phone_number(?,?);"));
                                                        prep_statement->setInt(1, account_number);
                                                        prep_statement->setInt(2, new_phone_number);

                                                        prep_statement->executeUpdate();

                                                        password.clear();

                                                        break;
                                                    }

                                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                                    k--;

                                                } while (k);

                                                k = 3;

                                                break;
                                            }

                                        } while (options4);

                                        break;

                                    case 2: // Change Password
                                        BANK ::authentification_message(connection, account_number, hash_password);

                                        do
                                        {
                                            cin >> password;
                                            cout << endl;

                                            if (BANK ::verifying_password(password, hash_password))
                                            {
                                                cout << "Enter the New Password: ";
                                                cin >> new_password;
                                                cout << endl;

                                                do
                                                {
                                                    cout << "Enter the New Password Confirmation: ";
                                                    cin >> new_password_confirmation;
                                                    cout << endl;

                                                } while (new_password.compare(new_password_confirmation));

                                                new_hash_password = BANK ::hashing_password(new_password);

                                                call_insert_or_update_hashed_password(connection, account_number, new_hash_password);

                                                password.clear();
                                                new_password.clear();

                                                break;
                                            }

                                            cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                            k--;

                                        } while (k);

                                        k = 3;

                                        break;

                                    case 3: // Forget Password
                                        cout << "In order to Change the password, We shall make sure that One is Who is proclaims to be, so Please Provide us with the Following Information for the Authentification Process" << endl;
                                        cout << endl;

                                        cout << "Enter Account Number: " << endl;
                                        cin >> account_number;
                                        cout << endl;

                                        cout << "Enter National ID Number: " << endl;
                                        cin >> national_ID;
                                        cout << endl;

                                        cout << "Enter Date of Birth: " << endl;
                                        cin >> date_birth;
                                        cout << endl;

                                        if (BANK ::authentification_check(connection, account_number, national_ID, date_birth))
                                        {
                                            cout << "Enter New Password: ";
                                            cin >> new_password;
                                            cout << endl;

                                            do
                                            {
                                                cout << "Enter New Password Confirmation: ";
                                                cin >> new_password_confirmation;
                                                cout << endl;

                                            } while (new_password.compare(new_password_confirmation));

                                            new_hash_password = BANK ::hashing_password(new_password);

                                            call_insert_or_update_hashed_password(connection, account_number, new_hash_password);

                                            new_password.clear();
                                        }

                                        else
                                            cout << "The provided Information are Incorrect" << endl;

                                        break;
                                    }

                                } while (options3);

                                break;

                            case 8: // Transaction History
                                BANK ::authentification_message(connection, account_number, hash_password);

                                do
                                {
                                    cin >> password;
                                    cout << endl;

                                    if (BANK ::verifying_password(password, hash_password))
                                    {
                                        Transactions ::display_transactions_history(connection, account_number);
                                        password.clear();

                                        break;
                                    }

                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;

                            case 9: // Delete an Account
                                BANK ::authentification_message(connection, account_number, hash_password);
                                ;

                                do
                                {
                                    cin >> password;
                                    cout << endl;

                                    if (BANK ::verifying_password(password, hash_password))
                                    {
                                        Account ::remove_accounts(connection, account_number);

                                        password.clear();

                                        break;
                                    }

                                    cout << "Incorrect password. Chances left: " << k - 1 << endl;
                                    k--;

                                } while (k);

                                k = 3;

                                break;
                            }

                        } while (options2);

                        break;

                    case 3: // Information on the Bank
                        cout << "Title The CROSS_CONTINENTAL TREASURY BANK: A Compendium of Financial Excellence" << endl;

                        cout << "CHAPTER 1: INTRODUCTION TO THE CROSS-CONTINENTAL TREASURY BANK" << endl;
                        cout << "SECTION 1.1: Our Vision and Mission" << endl;
                        cout << endl;
                        cout << "The CROSS-CONTINENTAL TREASURY BANK envisions a world where financial services transcend borders, empower individuals, and foster economic growth." << endl;
                        cout << "Our mission is to provide cutting-edge banking solutions with a commitment to integrity, innovation, and client satisfaction." << endl;
                        cout << "SECTION 1.2: Core Values" << endl;
                        cout << endl;
                        cout << "Integrity: Upholding the highest ethical standards in all interactions." << endl;
                        cout << "Innovation: Pioneering financial solutions that adapt to the dynamic global landscape." << endl;
                        cout << "Client Satisfaction: Placing our clients at the center and ensuring their success is our success." << endl;
                        cout << endl;
                        cout << endl;
                        cout << endl;

                        cout << "CHAPTER 2: THE PILLARS OF FINANCIAL SECURITY" << endl;
                        cout << endl;
                        cout << "SECTION 2.1: Robust Security Protocols" << endl;
                        cout << endl;
                        cout << "Implementing state-of-the-art encryption and cybersecurity measures to safeguard client information." << endl;
                        cout << "Regular audits and assessments to fortify our defenses against emerging threats." << endl;
                        cout << "SECTION 2.2: Compliance and Regulatory Adherence" << endl;
                        cout << endl;
                        cout << "Strict adherence to international banking regulations and compliance standards." << endl;
                        cout << "Continuous training and development for staff to stay abreast of regulatory changes." << endl;
                        cout << endl;
                        cout << endl;
                        cout << endl;

                        cout << "CHAPTER 3: SERVICES TAILORED TO YOUR AMBITIONS" << endl;
                        cout << endl;
                        cout << "SECTION 3.1: Personalized Banking Solutions" << endl;
                        cout << endl;
                        cout << "Tailoring financial services to meet the unique needs and aspirations of our diverse clientele." << endl;
                        cout << "Offering a wide array of accounts, investment options, and lending services." << endl;
                        cout << "SECTION 3.2: Global Accessibility" << endl;
                        cout << endl;
                        cout << "A network of branches and ATMs spanning continents, ensuring clients can access their funds whenever and wherever they need." << endl;
                        cout << endl;
                        cout << endl;
                        cout << endl;

                        cout << "CHAPTER 4: EMPOWERING FINANCIAL LITERACY" << endl;
                        cout << endl;
                        cout << "SECTION 4.1: Educational Initiatives" << endl;
                        cout << endl;
                        cout << "The CROSS-CONTINENTAL TREASURY BANK commits to promoting financial literacy through workshops, seminars, and accessible online resources." << endl;
                        cout << "Encouraging informed decision-making and responsible financial behavior." << endl;
                        cout << endl;
                        cout << endl;
                        cout << endl;

                        cout << "CHAPTER 5: ENVIRONMENTAL AND SOCIAL RESPONSABILITY" << endl;
                        cout << endl;
                        cout << "SECTION 5.1: Sustainable Banking Practices" << endl;
                        cout << endl;
                        cout << "Integrating environmental and social responsibility into our business model." << endl;
                        cout << "Investing in eco-friendly initiatives and supporting community development projects." << endl;
                        cout << endl;
                        cout << endl;
                        cout << endl;

                        cout << "CHAPTER 6: THE FUTURE OF BANKING" << endl;
                        cout << endl;
                        cout << "SECTION 6.1: Technological Advancements" << endl;
                        cout << endl;
                        cout << "Embracing cutting-edge technology to enhance efficiency, security, and user experience." << endl;
                        cout << "Exploring blockchain, artificial intelligence, and other innovations to shape the future of banking." << endl;
                        cout << "SECTION 6.2: Continuous Improvement" << endl;
                        cout << endl;
                        cout << "A commitment to continuous improvement through client feedback, market analysis, and strategic partnerships." << endl;
                        cout << endl;
                        cout << endl;
                        cout << endl;

                        cout << "CHAPTER 7: CLIENT-CENTRIC APPROACH" << endl;
                        cout << endl;
                        cout << "SECTION 7.1: Personalized Customer Service" << endl;
                        cout << endl;
                        cout << "A dedicated customer support team ensuring a seamless and delightful banking experience." << endl;
                        cout << "Proactive resolution of issues and a commitment to exceeding client expectations." << endl;
                        cout << endl;
                        cout << endl;
                        cout << endl;

                        cout << "CHAPTER 8: THANKS FOR CHOOSING OUR BANK" << endl;
                        cout << endl;
                        cout << "In the ever-evolving world of finance, The CROSS-CONTINENTAL TREASURY BANK stands as a beacon of reliability, innovation, and commitment." << endl;
                        cout << " This compendium serves as a testament to our dedication to providing unparalleled financial services on a global scale. " << endl;
                        cout << " Join us in the pursuit of financial excellence, where your aspirations find a home in The CROSS-CONTINENTAL TREASURY BANK." << endl;

                        break;
                    }

                } while (options1);

                break;

            case 0: // Exit
                cout << "Thanks for having choose CROSS-CONTINENTAL TREASURY BANK, Have a Good Day" << endl;

                exit(1);

                break;
            }

        } while (options);

        connection->close();
        delete connection;
    }
    catch (sql ::SQLException &e)
    {
        cerr << "SQL ERROR: " << e.what() << endl;
    }
    catch (const std ::exception &e)
    {
        cout << "C++ ERROR: " << e.what() << endl;
    }
}
