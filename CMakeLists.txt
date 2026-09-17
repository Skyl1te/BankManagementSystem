cmake_minimum_required(VERSION 4.2)
project(BankManagementSystem C)

set(CMAKE_C_STANDARD 17)

add_executable(BankManagementSystem main.c
        bank.h
        bank.c)
