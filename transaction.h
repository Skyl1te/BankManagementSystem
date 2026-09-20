#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "bank.h"

void PrintTransactionTime(time_t timestamp);

void CreateTransaction(
    TransactionType type,
    int senderIndex,
    int receiverIndex,
    double amount
);

void ShowTransactionsHistory(void);
void ShowAccountTransactions(void);

#endif