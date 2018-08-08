#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "tree.h"
#include "pretty.h"
#include "weed.h"
#include "symbol.h"
#include "code.h"

int yyparse();
int yylex(); 

// The program
PROGRAM *theprogram;

/* Function to remove extension of a filename */
char *removeEXTENSION(char *mystr);

char *removeEXTENSION(char *mystr) {
    
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';

    return retstr;
}

// Print tokens mode
int verbose_mode; 
int symbol_mode;

int main(int argc, char **argv)
{	
	// Scan mode
	if (strncmp(argv[1], "scan", 4) == 0) {
        while (yylex());
        printf("OK\n");
        exit(0);
    }
    // Tokens mode
    else if (strncmp(argv[1], "tokens", 6) == 0) {
        verbose_mode = 1;
        while (yylex());
        exit(0);
    }
    // Parse mode
    else if (strncmp(argv[1], "parse", 5) == 0) {
        yyparse();
        weedPROGRAM(theprogram);
        printf("OK\n");
        exit(0);
    }
    // Pretty mode
    else if (strncmp(argv[1], "pretty", 6) == 0) {
    	yyparse();
        prettyPROGRAM(theprogram);
        exit(0);
    }
    // Symbol mode
    else if (strncmp(argv[1], "symbol", 6) == 0) {
        symbol_mode = 1;
        yyparse();
        weedPROGRAM(theprogram);
        symPROGRAM(theprogram);
        printf("OK\n");
        exit(0);
    }
    // Typecheck mode 
    else if (strncmp(argv[1], "typecheck", 9) == 0) {
        yyparse();
        weedPROGRAM(theprogram);
        symPROGRAM(theprogram);
        printf("OK\n");
        exit(0);
    }
    // Codegen mode
    else if (strncmp(argv[1], "codegen", 7) == 0) {
        yyparse();
        weedPROGRAM(theprogram);
        symPROGRAM(theprogram);

        char *updated = removeEXTENSION(strdup(argv[2]));
        strcat(updated, ".c");
        codePROGRAM(theprogram, updated);

        printf("OK\n");
        exit(0);
    }

	return 0;
}