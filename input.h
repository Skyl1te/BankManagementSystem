#ifndef INPUT_H
#define INPUT_H

int ReadInt(const char *message);
double ReadDouble(const char *message);
void ReadString(const char *message, char value[50]);
double ReadPositiveDouble(const char *message);

#endif