/**
 * This is the main c file to read the program. This c file runs the whole
 * application.
 * @author Bikash Mainali
 *
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
#include "lmedit.h"
#include "inputReader.h"

int printInfo(char* file){    
    /// print file basic info
    if(table->entry == 0){//object module
        printf("File %s is an R2K object module\n",file);
    }else{
        printf("File %s is an R2K load module  (entry point 0x%08x)\n",file,table->entry);
    }
    //0000111 1001 00010
    //1111111 0000 00000 == 0xfe00 for year
    //------------------
    //0000111 0000 00000 >> 111 = 7
    //0000000 1111 00000 == 0x1e0
    //0000000 0000 11111 == 0x1f
    printf("Module version:  2%03d/%02d/%02d\n",(table->version & 0xfe00)>>9,(table->version & 0x1e0)>>5,(table->version & 0x1f));
    /// print data sections
    printData();
    return 0;
}

int readFile(char* file)
{
    fp = fopen(file, "r+");
    
    if (!fp)
    { 
		perror(file);
		return 1;
	}else{
	    if(readHeader(file)==1){
	        fclose(fp);
	        return 1;
	    }
	    readTableData();
	}
	printInfo(file);
	return 0;
}

int printStart(){
    if(section==0){//text
        printf("text[%d] >",count);
    }else if(section==1){/// rdata
        printf("rdata[%d] >",count);
    }else if(section==2){/// data
        printf("data[%d] >",count);
    }else if(section==3){/// sdata
        printf("sdata[%d] >",count);
    }else if(section==4){/// sbss
        printf("sbss[%d] >",count);
    }else if(section==5){/// bss
        printf("bss[%d] >",count);
    }else if(section==6){/// reltab
        printf("reltab[%d] >",count);
    }else if(section==7){/// reftab
        printf("reftab[%d] >",count);
    }else if(section==8){/// symtab
        printf("symtab[%d] >",count);
    }else if(section==9){/// string
        printf("string[%d] >",count);
    }
    return 0;
}

/**
 * this program read the command line.
 * if proper file is given it read and analyse the data in the file and allow to do different task in the that file.
 * it is the stating point of the program
 * @param argc number of argument count
 * @param argv arguments of the program
 * @return EXIT_FAILURE if program failed. else EXIT_SUCCESS.
 */
int main(int argc, char *argv[])
{
    if(argc<=1)
    {
        fprintf(stderr, "usage: lmedit file\n");
        exit(EXIT_FAILURE);
    }

    for(int n=1;n<argc;++n)
    {
        table = malloc(sizeof(exec_t));
	    if(readFile(argv[n]) == 1){
	        return 1;
	    }
    }
    section = 0;
    count = 1;
    /// loop until user enter "quit"
    while(1)
    {
        //print the fist section data info "text[N] > "
        printStart();
        if(getData() ==-1){
            break;
        }
        ++count;
    }
    return EXIT_SUCCESS;
}
