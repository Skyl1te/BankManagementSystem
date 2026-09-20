//
// Created by LOQ on 9/17/2026.
//

#include <stdio.h>
#include <stdlib.h>

#include "bank.h"
#include "input.h"
#include "transaction.h"

Account *accounts = NULL;
Transaction *transactions = NULL;

int accountsCapacity = 0;
int accountCount = 0;
int nextID = 1;

int transactionsCapacity = 0;
int transactionCount = 0;
int nextTransactionID = 1;


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

    Account *temp = realloc(accounts, newCapacity * sizeof(Account));

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

bool CheckAccountStatus(int accountIndex)
{
    if (!accounts[accountIndex].status)
    {
        printf("Account %d is closed.\n", accounts[accountIndex].id);
        return false;
    }

    return true;
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

    while (true)
    {
        user.balance = ReadDouble("Enter balance");

        if (user.balance >= 0)
        {
            break;
        }

        printf("Balance cannot be negative.\n");
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

    int choice;

    while (true)
    {
        choice = ReadInt(
            "What do you want to edit?\n"
            "1. Name\n"
            "2. Surname\n"
            "3. Name and surname\n"
            "Your choice"
        );

        if (choice >= 1 && choice <= 3)
        {
            break;
        }

        printf("Invalid option.\n");
    }

    switch (choice)
    {
        case 1:
        {
            ReadString("Enter new name", accounts[index].name);
            printf("Name updated successfully.\n");
            break;
        }

        case 2:
        {
            ReadString("Enter new surname", accounts[index].surname);
            printf("Surname updated successfully.\n");
            break;
        }

        case 3:
        {
            ReadString("Enter new name", accounts[index].name);
            ReadString("Enter new surname", accounts[index].surname);

            printf("Name and surname updated successfully.\n");
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

    double amount = ReadPositiveDouble("Enter amount");

    accounts[index].balance += amount;

    printf("Deposit successful.\n");
    printf("New balance: %.2f\n", accounts[index].balance);

    CreateTransaction(DEPOSIT, index, -1, amount);
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

    double amount;

    while (true)
    {
        amount = ReadPositiveDouble("Enter amount");

        if (amount <= accounts[index].balance)
        {
            break;
        }

        printf("Not enough money.\n");
    }

    accounts[index].balance -= amount;

    printf("Withdraw successful.\n");
    printf("New balance: %.2f\n", accounts[index].balance);

    CreateTransaction(WITHDRAW, index, -1, amount);
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

    double amount;

    while (true)
    {
        amount = ReadPositiveDouble("Enter the amount to send");

        if (amount <= accounts[senderIndex].balance)
        {
            break;
        }

        printf("You don't have enough money to send.\n");
    }

    accounts[senderIndex].balance -= amount;
    accounts[receiverIndex].balance += amount;

    printf("Transfer successful.\n");
    printf("Sender balance: %.2f\n", accounts[senderIndex].balance);
    printf("Receiver balance: %.2f\n", accounts[receiverIndex].balance);

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
    int id = ReadInt("Enter ID of the account you want to delete");
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
    int id = ReadInt("Enter ID of the account you want to close");
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
    int id = ReadInt("Enter ID of the account you want to open");
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


/* Bank statistics */

void ShowBankStatistics(void)
{
    if (accountCount <= 0)
    {
        printf("No accounts found.\n");
        return;
    }

    int activeAccounts = 0;
    int closedAccounts = 0;

    double totalBalance = 0.0;
    double maxBalance = accounts[0].balance;

    int maxBalanceIndex = 0;

    for (int i = 0; i < accountCount; i++)
    {
        if (accounts[i].status)
        {
            activeAccounts++;
        }
        else
        {
            closedAccounts++;
        }

        totalBalance += accounts[i].balance;

        if (accounts[i].balance > maxBalance)
        {
            maxBalance = accounts[i].balance;
            maxBalanceIndex = i;
        }
    }

    double averageBalance = totalBalance / accountCount;

    printf("\n=== Bank Statistics ===\n");
    printf("Total accounts: %d\n", accountCount);
    printf("Active accounts: %d\n", activeAccounts);
    printf("Closed accounts: %d\n", closedAccounts);
    printf("Total balance: %.2f\n", totalBalance);
    printf("Average balance: %.2f\n", averageBalance);

    printf(
        "Highest balance: %.2f - %s %s (ID %d)\n",
        maxBalance,
        accounts[maxBalanceIndex].name,
        accounts[maxBalanceIndex].surname,
        accounts[maxBalanceIndex].id
    );

    printf("Total transactions: %d\n", transactionCount);
}