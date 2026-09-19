#include <stdio.h>
#include <stdlib.h>

#include "bank.h"

void (*BankAction)(void);

int main(void)
{
    accountsCapacity = 10;
    transactionsCapacity = 10;

    accounts = malloc(
        accountsCapacity * sizeof(Account)
    );

    transactions = malloc(
        transactionsCapacity * sizeof(Transaction)
    );

    if (accounts == NULL || transactions == NULL)
    {
        printf("Memory allocation failed.\n");

        free(accounts);
        free(transactions);

        return 1;
    }

    do
    {
        printf("\n\t=== Bank Management System ===\n");

        printf("1. Create account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Transfer\n");
        printf("5. Show account\n");
        printf("6. List accounts\n");
        printf("7. Delete account\n");
        printf("8. Transaction history\n");
        printf("9. Close account\n");
        printf("10. Open account\n");
        printf("0. Exit\n");

        int choice = ReadInt("Enter");

        switch (choice)
        {
            case 1:
                BankAction = CreateAccount;
                BankAction();
                break;

            case 2:
                BankAction = Deposit;
                BankAction();
                break;

            case 3:
                BankAction = Withdraw;
                BankAction();
                break;

            case 4:
                BankAction = CreateTransfer;
                BankAction();
                break;

            case 5:
                BankAction = ShowAccount;
                BankAction();
                break;

            case 6:
                BankAction = ListAccounts;
                BankAction();
                break;

            case 7:
                BankAction = DeleteAccount;
                BankAction();
                break;

            case 8:
                BankAction = ShowTransactionsHistory;
                BankAction();
                break;

            case 9:
                BankAction = CloseAccount;
                BankAction();
                break;

            case 10:
                BankAction = OpenAccount;
                BankAction();
                break;

            case 0:
                printf("Goodbye!\n");

                free(accounts);
                free(transactions);

                return 0;

            default:
                printf("Invalid option.\n");
                break;
        }

    } while (1);
}