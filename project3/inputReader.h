/**
 * this class contains all the global variable , struct and function declaration need to read,
 * write and get commands from the user.
 */
#ifndef INPUTREADER_H
#define INPUTREADER_H

#include "exec.h"
#include "lmedit.h"
#include "tables.h"

///for checking NULL or empty for A, N, T, V
#define nul 0xFFFFFFFF

///declare
char buf[128];
char sect[32];

int section; ///current section
int count;   ///total count
int repeat;  ///for repeating history use

/// A[,N][:T][=V]
uint32_t Aindex, Aaddress; //A is index, and A1 is address
uint32_t NCount;
uint32_t Ttype;  /// 1, 2, 3 ( bits, half, full )
uint32_t VRepValue;
char A[30];
char N[30];
char T[30];
char V[30];

/// for write use
uint32_t wAddress; /// this is the number of bytes to be wrote
uint32_t wValue; ///this is the value to be assign

int wIndex; /// this is the index from the beginning of the file
int numByteToWrite; /// this is the number of bytes to be wrote
int canWrite; ///1 if "=V" is used and value has not been assigned yet, otherwise 0
int wType; ///1 - bits, 2 - half word, 3 - full

// struct for history functionality
typedef struct{
    int count;
    char data[30];
}hist;

// array for storing histories
hist history[10];

/**
 * function that read data from user. It decide which action to call based on diferent command passed by the user.
 * @return if quit command is passed after writing then -1 else 0;
 */
int getData();

int sectionFunction();

int quitFunction();

int writeFunction();

int printSize();

#endif
