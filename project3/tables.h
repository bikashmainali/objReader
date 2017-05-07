/**
 * This c file contains all the function related to table.
 * It read data data print specific data
 *
 * @author Bikash Maianli
 *
*/

#ifndef TABLES_H
#define TABLES_H
// need for relent_t, refent_t and syment_t
#include "exec.h"
// used for different variable declared in lmedit.h
#include "lmedit.h"

int readIndex;
/**
 * print information of given relocation table
 */
void printrel(relent_t);

/**
 * print information of the given reference table
 * @return 0 if the print is successful
 */
int printref(refent_t);

/**
 * print information of the given symbol table
 * @return 0 if the print is successful
 */
int printsym(syment_t);

/**
 * free all dynamically allocated table.
 * @return 0 if all the free is suceeful
 */
int freeTables();

/**
 * read all the table data. It reads all data from the file given.
 * @return 0 after reading all the input or data from the file.
 */
int readTableData();

/**
 * print data of the given file. This prints all table information table. like data, rdata,
 * symboltable, reference table, relocation table, sbss bss
 *
 */
void printData();

/**
 * this method read head of the given file this method read magic number, convert date,
 * and read other information.
 * @param file to read from
 * @return return 1 if error exists( if magic number is not 0xface ) otherwise 0
 */
int readHeader(char* file);

#endif
