#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "bank.h"

void PrintTransactionTime(time_t timestamp);

bool EnsureTransactionCapacity(void);

void CreateTransaction(
    TransactionType type,
    int senderIndex,
    int receiverIndex,
    long long amount
);

void ShowTransactionsHistory(void);
void ShowAccountTransactions(void);

#endif