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


/* Input helpers */

int ReadInt(const char *message)
{
    int value;

    printf("%s: ", message);

    while (scanf("%d", &value) != 1)
    {
        printf("Invalid input. Enter a number: ");

        while (getchar() != '\n')
        {
        }
    }

    return value;
}


double ReadDouble(const char *message)
{
    double value;

    printf("%s: ", message);

    while (scanf("%lf", &value) != 1)
    {
        printf("Invalid input. Enter a number: ");

        while (getchar() != '\n')
        {
        }
    }

    return value;
}


void ReadString(const char *message, char value[50])
{
    printf("%s: ", message);

    scanf("%49s", value);
}


/* Memory management */

int ResizeAccounts(void)
{
    int newCapacity;

    if (accountsCapacity == 0)
    {
        newCapacity = 10;
    }
    else
    {
        newCapacity = accountsCapacity * 2;
    }

    Account *temp = realloc(
        accounts,
        newCapacity * sizeof(Account)
    );

    if (temp == NULL)
    {
        printf("Failed to expand accounts memory.\n");
        return 0;
    }

    accounts = temp;
    accountsCapacity = newCapacity;

    return 1;
}


int ResizeTransactions(void)
{
    int newCapacity;

    if (transactionsCapacity == 0)
    {
        newCapacity = 10;
    }
    else
    {
        newCapacity = transactionsCapacity * 2;
    }

    Transaction *temp = realloc(
        transactions,
        newCapacity * sizeof(Transaction)
    );

    if (temp == NULL)
    {
        printf("Failed to expand transactions memory.\n");
        return 0;
    }

    transactions = temp;
    transactionsCapacity = newCapacity;

    return 1;
}


/* Account helpers */

/* Returns the index of the account, or -1 if it does not exist. */
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


/* Returns true if the account is active. */
bool CheckAccountStatus(int accountIndex)
{
    if (!accounts[accountIndex].status)
    {
        printf(
            "Account %d is closed.\n",
            accounts[accountIndex].id
        );

        return false;
    }

    return true;
}


/* Transaction operations */

void CreateTransaction(
    TransactionType type,
    int senderIndex,
    int receiverIndex,
    double amount
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


/* Account operations */

void CreateAccount(void)
{
    if (accountCount >= accountsCapacity)
    {
        if (!ResizeAccounts())
        {
            return;
        }
    }

    Account user;

    ReadString("Enter name", user.name);
    ReadString("Enter surname", user.surname);

    user.balance = ReadDouble("Enter balance");

    if (user.balance < 0)
    {
        printf("Invalid balance.\n");
        return;
    }

    user.id = nextID;
    user.status = true;

    nextID++;

    accounts[accountCount] = user;
    accountCount++;

    printf("Account created successfully.\n");
    printf("Account ID: %d\n", user.id);

    printf(
        "ID: %d, Name: %s, Surname: %s, Balance: %.2f, Status: ACTIVE\n",
        user.id,
        user.name,
        user.surname,
        user.balance
    );
}

void EditAccount(void)
{
    int id = ReadInt("Enter account ID you want to edit");
    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    int choice = ReadInt(
        "What do you want to edit?\n"
        "1. Name\n"
        "2. Surname\n"
        "3. Name and surname\n"
        "Your choice"
    );

    switch (choice)
    {
        case 1:
        {
            ReadString(
                "Enter new name",
                accounts[index].name
            );

            printf("Name updated successfully.\n");
            break;
        }

        case 2:
        {
            ReadString(
                "Enter new surname",
                accounts[index].surname
            );

            printf("Surname updated successfully.\n");
            break;
        }

        case 3:
        {
            ReadString(
                "Enter new name",
                accounts[index].name
            );

            ReadString(
                "Enter new surname",
                accounts[index].surname
            );

            printf("Name and surname updated successfully.\n");
            break;
        }

        default:
        {
            printf("Invalid option.\n");
            break;
        }
    }
}

void Deposit(void)
{
    int id = ReadInt("Enter account ID");

    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    if (!CheckAccountStatus(index))
    {
        return;
    }

    double amount = ReadDouble("Enter amount");

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
    int id = ReadInt("Enter account ID");

    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    if (!CheckAccountStatus(index))
    {
        return;
    }

    double amount = ReadDouble("Enter amount");

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
    int senderID = ReadInt("Sender ID");
    int receiverID = ReadInt("Receiver ID");

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

    if (!CheckAccountStatus(senderIndex))
    {
        return;
    }

    if (receiverIndex == -1)
    {
        printf("Invalid receiver ID.\n");
        return;
    }

    if (!CheckAccountStatus(receiverIndex))
    {
        return;
    }

    double amount = ReadDouble("Enter the amount to send");

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
    int id = ReadInt("Enter account ID");

    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    printf(
        "ID: %d, Name: %s, Surname: %s, Balance: %.2f, Status: %s\n",
        accounts[index].id,
        accounts[index].name,
        accounts[index].surname,
        accounts[index].balance,
        accounts[index].status ? "ACTIVE" : "CLOSED"
    );
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

    if (!found)
    {
        printf("No transactions found for this account.\n");
    }
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
            "ID: %d, Name: %s, Surname: %s, Balance: %.2f, Status: %s\n",
            accounts[i].id,
            accounts[i].name,
            accounts[i].surname,
            accounts[i].balance,
            accounts[i].status ? "ACTIVE" : "CLOSED"
        );
    }
}


void DeleteAccount(void)
{
    int id = ReadInt(
        "Enter ID of the account you want to delete"
    );

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


void CloseAccount(void)
{
    int id = ReadInt(
        "Enter ID of the account you want to close"
    );

    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    if (!accounts[index].status)
    {
        printf("Account is already closed.\n");
        return;
    }

    accounts[index].status = false;

    printf("Account closed successfully.\n");
}


void OpenAccount(void)
{
    int id = ReadInt(
        "Enter ID of the account you want to open"
    );

    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    if (accounts[index].status)
    {
        printf("Account is already opened.\n");
        return;
    }

    accounts[index].status = true;

    printf("Account opened successfully.\n");
}


/* Transaction history */

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