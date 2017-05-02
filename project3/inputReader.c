/**
 * Implementation of the main project3 part. This c file read data from user,
 * Write to file, print data from certain location if that location is valid.
 * @author Bikash Mainali
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "inputReader.h"

static int printTable(int type, int number, int index) {
    if (type == 6) {
        for (int i = 0; i < number; i++) {
            printrel(relT[index]);
            index++;
        }
    }
    if (type == 7) {
        for (int i = 0; i < number; i++) {
            printref(refT[i]);
            index++;
        }
    }
    if (type == 8) {
        for (int i = 0; i < number; i++) {
            printsym(symT[i]);
            index++;
        }
    }
    return 0;
}

/// function for printing text and data sections
///
/// @param int type( 1 (b), 2(h), 3(w) ), int number, int index
/// @returns 0 if no error
static int printallData(int index, int number, int type) {
    if (type == 1) { //byte
        for (int i = 0; i < number; i++) {
            printf("   0x%08x = 0x%02x\n", Aaddress, allData[index]);
            index += 1;
            Aaddress += 2;
        }
    }
    if (type == 2) { //half word - 2bytes
        for (int i = 0; i < number; i++) {
            printf("   0x%08x = 0x%02x", Aaddress, allData[index]);
            printf("%02x\n", allData[index + 1]);
            index += 2;
            Aaddress += 2;
        }
    }
    if (type == 3) { //word - 4bytes
        for (int i = 0; i < number; i++) {
            printf("   0x%08x = 0x%02x", Aaddress, allData[index]);
            printf("%02x", allData[index + 1]);
            printf("%02x", allData[index + 2]);
            printf("%02x\n", allData[index + 3]);
            index += 4;
            Aaddress += 4;
        }
    }
    return 0;
}

/**
 * function that is used for else case to analyse different data or value passed
 * all error checking, printing are done in this function
 * sscanf is used to match certain pattern passed by the user A[,N][:T][=V]
 * @return 0 if there is not error.
 */
static int analyse() {
    /// a=-1
    if ((Aindex== nul) && (NCount == nul) && (Ttype == nul) && (VRepValue == nul)) {
        if (table->entry != 0) {
            fprintf(stderr, "error:  '0xffffffff' is not a valid address\n");
            return 1;
        }
        fprintf(stderr, "error:  '-1' is not a valid address\n");
        return 1;
    }    /// 0:h - Print a 16-bit halfword starting at location 0   (A:T)
    else if ((Aindex!= nul) && (NCount == nul) && (Ttype != nul) && (VRepValue == nul)) {
        ///===========table section below=============
        if ((section == 6) || (section == 7) || (section == 8)) {
            fprintf(stderr, "error:  ':%s' is not valid in table sections\n",
                    T);
            return 1;
        }
        ///===========text and data sections below====
        int a = (int) Aindex;

        /// check if index is valid
        if ((a < 0) || (a >= (int) table->data[section])) {
            sscanf(A, "%s", sect);
            fprintf(stderr, "error:  '%s' is not a valid address\n", sect);
            return 1;
        }

        if ((section == 0) || (section == 1) || (section == 2) ||
            (section == 3) || (section == 4) || (section == 5) ||
            (section == 9)) {
            ///printf("section:%d\n",section);
            int index = (int) Aindex;
            for (int i = 0; i < section; i++) {
                index += (int) table->data[i];
            }
            ///printf("T:%d\n",T);
            printallData(index, 1, (int) Ttype);
        }

        return 0;
    }
    else if ((Aindex!= nul) && (NCount == nul) && (Ttype == nul) && (VRepValue == nul)) {
        /// 0 - Print a 32-bit word starting at location 0     (A)
        int a = (int) Aindex;

        ///===========table section below=============
        if ((section == 6) || (section == 7) || (section == 8)) {
            if ((a < 0) || (a >= ((int) table->data[section]))) {
                sscanf(A, "%s", sect);
                fprintf(stderr, "error:  '%s' is not a valid address\n", sect);
                return 1;
            }
            printTable((int) section, 1, (int) Aindex); /// type, number, index
            return 0;
        }

        ///===========text and data sections below====
        /// check if index is valid
        if ((a < 0) || (a >= ((int) table->data[section]) - 3)) {
            sscanf(A, "%s", sect);
            fprintf(stderr, "error:  '%s' is not a valid address\n", sect);
            return 1;
        }

        if ((section == 0) || (section == 1) || (section == 2) ||
            (section == 3) || (section == 4) || (section == 5) ||
            (section == 9)) {
            ///printf("section:%d\n",section);
            int index = (int) Aindex;
            for (int i = 0; i < section; i++) {
                index += (int) table->data[i];
            }
            printallData(index, 1, 3);
        }

        return 0;
    }
         /// 40,4 - Print 4 32-bit words starting at location 40  (A,N)
    else if ((Aindex != nul) && (NCount != nul) && (Ttype == nul) && (VRepValue == nul)) {
        int a = (int) Aindex;
        int n = (int) NCount;

        ///===========table section below=============
        if ((section == 6) || (section == 7) || (section == 8)) {
            if ((a < 0) || (a > ((int) table->data[section]))) {
                sscanf(A, "%s", sect);
                fprintf(stderr, "error:  '%s' is not a valid address\n", sect);
                return 1;
            }
            if ((a < 0) || (a + n > ((int) table->data[section]))) {
                fprintf(stderr, "error:  '%d' is not a valid count\n", NCount);
                return 1;
            }
            printTable((int) section, n, a); /// type, number, index
            return 0;
        }
        ///===========text and data sections below====
        int m = 0;
        if ((int) Ttype == 2) {
            m = 1;
        }
        if ((int) Ttype == 3) {
            m = 3;
        }
        /// check if index is valid
        if ((a < 0) || (a >= (int) table->data[section] - m)) {
            sscanf(A, "%s", sect);
            fprintf(stderr, "error:  '%s' is not a valid address\n", sect);
            return 1;
        }
        /// check if count is valid
        if ((a < 0) || ((a + n) >= (int) table->data[section])) {
            fprintf(stderr, "error:  '%d' is not a valid count\n", NCount);
            return 1;
        }
        if ((section == 0) || (section == 1) || (section == 2) ||
            (section == 3) || (section == 4) || (section == 5) ||
            (section == 9)) {
            ///printf("section:%d\n",section);
            int index = (int) Aindex;
            for (int i = 0; i < section; i++) {
                index += (int) table->data[i];
            }
            ///printf("T:%d\n",T);
            printallData(index, (int) NCount, 3);
        }
        return 0;
    }
        /// 20,10:b - Print 10 bytes starting at location 20  (A,N:T)
    else if ((Aindex!= nul) && (NCount != nul) && (Ttype != nul) && (VRepValue == nul)) {
        ///===========table section below=============
        if ((section == 6) || (section == 7) || (section == 8)) {
            fprintf(stderr, "error:  ':%s' is not valid in table sections\n",
                    T);
            return 1;
        }
        ///===========text and data sections below====
        int a = (int) Aindex;
        int n = (int) NCount;
        int m = 1;

        if ((int) Ttype == 2) {
            m = 2;
        }

        if ((int) Ttype == 3) {
            m = 4;
        }
        /// check if count is valid
        if ((a < 0) || ((a + n * m) > (int) table->data[section])) {
            fprintf(stderr, "error:  '%d' is not a valid count\n", NCount);
            return 1;
        }
        if ((section == 0) || (section == 1) || (section == 2) ||
            (section == 3) || (section == 4) || (section == 5) ||
            (section == 9)) {
            ///printf("section:%d\n",section);
            int index = (int) Aindex;
            for (int i = 0; i < section; i++) {
                index += (int) table->data[i];
            }
            ///printf("T:%d\n",T);
            printallData(index, (int) NCount, (int) Ttype);
        }
        return 0;
    }
    /// 20=42 Assign the value 42 to the 32-bit word at location 20 (A=V)
    else if ((Aindex != nul) && (NCount == nul) && (Ttype == nul) && (VRepValue != nul)) {
        ///===========table section below=============
        if ((section == 6) || (section == 7) || (section == 8)) {
            fprintf(stderr, "error:  '=%s' is not valid in table sections\n",
                    V);
            return 1;
        }

        ///===========text and data sections below====
        wValue = VRepValue;
        wAddress = Aindex;
        wIndex = sizeof(exec_t) + (int) Aindex +
                 3; ///since it's big endian, we need to write at the last 8 bits of 32 bits
        for (int i = 0; i < section; i++) {
            wIndex += (int) table->data[i];
        }
        numByteToWrite = 1;
        canWrite = 1;
        wType = 3; ///full words
        printf("   0x%08x is now 0x%08x\n", wAddress, wValue);
        return 0;
    }
        /// 0,4:b=5 Assign the value 5 to each of 4 bytes beginning at location 0 (A,N:T=V)
    else if ((Aindex != nul) && (NCount != nul) && (Ttype != nul) && (VRepValue != nul)) {
        ///===========table section below=============
        if ((section == 6) || (section == 7) || (section == 8)) {
            fprintf(stderr, "error:  ':%s' is not valid in table sections\n",
                    T);
            return 1;
        }
        ///check address and count
        int ai = (int) Aindex;
        int nc = (int) NCount;
        int adPlus = 0;
        if ((int) Ttype == 2) { adPlus = 1; }
        if ((int) Ttype == 3) { adPlus = 3; }
        /// check if index is valid
        if ((ai < 0) || (ai >= (int) table->data[section] - adPlus)) {
            sscanf(A, "%s", sect);
            fprintf(stderr, "error:  '%s' is not a valid address\n", sect);
            return 1;
        }
        if ((ai < 0) || ((ai + nc) >= (int) table->data[section])) {
            fprintf(stderr, "error:  '%d' is not a valid count\n", NCount);
            return 1;
        }
        ///===========write below====
        wValue = VRepValue;
        wAddress = Aindex;
        numByteToWrite = (int) NCount;
        canWrite = 1;
        wIndex = sizeof(exec_t) + (int) Aindex;
        if (Ttype == 1) {///bits
            wType = 1;
            uint32_t tempadd = wAddress;
            for (int i = 0; i < numByteToWrite; i++) {
                printf("   0x%02x is now 0x%02x\n", tempadd, wValue);
                tempadd += 1;
            }
        } else if (Ttype == 2) {///half
            wType = 2;
            wIndex += 1;
            uint32_t tempadd = wAddress;
            for (int i = 0; i < numByteToWrite; i++) {
                printf("   0x%04x is now 0x%04x\n", tempadd, wValue);
                tempadd += 4;
            }
        } else if (Ttype == 3) {///full
            wType = 3;
            wIndex += 3;
            uint32_t tempadd = wAddress;
            for (int i = 0; i < numByteToWrite; i++) {
                printf("   0x%08x is now 0x%08x\n", tempadd, wValue);
                tempadd += 4;
            }
        }
        for (int i = 0; i < section; i++) {
            wIndex += (int) table->data[i];
        }
        return 0;
    }/// 20,10=b - write 10 bytes starting at location 20  (A,N=T)
    else if ((Aindex != nul) && (NCount != nul) && (Ttype == nul) && (VRepValue != nul)) {
        ///===========table section below=============
        if ((section == 6) || (section == 7) || (section == 8)) {
            fprintf(stderr, "error:  '=%s' is not valid in table sections\n",
                    V);
            return 1;
        }
        ///check address and count
        int ai = (int) Aindex;
        int nc = (int) NCount;
        int adPlus = 0;
        if ((int) Ttype == 2) { adPlus = 1; }
        if ((int) Ttype == 3) { adPlus = 3; }
        /// check if index is valid
        if ((ai < 0) || (ai >= (int) table->data[section] - adPlus)) {
            sscanf(A, "%s", sect);
            fprintf(stderr, "error:  '%s' is not a valid address\n", sect);
            return 1;
        }
        if ((ai < 0) || ((ai + nc) >= (int) table->data[section])) {
            fprintf(stderr, "error:  '%d' is not a valid count\n", NCount);
            return 1;
        }
        wValue = VRepValue;
        wAddress = Aindex;
        wIndex = sizeof(exec_t) + (int) Aindex +
                     3; ///since it's big endian, we need to write at the last 8 bits of 32 bits
        for (int i = 0; i < section; i++) {
            wIndex += (int) table->data[i];
        }
        numByteToWrite = (int) NCount;
        canWrite = 1;
        wType = 3; ///full

        uint32_t tempadd = wAddress;
        for (int i = 0; i < numByteToWrite; i++) {
            printf("   0x%08x is now 0x%08x\n", tempadd, wValue);
            tempadd += 4;
        }
    }
    return 0;
}

int readElseCaseData(){
    strcpy(A,"");strcpy(N,"");strcpy(T,"");strcpy(V,"");
    Aindex= -1; NCount=-1; Ttype=-1; VRepValue=-1;
    //read data entered if it is in ceratin format
    sscanf(buf, "%*[^:]: %c %*[^\n]\n", T);
    sscanf(buf, "%[^,], %s %*[^\n]\n", A, N);
    sscanf(buf, "%*[^,], %*[^=]= %s %*[^\n]\n", V);
    sscanf(buf, "%*[^=]= %s %*[^\n]\n", V);
    sscanf(buf, "%*[^,], %*[^:]: %c %*[^\n]\n", T);
    // case A
    if (table->entry) {/// load module
        if (sscanf(A, "%X", &Aindex) == 1) {
            Aaddress = Aindex;
            ///printf("A:%d\n",A);
        } else {
            sscanf(A, "%s", sect);
            fprintf(stderr, "error:  '%s' is not a valid address\n", sect);
            return -1;
        }
    } else {/// object module
        if (sscanf(A, "%u", &Aindex) == 1) {
            Aaddress = Aindex;
            ///printf("A:%d\n",A);
        } else {
            sscanf(A, "%s", sect);
            fprintf(stderr, "error:  '%s' is not a valid address\n", sect);
            return -1;
        }
    }
    //case N
    if (sscanf(N, "%u", &NCount) == 1) {
        ///printf("N:%d\n",N);
    } else {
        if (N[0]) {
            sscanf(N, "%s", sect);
            fprintf(stderr, "error:  '%s' is not a valid count\n", sect);
            return -1;
        }
    }
    //case T
    if (T[0]) {
        /// printf("T\n");
        if (strcmp(T, "b") == 0) {
            Ttype = 1;
        } else if (strcmp(T, "h") == 0) {
            Ttype = 2;
        } else if (strcmp(T, "w") == 0) {
            Ttype = 3;
        } else {
            fprintf(stderr, "error:  '%s' is not a table sections\n", T);
            return -1;
        }
    }
    //case V
    if (sscanf(V, "%u", &VRepValue) == 1) {
        VRepValue = (int) VRepValue;
        ///printf("V:%d\n",V);
    }
    return 0;
}

int elseCaseFunction(){
    /// initilization
    if (readElseCaseData() == -1){
        return 0;
    }
    /// start editing
    if (table->entry) {/// load module
        /// a=-1
        if ((Aindex == nul) && (NCount == nul) && (Ttype == nul) && (VRepValue == nul)) {
            if (table->entry != 0) {
                fprintf(stderr,
                        "error:  '0xffffffff' is not a valid address\n");
                return 1;
            }
            fprintf(stderr, "error:  '-1' is not a valid address\n");
            return 1;
        }
        if (Aindex != nul) {
            if (section == 0) {///text
                Aaddress -= TEXT_BEGIN;
            }
            if (section == 1) {///rdata
                Aaddress -= DATA_BEGIN;
            }
            if (section == 2) {///data
                Aaddress -= DATA_BEGIN;
                // take out the space that data occupied
                Aaddress -= table->data[1];
            }
            if (section == 3) {///sdata
                Aaddress -= DATA_BEGIN;
                Aaddress -= table->data[1];
                Aaddress -= table->data[2];
            }
            if (section == 9) {///str
                ///do nothing
            }
        }
        //analyze data
        analyse();
    } else {
        // analyze data
        analyse();
    }
}

int printSize() {
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
}

int writeFunction() {
    if (canWrite) {
            if (wType == 3) {
                for (int i = 0; i < numByteToWrite; i++) {
                    fseek(fp, wIndex, SEEK_SET);
                    fwrite(&wValue, 1, 1, fp);
                    freeTables();
                    fseek(fp, sizeof(exec_t), SEEK_SET);
                    readTableData();
                    Aindex += 4;
                }
                canWrite = 0; /// make no writable after writing
            } else if (wType == 1) {
                for (int i = 0; i < numByteToWrite; i++) {
                    ///printf("   0x%02x is now 0x%02x\n",wAddress,wValue);
                    fseek(fp, wIndex, SEEK_SET);
                    fwrite(&wValue, 1, 1, fp);
                    freeTables();
                    fseek(fp, sizeof(exec_t), SEEK_SET);
                    readTableData();
                    Aindex += 1;
                }
                canWrite = 0; /// make no writable after writing
            }
        }
    return 0;
}

int quitFunction() {
    if (canWrite) {
            while (1) {
                printf("Discard modifications (yes or no)?");
                if (fgets(buf, 128, stdin) == NULL) {
                    if (ferror(stdin)) { perror("error occurred in file"); }
                }
                if (strcmp(buf, "yes\n") == 0) {
                    freeTables();
                    fclose(fp);
                    //everything is done. quit
                    return -1;
                }
                //don't quit
                if (strcmp(buf, "no\n") == 0) {
                    return 0;
                }
            }
        }
    freeTables();
    fclose(fp);
    return -1;
}

int sectionFunction() {
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
}

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
    if (sscanf(buf, "section %s", sect) == 1) {
        return sectionFunction();
    } else if (strncmp(buf, "quit", 4) == 0) {
        return quitFunction();
    }else if (strncmp(buf, "write",5) == 0) {
        return writeFunction();
    } else if (strncmp(buf, "size", 4) == 0) {
        return printSize();
    } else {
        return elseCaseFunction();
    }
}
