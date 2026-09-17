//
// Created by LOQ on 9/17/2026.
//

#ifndef BANKMANAGEMENTSYSTEM_BANK_H
#define BANKMANAGEMENTSYSTEM_BANK_H

typedef struct
{
    int id;
    char name[50];
    char surname[50];
    double balance;
} Account;

typedef enum
{
    DEPOSIT,
    WITHDRAW,
    TRANSFER
} TransactionType;

typedef struct
{
    TransactionType type;

    int senderID;
    int receiverID;

    double amount;

    char senderName[50];
    char receiverName[50];

} Transaction;

/* Global variables */

extern Account *accounts;
extern Transaction *transactions;

extern int accountsCapacity;
extern int accountCount;
extern int nextID;

extern int transactionsCapacity;
extern int transactionCount;

/* Functions */

int findAccountIndex(int id);

void CreateTransaction(
    TransactionType type,
    int senderIndex,
    int receiverIndex,
    double amount
);

void CreateAccount(void);
void Deposit(void);
void Withdraw(void);
void CreateTransfer(void);

void ShowAccount(void);
void ListAccounts(void);
void DeleteAccount(void);

void ShowTransactionsHistory(void);


#endif //BANKMANAGEMENTSYSTEM_BANK_H
