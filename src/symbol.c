#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"
#include "type.h"

/* TYPE INITIATION FUNCTION */
void initBaseTypes() 
{
	intTYPE = malloc(sizeof(TYPEK));
	intTYPE->name = "int";
	intTYPE->base = intTYPE;

	float64TYPE = malloc(sizeof(TYPEK));
	float64TYPE->name = "float64";
	float64TYPE->base = float64TYPE;

	runeTYPE = malloc(sizeof(TYPEK));
	runeTYPE->name = "rune";
	runeTYPE->base = runeTYPE;

	boolTYPE = malloc(sizeof(TYPEK));
	boolTYPE->name = "bool";
	boolTYPE->base = boolTYPE;

	stringTYPE = malloc(sizeof(TYPEK));
	stringTYPE->name = "string";
	stringTYPE->base = stringTYPE;
}

/* SYMBOL FUNTIONS */
int Hash(char *str) 
{ 
	unsigned int hash = 0;	
	while (*str) hash = (hash << 1) + *str++; 
	return hash % HashSize;
}

SymbolTable *initSymbolTable() 
{ 
	SymbolTable *t;
	int i;
	t = malloc(sizeof(SymbolTable));
	for (i = 0; i < HashSize; i++) t->table[i] = NULL;
	t->next = NULL;
	return t;
}

SymbolTable *scopeSymbolTable(SymbolTable *s) 
{ 
	SymbolTable *t;
	t = initSymbolTable();
	t->next = s;
	return t;
}

SYMBOL *putSymbol(SymbolTable *t, char *name, SymbolKind kind) 
{ 
	int i = Hash(name);
	SYMBOL *s;
	for (s = t->table[i]; s; s = s->next) {
		if (strcmp(s->name,name) == 0) return s;
	}
	s = malloc(sizeof(SYMBOL));
	s->name = name;
	s->kind = kind;
	s->next = t->table[i];
	t->table[i] = s;

	return s;
}
 
SYMBOL *getSymbol(SymbolTable *t, char *name) 
{ 
	int i = Hash(name);
  	SYMBOL *s;
  	for (s = t->table[i]; s; s = s->next) {
    	if (strcmp(s->name, name)==0) return s;
 	}
	if (t->next == NULL) return NULL;
	return getSymbol(t->next, name);
}

int defSymbol(SymbolTable *t, char *name) 
{ 
	int i = Hash(name);
	SYMBOL *s;
	for (s = t->table[i]; s; s = s->next) {
		if (strcmp(s->name, name) == 0) return 1;
	}
	return 0;
}

/* PROGRAM SYMBOL FUNCTION */
void symPROGRAM(PROGRAM *p)
{
	goliteLib = initSymbolTable();

	globalORDER = 0;
	globalID = 0;
	ptrID = 0;
	initID = 0;

	head_inits = NULL;

	symPRE(goliteLib);

	symInterfacePROGRAM(p, goliteLib);
	symImplementationPROGRAM(p, goliteLib);

	if (symbol_mode) {
		printf("\n");
	}
}

/* PRE-MAPPING SYMBOL FUNCTION */
void symPRE(SymbolTable *sym) 
{	
	SYMBOL *s1, *s2, *s3, *s4, *s5, *s6, *s7;
	TYPE *t1, *t2, *t3, *t4, *t5, *t6, *t7;

	t1 = malloc(sizeof(TYPE));
	t2 = malloc(sizeof(TYPE));
	t3 = malloc(sizeof(TYPE));
	t4 = malloc(sizeof(TYPE));
	t5 = malloc(sizeof(TYPE));
	t6 = malloc(sizeof(TYPE));
	t7 = malloc(sizeof(TYPE));

	initBaseTypes();

	s1 = putSymbol(sym, "true", trueSym);
	s1->type = t1;
	s1->type->typeK = NULL;
	s1->order = globalORDER;
	globalORDER++;

	s2 = putSymbol(sym, "false", falseSym);
	s2->type = t2;
	s2->type->typeK = NULL;
	s2->order = globalORDER;
	globalORDER++;

	s3 = putSymbol(sym, "int", intSym);
	s3->type = t3;
	s3->type->typeK = intTYPE;
	s3->order = globalORDER;
	globalORDER++;

	s4 = putSymbol(sym, "float64", float64Sym);
	s4->type = t4;
	s4->type->typeK = float64TYPE;
	s4->order = globalORDER;
	globalORDER++;

	s5 = putSymbol(sym, "rune", runeSym);
	s5->type = t5;
	s5->type->typeK = runeTYPE;
	s5->order = globalORDER;
	globalORDER++;

	s6 = putSymbol(sym, "bool", boolSym);
	s6->type = t6;
	s6->type->typeK = boolTYPE;
	s6->order = globalORDER;
	globalORDER++;

	s7 = putSymbol(sym, "string", stringSym);
	s7->type = t7;
	s7->type->typeK = stringTYPE;
	s7->order = globalORDER;
	globalORDER++;
}

/* PROGRAM IDENTIFIER FUNCTION */ 
void symInterfacePROGRAM(PROGRAM *p, SymbolTable *sym)
{
	if (p != NULL) {

		symInterfacePACKAGE(p->package, sym);
		symInterfaceDECLARATION(p->declarations, sym);
	}
}

/* PACKAGE IDENTIFIER FUNCTION */
void symInterfacePACKAGE(PACKAGE *p, SymbolTable *sym)
{	
	SYMBOL *s;

	if (p != NULL) {

		if (defSymbol(sym, p->name)) {
			printf("Error: ill-defined package (identifier taken).\n");
			exit(1);
		}
		else {
			
			if (strcmp(p->name, "_") != 0) {

				s = putSymbol(sym, p->name, packageSym);

				s->val.packageS = p;
				s->order = globalORDER;

				globalORDER++;

				if (symbol_mode) {
					printf("\n SYMBOL TABLE \n");
					printf("<<IDENTIFIER>> | <<CATEGORY>> | <<TYPE>>\n\n");
					printf("%s | package | --- \n", p->name);	
				}
			}
		}
	}
}

/* DECLARATION IDENTIFIER FUNCTION */
void symInterfaceDECLARATION(DECLARATION *d, SymbolTable *sym)
{
	SYMBOL *s; 

	if (d != NULL) {

		switch (d->kind) {

			case k_declarationVariable:

				symInterfaceVARIDL(d ,d->val.variableD.ids, sym);
				break;
			case k_declarationType:
				symInterfaceTYPEIDL(d, d->val.typeD.ids, sym);
				break;
			case k_declarationFunction:

				if (defSymbol(sym, d->val.functionD.id)) {

					s = getSymbol(sym, d->val.functionD.id);

					if (s != NULL) {

						if (strcmp(d->val.functionD.id, "init") == 0) {

							s = putSymbol(sym, d->val.functionD.id, functionSym);
							s->val.declarationS = d;
							s->type = d->val.functionD.returnType;
							s->order = globalORDER;

							globalORDER++;
						}
						else {

							switch (s->kind) {

								case packageSym:
									if (strcmp(s->name, "main") == 0) {
										s->kind = functionSym;
										s->val.declarationS = d;
										s->type = NULL;
										s->order = globalORDER;
										globalORDER++;
									}
									else {
										printf("Error: ill-defined function (identifier taken).\n");
										exit(1);
									}
									break;
								case variableSym:
									printf("Error: ill-defined function (identifier taken).\n");
									exit(1);
									break;
								case typeSym:
									printf("Error: ill-defined function (identifier taken).\n");
									exit(1);
									break;
								case functionSym:
									printf("Error: ill-defined function (identifier taken).\n");
									exit(1);
									break;
								case formalSym:
									printf("Error: ill-defined function (illegal scope).\n");
									exit(1);
									break;
								case intSym:
									s->kind = functionSym;
									s->val.declarationS = d;
									s->type = d->val.functionD.returnType;
									break;
								case float64Sym:
									s->kind = functionSym;
									s->val.declarationS = d;
									s->type = d->val.functionD.returnType;
									break;
								case runeSym:
									s->kind = functionSym;
									s->val.declarationS = d;
									s->type = d->val.functionD.returnType;
									break;
								case boolSym:
									s->kind = functionSym;
									s->val.declarationS = d;
									s->type = d->val.functionD.returnType;
									break;
								case stringSym:
									s->kind = functionSym;
									s->val.declarationS = d;
									s->type = d->val.functionD.returnType;
									break;
								case trueSym:
									s->kind = functionSym;
									s->val.declarationS = d;
									s->type = d->val.functionD.returnType;
									break;
								case falseSym:
									s->kind = functionSym;
									s->val.declarationS = d;
									s->type = d->val.functionD.returnType;	
									break;
								case ivariableSym:
									printf("Error: ill-defined function (identifier taken).\n");
									exit(1);
									break;
							}
						}
					} 
				}
				else {

					if (strcmp(d->val.functionD.id, "_") != 0) {

						s = putSymbol(sym, d->val.functionD.id, functionSym);
						s->val.declarationS = d;
						s->type = d->val.functionD.returnType;
						s->order = globalORDER++;
						globalORDER++;
					}
				}

				break;
		} 

		symInterfaceDECLARATION(d->next, sym);
	}
}

/* VARIABLE DECLARATION IDENTIFIER FUNCTION */
void symInterfaceVARIDL(DECLARATION *d, IDL *id, SymbolTable *sym) 
{	
	SYMBOL *s;

	if (id != NULL) {

		symInterfaceVARIDL(d, id->next, sym);
		
		if (defSymbol(sym, id->name)) {

			s = getSymbol(sym, id->name);

			if (s == NULL) {
				printf("Error: variable undefined.\n");
				exit(1);
			}

			switch (s->kind) {

				case packageSym:
					printf("Error: ill-defined variable (identifier taken).\n");
					exit(1);
					break;
				case variableSym:
					printf("Error: ill-defined variable (identifier taken).\n");
					exit(1);
					break;
				case typeSym:
					printf("Error: ill-defined variable (identifier taken).\n");
					exit(1);
					break;
				case functionSym:
					printf("Error: ill-defined variable (identifier taken).\n");
					exit(1);
					break;
				case formalSym:
					printf("Error: ill-defined variable (identifier taken).\n");
					exit(1);
					break;
				case intSym:
					s->kind = ivariableSym;
					s->val.declarationS = d;
					break;
				case float64Sym:
					s->kind = ivariableSym;
					s->val.declarationS = d;
					break;
				case runeSym:
					s->kind = ivariableSym;
					s->val.declarationS = d;
					break;
				case boolSym:
					s->kind = ivariableSym;
					s->val.declarationS = d;
					break;
				case stringSym:
					s->kind = ivariableSym;
					s->val.declarationS = d;
					break;
				case trueSym:
					s->kind = variableSym;
					s->val.declarationS = d;
					break;
				case falseSym:
					s->kind = variableSym;
					s->val.declarationS = d;
					break;
				case ivariableSym:
					printf("Error: ill-defined variable (identifier taken).\n");
					exit(1);
					break;
			}

			switch (d->val.variableD.format) {

				case 1:
					s->type = d->val.variableD.type;
					break;
				case 2:
					s->type = NULL;
					break;
				case 3:
					s->type = d->val.variableD.type;
					break;
			}
		}
		else {

			s = getSymbol(sym, id->name);

			if (s != NULL) {
				if (s->kind == intSym || s->kind == float64Sym || s->kind == runeSym || s->kind == stringSym || s->kind == boolSym) {
					s->kind = ivariableSym;
					s->val.declarationS = d;
					goto EXCPT;
				}
				d->overrideable = 1;
			}

			switch (d->val.variableD.format) {

				// var x T
				case 1:

					if (strcmp(id->name, "_") != 0) {

						s = putSymbol(sym, id->name, variableSym);

						s->val.declarationS = d;
						s->type = d->val.variableD.type;
						s->order = globalORDER;
						globalORDER++;
					}
					break;
				// var x = exp
				case 2:

					if (strcmp(id->name, "_") != 0) {
						s = putSymbol(sym, id->name, variableSym);
						s->val.declarationS = d;
						s->type = NULL;
						s->order = globalORDER;
						globalORDER++;
					}
					break;
				// var x T = exp
				case 3: 

					if (strcmp(id->name, "_") != 0) {
						s = putSymbol(sym, id->name, variableSym);
						s->val.declarationS = d;
						s->type = d->val.variableD.type;
						s->order = globalORDER;
						globalORDER++;
					}
					break;
			}

			EXCPT:
			;
		}		
	}
}

/* TYPE DECLARATION IDENTIFIER FUNCTION */
void symInterfaceTYPEIDL(DECLARATION *d, IDL *id, SymbolTable *sym) 
{	
	SYMBOL *s;

	if (id != NULL) {

		symInterfaceTYPEIDL(d, id->next, sym);

		if (defSymbol(sym, id->name)) {
			
			s = getSymbol(sym, id->name); 

			if (s == NULL) {
				printf("Error: type variable undefined.\n");
				exit(1);
			}

			switch (s->kind) {
				
				case packageSym:
					printf("Error: ill-defined type (identifier taken).\n");
					exit(1);
					break;
				case variableSym:
					printf("Error: ill-defined type (identifier taken).\n");
					exit(1);
					break;
				case typeSym:
					printf("Error: ill-defined type (identifier taken).\n");
					exit(1);
					break;
				case functionSym:	
					printf("Error: ill-defined type (identifier taken).\n");
					exit(1);
					break;
				case formalSym:
					printf("Error: ill-defined type (identifier taken).\n");
					exit(1);
					break; 
				case intSym:
					s->kind = typeSym;
					s->val.declarationS = d;
					s->type = d->val.typeD.type;
					break; 
				case float64Sym:
					s->kind = typeSym;
					s->val.declarationS = d;
					s->type = d->val.typeD.type;
					break; 
				case runeSym:
					s->kind = typeSym;
					s->val.declarationS = d;
					s->type = d->val.typeD.type;
					break; 
				case boolSym:
					s->kind = typeSym;
					s->val.declarationS = d;
					s->type = d->val.typeD.type;
					break; 
				case stringSym:
					s->kind = typeSym;
					s->val.declarationS = d;
					s->type = d->val.typeD.type;
					break;
				case trueSym:
					s->kind = typeSym;
					s->val.declarationS = d;
					s->type = d->val.typeD.type;
					break; 
				case falseSym:
					s->kind = typeSym;
					s->val.declarationS = d;
					s->type = d->val.typeD.type;
					break;
				case ivariableSym:
					printf("Error: ill-defined type (identifier taken).\n");
					exit(1);
					break;
			}
		}
		else {

			if (strcmp(id->name, "_") != 0) {

				s = putSymbol(sym, id->name, typeSym);
				s->val.declarationS = d;
				s->type = d->val.typeD.type;
				s->order = globalORDER;
				globalORDER++;
			}		
		}
	}
}

/* PROGRAM SYMBOL ANALYSIS */
void symImplementationPROGRAM(PROGRAM *p, SymbolTable *sym)
{
	if (p != NULL) {
		symImplementationDECLARATION(p->declarations, sym);
	}
}

/* DECLARATION SYMBOL ANALYSIS */
void symImplementationDECLARATION(DECLARATION *d, SymbolTable *sym)
{	
	SymbolTable *funcSym;
	char buf[500] = "";

	if (d != NULL) {

		switch (d->kind) {

			case k_declarationVariable:

				filterInit(d->val.variableD.ids);

				switch (d->val.variableD.format) {

					// var x T
					case 1:

						symImplementationTYPE(d->val.variableD.type, sym);
						symImplementationIDL3(d->val.variableD.ids, d->val.variableD.type, sym);
					
						printVariableSymbol(d->val.variableD.ids, d->val.variableD.type, 0, sym);
						break;
					// var x = exp
					case 2:
						symImplementationEXP(d, d->val.variableD.expls, sym);
						symImplementationIDL(d->val.variableD.ids, d->val.variableD.expls, sym);
						checkDeclarationFct(d->val.variableD.expls, sym);
						printVariableSymbol(d->val.variableD.ids, d->val.variableD.type, 1, sym);
						break;
					
					// var x T = exp
					case 3:

						symImplementationTYPE(d->val.variableD.type, sym);
						symImplementationEXP(d, d->val.variableD.expls, sym);
						symImplementationIDL3(d->val.variableD.ids, d->val.variableD.type, sym);

						checkArrayDeclaration(d->val.variableD.type, d->val.variableD.expls, sym);
	
						checkVariableDeclaration(d->val.variableD.type, d->val.variableD.expls);

						printVariableSymbol(d->val.variableD.ids, d->val.variableD.type, 0, sym);

						break;
				}

				break;
			case k_declarationType:
				
				filterInit(d->val.typeD.ids);
				filterRecursive(d->val.typeD.ids, d->val.typeD.type, sym);

				symImplementationTYPE(d->val.typeD.type, sym);

				symImplementationIDL2(d->val.typeD.ids, d, d->val.typeD.type, sym);
				
				printTypeDeclaration(d->val.typeD.ids, d->val.typeD.type, sym);

				break;
			case k_declarationFunction:

				checkSpecialFunctions(d);

				funcSym = scopeSymbolTable(sym);

				symImplementationFORMALTYPE(d->val.functionD.arguments, funcSym);
				symImplementationFORMALIDENTIFIER(d->val.functionD.arguments, funcSym);
				
				symImplementationTYPE(d->val.functionD.returnType, funcSym);
				
				if (d->val.functionD.returnType != NULL && d->val.functionD.body == NULL) {
					printf("Error: return statement expected.\n");
					exit(1);
				}

				if (symbol_mode) {
					
					if (strcmp(d->val.functionD.id, "_") != 0) {
						
						functionTypes(d->val.functionD.arguments, d->val.functionD.returnType, buf);

						functionReturnType(d->val.functionD.returnType, buf);

						if (d->val.functionD.returnType != NULL) {
							printf("%s | function declaration | %s \n", d->val.functionD.id, buf);
						}
						else {
							printf("%s | function declaration | --- \n", d->val.functionD.id);
						}
					}
				}

				//if (symbol_mode) printf("\t{\n");
				symImplementationSTATEMENT(d, d->val.functionD.body, funcSym);
				//if (symbol_mode) printf("\t}\n");

				break;
		}

		symImplementationDECLARATION(d->next, sym);
	}
}

/* TYPE ANALYSIS */
void symImplementationTYPE(TYPE *t, SymbolTable *sym)
{
	SYMBOL *s;

	if (t != NULL) {

		switch (t->kind) {

			case k_typeReference:

				s = getSymbol(sym, t->val.refT.id);

				if (s != NULL) {
					
					switch (s->kind) {

						case packageSym:
							printf("Error: ill-defined type (package identifier).\n");
							exit(1);
							break;
						case variableSym:

							if (s->ok) {
								s->ok = 0;
								goto DONE;
							}
							printf("Error: ill-defined type (variable identifier).\n");
							exit(1);
							break;
						case typeSym:
							t->typeK = s->typeK;
							t->isArray = s->type->isArray;
							break;
						case functionSym:
							printf("Error: ill-defined type (function identifier).\n");
							exit(1);
							break;
						case formalSym:
							printf("Error: ill-defined type (formal identifier).\n");
							exit(1);
							break;
						case intSym:
							t->typeK = intTYPE;
							t->isArray = 0;
							break;
						case float64Sym:
							t->typeK = float64TYPE;
							t->isArray = 0;
							break;
						case runeSym:
							t->typeK = runeTYPE;
							t->isArray = 0;
							break;
						case boolSym:
							t->typeK = boolTYPE;
							t->isArray = 0;
							break;
						case stringSym:
							t->typeK = stringTYPE;
							t->isArray = 0;
							break;
						case trueSym:
							printf("Error: ill-defined type (boolean value).\n");
							exit(1);
							break;
						case falseSym:
							printf("Error: ill-defined type (boolean value).\n");
							exit(1);
						case ivariableSym:
							s->kind = variableSym;
							if (strcmp(s->name, "int") == 0) {
								t->typeK = intTYPE;
								t->isArray = 0;
							}
							else if (strcmp(s->name, "float64") == 0) {
								t->typeK = float64TYPE;
								t->isArray = 0;
							}
							else if (strcmp(s->name, "rune") == 0) {
								t->typeK = runeTYPE;
								t->isArray = 0;
							}
							else if (strcmp(s->name, "string") == 0) {
								t->typeK = stringTYPE;
								t->isArray = 0;
							}
							else if (strcmp(s->name, "bool") == 0) {
								t->typeK = boolTYPE;
								t->isArray = 0;
								
							}
							break;
					}
				}
				else {
					printf("Error: type undefined.\n");
					exit(1);
				}
				break;
			case k_typeArray:
				symImplementationTYPE(t->val.arrayT.type, sym);
				
				t->typeK = t->val.arrayT.type->typeK;
				t->isArray = 1 + t->val.arrayT.type->isArray;

				break;
			case k_typeParenthesis:
				symImplementationTYPE(t->val.parT.type, sym);
				t->typeK = t->val.parT.type->typeK;
				t->isArray = t->val.parT.type->isArray;
				break;
		}

		DONE: ;
	}
}

/* FORMAL SYMBOL ANALYSIS */
void symImplementationFORMALTYPE(FORMAL *f, SymbolTable *sym)
{
	if (f != NULL) {

		symImplementationFORMALTYPE(f->next, sym);
		symImplementationTYPE(f->type, sym);	
	}
}

void symImplementationFORMALIDENTIFIER(FORMAL *f, SymbolTable *sym)
{	
	SYMBOL *s;

	if (f != NULL) {

		symImplementationFORMALIDENTIFIER(f->next, sym);

		if (strcmp(f->name, "_") != 0) {

			if (defSymbol(sym, f->name)) {
					printf("Error: formal defined.\n");
					exit(1);
			}
			else {
				s = putSymbol(sym, f->name, formalSym);
				s->val.formalS = f;
				s->type = f->type;
				s->order = globalORDER;
				globalORDER++;
			}

			if (symbol_mode) {
				formalType(f->type, f);			
			}
		}
	}
}

/* STATEMENT IDENTIFIER ANALYSIS */
void symImplementationSTATEMENT(DECLARATION *d, STATEMENT *s, SymbolTable *sym)
{	
	SymbolTable *localSym;
	SymbolTable *localSym2;

	if (s != NULL) {

		switch (s->kind) {

			case k_statementBlock:

				localSym = scopeSymbolTable(sym);

				//if (symbol_mode) printf("\t{\n");
				symImplementationSTATEMENT(d, s->val.blockS.body, localSym);
				//if (symbol_mode) printf("\t}\n");

				break;
			case k_statementExpression:
				symImplementationEXP(d, s->val.expressionS.exp, sym);
				filterExpressionStatement(s->val.expressionS.exp, sym);
				break;
			case k_statementAssignment:
				
				symImplementationEXP(d, s->val.assignmentS.ids, sym);
				symImplementationEXP(d, s->val.assignmentS.exp, sym);
				
				// Left-hand side verification
				
				checkAssignmentLeft(s->val.assignmentS.ids, sym);

				// Compare both sides

				checkAssignmentType(s->val.assignmentS.ids, s->val.assignmentS.exp, sym);

				break;
			case k_statementDeclaration:
				symInterfaceDECLARATION(s->val.declarationS.decl, sym);
				symImplementationDECLARATION(s->val.declarationS.decl, sym);
				break;
			case k_statementIncrement:
				symImplementationEXP(d, s->val.incrementS.exp, sym);
				checkIncrement(s->val.incrementS.exp, sym);
				break;
			case k_statementDecrement:
				symImplementationEXP(d, s->val.decrementS.exp, sym);
				checkDecrement(s->val.incrementS.exp, sym);
				break;
			case k_statementPrint:
				symImplementationEXP(d, s->val.printS.printlist, sym);
				checkPrint(s->val.printS.printlist, sym);
				break;
			case k_statementPrintln:

				symImplementationEXP(d, s->val.printlnS.printlist, sym);
				
				checkPrintln(s->val.printlnS.printlist, sym);
				
				break;
			case k_statementReturn:

				symImplementationEXP(d, s->val.returnS.exp, sym);

				if (s->val.returnS.exp != NULL) checkReturn(s->val.returnS.exp, d->val.functionD.returnType);
				
				break;
			case k_statementIf:
				symImplementationEXP(d, s->val.ifS.condition, sym);

				localSym = scopeSymbolTable(sym);

				//if (symbol_mode) printf("\t{\n");
				symImplementationSTATEMENT(d, s->val.ifS.body, localSym);
				//if (symbol_mode) printf("\t}\n");

				checkIfCondition(s->val.ifS.condition);

				break;
			case k_statementIfElse:
				symImplementationEXP(d, s->val.ifelseS.condition, sym);
				checkIfCondition(s->val.ifelseS.condition);

				localSym = scopeSymbolTable(sym);

				//if (symbol_mode) printf("\t{\n");
				symImplementationSTATEMENT(d, s->val.ifelseS.thenPart, localSym);
				//if (symbol_mode) printf("\t}\n");

				if (s->nextIf != NULL) {
					symImplementationSTATEMENT(d, s->nextIf, sym);
				}
				else {
					localSym2 = scopeSymbolTable(sym);

					//if (symbol_mode) printf("\t{\n");
					symImplementationSTATEMENT(d, s->val.ifelseS.elsePart, localSym2);
					//if (symbol_mode) printf("\t}\n");
				}
				break;
			case k_statementSwitch:

				symImplementationEXP(d, s->val.switchS.condition, sym);
				symImplementationSTATEMENT(d, s->val.switchS.clauses, sym);

				checkSwitch(s->val.switchS.condition, s->val.switchS.clauses, sym);
				break;
			case k_statementSwitchCase:
				
				symImplementationEXP(d, s->val.caseclauseS.explist, sym);
				symImplementationSTATEMENT(d, s->nextClause, sym);

				localSym = scopeSymbolTable(sym);

				//if (symbol_mode) printf("\t{\n");
				symImplementationSTATEMENT(d, s->val.caseclauseS.content, localSym);
				//if (symbol_mode) printf("}\n");

				break;
			case k_statementSwitchDefault:

				symImplementationSTATEMENT(d, s->nextClause, sym);

				localSym = scopeSymbolTable(sym);

				//if (symbol_mode) printf("\t{\n");
				symImplementationSTATEMENT(d, s->val.defaultclauseS.content, localSym);
				//if (symbol_mode) printf("}\n");

				break;
			case k_statementFor:
				
				symImplementationSTATEMENT(d, s->val.forS.initStmt, sym);
				symImplementationEXP(d, s->val.forS.condition, sym);

				symImplementationSTATEMENT(d, s->val.forS.postStmt, sym);
				
				checkFor(s->val.forS.condition);

				localSym = scopeSymbolTable(sym);

				//if (symbol_mode) printf("\t{\n");
				symImplementationSTATEMENT(d, s->val.forS.body, localSym);
				//if (symbol_mode) printf("\t}\n");

				break;
			case k_statementBreak:
				break;
			case k_statementContinue:
				break;
			case k_statementSkip:
				break;
		}

		symImplementationSTATEMENT(d, s->next, sym);
	}
}

/* EXPRESSION SYMBOL ANALYSIS */
void symImplementationEXP(DECLARATION *par, EXP *e, SymbolTable *sym)
{	
	SYMBOL *s;
	SYMBOL *s2;

	if (e != NULL) {

		symImplementationEXP(par, e->next, sym);

		switch (e->kind) {

			case k_expressionIdentifier:

				
				if (strcmp(e->val.identifierE.id, "true") == 0 || strcmp(e->val.identifierE.id, "false") == 0) {
					
					goto BOOL_LABEL;
				}

				if (strcmp(e->val.identifierE.id, "_") != 0) {

					s = getSymbol(sym, e->val.identifierE.id);

					if (s != NULL) {

						switch (s->kind) {

							case packageSym:
								printf("Error: ill-defined identifier expression.\n");
								exit(1);
								break;
							case variableSym:
								
								switch (s->val.declarationS->val.variableD.format) {

									case 1:
										e->typeK = s->val.declarationS->val.variableD.type->typeK;
										e->isArray = s->val.declarationS->val.variableD.type->isArray;
										break;
									case 2:

										setVariableType3(e, s->val.declarationS->val.variableD.ids);

										break;
									case 3:
										e->typeK = s->val.declarationS->val.variableD.type->typeK;
										e->isArray = s->val.declarationS->val.variableD.type->isArray;
										break;
								}

								break;
							case typeSym:
								printf("Error: ill-defined identifier expression.\n");
								exit(1);
								break;
							case functionSym:
								printf("Error: ill-defined identifier expression.\n");
								exit(1);
								break;
							case formalSym:
								e->typeK = s->val.formalS->type->typeK;
								break;
							case intSym:
								printf("Error: ill-defined identifier expression.\n");
								exit(1);
								break;
							case float64Sym:
								printf("Error: ill-defined identifier expression.\n");
								exit(1);
								break;
							case runeSym:
								printf("Error: ill-defined identifier expression.\n");
								exit(1);
								break;
							case boolSym:
								printf("Error: ill-defined identifier expression.\n");
								exit(1);
								break;
							case stringSym:
								printf("Error: ill-defined identifier expression.\n");
								exit(1);
								break;
							case trueSym:
								e->typeK = boolTYPE;
								break;
							case falseSym:
								e->typeK = boolTYPE;
								break;
							case ivariableSym:
								switch (s->val.declarationS->val.variableD.format) {

									case 1:
										e->typeK = s->val.declarationS->val.variableD.type->typeK;
										e->isArray = s->val.declarationS->val.variableD.type->isArray;
										break;
									case 2:

										setVariableType3(e, s->val.declarationS->val.variableD.ids);

										break;
									case 3:
										e->typeK = s->val.declarationS->val.variableD.type->typeK;
										e->isArray = s->val.declarationS->val.variableD.type->isArray;
										break;
								}
								break;
						}

					}
					else {
						printf("Error: identifier expression undefined.\n");
						exit(1);
					}
				}

				break;
			case k_expressionParenthesis:
				symImplementationEXP(par, e->val.parenthesisE.body, sym);
				e->typeK = e->val.parenthesisE.body->typeK;
				break;
			case k_expressionInt:
				e->typeK = intTYPE;
				break;
			case k_expressionFloat64:
				e->typeK = float64TYPE;
				break;
			case k_expressionBool:

				BOOL_LABEL:
				
				s = NULL;

				// true
				if (strcmp(e->val.identifierE.id, "true") == 0) {
					s = getSymbol(sym, "true");
				}
				// false
				else {
					s = getSymbol(sym, "false");
				}

				if (s != NULL) {

					switch (s->kind) {

						case packageSym:
							printf("Error: ill-defined boolean value.\n");
							exit(1);
							break;
						case variableSym:
							printf("Error: ill-defined boolean value.\n");
							exit(1);
							break;
						case typeSym:
							printf("Error: ill-defined boolean value.\n");
							exit(1);
							break;
						case functionSym:
							printf("Error: ill-defined boolean value.\n");
							exit(1);
							break;
						case formalSym:
							printf("Error: ill-defined boolean value.\n");
							exit(1);
							break;
						case intSym:
							printf("Error: ill-defined boolean value.\n");
							exit(1);
							break;
						case float64Sym:
							printf("Error: ill-defined boolean value.\n");
							exit(1);
							break;
						case runeSym:
							printf("Error: ill-defined boolean value.\n");
							exit(1);
							break;
						case boolSym:
							printf("Error: ill-defined boolean value.\n");
							exit(1);
							break;
						case stringSym:
							printf("Error: ill-defined boolean value.\n");
							exit(1);
							break;
						case trueSym:
							e->typeK = boolTYPE;
							break;
						case falseSym:
							e->typeK = boolTYPE;
							break;
						case ivariableSym:
							printf("Error: ill-defined boolean value.\n");
							exit(1);
							break;
					}
				}
				else {
					printf("Error: boolean value undefined.\n");
					exit(1);
				}

				break;
			case k_expressionString:
				e->typeK = stringTYPE;
				break;
			case k_expressionRune:
				e->typeK = runeTYPE;
				break;
			case k_expressionUplus:
				symImplementationEXP(par, e->val.uplusE.body, sym);
				checkUplus(e->val.uplusE.body);
				e->typeK = e->val.uplusE.body->typeK;
				break;
			case k_expressionUminus:
				symImplementationEXP(par, e->val.uminusE.body, sym);
				checkUminus(e->val.uminusE.body);
				e->typeK = e->val.uplusE.body->typeK;
				break;
			case k_expressionUnot:
				symImplementationEXP(par, e->val.unotE.body, sym);
				checkUnot(e->val.unotE.body);
				e->typeK = e->val.uplusE.body->typeK;
				break;
			case k_expressionUxor:
				symImplementationEXP(par, e->val.uxorE.body, sym);
				checkUxor(e->val.uxorE.body);
				e->typeK = e->val.uplusE.body->typeK;
				break;
			case k_expressionTor:
				symImplementationEXP(par, e->val.torE.left, sym);
				symImplementationEXP(par, e->val.torE.right, sym);
				checkTor(e, e->val.torE.left, e->val.torE.right);
				break;
			case k_expressionTand:
				symImplementationEXP(par, e->val.tandE.left, sym);
				symImplementationEXP(par, e->val.tandE.right, sym);
				checkTand(e, e->val.tandE.left, e->val.tandE.right);
				break;
			case k_expressionEq:
				symImplementationEXP(par, e->val.eqE.left, sym);
				symImplementationEXP(par, e->val.eqE.right, sym);
				checkEq(e, e->val.eqE.left, e->val.eqE.right, sym);
				break;
			case k_expressionNeq: 
				symImplementationEXP(par, e->val.neqE.left, sym);
				symImplementationEXP(par, e->val.neqE.right, sym);
				checkNeq(e, e->val.neqE.left, e->val.neqE.right, sym);
				break;
			case k_expressionL:
				symImplementationEXP(par, e->val.lE.left, sym);
				symImplementationEXP(par, e->val.lE.right, sym);
				checkL(e, e->val.lE.left, e->val.lE.right);
				break;
			case k_expressionLeq:
				symImplementationEXP(par, e->val.leqE.left, sym);
				symImplementationEXP(par, e->val.leqE.right, sym);
				checkLeq(e, e->val.leqE.left, e->val.leqE.right);
				break;
			case k_expressionG:
				symImplementationEXP(par, e->val.gE.left, sym);
				symImplementationEXP(par, e->val.gE.right, sym);
				checkG(e, e->val.gE.left, e->val.gE.right);
				break;
			case k_expressionGeq:
				symImplementationEXP(par, e->val.geqE.left, sym);
				symImplementationEXP(par, e->val.geqE.right, sym);
				checkGeq(e, e->val.geqE.left, e->val.geqE.right);
				break;
			case k_expressionPlus:
				symImplementationEXP(par, e->val.plusE.left, sym);
				symImplementationEXP(par, e->val.plusE.right, sym);
				checkPlus(e, e->val.plusE.left, e->val.plusE.right);
				break;
			case k_expressionMinus:
				symImplementationEXP(par, e->val.minusE.left, sym);
				symImplementationEXP(par, e->val.minusE.right, sym);
				checkMinus(e, e->val.minusE.left, e->val.minusE.right);
				break;
			case k_expressionOr:
				symImplementationEXP(par, e->val.orE.left, sym);
				symImplementationEXP(par, e->val.orE.right, sym);
				checkOr(e, e->val.orE.left, e->val.orE.right);
				break;
			case k_expressionXor:
				symImplementationEXP(par, e->val.xorE.left, sym);
				symImplementationEXP(par, e->val.xorE.right, sym);
				checkXor(e, e->val.xorE.left, e->val.xorE.right);
				break;	
			case k_expressionMultiply:
				symImplementationEXP(par, e->val.multiplyE.left, sym);
				symImplementationEXP(par, e->val.multiplyE.right, sym);
				checkMultiply(e, e->val.multiplyE.left, e->val.multiplyE.right);
				break;
			case k_expressionDivide:
				symImplementationEXP(par, e->val.divideE.left, sym);
				symImplementationEXP(par, e->val.divideE.right, sym);
				checkDivide(e, e->val.divideE.left, e->val.divideE.right);
				break;
			case k_expressionMod:
				symImplementationEXP(par, e->val.modE.left, sym);
				symImplementationEXP(par, e->val.modE.right, sym);
				checkMod(e, e->val.modE.left, e->val.modE.right);
				break;
			case k_expressionLshift:
				symImplementationEXP(par, e->val.lshiftE.left, sym);
				symImplementationEXP(par, e->val.lshiftE.right, sym);
				checkLshift(e, e->val.lshiftE.left, e->val.lshiftE.right);
				break;
			case k_expressionRshift:
				symImplementationEXP(par, e->val.rshiftE.left, sym);
				symImplementationEXP(par, e->val.rshiftE.right, sym);
				checkRshift(e, e->val.rshiftE.left, e->val.rshiftE.right);
				break;
			case k_expressionAnd:
				symImplementationEXP(par, e->val.andE.left, sym);
				symImplementationEXP(par, e->val.andE.right, sym);
				checkAnd(e, e->val.andE.left, e->val.andE.right);
				break;
			case k_expressionNand:
				symImplementationEXP(par, e->val.nandE.left, sym);
				symImplementationEXP(par, e->val.nandE.right, sym);
				checkNand(e, e->val.nandE.left, e->val.nandE.right);
				break;

			case k_expressionFunctionCall:

				if (strcmp(e->val.funcE.id,"init") == 0) {
					printf("Error: special function cannot be called inside a function scope.\n");
					exit(1);
				}

				if (strcmp(e->val.funcE.id,"main") == 0) {
					printf("Error: special function cannot be called inside a function scope.\n");
					exit(1);
				}

				s = getSymbol(sym, e->val.funcE.id);

				if (s != NULL) {

					switch (s->kind) {

						case packageSym:
							printf("Error: ill-defined function/type cast.\n");
							exit(1);
							break;
						case variableSym:
							printf("Error: ill-defined function/type cast.\n");
							exit(1);
							break;
						case typeSym:

							e->isCast = 1;

							if (e->val.funcE.explist == NULL) {
								printf("Error: cannot cast a null value.\n");
								exit(1);
							}

							if (countCast(e->val.funcE.explist) > 1) {
								printf("Error: cannot cast more than a value.\n");
								exit(1);
							}

							if (s->type->isArray) {
								printf("Error: cannot cast with an array conversion.\n");
								exit(1);
							}

							symImplementationEXP(par, e->val.funcE.explist, sym);
							checkTypeSymCast(s->val.declarationS->val.typeD.type, e->val.funcE.explist, sym);
							e->typeK = s->typeK;
							break;
						case functionSym:

							if (par->kind == k_declarationFunction) {

								s2 = getSymbol(sym, par->val.functionD.id);
								checkPreDeclaration(s2->order, s->order);
							}

							e->isCast = 0;

							// Special function check
							checkSpecialFunctionScope(e);
							// formal type(s) pre-verification 
							symImplementationEXP(par, e->val.funcE.explist, sym);
							checkFunctionExpressionFormal(s->val.declarationS->val.functionD.arguments, e->val.funcE.explist);
							// return type
							if (s->val.declarationS->val.functionD.returnType != NULL) e->typeK = s->val.declarationS->val.functionD.returnType->typeK;
							break;
						case formalSym:
							printf("Error: ill-defined function/type cast.\n");
							exit(1);
							break;
						case intSym:

							if (e->val.funcE.explist == NULL) {
								printf("Error: cannot cast a null value.\n");
								exit(1);
							}

							if (countCast(e->val.funcE.explist) > 1) {
								printf("Error: cannot cast more than a value.\n");
								exit(1);
							}

							e->isCast = 2;

							symImplementationEXP(par, e->val.funcE.explist, sym);

							if (e->val.funcE.explist->typeK->base == intTYPE) {
								e->typeK = intTYPE;
							}
							else if (e->val.funcE.explist->typeK->base == float64TYPE) {
								e->typeK = intTYPE;
							}
							else if (e->val.funcE.explist->typeK->base == runeTYPE) {
								e->typeK = intTYPE;
							}
							else {
								printf("Error: illegal int casting.\n");
								exit(1);
							}
							break;
						case float64Sym:

							if (e->val.funcE.explist == NULL) {
								printf("Error: cannot cast a null value.\n");
								exit(1);
							}

							if (countCast(e->val.funcE.explist) > 1) {
								printf("Error: cannot cast more than a value.\n");
								exit(1);
							}

							e->isCast = 3;

							symImplementationEXP(par, e->val.funcE.explist, sym);

							if (e->val.funcE.explist->typeK->base == float64TYPE) {
								e->typeK = float64TYPE;
							}
							else if (e->val.funcE.explist->typeK->base == intTYPE) {
								e->typeK = float64TYPE;
							}
							else if (e->val.funcE.explist->typeK->base == runeTYPE) {
								e->typeK = float64TYPE;
							}
							else {
								printf("Error: illegal float64 casting.\n");
								exit(1);
							}
							break;
						case runeSym:

							if (e->val.funcE.explist == NULL) {
								printf("Error: cannot cast a null value.\n");
								exit(1);
							}

							if (countCast(e->val.funcE.explist) > 1) {
								printf("Error: cannot cast more than a value.\n");
								exit(1);
							}

							e->isCast = 4;

							symImplementationEXP(par, e->val.funcE.explist, sym);

							if (e->val.funcE.explist->typeK->base == float64TYPE) {
								e->typeK = runeTYPE;
							}
							else if (e->val.funcE.explist->typeK->base == intTYPE) {
								e->typeK = runeTYPE;
							}
							else if (e->val.funcE.explist->typeK->base == runeTYPE) {
								e->typeK = runeTYPE;
							}
							else {
								printf("Error: illegal rune casting.\n");
								exit(1);
							}
							break;
						case boolSym:

							if (e->val.funcE.explist == NULL) {
								printf("Error: cannot cast a null value.\n");
								exit(1);
							}

							if (countCast(e->val.funcE.explist) > 1) {
								printf("Error: cannot cast more than a value.\n");
								exit(1);
							}

							e->isCast = 5;

							symImplementationEXP(par, e->val.funcE.explist, sym);

							if (e->val.funcE.explist->typeK->base == boolTYPE) {
								e->typeK = boolTYPE;
							}
							else {
								printf("Error: illegal bool casting.\n");
								exit(1);
							}
							break;
						case stringSym:

							if (e->val.funcE.explist == NULL) {
								printf("Error: cannot cast a null value.\n");
								exit(1);
							}

							e->isCast = 6;

							if (countCast(e->val.funcE.explist) > 1) {
								printf("Error: cannot cast more than a value.\n");
								exit(1);
							}

							symImplementationEXP(par, e->val.funcE.explist, sym);

							if (e->val.funcE.explist->typeK->base == stringTYPE) {
								e->typeK = stringTYPE;
							}
							else if (e->val.funcE.explist->typeK->base == intTYPE) {
								e->typeK = stringTYPE;
							}
							else if (e->val.funcE.explist->typeK->base == runeTYPE) {
								e->typeK = stringTYPE;
							}
							else {
								printf("Error: illegal string casting.\n");
								exit(1);
							}
							break;
						case trueSym:
							printf("Error: ill-defined function/type cast.\n");
							exit(1);
							break;
						case falseSym:
							printf("Error: ill-defined function/type cast.\n");
							exit(1);
							break;
						case ivariableSym:
							printf("Error: ill-defined function/type cast.\n");
							exit(1);
							break;
					}
				}
				else {
					printf("Error: function/type cast undefined.\n");
					exit(1);
				}

				break;
			case k_expressionArray:

				symImplementationEXP(par, e->val.arrayE.value, sym);

				if (e->val.arrayE.value->typeK->base != intTYPE) {
					printf("Error: illegal array construct.\n");
					exit(1);
				}


				//array expression pre-verification
				checkArray(par, e, sym);	
				checkBound(e, e, sym);
				break;
		}
	}
}

/* IDENTIFIER LIST TYPE ANALYSIS */
void symImplementationIDL(IDL *id, EXP *e, SymbolTable *sym)
{
	if (id != NULL && e != NULL) {

		symImplementationIDL(id->next, e->next, sym);

		if (strcmp(id->name, "_") != 0) id->typeK = e->typeK;
	}
}

void printTable(SymbolTable *sym) 
{
	int i = 0;

	for (i = 0; i < HashSize; i++) {
		if (sym->table[i] != NULL) {
			printf("%s\n",sym->table[i]->name);
		}
  	}
}

void symImplementationIDL2(IDL *id, DECLARATION *d, TYPE *t, SymbolTable *sym)
{	
	SYMBOL *s;
	TYPEK *ref;

	if (id != NULL && t != NULL) {

		symImplementationIDL2(id->next, d, t, sym);

		s = getSymbol(sym, id->name);

		if (s != NULL) {
			
			switch (s->kind) {

				case packageSym:
					printf("Error: package identifier.\n");
					exit(1);
					break;
				case variableSym:
					printf("Error: variable declaration identifier.\n");
					exit(1);
					break;
				case typeSym:
				
					ref = malloc(sizeof(TYPEK));
					ref->name = id->name;
				
					ref->base = t->typeK;
				
					s->typeK = ref; //
					
					s->type->isArray = t->isArray;
					
					id->typeK = ref;
					
					break;
				case functionSym:
					printf("Error: function identifier.\n");
					exit(1);
					break;
				case formalSym:
					printf("Error: formal identifier.\n");
					exit(1);
					break;
				case intSym:
					printf("Error: int identifier.\n");
					exit(1);
					break;
				case float64Sym:
					printf("Error: float64 identifier.\n");
					exit(1);
					break;
				case runeSym:
					printf("Error: rune identifier.\n");
					exit(1);
					break;
				case boolSym:
					printf("Error: bool identifier.\n");
					exit(1);
					break;
				case stringSym:
					printf("Error: string identifier.\n");
					exit(1);
					break;
				case trueSym:
					printf("Error: boolean value.\n");
					exit(1);
					break;
				case falseSym:
					printf("Error: boolean value.\n");
					exit(1);
					break;
				case ivariableSym:
					printf("Error: variable declaration identifier.\n");
					exit(1);
					break;
			}
		}
		else {
			printf("Error: issue with first pass.\n");
			exit(1);
		}	
	}
}

void symImplementationIDL3(IDL *id, TYPE *t, SymbolTable *sym)
{	
	SYMBOL *s;

	if (id != NULL && t != NULL) {

		symImplementationIDL3(id->next, t, sym);

		s = getSymbol(sym, id->name);

		if (strcmp(id->name, "_") != 0) {
			id->typeK = t->typeK;
			s->typeK = t->typeK;
		}
	}
}

/* HELPER FUNCTION SYMBOL PRINT*/
void printVariableSymbol(IDL *ids, TYPE *t, int noType, SymbolTable *sym)
{	
	char arrBuf[1000] = "";

	if (ids != NULL) {

		if (strcmp(ids->name, "_") != 0) {

			if (symbol_mode) {

				if (defSymbol(sym, ids->name)) {

					if (noType) {

						printVariableSymbol(ids->next, t, noType, sym);

						if (strcmp(ids->name, "_") != 0) {
							
							printf(" %s | variable declaration | <<infer>> \n", ids->name);	
						}
					}
					else {

						if (t->isArray && t->kind == k_typeArray) {	

							findArrayString(t, arrBuf);

							printVariableSymbol(ids->next, t, noType, sym);

							if (strcmp(ids->name, "_") != 0) {

								printf(" %s | variable declaration | %s \n", ids->name, arrBuf);	
							}
						}
						else {

							printVariableSymbol(ids->next, t, noType, sym);

							if (strcmp(ids->name, "_") != 0) {

								printf(" %s | variable declaration | %s \n", ids->name, t->typeK->name);	
							}
						}
					}
				}
				else {
					printf("Error: variable declaration undefined.\n");
					exit(1);
				}
			}
		}
	}	
}

void printTypeDeclaration(IDL *ids, TYPE *t, SymbolTable *sym)
{	
	char arrBuf[1000] = "";

	if (ids != NULL) {

		if (strcmp(ids->name, "_") != 0) {

			if (symbol_mode) {

				if (defSymbol(sym, ids->name)) {

					switch (t->kind) {

						case k_typeReference:
							printTypeDeclaration(ids->next, t, sym);
							printf(" %s | type declaration | %s \n", ids->name, t->typeK->name);
							break;
						case k_typeArray:
							printTypeDeclaration(ids->next, t, sym);
							findArrayString(t, arrBuf);
							printf(" %s | type declaration | %s \n", ids->name, arrBuf);
							break;

						case k_typeParenthesis:
							printTypeDeclaration(ids, t->val.parT.type, sym);
							break;
					}
				}
				else {
					printf("Error: type declaration undefined.\n");
					exit(1);
				}			
			}
		}
	}
}

void functionTypes(FORMAL *f, TYPE *t, char *buf)
{	
	char arrBuf[1000] = "";

	if (f != NULL && t != NULL) {

		functionTypes(f->next, t, buf);

		if (f->type->isArray) {
			
			findArrayString(f->type, arrBuf);
			strcat(buf, arrBuf);
		}
		else {
			strcat(buf, " ");
			strcat(buf, f->type->typeK->name);
			strcat(buf, " ");
		}
	}
}

void functionReturnType(TYPE *t, char *buf)
{		
	char arrBuf[1000] = "";

	if (t != NULL) {

		if (t->isArray) {

			findArrayString(t, arrBuf);
			strcat(buf, ": ");
			strcat(buf, arrBuf);
		}
		else {
			strcat(buf, ": ");
			strcat(buf, t->typeK->name);
		}
	}
}

void formalType(TYPE *t, FORMAL *f)
{	
	char arrBuf[1000] = "";

	if (f != NULL && t != NULL) {

		if (t->isArray) {

			findArrayString(t, arrBuf);

			printf("%s | formal | %s \n", f->name, arrBuf);
		}
		else {

			printf("%s | formal | %s \n", f->name, t->typeK->name);
		}	
	}
}

void findArrayString(TYPE *t, char *arrBuf) 
{	
	char buf[12];

	if (t != NULL) {

		sprintf(buf, "%d", t->val.arrayT.value);
			
		strcat(arrBuf, "[");
		strcat(arrBuf, buf);
		strcat(arrBuf, "]");

		switch (t->val.arrayT.type->kind) {

			case k_typeReference:
				strcat(arrBuf, t->typeK->name);
				strcat(arrBuf, "\0");
				break;
			case k_typeArray:
				findArrayString(t->val.arrayT.type, arrBuf);
				break;
			case k_typeParenthesis:
				findArrayString(t->val.parT.type, arrBuf);
				break;
		}	
	}
}

void checkBound(EXP *par, EXP *e, SymbolTable *sym)
{	
	SYMBOL *s;

	if (e != NULL) {

		if (e->kind == k_expressionIdentifier) {

			s = getSymbol(sym, e->val.identifierE.id);
			
			if (s->type != NULL) {
				compareBound(s->type, par);
			}
		}
		else {
			checkBound(par, e->val.arrayE.exp, sym);
		}
	}
}

void compareBound(TYPE *t, EXP *e)
{	
	int stack[10] = {0};

	if (t != NULL && e != NULL) {

		fillLeft(stack, t, 0, t->isArray);
		reverseArray(stack, 0, 9);
		compareRight(stack, e, 10 - 1, 10);
	}
}

void reverseArray(int arr[], int start, int end)
{
    int temp;
    while (start < end)
    {
        temp = arr[start];   
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }   
}

void fillLeft(int arr[], TYPE *t, int start, int end)
{
	if (t != NULL) {

		if (start == end) return;

		switch (t->kind) {

			case k_typeReference:
				break;
			case k_typeArray:
				arr[start] = t->val.arrayT.value;
				fillLeft(arr, t->val.arrayT.type, start+1, end);
				break;
			case k_typeParenthesis:
				fillLeft(arr, t->val.parT.type, start, end);
				break;
		}
	}
}

void compareRight(int arr[], EXP *e, int start, int end)
{
	if (e != NULL) {

		if (e->kind == k_expressionIdentifier || start == end) return;

		if (e->val.arrayE.value->val.intconst >= arr[start]) {
			printf("Error: out-of-bounds\n");
			exit(1);
		}

		compareRight(arr ,e->val.arrayE.exp, start+1, end);
	}
}





