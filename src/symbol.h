#include "tree.h"

#define HashSize 317

/* NOTE:
 * The following declarations are the functions to help build the symbol table. 
 */

extern int symbol_mode;

typedef struct SymbolTable 
{
    SYMBOL *table[HashSize];
    struct SymbolTable *next;
} SymbolTable;

SymbolTable *initSymbolTable();

SymbolTable *scopeSymbolTable(SymbolTable *t);

SYMBOL *putSymbol(SymbolTable *t, char *name, SymbolKind kind);

SYMBOL *getSymbol(SymbolTable *t, char *name);

int defSymbol(SymbolTable *t, char *name);

SymbolTable *goliteLib;

// Symbol functions 
void symPROGRAM(PROGRAM *p);
void symPRE(SymbolTable *sym);

// First pass functions
void symInterfacePROGRAM(PROGRAM *p, SymbolTable *sym);
void symInterfacePACKAGE(PACKAGE *p, SymbolTable *sym);
void symInterfaceDECLARATION(DECLARATION *d, SymbolTable *sym);
void symInterfaceVARIDL(DECLARATION *d, IDL *id, SymbolTable *sym);
void symInterfaceTYPEIDL(DECLARATION *d, IDL *id, SymbolTable *sym);

// Second pass functions
void symImplementationPROGRAM(PROGRAM *p, SymbolTable *sym);
void symImplementationDECLARATION(DECLARATION *d, SymbolTable *sym);
void symImplementationTYPE(TYPE *t, SymbolTable *sym);
void symImplementationFORMALTYPE(FORMAL *f, SymbolTable *sym);
void symImplementationFORMALIDENTIFIER(FORMAL *f, SymbolTable *sym);
void symImplementationSTATEMENT(DECLARATION *d, STATEMENT *s, SymbolTable *sym);
void symImplementationEXP(DECLARATION *par, EXP *e, SymbolTable *sym);
void symImplementationIDL3(IDL *id, TYPE *t, SymbolTable *sym);

// Symbol print functions
void printVariableSymbol(IDL *ids, TYPE *t, int noType, SymbolTable *sym);
void printTypeDeclaration(IDL *ids, TYPE *t, SymbolTable *sym);
void functionTypes(FORMAL *f, TYPE *t, char *buf);
void functionReturnType(TYPE *t, char *buf);
void formalType(TYPE *t, FORMAL *f);
void printTable(SymbolTable *sym);
void findArrayString(TYPE *t, char *buf);
void checkBound(EXP *par, EXP *e, SymbolTable *sym);
void compareBound(TYPE *t, EXP *e);
void reverseArray(int arr[], int start, int end);
void fillLeft(int arr[], TYPE *t, int start, int end);
void compareRight(int arr[], EXP *e, int start, int end);



