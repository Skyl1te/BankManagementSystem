#include <stdio.h>

#include "bank.h"
#include "storage.h"

void SaveData(void)
{
    FILE *file = fopen("bank.dat", "wb");

    if (file == NULL)
    {
        printf("Failed to save data.\n");
        return;
    }

    fwrite(&accountCount, sizeof(int), 1, file);
    fwrite(&nextID, sizeof(int), 1, file);
    fwrite(accounts, sizeof(Account), accountCount, file);

    fwrite(&transactionCount, sizeof(int), 1, file);
    fwrite(&nextTransactionID, sizeof(int), 1, file);
    fwrite(transactions, sizeof(Transaction), transactionCount, file);

    fclose(file);

    printf("Data saved successfully.\n");
}

void LoadData(void)
{
    FILE *file = fopen("bank.dat", "rb");

    if (file == NULL)
    {
        return;
    }

    fread(&accountCount, sizeof(int), 1, file);
    fread(&nextID, sizeof(int), 1, file);

    while (accountsCapacity < accountCount)
    {
        if (!ResizeAccounts())
        {
            fclose(file);
            return;
        }
    }

    fread(accounts, sizeof(Account), accountCount, file);

    fread(&transactionCount, sizeof(int), 1, file);
    fread(&nextTransactionID, sizeof(int), 1, file);

    while (transactionsCapacity < transactionCount)
    {
        if (!ResizeTransactions())
        {
            fclose(file);
            return;
        }
    }

    fread(transactions, sizeof(Transaction), transactionCount, file);

    fclose(file);

    printf("Data loaded successfully.\n");
}