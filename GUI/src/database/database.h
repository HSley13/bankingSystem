#pragma once
#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <vector>
#include <QWidget>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <argon2.h>

class connection_details
{
public:
    std ::string server, user, password;
};

sql ::Connection *connection_setup(connection_details *ID);

double check_balance(sql ::Connection *connection, int account_number);

void Qt_display_balance(sql ::Connection *connection, int account_number);

void call_insert_or_update_hashed_password(sql ::Connection *connection, int account_number, const std ::string hash_password);

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
class Transactions
{
public:
    static void deposit(sql ::Connection *connection, const double amount_to_deposit, int account_number);

    static void Qt_deposit(sql ::Connection *connection, const double amount_to_deposit, int account_number);

    static void withdrawal(sql ::Connection *connection, const double sum_to_withdraw, int account_number);

    static void Qt_withdrawal(sql ::Connection *connection, const double sum_to_withdraw, int account_number);

    static void transfer(sql ::Connection *connection, const double amount_to_send, int account_number1, int account_number2);

    static void Qt_transfer(sql ::Connection *connection, const double amount_to_send, int account_number1, int account_number2);

    static void borrow(sql ::Connection *connection, const double amount_to_borrow, int account_number);

    static void Qt_borrow(sql ::Connection *connection, const double amount_to_borrow, int account_number);

    static void display_transactions_history(sql ::Connection *connection, int account_number);

    static void Qt_display_transactions_history(sql ::Connection *connection, int account_number);

    static void insert_transactions(sql ::Connection *connection, int account_number, std ::string details, double amount);

    static void insert_borrowal(sql ::Connection *connection, int account_number, const double amount_to_borrow, const double borrowal_interest_rate);
};

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
class Account
{
public:
    static bool are_all_same(int phone_number);

    static void create_account(sql ::Connection *connection, int account_number, std ::string national_ID, std ::string first_name, std ::string last_name, std ::string date_birth, int phone_number, std ::string email, std ::string address, const double balance, const double interest_rate, std ::string hash_password);

    static void Qt_create_account(sql ::Connection *connection, int account_number, std ::string national_ID, std ::string first_name, std ::string last_name, std ::string date_birth, int phone_number, std ::string email, std ::string address, const double balance, const double interest_rate, std ::string hash_password);

    static void remove_accounts(sql ::Connection *connection, int account_number);

    static void Qt_remove_accounts(sql ::Connection *connection, int account_number);
};

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
class BANK : public Account, public Transactions
{
public:
    static std ::string generate_random_salt(size_t len);

    static std ::string hashing_password(std ::string &password);

    static bool verifying_password(std ::string password, std ::string &hashed_password);

    static std ::string retrieve_hashed_password(sql ::Connection *connection, int account_number);

    static std ::string Qt_retrieve_hashed_password(sql ::Connection *connection, int account_number);

    static std ::string retrieve_adm_hashed_password(sql ::Connection *connection, int account_number);

    static std ::string Qt_retrieve_adm_hashed_password(sql ::Connection *connection, int account_number);

    static std ::string retrieve_interest_rate_initial_timestamp(sql ::Connection *connection, int account_number);

    static int calculate_interest_rate_time_elapsed(sql ::Connection *connection, const std::string initial_timestamp);

    static void apply_interest_rate_to_balance(sql ::Connection *connection, int account_number);

    static bool authentification_check(sql ::Connection *connection, int account_number, std ::string national_ID, std ::string date_birth);

    static bool authentification_message(sql ::Connection *connection, int &account_number, std ::string &hash_password);

    static bool adm_authentification_message(sql ::Connection *connection, int &account_number, std ::string &hash_password);

    static void create_adm(sql ::Connection *connection, int account_number, std ::string hash_password);

    static void display_accounts_table(sql ::Connection *connection);

    static void Qt_display_accounts_table(sql ::Connection *connection);

    static void display_specific_accounts(sql ::Connection *connection, int account_number);

    static void Qt_display_specific_accounts(sql ::Connection *connection, int account_number);

    static void display_people_in_debt(sql ::Connection *connection);

    static void Qt_display_people_in_debt(sql ::Connection *connection);

    static void display_specific_accounts_in_debt(sql ::Connection *connection, int account_number);

    static void Qt_display_specific_accounts_in_debt(sql ::Connection *connection, int account_number);
};