#include <stdio.h>
#include <string.h>

#include "bank.h"
#include "input.h"
#include "transaction.h"

void PrintTransactionTime(time_t timestamp)
{
    char *timeString = ctime(&timestamp);

    if (timeString == NULL)
    {
        printf("[Unknown time] ");
        return;
    }

    timeString[strlen(timeString) - 1] = '\0';

    printf("[%s] ", timeString);
}

void CreateTransaction(
    TransactionType type,
    int senderIndex,
    int receiverIndex,
    long long amount
)
{
    if (transactionCount >= transactionsCapacity)
    {
        if (!ResizeTransactions())
        {
            return;
        }
    }

    Transaction transaction;

    transaction.id = nextTransactionID;
    nextTransactionID++;

    transaction.timestamp = time(NULL);

    transaction.type = type;
    transaction.amount = amount;

    transaction.senderID = accounts[senderIndex].id;

    strcpy(
        transaction.senderName,
        accounts[senderIndex].name
    );

    if (receiverIndex != -1)
    {
        transaction.receiverID = accounts[receiverIndex].id;

        strcpy(
            transaction.receiverName,
            accounts[receiverIndex].name
        );
    }
    else
    {
        transaction.receiverID = -1;

        strcpy(
            transaction.receiverName,
            "-"
        );
    }

    transactions[transactionCount] = transaction;
    transactionCount++;
}

void ShowTransactionsHistory(void)
{
    if (transactionCount == 0)
    {
        printf("No transactions found.\n");
        return;
    }

    printf("\n=== Transaction History ===\n");

    for (int i = 0; i < transactionCount; i++)
    {
        PrintTransactionTime(transactions[i].timestamp);

        if (transactions[i].type == DEPOSIT)
        {
            printf(
                "Transaction ID: %d | DEPOSIT: %s (ID %d) deposited ",
                transactions[i].id,
                transactions[i].senderName,
                transactions[i].senderID
            );

            PrintMoney(transactions[i].amount);
            printf("\n");
        }
        else if (transactions[i].type == WITHDRAW)
        {
            printf(
                "Transaction ID: %d | WITHDRAW: %s (ID %d) withdrew ",
                transactions[i].id,
                transactions[i].senderName,
                transactions[i].senderID
            );

            PrintMoney(transactions[i].amount);
            printf("\n");
        }
        else if (transactions[i].type == TRANSFER)
        {
            printf(
                "Transaction ID: %d | TRANSFER: %s (ID %d) sent ",
                transactions[i].id,
                transactions[i].senderName,
                transactions[i].senderID
            );

            PrintMoney(transactions[i].amount);

            printf(
                " to %s (ID %d)\n",
                transactions[i].receiverName,
                transactions[i].receiverID
            );
        }
    }
}

void ShowAccountTransactions(void)
{
    int id = ReadInt("Enter account ID");
    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    int found = 0;

    printf("\n=== Account Transaction History ===\n");

    for (int i = 0; i < transactionCount; i++)
    {
        if (
            transactions[i].senderID == id ||
            transactions[i].receiverID == id
        )
        {
            found = 1;

            PrintTransactionTime(transactions[i].timestamp);

            if (transactions[i].type == DEPOSIT)
            {
                printf(
                    "Transaction ID: %d | DEPOSIT: %s (ID %d) deposited ",
                    transactions[i].id,
                    transactions[i].senderName,
                    transactions[i].senderID
                );

                PrintMoney(transactions[i].amount);
                printf("\n");
            }
            else if (transactions[i].type == WITHDRAW)
            {
                printf(
                    "Transaction ID: %d | WITHDRAW: %s (ID %d) withdrew ",
                    transactions[i].id,
                    transactions[i].senderName,
                    transactions[i].senderID
                );

                PrintMoney(transactions[i].amount);
                printf("\n");
            }
            else if (transactions[i].type == TRANSFER)
            {
                printf(
                    "Transaction ID: %d | TRANSFER: %s (ID %d) sent ",
                    transactions[i].id,
                    transactions[i].senderName,
                    transactions[i].senderID
                );

                PrintMoney(transactions[i].amount);

                printf(
                    " to %s (ID %d)\n",
                    transactions[i].receiverName,
                    transactions[i].receiverID
                );
            }
        }
    }

    if (!found)
    {
        printf("No transactions found for this account.\n");
    }
}