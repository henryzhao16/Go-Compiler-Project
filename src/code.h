#include "tree.h"

FILE *emitFILE;

/* NOTE:
 * The following are the function declarations of the code generator.
 */ 

void codePROGRAM(PROGRAM *p, char *name);
void codePACKAGE(PACKAGE *p);
void codeDECLARATION(DECLARATION *d);
void codeTYPE(TYPE *t);
void codeTYPE_g(TYPE *t);
void codeTYPESP(TYPE *t);
void codeTYPE_f(TYPE *t);
void codeFORMALTYPE(TYPE *t);
void codeIDL(TYPE *t, IDL *id);
void codeIDLTYPE(TYPE *t, IDL *id);
void codeFORMAL(FORMAL *f);
void codeSTATEMENT(DECLARATION *d, STATEMENT *s, char* continue_end);
void codeSTATEMENT2(DECLARATION *d, STATEMENT *s);
void codeEXP(EXP *e);
void codeINIT(DECLARATION *d);

// Functions dealing with special cases
void codeLINE(TYPE *t, IDL *id, EXP *e);
void codeTOPDOWN(IDL *id, EXP *e);
void codeONEIDL(IDL *id);
void codeONEEXP(EXP *e);
void codePRINT(EXP *e);
void codePRINT_w(EXP *e);
void codeSTATEMENTEXP(EXP *e);
void codeASSIGNMENT(EXP *id, EXP *e);
void codeASSIGNMENT2(EXP *id, EXP *e);
void codeSWITCHFUL(DECLARATION *d, char* case_id, EXP *e, STATEMENT *s, char *label, char *end);
void codeSWITCHLESS(DECLARATION *d, STATEMENT *s, char *label, char *end);
void inferTYPE(EXP *e);
void codeCASE(DECLARATION *d, EXP *e, STATEMENT *s, char *label, char *end);
void codeCASE_v(DECLARATION *d, char* case_id, EXP *condition, EXP *e, STATEMENT *s, char *label, char *end);
void codeDEFAULT(STATEMENT *s, char *label, char *end);
void codeDEFAULTLABEL(DECLARATION *d, STATEMENT *s);
char* findArrayName(EXP *e);
void codeARRAY(TYPE *t, char* buf);
void codeINITS();
void codeFEXP(EXP *e);
void codeTYPEK(TYPEK *tk);
void codeSTATEMENT_switch(DECLARATION *d, STATEMENT *s, char *endlabel);
void codeCONCAT(EXP *e, char* id);
void codeCAST(EXP *e);
void codeCAST2(EXP *e, char *buf);
void codeTMP(EXP *e);
void codeASSGNARRAY(EXP *left, EXP *right);
void codeARG(EXP *e);
void putARG(EXP *e);

// Helper Functions
void codeFOR();
void codeSC();
void codeWHILE();
void codeINF();
void codeBREAK();
void codeCONTINUE();
void codeIF();
void codeELSE();
void codeCOMMA();
void codeQUOTE();
void codeSPACE();
void codePRINTF();
void codeRETURN();
void codeINC();
void codeDEC();
void codeTYPEDEF();
void codeLBRACE();
void codeRBRACE();
void codeLSHIFT();
void codeRSHIFT();
void codeAND();
void codeNAND();
void codeMUL();
void codeDIV();
void codeMOD();
void codePLUS();
void codeMINUS();
void codeOR();
void codeXOR();
void codeL();
void codeLEQ();
void codeG();
void codeGEQ();
void codeTOR();
void codeTAND();
void codeEQ();
void codeNEQ();
void codeUPLUS();
void codeUMINUS();
void codeUNOT();
void codeUXOR();
void codeINT(int n);
void codeFLOAT64(char *n);
void codeBOOL(int n);
void codeSTRING(char *n);
void codeRUNE(char *n);
void codeLPAR();
void codeRPAR();
void codeSEMICOLON();
void codeEQUAL();
void codeIDE(char *id);
void codeLQUOTE();
void codeRQUOTE();
void codeSLQUOTE();
void codeSRQUOTE();
void codeLBRACKET();
void codeRBRACKET();
void codeELSEIF();
void codeGOTO();
void golite_cmp_array_fct();
