/*
**   lmedit.c
**
**   Edit R2000K Architecture Object
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
//#include "tables.c"
#include "lmedit.h"
#include "editor.h"
/// function to print information
///
/// @param char* file
/// @returns 0 if no error
int printInfo(char* file){    
    /// print file basic info
    if(table->entry == 0){//object module
        printf("File %s is an R2K object module\n",file);
    }else{
        printf("File %s is an R2K load module  (entry point 0x%08x)\n",file,table->entry);
    }
    
    /// print module version
    printf("Module version:  2%03d/%02d/%02d\n",(table->version & 0xfe00)>>9,(table->version & 0x1e0)>>5,(table->version & 0x1f));
    /// print data sections
    printData();
    return 0;
}

/// a function to analyze a R2K file
///
/// @param char* file
/// @returns 0 if no error
int readFile(char* file)
{
    pFile = fopen(file, "r+");
    
    if (!pFile) 
    { 
		perror(file);
		return 1;
	}else{
	    if(readFileHeader(file)==1){
	        fclose(pFile);
	        return 1;
	    }
	    readTableData();
	}
	printInfo(file);
	return 0;
}

/// main function for the program
///
/// @param void
/// @returns 0 if no error
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

int main(int argc, char *argv[])
{
    /// check arguments
    if(argc<=1)
    { /// if missing arguments, print error and exit
        fprintf(stderr, "usage: lmedit file\n");
        exit(EXIT_FAILURE);
    }

    /// open each file and analyze if available
    for(int n=1;n<argc;++n)
    {
        table = malloc(sizeof(exec_t));
        ///use analysis function
	    if(readFile(argv[n]) == 1){
	        return 1;
	    }
    }
    section = 0;
    count = 1;
    /// loop until user enter "quit"
    while(1)
    {   //print the fist section data info "text[N] > "
        printStart();
        if(getData() ==-1){
            break;
        }
        ++count;
    }
    return EXIT_SUCCESS;
}
