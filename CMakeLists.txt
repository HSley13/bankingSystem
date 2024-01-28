cmake_minimum_required(VERSION 3.28.1)
project(BankingSystem)

set(CMAKE_CXX_STANDARD 17)

add_executable(BankingSystem main.cpp)
target_link_libraries(BankingSystem argon2 ssl crypto mysqlcppconn)

target_link_directories(BankingSystem PRIVATE /opt/homebrew/lib)
target_link_directories(BankingSystem PRIVATE /usr/local/mysql-connector-c++-8.2.0/lib64)

target_include_directories(BankingSystem PRIVATE /opt/homebrew/include)
target_include_directories(BankingSystem PRIVATE /usr/local/mysql-connector-c++-8.2.0/include/jdbc)
