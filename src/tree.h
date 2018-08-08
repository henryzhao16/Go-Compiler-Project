#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

/* NOTE:
 * The following functions and data structures consist of the design pattern of the Abstract Syntax Tree.
 */

// Track order number
unsigned int globalORDER;
unsigned int ptrID;
unsigned int globalID;
unsigned int initID;

// Symbol Kind
typedef enum { packageSym, variableSym, typeSym, functionSym, formalSym, intSym, float64Sym, runeSym, boolSym, stringSym, trueSym, falseSym, ivariableSym } SymbolKind;

 // Symbol struct 
typedef struct SYMBOL {
    char *name;
    SymbolKind kind;
    union {
    	struct PACKAGE *packageS; // used
    	struct DECLARATION *declarationS; // used
    	struct TYPE *typeS;	
    	struct FORMAL *formalS; // used
    	struct STATEMENT *statementS; 
    	struct EXP *expS; 
    } val;
    struct TYPE *type;
    struct TYPEK *typeK;
    struct SYMBOL *next;
    int order;
    int ok;
} SYMBOL; 

// Program struct 
typedef struct PROGRAM {
	char *name;
	struct PACKAGE *package;
	struct DECLARATION *declarations;
} PROGRAM;

// Package struct
typedef struct PACKAGE {
	char *name;
	int lineno;
} PACKAGE;

// Declaration struct
typedef struct DECLARATION {
	int lineno;
	enum {
		k_declarationVariable,
		k_declarationType,
		k_declarationFunction
	} kind;
	union {
		struct { struct IDL *ids; struct TYPE *type; struct EXP *expls; int format; int expressionAssigned; } variableD; // Format value 1 for type only, 2 for expresion only and 3 for type & expression
		struct { struct IDL *ids; struct TYPE *type; } typeD;
		struct { char *id; struct FORMAL *arguments; struct TYPE *returnType; struct STATEMENT *body; } functionD;
	} val;
	struct DECLARATION *next;
	int overrideable;
} DECLARATION;

// Identifier list struct
typedef struct IDL {
	char *name;
	int lineno;
	struct IDL *next;
	struct TYPEK *typeK;
} IDL;

// Formal struct 
typedef struct FORMAL {
	char *name;
	int lineno;
	struct TYPE *type;
	struct FORMAL *next;
} FORMAL;

// Type struct
typedef struct TYPE {
	int lineno;
	enum {
		k_typeReference,
		k_typeArray,
		k_typeParenthesis
	} kind;
	union {
		struct { char *id; } refT;
		struct { int value; struct TYPE *type; } arrayT;
		struct { struct TYPE *type; } parT;
	} val;
	struct TYPEK *typeK;
	int isArray;
} TYPE;

typedef struct TYPEK {
	char *name;
	struct TYPEK *base;
} TYPEK;

// Statement struct
typedef struct STATEMENT {
	int lineno;
	enum {
		k_statementBlock,
		k_statementExpression,
		k_statementAssignment,
		k_statementDeclaration,
		k_statementIncrement,
		k_statementDecrement,
		k_statementPrint,
		k_statementPrintln,
		k_statementReturn,
		k_statementIf,
		k_statementIfElse,
		k_statementSwitch,
		k_statementSwitchCase,
		k_statementSwitchDefault,
		k_statementFor,
		k_statementBreak,
		k_statementContinue,
		k_statementSkip
	} kind;
	union {
		struct { struct STATEMENT *body; } blockS;
		struct { struct EXP *exp; } expressionS;
		struct { struct EXP *ids; struct EXP *exp; } assignmentS;
		struct { struct DECLARATION *decl; } declarationS;
		struct { struct EXP *exp; } incrementS;
		struct { struct EXP *exp; } decrementS;
		struct { struct EXP *printlist; } printS;
		struct { struct EXP *printlist; } printlnS;
		struct { struct EXP *exp; } returnS;
		struct { struct EXP *condition; struct STATEMENT *body; } ifS;
		struct { struct EXP *condition; struct STATEMENT *thenPart; struct STATEMENT *elsePart; } ifelseS;
		struct { struct EXP *condition; struct STATEMENT *clauses; } switchS;
		struct { struct EXP *explist; struct STATEMENT *content; } caseclauseS;
		struct { struct STATEMENT *content; } defaultclauseS;
		struct { struct STATEMENT *initStmt; struct EXP *condition; struct STATEMENT *postStmt; struct STATEMENT *body; int infinitel; int whilel; int threel; int keepSemiColon; } forS;
		struct { struct  DECLARATION *decl; } declS; 
	} val;
	struct STATEMENT *nextClause; 
	struct STATEMENT *nextIf;
	struct STATEMENT *next;
} STATEMENT;

// Expression struct
typedef struct EXP {
	int lineno;
	enum {
		k_expressionIdentifier,
		k_expressionParenthesis,
		k_expressionInt, 
		k_expressionFloat64,
		k_expressionBool,
		k_expressionString,
		k_expressionRune,
		k_expressionUplus,
		k_expressionUminus,
		k_expressionUnot,
		k_expressionUxor,
		k_expressionTor,
		k_expressionTand,
		k_expressionEq,
		k_expressionNeq, 
		k_expressionL,
		k_expressionLeq,
		k_expressionG,
		k_expressionGeq,
		k_expressionPlus,
		k_expressionMinus,
		k_expressionOr,
		k_expressionXor,
		k_expressionMultiply,
		k_expressionDivide,
		k_expressionMod,
		k_expressionLshift,
		k_expressionRshift,
		k_expressionAnd,
		k_expressionNand,
		k_expressionFunctionCall,
		k_expressionArray
	} kind;
	union {
		struct { char *id; } identifierE;
		struct { struct EXP *body; } parenthesisE;
		int intconst;
		char *floatconst;
		int boolconst;
		char *stringconst;
		char *runeconst;
		struct { struct EXP *body; } uplusE;
		struct { struct EXP *body; } uminusE;
		struct { struct EXP *body; } unotE;
		struct { struct EXP *body; } uxorE;
		struct { struct EXP *left; struct EXP *right; } torE;
		struct { struct EXP *left; struct EXP *right; } tandE;
		struct { struct EXP *left; struct EXP *right; } eqE;
		struct { struct EXP *left; struct EXP *right; } neqE;
		struct { struct EXP *left; struct EXP *right; } lE;
		struct { struct EXP *left; struct EXP *right; } leqE;
		struct { struct EXP *left; struct EXP *right; } gE;
		struct { struct EXP *left; struct EXP *right; } geqE;
		struct { struct EXP *left; struct EXP *right; } plusE;
		struct { struct EXP *left; struct EXP *right; } minusE;
		struct { struct EXP *left; struct EXP *right; } orE;
		struct { struct EXP *left; struct EXP *right; } xorE;
		struct { struct EXP *left; struct EXP *right; } multiplyE;
		struct { struct EXP *left; struct EXP *right; } divideE;
		struct { struct EXP *left; struct EXP *right; } modE;
		struct { struct EXP *left; struct EXP *right; } lshiftE;
		struct { struct EXP *left; struct EXP *right; } rshiftE;
		struct { struct EXP *left; struct EXP *right; } andE;
		struct { struct EXP *left; struct EXP *right; } nandE;
		struct { char *id; struct EXP *explist; } funcE;
		struct { struct EXP *exp; struct EXP *value; } arrayE;
	} val;
	struct EXP *next;
	struct TYPEK *typeK;
	int isCast;
	int isArray;
} EXP;

/* HELPER DATA STRUCTURES + FUNCTIONS */
struct node {
   char* data;
   int key;
   struct node *next;
   struct node *prev;
};

struct node *head_inits;

void insertFirst(int key, char* data);
void printLL();

// Program function
PROGRAM *makePROGRAM(PACKAGE *package, DECLARATION *declarations);

// Package function
PACKAGE *makePACKAGE(char *name);

// Declaration functions
DECLARATION *makeDECLARATION_variable(IDL *ids, TYPE *type, EXP *expls, DECLARATION *next, int format);
DECLARATION *makeDECLARATION_type(IDL *ids, TYPE *type, DECLARATION *next);
DECLARATION *makeDECLARATION_function(char *name, FORMAL *args, TYPE *returnType, STATEMENT *body, DECLARATION *next);

// Identifier list function
IDL *makeIDENTIFIERLIST(char *name, IDL *nextIdentifier);

// Formal function
FORMAL *makeFORMAL(char *name, TYPE *type, FORMAL *next);

// Type functions
TYPE *makeTYPE_ref(char *name);
TYPE *makeTYPE_ARRAY(int value, TYPE *type);
TYPE *makeTYPE_parenthesis(TYPE *type);

// Statement functions
STATEMENT *makeSTATEMENT_block(STATEMENT *body, STATEMENT *next);
STATEMENT *makeSTATEMENT_expression(EXP *exp, STATEMENT *next);
STATEMENT *makeSTATEMENT_assignment(EXP *ids, EXP *exp, STATEMENT *next);
STATEMENT *makeSTATEMENT_increment(EXP *exp, STATEMENT *next);
STATEMENT *makeSTATEMENT_decrement(EXP *exp, STATEMENT *next);
STATEMENT *makeSTATEMENT_print(EXP *printlist, STATEMENT *next);
STATEMENT *makeSTATEMENT_println(EXP *printlist, STATEMENT *next);
STATEMENT *makeSTATEMENT_return(EXP *exp, STATEMENT *next);
STATEMENT *makeSTATEMENT_if(EXP *condition, STATEMENT *body, STATEMENT *next);
STATEMENT *makeSTATEMENT_ifelse(EXP *condition, STATEMENT *thenPart, STATEMENT *elsePart, STATEMENT *nextIf, STATEMENT *next);
STATEMENT *makeSTATEMENT_switch(EXP *condition, STATEMENT *clauses, STATEMENT *next);
STATEMENT *makeSTATEMENT_switchcase(EXP *expressionList, STATEMENT *content, STATEMENT *nextClause);
STATEMENT *makeSTATEMENT_switchdefault(STATEMENT *content, STATEMENT *nextClause);
STATEMENT *makeSTATEMENT_for(STATEMENT *initStmt, EXP *condition, STATEMENT *postStmt, STATEMENT *body, STATEMENT *next, int keepSemiColon);
STATEMENT *makeSTATEMENT_break(STATEMENT *next);
STATEMENT *makeSTATEMENT_continue(STATEMENT *next);
STATEMENT *makeSTATEMENT_declaration(DECLARATION *d, STATEMENT *next);
STATEMENT *makeSTATEMENT_skip(STATEMENT *next);

// Expression functions
EXP *makeEXP_identifier(char *name);
EXP *makeEXP_parenthesis(EXP *body);
EXP *makeEXP_intconst(int n);
EXP *makeEXP_boolconst(int n);
EXP *makeEXP_floatconst(char *n);
EXP *makeEXP_stringconst(char *n);
EXP *makeEXP_runeconst(char *n);
EXP *makeEXP_uplus(EXP *body);
EXP *makeEXP_uminus(EXP *body);
EXP *makeEXP_unot(EXP *body);
EXP *makeEXP_uxor(EXP *body);
EXP *makeEXP_tor(EXP *left, EXP *right);
EXP *makeEXP_tand(EXP *left, EXP *right);
EXP *makeEXP_eq(EXP *left, EXP *right);
EXP *makeEXP_neq(EXP *left, EXP *right);
EXP *makeEXP_l(EXP *left, EXP *right);
EXP *makeEXP_leq(EXP *left, EXP *right);
EXP *makeEXP_g(EXP *left, EXP *right);
EXP *makeEXP_geq(EXP *left, EXP *right);
EXP *makeEXP_plus(EXP *left, EXP *right);
EXP *makeEXP_minus(EXP *left, EXP *right);
EXP *makeEXP_or(EXP *left, EXP *right);
EXP *makeEXP_xor(EXP *left, EXP *right);
EXP *makeEXP_multiply(EXP *left, EXP *right);
EXP *makeEXP_divide(EXP *left, EXP *right);
EXP *makeEXP_mod(EXP *left, EXP *right);
EXP *makeEXP_lshift(EXP *left, EXP *right);
EXP *makeEXP_rshift(EXP *left, EXP *right);
EXP *makeEXP_and(EXP *left, EXP *right);
EXP *makeEXP_nand(EXP *left, EXP *right);
EXP *makeEXP_functionCall(char *name, EXP *expressionList);
EXP *makeEXP_array(EXP *exp, EXP *value);

#endif /* !TREE_H */

