/*
 * This file contains information related with all table, read data and store in table.
 * This also read for header, all data file and other stuff related to table.
 *
 * @author Bikash Mainali
 */

#define _BSD_SOURCE
#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "tables.h"

void printrel(relent_t rel) {
    char *tab[10] = {"text", "rdata", "data", "sdata", "sbss", "bss",
                     "reltab",
                     "reftab", "symtab", "strings"};
    if (rel.section <= 6) {
        printf("   0x%08x (text) type 0x%04x\n", rel.addr, rel.type);
    } else {
        printf("   0x%08x 0x%04x type 0x%04x\n", rel.addr, rel.section, rel.type);
    }
}

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

int freeTables() {
    if (allData) {
        free(allData);
        allData = NULL;
    }
    if (relT) {
        free(relT);
        relT = NULL;
    }
    if (refT) {
        free(refT);
        refT = NULL;
    }
    if (symT) {
        free(symT);
        symT = NULL;
    }
    if (str) {
        free(str);
        str = NULL;
    }
    return 0;
}

int readTableData() {
    /// pass through text and data contents
    /// read data
    int size = (int) (table->data[0] + table->data[1] + table->data[2] +
                      table->data[3] + table->data[4] + table->data[5]);
    // all data for respective fields like text, rdata, and all
    readIndex = size+ sizeof(table);
    allData = malloc(size * sizeof(uint8_t));
    fread(allData, size, 1, fp);

    /// assign data to dynamic memory
    if (table->data[6] != 0) {/// Relocation table
        relT = malloc(table->data[6] * sizeof(relent_t));
        fread(relT, table->data[6] * sizeof(relent_t), 1, fp);
        uint16_t data;
        ///convert
        for (int i = 0; i < (int) table->data[6]; ++i) {
            relT[i].addr = be32toh(relT[i].addr);
            data = be16toh(relT[i].section);
            relT[i].section = (data & 0xff00) >> 8;
        }
    }

    if (table->data[7] != 0) {/// Reference table
        refT = malloc(table->data[7] * sizeof(refent_t));
        fread(refT, table->data[7] * sizeof(refent_t), 1, fp);
        uint16_t data;
        //convert
        for (int i = 0; i < (int) table->data[7]; ++i) {
            refT[i].addr = be32toh(refT[i].addr);
            refT[i].sym = be32toh(refT[i].sym);
            data = be16toh(refT[i].section);
            refT[i].section = (data & 0xff00) >> 8;
        }
    }

    if (table->data[8] != 0) {/// Symbol table
        symT = malloc(table->data[8] * sizeof(syment_t));
        fread(symT, table->data[8] * sizeof(syment_t), 1, fp);

        //convert
        for (int i = 0; i < (int) table->data[8]; ++i) {
            symT[i].flags = be32toh(symT[i].flags);
            symT[i].value = be32toh(symT[i].value);
            symT[i].sym = be32toh(symT[i].sym);
        }
    }

    if (table->data[9] != 0) {/// String
        str = malloc(table->data[9] * sizeof(char));
        fread(str, table->data[9] * sizeof(char), 1, fp);
    }

    return 0;
}

void printData()
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
}

int readHeader(char* file){
    fread(&table->magic,2,1,fp);
    table->magic = be16toh(table->magic);
    if(table->magic!=HDR_MAGIC){
        fprintf(stderr, "error: %s is not an R2K object module (magic number 0x%08x)\n", file, table->magic);
        return 1;
    }
    /// get data for version flags and entry
    fread(&(table->version),2,1,fp);
    table->version = be16toh(table->version);

    fread(&(table->flags),4,1,fp);
    table->flags = be32toh(table->flags);

    fread(&(table->entry),4,1,fp);
    table->entry = be32toh(table->entry);

    for(int i=0;i<10;++i)
    {
        fread(&(table->data[i]),4,1,fp);
        table->data[i]=be32toh(table->data[i]);
    }
    return 0;
}
