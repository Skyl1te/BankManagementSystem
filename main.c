#include <stdio.h>
#include <string.h>

typedef struct
{
    int id;
    char name[50];
    char surname[50];
    double balance;
} Account;

typedef struct {
    // int id;
    double balance;
    char senderName[50];
    char receiverName[50];
} Transaction;

Account accounts[100];
Transaction transactions[100];

int accountCount = 0;
int transactionCount = 0;
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

void createAccount(void)
{
    Account user;

    user.id = nextID;
    nextID++;

    printf("Enter name: ");
    scanf("%49s", user.name);

    printf("Enter surname: ");
    scanf("%49s", user.surname);

    printf("Enter balance: ");
    scanf("%lf", &user.balance);

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

    Transaction transaction;

    strcpy(transaction.senderName, accounts[senderIndex].name);
    strcpy(transaction.receiverName, accounts[receiverIndex].name);
    transaction.balance = amount;

    transactions[transactionCount] = transaction;
    transactionCount++;
}

void ShowAccount(void)
{
    int id;
    printf("Print ID of the user you wanna find: ");
    scanf("%d", &id);

    int index = findAccountIndex(id);

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
        printf("%s sent %.2lf to %s\n", transactions[i].senderName,
            transactions[i].balance,
            transactions[i].receiverName);
    }
}

int main(void)
{

    Account u1 = {1, "user1", "surname1", 3214.23};
    Account u2 = {2, "user2", "surname2", 1423};
    Account u3 = {3, "user3", "surname3", 32};
    Account u4 = {4, "user4", "surname4", 14.23};

    accounts[0] = u1;
    accounts[1] = u2;
    accounts[2] = u3;
    accounts[3] = u4;

    accountCount = 4;
    nextID = 5;

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
                return 0;
            }

            default:
            {
                printf("Invalid option.\n");
                break;
            }
        }

    } while (1);

    return 0;
}