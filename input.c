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

double ReadPositiveDouble(const char *message)
{
    while (true)
    {
        double value = ReadDouble(message);

        if (value > 0)
        {
            return value;
        }

        printf("Value must be greater than 0.\n");
    }
}