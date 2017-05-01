/*
** Use for editing
**
** editor.h
**
** Author: Bikash Mainali
*/
#ifndef EDITOR_H
#define EDITOR_H

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
char A0[30];
char N0[30];
char T0[30];
char V0[30];

/// for write use
uint32_t writenaddress; /// this is the number of bytes to be wrote
uint32_t writevalue; ///this is the value to be assign

int writeindex; /// this is the index from the beginning of the file
int numByteToWrite; /// this is the number of bytes to be wrote
int canWrite; ///1 if "=V" is used and value has not been assigned yet, otherwise 0
int wtype; ///1 - bits, 2 - half word, 3 - full

// struct for history functionality
typedef struct{
    int index;
    char content[30];
}hist;

// array for storing histories
hist history[10];

/// function for editing mode
///
/// @param void
/// @returns 0 if no error
int getData();

#endif
