/**
 * store variable and function used by different function
 * and function in lmedit.c
 * @author Bikash Mainali
 */

#ifndef LMEDIT_H
#define LMEDIT_H

#include "exec.h"

//variable to store table information
exec_t * table;

//file name to read from
FILE* fp;
// current section number
int section;
// count of command typed by user. It is used in history as well
int count;
// store data and information of all the table. this program heavily depends on this variable for differnet action
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

/**
 * open a given file, read the file data.
 * @param file file to read from
 * @return 1 if there if some problem in opening the file else 0
 */
int readFile(char* file);

/** print the first text about string text[N] >  or different section
  * @param void
  * @returns 0 if no error
  */
int printStart();

#endif
