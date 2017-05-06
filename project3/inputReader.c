/**
 * Implementation of the main project3 part. This c file read data from user,
 * Write to file, print data from certain location if that location is valid.
 * @author Bikash Mainali
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "inputReader.h"
#define _BSD_SOURCE
#include <endian.h>
#include <math.h>
#include <stdlib.h>
#include "tables.h"

/**
 * add command given to the history
 */
static void addHistory()
{
    //check if history array is full of his
    if((history[9].count) ){  //if entries is full
        for(int i=0;i<9;i++){
            strcpy(history[i].data,history[i+1].data);
            history[i].count = history[i+1].count;
        }
        history[9].count = count;
        strcpy(history[9].data,buf);
        return;
    }
    //if not full, find a empty entry and put info in it
    for(int i=0;i<10;i++){
        if(! (history[i].count) ){
            history[i].count = count;
            strcpy(history[i].data,buf);
            return;
        }
    }
    return;
}

/**
 * print 10 last history fom our history array
 */
static void printHistory(){
    for(int i=0;i<10;i++){
        if((history[i].count) ){
            printf("%3d   %s",history[i].count,history[i].data);
        }
    }
    printf("\n");
    return;
}

/**
 * print the history number given. If the number is in the history, print
 * that command in the history else give user error message.
 * @param in number to print.
 */
static void repeatHistory(int in){
    /// if no long in history (in < history[0].index)
    if(in < history[0].count){
        fprintf( stderr, "error:  command %d is no longer in the command history\n", in);
        return;
    }
    /// if no long in history (in > history[8].index)
    int max=0;
    for(int i=0;i<10;i++){
        if((history[i].count)){
            max = i;
        }
    }
    ///printf("max:%d\n",max);
    if(in > history[max].count){
        fprintf( stderr, "error:  command %d has not yet been entered\n", in);
        return;
    }
    /// otherwise, repeat command
    for(int i=0;i<10;i++){
        if(history[i].count == in){
            strcpy(buf,history[i].data);
        }
    }
    /// print
    ++count;
    printStart();
    printf("%s",buf);
    return;
}

/**
 * print he table based on the different.
 * @param type  section to print
 * @param number number of times to print
 * @param index index to start print from
 * @return
 */
static void printTable(int type, int number, int index) {
    if (type == 6) {
        for (int i = 0; i < number; i++) {
            printrel(relT[index]);
            index++;
        }
    } else if (type == 7) {
        for (int i = 0; i < number; i++) {
            printref(refT[i]);
            index++;
        }
    } else if (type == 8) {
        for (int i = 0; i < number; i++) {
            printsym(symT[i]);
            index++;
        }
    }
    return;
}

/**
 * function to print data from
 * @param index it is number to start printing from
 * @param number number of times to print
 * @param type types to print it is b,h,w which is 1,2,3 respectively
 */
static void printallData(int index, int number, int type) {
    if (type == 1) { //byte
        for (int i = 0; i < number; i++) {
            printf("   0x%08x = 0x%02x\n", Aaddress, allData[index++]);
            //index += 1;
            Aaddress += 2;
        }
    } else  if (type == 2) { //half word - 2bytes
        for (int i = 0; i < number; i++) {
            printf("   0x%08x = 0x%02x", Aaddress, allData[index++]);
            printf("%02x\n", allData[index++]);
            //index += 2;
            Aaddress += 2;
        }
    } else if (type == 3) { //word - 4bytes
        for (int i = 0; i < number; i++) {
            printf("   0x%08x = 0x%02x", Aaddress, allData[index++]);
            printf("%02x", allData[index++]);
            printf("%02x", allData[index++]);
            printf("%02x\n", allData[index++]);
            //index += 4;
            Aaddress += 4;
        }
    }
    return;
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
    }
    // 0:h - Print a 16-bit halfword starting at location 0   (A:T)
    else if ((Aindex!= nul) && (NCount == nul) && (Ttype != nul) && (VRepValue == nul)) {
        ///===========table section below=============
        if ((section == 6) || (section == 7) || (section == 8)) {
            fprintf(stderr, "error:  ':%s' is not valid in table sections\n",
                    T);
            return 0;
        }
        ///===========text and data sections below====
        int a = (int) Aindex;

        /// check if index is valid
        if ((a < 0) || (a >= (int) table->data[section])) {
            sscanf(A, "%s", sect);
            fprintf(stderr, "error:  '%s' is not a valid address\n", sect);
            return 0;
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
            printTable((int) section, 1, (int) Aindex);
            return 0;
        }

        ///===========text and data sections below====
        /// check if index is valid
        if ((a < 0) || (a >= ((int) table->data[section]) - 3)) {
            sscanf(A, "%s", sect);
            fprintf(stderr, "error:  '%s' is not a valid address\n", sect);
            return 0;
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
                return 0;
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
        wType = 3; ///full words0,2
        int index =0;
        for (int i = 0; i < section; i++) {
            index += (int) table->data[i];
        }
        index = (index + (int)Aindex);
        *(allData + index) = (wValue&0xff000000)>> 24;
        *(allData + index + 1) = (wValue&0x000ff0000)>> 16;
        *(allData + index + 2) = (wValue&0x00000ff00)>> 8;
        *(allData + index + 3) = (wValue&0x0000000ff);
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
                *(allData + Aindex  + i ) = wValue;
                printf("   0x%08x is now 0x%02x\n", tempadd, wValue);
                tempadd += 1;
            }
        } else if (Ttype == 2) {///half
            wType = 2;
            wIndex += 1;
            uint32_t tempadd = wAddress;
            for (int i = 0; i < numByteToWrite*2;) {
                *(allData + Aindex  + i++ ) = wValue&0xff00;
                *(allData + Aindex  + i++ ) = wValue&0x00ff;
                printf("   0x%04x is now 0x%04x\n", tempadd, wValue);
                tempadd += 4;
            }
        } else if (Ttype == 3) {///full
            wType = 3;
            wIndex += 3;
            uint32_t tempadd = wAddress;
            for (int i = 0; i < numByteToWrite * 4; i++) {
                *(allData + Aindex  + i++ ) = ((wValue & 0xff000000)>>24);
                *(allData + Aindex  + i++ ) = ((wValue & 0x00ff0000)>>16);
                *(allData + Aindex  + i++ ) = ((wValue & 0x0000ff00)>>8);
                *(allData + Aindex  + i ) = wValue & 0x000000ff;
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
        for (int i = 0; i < numByteToWrite * 4; i++) {
            *(allData + Aindex  + i++ ) = ((wValue & 0xff000000)>>24);
            *(allData + Aindex  + i++ ) = ((wValue & 0x00ff0000)>>16);
            *(allData + Aindex  + i++ ) = ((wValue & 0x0000ff00)>>8);
            *(allData + Aindex  + i ) = wValue & 0x000000ff;
            printf("   0x%08x is now 0x%08x\n", tempadd, wValue);
            tempadd += 4;
        }
    }
    return 0;
}

/**
 * it read all the data provided by the user for else case.
 * It also returns friendly message if the value provided are invalid
 * @return -1 after print invalid option or 0 after completion of program
 */
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

/**
 * this funciton add starting value to the address if the address contain entry point
 * @return 0 if the data given is invalid
 */
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

/**
 * print the size of the section we are currently in.
 * @return 0 after completion of program
 */
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

/**
 * write the modified things to the file
 * @return 0 after competion  of the program
 */
int writeFunction() {
    if (canWrite) {
        int loop = sizeof(allData) / sizeof(uint8_t);
        for (int i = 0; i < loop; i++) {
            fseek(fp, readIndex, SEEK_SET);
            fwrite(&allData, sizeof(uint8_t), 1, fp);
            readIndex += sizeof(uint8_t);
        }
        canWrite = 0; /// make no writable after writing
    }
    return 0;
}

/**
 * Quit the program if user press yes after asking for input else return as nothing.
 * @return 0 after printing that message
 */
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
            }else if (strcmp(buf, "no\n") == 0) {
                return 0;
            }
        }
    }
    //freeTables();
    fclose(fp);
    return -1;
}
/**
 * print section information to the user. information as soon as user enter different section.
 * @return
 */
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

/**
 * reads user text and call function according to user commands.
 * @return -1 if the input cannot be read properly or int quit case. else 0
 */
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
    int number;
    if (sscanf(buf, "section %s", sect) == 1) {
        addHistory();
        return sectionFunction();
    } else if (strncmp(buf, "quit", 4) == 0) {
        addHistory();
        return quitFunction();
    }else if (strncmp(buf, "write",5) == 0) {
        addHistory();
        return writeFunction();
    } else if (strncmp(buf, "size", 4) == 0) {
        addHistory();
        return printSize();
    }else if (strncmp(buf, "history",7) ==0){
        addHistory();
        printHistory();
        return 0;
    } else if (sscanf(buf,"!%X",&number)){
        addHistory();
        repeatHistory(number);
        return 0;
    }else {
        addHistory();
        return elseCaseFunction();
    }
}

