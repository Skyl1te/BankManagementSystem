#ifndef BANK_H
#define BANK_H

#include <stdbool.h>
#include <time.h>

typedef struct
{
    int id;
    char name[50];
    char surname[50];
    long long balance;
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
    int id;
    TransactionType type;
    int senderID;
    int receiverID;
    long long amount;
    char senderName[50];
    char receiverName[50];
    time_t timestamp;
} Transaction;

extern Account *accounts;
extern Transaction *transactions;

extern int accountsCapacity;
extern int accountCount;
extern int nextID;

extern int transactionsCapacity;
extern int transactionCount;
extern int nextTransactionID;

int ResizeAccounts(void);
int ResizeTransactions(void);

int findAccountIndex(int id);
bool CheckAccountStatus(int accountIndex);

void CreateAccount(void);
void EditAccount(void);
void Deposit(void);
void Withdraw(void);
void CreateTransfer(void);

void ShowAccount(void);
void ListAccounts(void);
void ShowAccountsByName(void);

void DeleteAccount(void);
void CloseAccount(void);
void OpenAccount(void);

void ShowBankStatistics(void);

void SortAccounts(void);

#endif