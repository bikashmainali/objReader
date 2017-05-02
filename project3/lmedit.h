/*
**   lmedit.h
**
**   Edit header and store info in exec struct.
**
**   Author: Bikash Mainali
*/

#ifndef LMEDIT_H
#define LMEDIT_H

#include "exec.h"
//#include "tables.h"
//#include "editor.h"

/// function for printing data
exec_t * table;

/// file to be read
FILE* fp;

int section; ///current section
int count;   ///total count
/// store data from text and data sections
uint8_t* allData;

/// tables to store data
relent_t* relT;
refent_t* refT;
syment_t* symT;
char* str;

/// function to print information
///
/// @param char* file
/// @returns 0 if no error
int printInfo(char* file);

/// a function to analyze a R2K file
///
/// @param char* file
/// @returns 0 if no error
int Analysis(char* file);

/// main function for the program
///
/// @param void
/// @returns 0 if no error
int printStart();

#endif
