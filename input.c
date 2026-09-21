#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "input.h"

int ReadInt(const char *message)
{
    char input[100];
    int value;

    while (true)
    {
        printf("%s: ", message);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Input error.\n");
            continue;
        }

        if (sscanf(input, "%d", &value) == 1)
        {
            return value;
        }

        printf("Invalid input. Enter a number.\n");
    }
}

double ReadDouble(const char *message)
{
    char input[100];
    double value;

    while (true)
    {
        printf("%s: ", message);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Input error.\n");
            continue;
        }

        if (sscanf(input, "%lf", &value) == 1)
        {
            return value;
        }

        printf("Invalid input. Enter a number.\n");
    }
}

void ReadString(const char *message, char value[50])
{
    printf("%s: ", message);

    if (fgets(value, 50, stdin) == NULL)
    {
        value[0] = '\0';
        return;
    }

    value[strcspn(value, "\n")] = '\0';
}

long long ReadMoney(const char *message)
{
    double value = ReadDouble(message);

    if (value >= 0)
    {
        return (long long)(value * 100 + 0.5);
    }

    return (long long)(value * 100 - 0.5);
}

long long ReadPositiveMoney(const char *message)
{
    while (true)
    {
        long long value = ReadMoney(message);

        if (value > 0)
        {
            return value;
        }

        printf("Value must be greater than 0.\n");
    }
}

void PrintMoney(long long amount)
{
    if (amount < 0)
    {
        printf("-");
        amount = -amount;
    }

    printf("%lld.%02lld", amount / 100, amount % 100);
}