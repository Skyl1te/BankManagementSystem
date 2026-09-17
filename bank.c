//
// Created by LOQ on 9/17/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bank.h"

Account *accounts = NULL;
Transaction *transactions = NULL;

int accountsCapacity = 0;
int accountCount = 0;
int nextID = 1;

int transactionsCapacity = 0;
int transactionCount = 0;


int findAccountIndex(int id)
{
    for (int i = 0; i < accountCount; i++)
    {
        if (accounts[i].id == id)
        {
            return i;
        }
    }

    return -1;
}


void CreateTransaction(
    TransactionType type,
    int senderIndex,
    int receiverIndex,
    double amount
)
{
    if (transactionCount >= transactionsCapacity)
    {
        int newTransactionsCapacity = transactionsCapacity * 2;

        Transaction *temp = realloc(
            transactions,
            newTransactionsCapacity * sizeof(Transaction)
        );

        if (temp == NULL)
        {
            printf("Failed to expand transactions memory.\n");
            return;
        }

        transactions = temp;
        transactionsCapacity = newTransactionsCapacity;
    }

    Transaction transaction;

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


void CreateAccount(void)
{
    if (accountCount >= accountsCapacity)
    {
        int newAccountsCapacity = accountsCapacity * 2;

        Account *temp = realloc(
            accounts,
            newAccountsCapacity * sizeof(Account)
        );

        if (temp == NULL)
        {
            printf("Failed to expand accounts memory.\n");
            return;
        }

        accounts = temp;
        accountsCapacity = newAccountsCapacity;
    }

    Account user;

    printf("Enter name: ");
    scanf("%49s", user.name);

    printf("Enter surname: ");
    scanf("%49s", user.surname);

    printf("Enter balance: ");
    scanf("%lf", &user.balance);

    if (user.balance < 0)
    {
        printf("Invalid balance.\n");
        return;
    }

    user.id = nextID;
    nextID++;

    accounts[accountCount] = user;
    accountCount++;

    printf("Account created successfully.\n");
    printf("Account ID: %d\n", user.id);

    printf(
        "ID: %d, Name: %s, Surname: %s, Balance: %.2f\n",
        user.id,
        user.name,
        user.surname,
        user.balance
    );
}


void Deposit(void)
{
    int id;
    double amount;

    printf("Enter account ID: ");
    scanf("%d", &id);

    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter amount: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount.\n");
        return;
    }

    accounts[index].balance += amount;

    printf("Deposit successful.\n");
    printf("New balance: %.2f\n", accounts[index].balance);

    CreateTransaction(
        DEPOSIT,
        index,
        -1,
        amount
    );
}


void Withdraw(void)
{
    int id;
    double amount;

    printf("Enter account ID: ");
    scanf("%d", &id);

    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter amount: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount.\n");
        return;
    }

    if (amount > accounts[index].balance)
    {
        printf("Not enough money.\n");
        return;
    }

    accounts[index].balance -= amount;

    printf("Withdraw successful.\n");
    printf("New balance: %.2f\n", accounts[index].balance);

    CreateTransaction(
        WITHDRAW,
        index,
        -1,
        amount
    );
}


void CreateTransfer(void)
{
    int senderID;
    int receiverID;

    double amount;

    printf("Sender ID: ");
    scanf("%d", &senderID);

    printf("Receiver ID: ");
    scanf("%d", &receiverID);

    if (senderID == receiverID)
    {
        printf("Can't send to the same account.\n");
        return;
    }

    int senderIndex = findAccountIndex(senderID);
    int receiverIndex = findAccountIndex(receiverID);

    if (senderIndex == -1)
    {
        printf("Invalid sender ID.\n");
        return;
    }

    if (receiverIndex == -1)
    {
        printf("Invalid receiver ID.\n");
        return;
    }

    printf("Enter the amount to send: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount.\n");
        return;
    }

    if (amount > accounts[senderIndex].balance)
    {
        printf("You don't have enough money to send.\n");
        return;
    }

    accounts[senderIndex].balance -= amount;
    accounts[receiverIndex].balance += amount;

    printf("Transfer successful.\n");

    printf(
        "Sender balance: %.2f\n",
        accounts[senderIndex].balance
    );

    printf(
        "Receiver balance: %.2f\n",
        accounts[receiverIndex].balance
    );

    CreateTransaction(
        TRANSFER,
        senderIndex,
        receiverIndex,
        amount
    );
}


void ShowAccount(void)
{
    int id;

    printf("Enter account ID: ");
    scanf("%d", &id);

    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    printf(
        "ID: %d, Name: %s, Surname: %s, Balance: %.2f\n",
        accounts[index].id,
        accounts[index].name,
        accounts[index].surname,
        accounts[index].balance
    );
}


void ListAccounts(void)
{
    if (accountCount == 0)
    {
        printf("No accounts found.\n");
        return;
    }

    printf("\n=== Accounts ===\n");

    for (int i = 0; i < accountCount; i++)
    {
        printf(
            "ID: %d, Name: %s, Surname: %s, Balance: %.2f\n",
            accounts[i].id,
            accounts[i].name,
            accounts[i].surname,
            accounts[i].balance
        );
    }
}


void DeleteAccount(void)
{
    int id;

    printf("Enter ID of the account you want to delete: ");
    scanf("%d", &id);

    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    for (int i = index; i < accountCount - 1; i++)
    {
        accounts[i] = accounts[i + 1];
    }

    accountCount--;

    printf("Account deleted successfully.\n");
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
        if (transactions[i].type == DEPOSIT)
        {
            printf(
                "DEPOSIT: %s (ID %d) deposited %.2f\n",
                transactions[i].senderName,
                transactions[i].senderID,
                transactions[i].amount
            );
        }
        else if (transactions[i].type == WITHDRAW)
        {
            printf(
                "WITHDRAW: %s (ID %d) withdrew %.2f\n",
                transactions[i].senderName,
                transactions[i].senderID,
                transactions[i].amount
            );
        }
        else if (transactions[i].type == TRANSFER)
        {
            printf(
                "TRANSFER: %s (ID %d) sent %.2f to %s (ID %d)\n",
                transactions[i].senderName,
                transactions[i].senderID,
                transactions[i].amount,
                transactions[i].receiverName,
                transactions[i].receiverID
            );
        }
    }
}