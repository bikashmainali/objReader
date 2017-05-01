/*
**   tables.c
**
**   Read tables and store info in relent, refent, and syment respectively.
**
**   Also, provide a function to print tables information
**
**   Author: Bikash Mainali
*/

#define _BSD_SOURCE

#include <endian.h>

#include <stdio.h>   //standard input/output
#include <stdlib.h>  //standard c library
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <stdint.h>

#include "tables.h"

/// print data info in header
///
/// @param void
/// @returns 0 if no error
int printrel(relent_t rel) {
    if ((int) rel.section == 1) {
        printf("   0x%08x (text) type 0x%04x\n", rel.addr, rel.type);
        return 0;
    }
    if ((int) rel.section == 2) {
        printf("   0x%08x (rdata) type 0x%04x\n", rel.addr, rel.type);
        return 0;
    }
    if ((int) rel.section == 3) {
        printf("   0x%08x (data) type 0x%04x\n", rel.addr, rel.type);
        return 0;
    }
    if ((int) rel.section == 4) {
        printf("   0x%08x (sdata) type 0x%04x\n", rel.addr, rel.type);
        return 0;
    }
    if ((int) rel.section == 5) {
        printf("   0x%08x (sbss) type 0x%04x\n", rel.addr, rel.type);
        return 0;
    }
    if ((int) rel.section == 6) {
        printf("   0x%08x (bss) type 0x%04x\n", rel.addr, rel.type);
        return 0;
    }
    printf("   0x%08x 0x%04x type 0x%04x\n", rel.addr, rel.section, rel.type);
    return 0;
}

/// print data info in header
///
/// @param void
/// @returns 0 if no error
int printref(refent_t ref) {
    printf("   0x%08x type 0x%04x symbol ", ref.addr, ref.type);
    int index = ref.sym;
    while (1) {
        printf("%c", str[index]);
        index++;
        if (str[index] == 0) {
            printf("\n");
            return 0;
        }
    }
}

/// print data info in header
///
/// @param void
/// @returns 0 if no error
int printsym(syment_t sym) {
    printf("   value 0x%08x flags 0x%08x symbol ", sym.value, sym.flags);
    int index = sym.sym;
    while (1) {
        printf("%c", str[index]);
        index++;
        if (str[index] == 0) {
            printf("\n");
            return 0;
        }
    }
}

/// print data info in header
///
/// @param void
/// @returns 0 if no error
int printTables() {
    if (table->data[6] != 0) {/// Relocation table
        printf("Relocation information:\n");
        for (int i = 0; i < (int) table->data[6]; ++i) {
            printrel(relTable[i]);
        }
    }

    if (table->data[7] != 0) {/// Relocation table
        printf("Reference information:\n");
        for (int i = 0; i < (int) table->data[6]; ++i) {
            printref(refTable[i]);
        }
    }

    if (table->data[8] != 0) {/// Relocation table
        printf("Symbol table:\n");
        for (int i = 0; i < (int) table->data[8]; ++i) {
            printsym(symTable[i]);
        }
    }
    return 0;
}

/// free dynamical allocation
///
/// @param void
/// @returns 0 if no error
int freeTables() {
    if (textdata) {
        free(textdata);
        textdata = NULL;
    }
    if (relTable) {
        free(relTable);
        relTable = NULL;
    }
    if (refTable) {
        free(refTable);
        refTable = NULL;
    }
    if (symTable) {
        free(symTable);
        symTable = NULL;
    }
    if (str) {
        free(str);
        str = NULL;
    }
    return 0;
}

/// a function to analysis Header
///
/// @param void
/// @returns 0 if no error
int readTableData() {
    /// pass through text and data contents
    /// read data
    int size = (int) (table->data[0] + table->data[1] + table->data[2] +
                      table->data[3] + table->data[4] + table->data[5]);
    // all data for respective fields like text, rdata, and all
    textdata = malloc(size * sizeof(uint8_t));
    fread(textdata, size, 1, pFile);

    /// assign data to dynamic memory
    if (table->data[6] != 0) {/// Relocation table
        relTable = malloc(table->data[6] * sizeof(relent_t));
        fread(relTable, table->data[6] * sizeof(relent_t), 1, pFile);
        uint16_t data;
        ///convert
        for (int i = 0; i < (int) table->data[6]; ++i) {
            relTable[i].addr = be32toh(relTable[i].addr);
            data = be16toh(relTable[i].section);
            relTable[i].section = (data & 0xff00) >> 8;
        }
    }

    if (table->data[7] != 0) {/// Reference table
        refTable = malloc(table->data[7] * sizeof(refent_t));
        fread(refTable, table->data[7] * sizeof(refent_t), 1, pFile);
        uint16_t data;
        //convert
        for (int i = 0; i < (int) table->data[7]; ++i) {
            refTable[i].addr = be32toh(refTable[i].addr);
            refTable[i].sym = be32toh(refTable[i].sym);
            data = be16toh(refTable[i].section);
            refTable[i].section = (data & 0xff00) >> 8;
        }
    }

    if (table->data[8] != 0) {/// Symbol table
        symTable = malloc(table->data[8] * sizeof(syment_t));
        fread(symTable, table->data[8] * sizeof(syment_t), 1, pFile);

        //convert
        for (int i = 0; i < (int) table->data[8]; ++i) {
            symTable[i].flags = be32toh(symTable[i].flags);
            symTable[i].value = be32toh(symTable[i].value);
            symTable[i].sym = be32toh(symTable[i].sym);
        }
    }

    if (table->data[9] != 0) {/// String
        str = malloc(table->data[9] * sizeof(char));
        fread(str, table->data[9] * sizeof(char), 1, pFile);
    }

    return 0;
}

/// print data info in header
///
/// @param void
/// @returns 0 if no error
int printData()
{
    for(int i =0; i < 10; i++) {
        char *tab[10] = {"text", "rdata", "data", "sdata", "sbss", "bss",
                         "reltab",
                         "reftab", "symtab", "strings"};
        if (i >= 6 && i < 9) {
            if (table->data[section])
                printf("Section %s is %d entries long\n", tab[i],
                       table->data[i]);
        } else {
            if (table->data[i])
                printf("Section %s is %d bytes long\n", tab[i],
                       table->data[i]);
        }
    }
    return 0;
}

/// a function to analysis Header
///
/// @param char* file
/// @returns 0 if no error
int readFileHeader(char* file){

    fread(&table->magic,2,1,pFile);
    table->magic = be16toh(table->magic);
    if(table->magic!=HDR_MAGIC){
        fprintf(stderr, "error: %s is not an R2K object module (magic number 0x%08x)\n", file, table->magic);
        return 1;
    }

    /// get data for version flags and entry
    fread(&(table->version),2,1,pFile);
    table->version = be16toh(table->version);

    fread(&(table->flags),4,1,pFile);
    table->flags = be32toh(table->flags);

    fread(&(table->entry),4,1,pFile);
    table->entry = be32toh(table->entry);

    for(int i=0;i<10;++i)
    {
        fread(&(table->data[i]),4,1,pFile);
        table->data[i]=be32toh(table->data[i]);
    }

    return 0;
}
