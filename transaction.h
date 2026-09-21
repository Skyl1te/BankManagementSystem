#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "bank.h"

void PrintTransactionTime(time_t timestamp);

void CreateTransaction(
    TransactionType type,
    int senderIndex,
    int receiverIndex,
    long long amount
);

void ShowTransactionsHistory(void);
void ShowAccountTransactions(void);

bool EnsureTransactionCapacity(void);

#endif