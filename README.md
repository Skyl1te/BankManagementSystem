# Bank Management System

A console-based banking management system written in C.

The project demonstrates practical usage of structures, pointers, dynamic memory, file handling, function pointers, modular programming, transaction history, sorting, searching, and persistent data storage.

The system allows users to create and manage bank accounts, perform transactions, view statistics, search and sort accounts, and automatically save data between program launches.

---

## Features

### Account Management

- Create new bank accounts
- Edit account holder name and surname
- View account information
- List all accounts
- Search accounts by name or surname
- Delete accounts
- Open and close accounts
- Unique account IDs

### Banking Operations

- Deposit money
- Withdraw money
- Transfer money between accounts
- Prevent transactions on closed accounts
- Prevent transfers to the same account
- Validate available balance before withdrawals and transfers

### Transaction System

Every banking operation is stored in transaction history.

Supported transaction types:

- Deposit
- Withdraw
- Transfer

Each transaction contains:

- Unique transaction ID
- Transaction type
- Sender account ID
- Receiver account ID
- Sender name
- Receiver name
- Amount
- Date and time

The program supports:

- Global transaction history
- Transaction history for a specific account

### Account Sorting

Accounts can be sorted by:

- ID
- Name
- Balance

Sorting order can be selected:

- Ascending
- Descending

Sorting is implemented using the standard C `qsort()` function.

### Bank Statistics

The program can display:

- Total number of accounts
- Number of active accounts
- Number of closed accounts
- Total balance
- Average balance
- Account with the highest balance
- Total number of transactions

### Persistent Storage

Account and transaction data is saved to a binary file:

```text
bank.dat
```

The program automatically loads saved data when it starts and saves current data when the user exits.

Stored information includes:

- Accounts
- Transactions
- Next account ID
- Next transaction ID

This means account and transaction data remains available between program launches.

---

## Money Representation

Money is stored internally using `long long` instead of `double`.

For example:

```text
100.50
```

is stored internally as:

```text
10050
```

This represents the amount in cents.

Using integer values avoids common floating-point precision problems when performing financial calculations.

Example:

```text
10.50  -> 1050 cents
0.99   -> 99 cents
125.00 -> 12500 cents
```

---

## Project Structure

```text
BankManagementSystem/
│
├── main.c
│
├── bank.c
├── bank.h
│
├── input.c
├── input.h
│
├── transaction.c
├── transaction.h
│
├── storage.c
├── storage.h
│
├── CMakeLists.txt
├── .gitignore
├── LICENSE
└── README.md
```

---

## File Responsibilities

### `main.c`

Contains:

- Program entry point
- Main menu
- Dynamic memory initialization
- Function pointer used for menu actions
- Loading data on startup
- Saving data before exit
- Memory cleanup before program termination

### `bank.c` / `bank.h`

Contains the main banking logic:

- Account creation
- Account editing
- Deposits
- Withdrawals
- Transfers
- Account search
- Account deletion
- Account status management
- Bank statistics
- Account sorting
- Dynamic memory resizing

### `input.c` / `input.h`

Contains reusable input and output helpers:

- Integer input
- Floating-point input
- String input
- Money input
- Positive money validation
- Money formatting

### `transaction.c` / `transaction.h`

Contains transaction-related logic:

- Transaction creation
- Transaction IDs
- Transaction timestamps
- Transaction history
- Account-specific transaction history
- Transaction array capacity checks

### `storage.c` / `storage.h`

Handles persistent storage:

- Saving data to `bank.dat`
- Loading data from `bank.dat`
- Restoring accounts
- Restoring transactions
- Restoring ID counters

---

## Data Structures

### Account

```c
typedef struct
{
    int id;
    char name[50];
    char surname[50];
    long long balance;
    bool status;
} Account;
```

Each account contains:

- Unique ID
- Name
- Surname
- Balance
- Account status

---

### Transaction

```c
typedef struct
{
    int id;
    TransactionType type;
    int senderID;
    int receiverID;
    long long amount;
    char senderName[50];
    char receiverName[50];
    time_t timestamp;
} Transaction;
```

Each transaction stores a snapshot of the transaction participants and operation data.

This allows transaction history to remain readable even if an account is later deleted.

---

## Transaction Types

Transactions are represented using an enum:

```c
typedef enum
{
    DEPOSIT,
    WITHDRAW,
    TRANSFER
} TransactionType;
```

This makes the transaction logic easier to read and maintain.

---

## Dynamic Memory

Accounts and transactions are stored in dynamically allocated arrays.

Initial capacity:

```text
10 accounts
10 transactions
```

When an array becomes full, its capacity is doubled using `realloc()`.

Example:

```text
10 -> 20 -> 40 -> 80 -> ...
```

This allows the program to support a growing number of accounts and transactions without using fixed-size arrays.

---

## Transaction Safety

Before changing account balances, the program checks whether enough memory is available to store the corresponding transaction.

The operation follows this order:

```text
Validate account
        |
        v
Validate amount
        |
        v
Ensure transaction capacity
        |
        v
Update balance
        |
        v
Create transaction
```

This prevents situations where a balance changes but the transaction cannot be stored because memory allocation failed.

---

## Account Status

Accounts can have two states:

```text
ACTIVE
CLOSED
```

Closed accounts cannot:

- Deposit money
- Withdraw money
- Send transfers
- Receive transfers

Accounts can later be reopened.

---

## Account Search

Accounts can be searched by:

- Name
- Surname

The program compares the entered value with existing account data and displays all matching accounts.

---

## Account Sorting

The program supports sorting by:

```text
1. ID
2. Name
3. Balance
```

The user can also select sorting direction:

```text
1. Ascending
2. Descending
```

Examples:

Ascending by balance:

```text
10.00
50.00
100.00
500.00
```

Descending by balance:

```text
500.00
100.00
50.00
10.00
```

Sorting is implemented using `qsort()`.

---

## Bank Statistics

The statistics system calculates:

```text
Total accounts
Active accounts
Closed accounts
Total balance
Average balance
Highest balance
Total transactions
```

Example:

```text
=== Bank Statistics ===
Total accounts: 3
Active accounts: 2
Closed accounts: 1
Total balance: 2750.50
Average balance: 916.83
Highest balance: 1500.00 - John Smith (ID 2)
Total transactions: 6
```

---

## Persistent Storage

The program uses binary file storage.

The data file is:

```text
bank.dat
```

The program saves:

```text
accountCount
nextID
accounts
transactionCount
nextTransactionID
transactions
```

---

## Save Flow

When the user exits:

```text
Exit selected
      |
      v
SaveData()
      |
      v
Write account data
      |
      v
Write transaction data
      |
      v
Close file
      |
      v
Free memory
      |
      v
Program ends
```

---

## Load Flow

When the program starts:

```text
Program starts
      |
      v
Allocate memory
      |
      v
LoadData()
      |
      v
Open bank.dat
      |
      v
Restore accounts
      |
      v
Restore transactions
      |
      v
Main menu
```

If `bank.dat` does not exist yet, the program simply starts with empty account and transaction lists.

---

## Example Menu

```text
=== Bank Management System ===

1. Create account
2. Deposit
3. Withdraw
4. Transfer
5. Show account
6. List accounts
7. Delete account
8. Transaction history
9. Close account
10. Open account
11. Edit account
12. Account transaction history
13. Bank statistics
14. Search account
15. Sort accounts
0. Exit
```

---

## Example Account

```text
ID: 1, Name: John, Surname: Smith, Balance: 1250.50, Status: ACTIVE
```

---

## Example Transaction History

```text
[Mon Sep 21 15:32:14 2026] Transaction ID: 1 | DEPOSIT: John (ID 1) deposited 500.00
[Mon Sep 21 15:35:41 2026] Transaction ID: 2 | WITHDRAW: John (ID 1) withdrew 100.00
[Mon Sep 21 15:40:03 2026] Transaction ID: 3 | TRANSFER: John (ID 1) sent 250.00 to Alex (ID 2)
```

---

## Building the Project

The project uses CMake.

### Requirements

- C compiler with C11 support
- CMake 3.20 or newer

Compatible compilers include:

- GCC
- Clang
- MSVC

---

## Build with CMake

From the project directory:

```bash
cmake -S . -B build
cmake --build build
```

Then run the generated executable.

On Linux or macOS:

```bash
./build/BankManagementSystem
```

On Windows, depending on the CMake generator, the executable may be located in:

```text
build/Debug/
```

or:

```text
build/Release/
```

---

## CMake Configuration

```cmake
cmake_minimum_required(VERSION 3.20)

project(BankManagementSystem C)

set(CMAKE_C_STANDARD 11)

add_executable(
    BankManagementSystem
    main.c
    bank.c
    input.c
    transaction.c
    storage.c
)
```

---

## Concepts Used

This project demonstrates practical usage of many fundamental C concepts:

- Structures
- Enums
- Pointers
- Function pointers
- Dynamic memory allocation
- `malloc()`
- `realloc()`
- `free()`
- Header files
- Multiple source files
- Include guards
- Modular programming
- File I/O
- Binary files
- `fopen()`
- `fread()`
- `fwrite()`
- `fclose()`
- String handling
- `strcmp()`
- `strcpy()`
- `fgets()`
- `sscanf()`
- Time handling
- `time_t`
- `time()`
- `ctime()`
- Sorting with `qsort()`
- Input validation
- Dynamic arrays
- Persistent storage

---

## Current Architecture

The project is separated into modules based on responsibility:

```text
main
 |
 +---- bank
 |      |
 |      +---- account operations
 |      +---- statistics
 |      +---- sorting
 |      +---- memory management
 |
 +---- input
 |      |
 |      +---- user input
 |      +---- money parsing
 |      +---- money output
 |
 +---- transaction
 |      |
 |      +---- transaction creation
 |      +---- transaction history
 |      +---- timestamps
 |      +---- transaction memory checks
 |
 +---- storage
        |
        +---- save data
        +---- load data
```

This separation keeps unrelated logic out of `main.c` and makes the project easier to maintain and extend.

---

## Memory Management

The program allocates memory for accounts and transactions when it starts:

```c
accounts = malloc(accountsCapacity * sizeof(Account));
transactions = malloc(transactionsCapacity * sizeof(Transaction));
```

When more space is required, the arrays are expanded using `realloc()`.

Before program termination, allocated memory is released using:

```c
free(accounts);
free(transactions);
```

This prevents memory leaks during normal program execution.

---

## Function Pointer

The main menu uses a function pointer:

```c
void (*BankAction)(void);
```

Example:

```c
BankAction = Deposit;
BankAction();
```

This allows menu actions to be executed through a common function pointer.

---

## Money Handling

Balances and transaction amounts are stored as integer cents using:

```c
long long
```

For example:

```text
User enters: 123.45

Stored internally:
12345
```

When displayed, the program converts cents back to normal money format.

This approach is more reliable for financial calculations than storing money directly as floating-point values.

---

## Input Handling

Input is handled through reusable helper functions instead of calling `scanf()` directly throughout the program.

Examples:

```c
ReadInt()
ReadDouble()
ReadString()
ReadMoney()
ReadPositiveMoney()
```

This keeps input logic centralized and avoids repeating validation code across the project.

---

## Transaction History

Each transaction receives:

- Unique transaction ID
- Timestamp
- Transaction type
- Amount
- Sender information
- Receiver information when applicable

Transaction history can be displayed globally or filtered by account ID.

---

## Notes

The current storage implementation writes `Account` and `Transaction` structures directly into a binary file.

Because of this, changing the structure layout in future versions may make an older `bank.dat` file incompatible.

For an educational project, this approach keeps file handling simple while still demonstrating persistent storage.

A more advanced version could later use a versioned file format, text format, JSON, or a database.

---

## Educational Purpose

This project was created as a practical C programming exercise.

Its main goal is to practice:

- Program structure
- Memory management
- Pointers
- Data structures
- File handling
- Modular design
- Input validation
- Sorting algorithms
- Transaction logic
- Persistent application state

---

## License

This project is licensed under the MIT License.

See the `LICENSE` file for more information.

---

## Author

Developed by [Skyl1te](https://github.com/Skyl1te).

This project was created as a C programming project focused on learning low-level programming, memory management, modular architecture, data structures, file handling, and practical application design.
