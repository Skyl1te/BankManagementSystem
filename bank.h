//
// Created by LOQ on 9/17/2026.
//

#ifndef BANK_H
#define BANK_H
#include <stdbool.h>

typedef struct
{
    int id;
    char name[50];
    char surname[50];
    double balance;
    bool status;
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


/* Input */

int ReadInt(const char *message);
double ReadDouble(const char *message);
void ReadString(const char *message, char value[50]);


/* Memory */

int ResizeAccounts(void);
int ResizeTransactions(void);


/* Account helpers */

int findAccountIndex(int id);
bool CheckAccountStatus(int accountIndex);


/* Account functions */

void CreateAccount(void);
void Deposit(void);
void Withdraw(void);
void CreateTransfer(void);

void ShowAccount(void);
void ListAccounts(void);

void DeleteAccount(void);
void CloseAccount(void);
void OpenAccount(void);

void EditAccount(void);

/* Transactions */

void CreateTransaction(
    TransactionType type,
    int senderIndex,
    int receiverIndex,
    double amount
);

void ShowTransactionsHistory(void);

#endif //BANKMANAGEMENTSYSTEM_BANK_H
