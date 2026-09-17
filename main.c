#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    char name[50];
    char surname[50];
    double balance;
} Account;

typedef enum {
    DEPOSIT = 0,
    WITHDRAW = 1,
    TRANSFER = 2
} TransactionType;

typedef struct {
    TransactionType type;

    int senderID;
    int receiverID;

    double amount;

    char senderName[50];
    char receiverName[50];

} Transaction;

Account *accounts = NULL;
Transaction *transactions = NULL;

int transactionsCapacity = 0;
int transactionCount = 0;

int accountsCapacity = 0;
int accountCount = 0;
int nextID = 1;

void (*BankAction)(void);

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

void CreateTransaction(TransactionType type, int senderIndex, int receiverIndex, double amount) {
    if (transactionCount >= transactionsCapacity) {
        int newTransactionsCapacity = 2 * transactionsCapacity;

        Transaction *temp = realloc(transactions, newTransactionsCapacity * sizeof(Transaction));
        if (temp == NULL) {
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

    strcpy(transaction.senderName, accounts[senderIndex].name);

    if (receiverIndex != -1) {
        transaction.receiverID = accounts[receiverIndex].id;
        strcpy(transaction.receiverName, accounts[receiverIndex].name);
    }
    else {
        transaction.receiverID = -1;
        strcpy(transaction.receiverName, "-");
    }

    transactions[transactionCount] = transaction;
    transactionCount++;
}

void createAccount(void)
{
    if (accountCount >= accountsCapacity) {
        int newAccountsCapacity = 2 * accountsCapacity;

        Account *temp = realloc(accounts, newAccountsCapacity * sizeof(Account));
        if (temp == NULL) {
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

    if (user.balance < 0) {
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
        "%d %s %s %.2f\n",
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

    CreateTransaction(DEPOSIT, index, -1, amount);
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

    CreateTransaction(WITHDRAW, index, -1, amount);

}

void CreateTransfer(void)
{
    int senderID, receiverID;
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
        printf("Invalid input.\n");
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

    CreateTransaction(TRANSFER, senderIndex, receiverIndex, amount);

}

void ShowAccount(void)
{
    int id;
    printf("Print ID of the user you wanna find: ");
    scanf("%d", &id);

    int index = findAccountIndex(id);

    if (index == -1) {
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
    printf("Print ID of the user you wanna delete: ");
    scanf("%d", &id);

    int index = findAccountIndex(id);

    if (index == -1)
    {
        printf("Account not found.\n");
        return;
    }

    for (int i = index; i < accountCount-1; i++)
    {
        accounts[i] = accounts[i+1];
    }

    accountCount--;
}

void ShowTransactionsHistory(void)
{
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

int main(void)
{

    accountsCapacity = 10;
    transactionsCapacity = 10;

    accounts = malloc(accountsCapacity * sizeof(Account));
    transactions = malloc(transactionsCapacity * sizeof(Transaction));
    if (accounts == NULL || transactions == NULL)
    {
        printf("Memory allocation failed.\n");

        free(accounts);
        free(transactions);

        return 1;
    }

    do
    {
        int choice;

        printf("\n\t=== Bank Management System ===\n");
        printf("1. Create account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Transfer\n");
        printf("5. Show account\n");
        printf("6. List accounts\n");
        printf("7. Delete account\n");
        printf("8. Transaction history\n");
        printf("0. Exit\n");
        printf("Enter: ");

        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
            {
                BankAction = createAccount;
                BankAction();
                break;
            }

            case 2:
            {
                BankAction = Deposit;
                BankAction();
                break;
            }

            case 3:
            {
                BankAction = Withdraw;
                BankAction();
                break;
            }

            case 4:
            {
                BankAction = CreateTransfer;
                BankAction();
                break;
            }

            case 5:
            {
                BankAction = ShowAccount;
                BankAction();
                break;
            }

            case 6:
            {
                BankAction = ListAccounts;
                BankAction();
                break;
            }

            case 7:
            {
                BankAction = DeleteAccount;
                BankAction();
                break;
            }

            case 8:
            {
                BankAction = ShowTransactionsHistory;
                BankAction();
                break;
            }

            case 0:
            {
                printf("Goodbye!\n");
                free(accounts);
                free(transactions);
                return 0;
            }

            default:
            {
                printf("Invalid option.\n");
                break;
            }
        }

    } while (1);
}