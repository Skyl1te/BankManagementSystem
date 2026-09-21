#ifndef INPUT_H
#define INPUT_H

int ReadInt(const char *message);
double ReadDouble(const char *message);

void ReadString(const char *message, char value[50]);

long long ReadMoney(const char *message);
long long ReadPositiveMoney(const char *message);

void PrintMoney(long long amount);

#endif