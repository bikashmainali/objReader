/*
**   editor.c
**
**   Editing mode
**
**   Author: Bikash Mainali
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "editor.h"
#include "tables.h"

/// function for editing mode
///
/// @param void
/// @returns 0 if no error
int getData() {
    /// get input
    if (fgets(buf, 128, stdin) == NULL) {
        if (ferror(stdin)) {
            perror("error occurred in file");
        }
        if ( feof( stdin ) ) {
            return -1;
        }
    }
//    addInHistory();
    if (sscanf(buf, "section %s", sect) == 1) {
//        printf("section is %s", sect);
        if (strncmp(sect, "text", 4) == 0) {
            printf("Now editing section text\n");
            section = 0;
        } else if (strncmp(sect, "rdata", 5) == 0) {
            printf("Now editing section rdata\n");
            section = 1;
        } else if (strncmp(sect, "data", 4) == 0) {
            printf("Now editing section data\n");
            section = 2;
        } else if (strncmp(sect, "sdata", 5) == 0) {
            if (table->data[3] == 0) {
                fprintf(stderr, "error:  cannot edit sdata section\n");
            } else {
                printf("Now editing section sdata\n");
                section = 3;
            }
        } else if (strncmp(sect, "sbss", 4) == 0) {
            if (table->data[4] == 0) {
                fprintf(stderr, "error:  cannot edit sbss section\n");
            } else {
                printf("Now editing section sbss\n");
                section = 4;
            }
        } else if (strncmp(sect, "bss", 3) == 0) {
            if (table->data[5] == 0) {
                fprintf(stderr, "error:  cannot edit sbss section\n");
            } else {
                printf("Now editing section bss\n");
                section = 5;
            }
        } else if (strncmp(sect, "reltab", 6) == 0) {
            printf("Now editing section reltab\n");
            section = 6;
        } else if (strncmp(sect, "reftab", 6) == 0) {
            printf("Now editing section reftab\n");
            section = 7;
        } else if (strncmp(sect, "symtab",6) == 0) {
            printf("Now editing section symtab\n");
            section = 8;
        } else if (strncmp(sect, "string",6) == 0) {
            printf("Now editing section string\n");
            section = 9;
        } else {
            fprintf(stderr, "error: '%s' is not a valid section name\n", sect);
            return 0;
        }
        return 0;
    } else if (strncmp(buf, "quit", 4) == 0) {

    }else if (strncmp(buf, "write",5) == 0) {

    } else if (strncmp(buf, "size", 4) == 0) {
        char *tab[10] = {"text", "rdata", "data", "sdata", "sbss", "bss",
                         "reltab",
                         "reftab", "symtab", "strings"};
        if (section >= 6 && section < 9) {
            if (table->data[section])
                printf("Section %s is %d entries long\n", tab[section],
                       table->data[section]);
        } else {
            if (table->data[section])
                printf("Section %s is %d bytes long\n", tab[section],
                       table->data[section]);
        }
        return 0;
    } else {

    }

    return 0;
}
