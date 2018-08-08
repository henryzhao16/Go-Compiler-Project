#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"

void codePROGRAM(PROGRAM *p, char *name)
{	
	emitFILE = fopen(name,"w");

	/* Emit "includes" */
	fprintf(emitFILE, "%s\n", "#include <stdio.h>");
	fprintf(emitFILE, "%s\n", "#include <string.h>");
	fprintf(emitFILE, "%s\n", "#include <stdlib.h>");
	fprintf(emitFILE, "%s\n\n", "#include <stdbool.h>");
	fprintf(emitFILE, "%s\n\n", "#include <math.h>");

	/* Program */
	if (p != NULL) {
		codePACKAGE(p->package);
		golite_cmp_array_fct();
		codeINIT(p->declarations);
		codeDECLARATION(p->declarations);
	}

	fprintf(emitFILE, "\n");
    fclose(emitFILE); 
}

void codePACKAGE(PACKAGE *p)
{
	if (p != NULL) {
		// Comment to indicate used package
		fprintf(emitFILE, "//package %s\n\n", p->name);
	}
}

void codeINIT(DECLARATION *d)
{
	char ID[12];
	char init_id[50] = "init";

	if (d != NULL) {

		fprintf(emitFILE, "\n");

		switch (d->kind) {

			case k_declarationVariable:
				break;
			case k_declarationType:
				break;
			case k_declarationFunction:
				
				if (strcmp(d->val.functionD.id, "init") == 0) {

					if (d->val.functionD.returnType != NULL) {

						codeTYPE_f(d->val.functionD.returnType);
						fprintf(emitFILE, " ");
					}
					else {

						if (strcmp(d->val.functionD.id, "main") == 0) {

							fprintf(emitFILE, "int ");
						}
						else {
							
							fprintf(emitFILE, "void ");
						}
					}
					
					if (strcmp(d->val.functionD.id, "init") == 0) {

						sprintf(ID, "%d", globalID);
						strcat(init_id, ID);
						strcat(init_id, "\0");

						// Save to linked list
						insertFirst(globalID, init_id);
						//printLL();

						globalID++;
						initID++;

						fprintf(emitFILE, "%s", init_id);
					}
					else {
						codeIDE(d->val.functionD.id);
					}
					
					codeLPAR();
					codeFORMAL(d->val.functionD.arguments);
					codeRPAR();
					codeLBRACKET();

					if (strcmp(d->val.functionD.id, "main") == 0) {

						codeINITS();
					}

					codeSTATEMENT(d, d->val.functionD.body, NULL);

					// Explicit return statement 
					if (strcmp(d->val.functionD.id, "main") == 0) {

						fprintf(emitFILE, "return 0;\n");
					}

					codeRBRACKET();
				}

				break;
		}

		codeINIT(d->next);
	}


}

void codeDECLARATION(DECLARATION *d) 
{
	if (d != NULL) {

		fprintf(emitFILE, "\n");

		switch (d->kind) {

			case k_declarationVariable:
				
				switch (d->val.variableD.format) {

					// var x T
					case 1:
						codeIDL(d->val.variableD.type, d->val.variableD.ids);
						break;	

					// var x = exp
					case 2:	
						codeTOPDOWN(d->val.variableD.ids, d->val.variableD.expls);
						break;

					// var x T = exp
					case 3:
						codeLINE(d->val.variableD.type, d->val.variableD.ids, d->val.variableD.expls);
						break;
				}

				break;

			case k_declarationType:
				codeIDLTYPE(d->val.typeD.type, d->val.typeD.ids);
				break;

			case k_declarationFunction:

				if (strcmp(d->val.functionD.id, "_") != 0 && strcmp(d->val.functionD.id, "init") != 0) {

					if (d->val.functionD.returnType != NULL) {

						codeTYPE_f(d->val.functionD.returnType);
						fprintf(emitFILE, " ");
					}
					else {

						if (strcmp(d->val.functionD.id, "main") == 0) {

							fprintf(emitFILE, "int ");
						}
						else {
							
							fprintf(emitFILE, "void ");
						}
					}
					
					codeIDE(d->val.functionD.id);
					
					codeLPAR();
					codeFORMAL(d->val.functionD.arguments);
					codeRPAR();
					codeLBRACKET();

					if (strcmp(d->val.functionD.id, "main") == 0) {

						codeINITS();
					}

					codeSTATEMENT(d, d->val.functionD.body, NULL);

					// Explicit return statement 
					if (strcmp(d->val.functionD.id, "main") == 0) {

						fprintf(emitFILE, "return 0;\n");
					}

					codeRBRACKET();
				}

				break;
		}

		codeDECLARATION(d->next);
	}
}

void codeINITS()
{	
	/*
	if (head_inits != NULL) {

		struct node *ptr = head_inits;

		while (ptr != NULL) {

			fprintf(emitFILE, "%s()", ptr->data);
			codeSEMICOLON();

			ptr = ptr->next;
		}
	} */

	int count = 0;

	while (count < initID) {

		fprintf(emitFILE, "init%d()", count);
		codeSEMICOLON();
		count++;
	}
}

void codeTYPE_g(TYPE *t)
{
	if (t != NULL) {

		if (strcmp(t->typeK->name, "int") == 0) {
			fprintf(emitFILE, "int");
		}
		else if (strcmp(t->typeK->name, "float64") == 0) {
			fprintf(emitFILE, "float");
		}
		else if (strcmp(t->typeK->name, "rune") == 0) {
			fprintf(emitFILE, "char");
		}
		else if (strcmp(t->typeK->name, "bool") == 0) {
			fprintf(emitFILE, "bool");
		}
		else if (strcmp(t->typeK->name, "string") == 0) {
			fprintf(emitFILE, "char*");
		}
		else {
			fprintf(emitFILE, "%s", t->typeK->name);
		}
	}
}

void codeTYPE_f(TYPE *t)
{
	if (t != NULL) {

		if (strcmp(t->typeK->name, "int") == 0) {
			fprintf(emitFILE, "int");
		}
		else if (strcmp(t->typeK->name, "float64") == 0) {
			fprintf(emitFILE, "float");
		}
		else if (strcmp(t->typeK->name, "rune") == 0) {
			fprintf(emitFILE, "char");
		}
		else if (strcmp(t->typeK->name, "bool") == 0) {
			fprintf(emitFILE, "bool");
		}
		else if (strcmp(t->typeK->name, "string") == 0) {
			fprintf(emitFILE, "char*");
		}
		else {
			fprintf(emitFILE, "%s", t->typeK->name);
		}

		if (t->isArray) {

			fprintf(emitFILE, " *");
		}
	}
}

void codeTYPE(TYPE *t)
{	
	char buf[1000] = "";

	if (t != NULL) {

		if (strcmp(t->typeK->name, "int") == 0) {
			fprintf(emitFILE, "int");
		}
		else if (strcmp(t->typeK->name, "float64") == 0) {
			fprintf(emitFILE, "float");
		}
		else if (strcmp(t->typeK->name, "rune") == 0) {
			fprintf(emitFILE, "char");
		}
		else if (strcmp(t->typeK->name, "bool") == 0) {
			fprintf(emitFILE, "bool");
		}
		else if (strcmp(t->typeK->name, "string") == 0) {
			fprintf(emitFILE, "char*");
		}
		else {
			fprintf(emitFILE, "%s", t->typeK->name);
		}

		if (t->isArray) {

			codeARRAY(t, buf);
			strcat(buf, "\0");
			fprintf(emitFILE, "%s ", buf);
		}
	}
}

void codeARRAY(TYPE *t, char *arrBuf) 
{	
	char buf[12];

	if (t != NULL) {

		sprintf(buf, "%d", t->val.arrayT.value);
			
		strcat(arrBuf, "[");
		strcat(arrBuf, buf);
		strcat(arrBuf, "]");

		switch (t->val.arrayT.type->kind) {

			case k_typeReference:
				//strcat(arrBuf, t->typeK->name);
				strcat(arrBuf, "\0");
				break;
			case k_typeArray:
				codeARRAY(t->val.arrayT.type, arrBuf);
				break;
			case k_typeParenthesis:
				codeARRAY(t->val.parT.type, arrBuf);
				break;
		}	
	}
}

void codeTYPESP(TYPE *t)
{	
	if (t != NULL) {

		if (strcmp(t->typeK->base->name, "int") == 0) {
			fprintf(emitFILE, "int");
		}
		else if (strcmp(t->typeK->base->name, "float64") == 0) {
			fprintf(emitFILE, "float");
		}
		else if (strcmp(t->typeK->base->name, "rune") == 0) {
			fprintf(emitFILE, "char");
		}
		else if (strcmp(t->typeK->base->name, "bool") == 0) {
			fprintf(emitFILE, "bool");
		}
		else if (strcmp(t->typeK->base->name, "string") == 0) {
			fprintf(emitFILE, "char*");
		}
		else {
			fprintf(emitFILE, "%s", t->typeK->base->name);
		}
	}
}

void codeFORMALTYPE(TYPE *t)
{	
	if (t != NULL) {

		if (strcmp(t->typeK->name, "int") == 0) {
			fprintf(emitFILE, "int");
		}
		else if (strcmp(t->typeK->name, "float64") == 0) {
			fprintf(emitFILE, "float");
		}
		else if (strcmp(t->typeK->name, "rune") == 0) {
			fprintf(emitFILE, "char");
		}
		else if (strcmp(t->typeK->name, "bool") == 0) {
			fprintf(emitFILE, "bool");
		}
		else if (strcmp(t->typeK->name, "string") == 0) {
			fprintf(emitFILE, "char*");
		}
		else {
			fprintf(emitFILE, "%s", t->typeK->name);
		}
	}
}

void codeIDL(TYPE *t, IDL *id)
{	
	char buf[1000] = "";
	char inh[50] = "golite_";

	if (t != NULL && id != NULL) {

		codeIDL(t, id->next);

		if (strcmp(id->name, "_") != 0) {

			codeTYPE_g(t);

			if (strcmp(id->name, "typedef") == 0 || strcmp(id->name, "int") == 0 || strcmp(id->name, "float64") == 0 || strcmp(id->name, "rune") == 0 || strcmp(id->name, "bool") == 0 || strcmp(id->name, "string") == 0) {
				strcat(inh, id->name);
				//strcat(inh, '\0');
				fprintf(emitFILE, " %s", inh);
			} 
			else {
				fprintf(emitFILE, " %s", id->name);
			}

			if (!t->isArray && t->typeK != NULL && t->typeK->base != NULL && strcmp(t->typeK->base->name, "string") == 0) {
				fprintf(emitFILE, " = ");
				fprintf(emitFILE, "\"\"");
			}
			else if (!t->isArray && t->typeK != NULL && t->typeK->base != NULL && strcmp(t->typeK->base->name, "int") == 0) {
				fprintf(emitFILE, " = ");
				fprintf(emitFILE, "0");
			}
			else if (!t->isArray && t->typeK != NULL && t->typeK->base != NULL && strcmp(t->typeK->base->name, "float64") == 0) {
				fprintf(emitFILE, " = ");
				fprintf(emitFILE, "0.0");
			}
			else if (!t->isArray && t->typeK != NULL && t->typeK->base != NULL && strcmp(t->typeK->base->name, "rune") == 0) {
				fprintf(emitFILE, " = ");
				fprintf(emitFILE, "\'0\'");
			}
			else if (!t->isArray && t->typeK != NULL && t->typeK->base != NULL && strcmp(t->typeK->base->name, "bool") == 0) {
				fprintf(emitFILE, " = ");
				fprintf(emitFILE, "false");
			}
			
			if (t->isArray) {

				codeARRAY(t, buf);
				strcat(buf, "\0");
				fprintf(emitFILE, "%s", buf);
			}

			codeSEMICOLON();
		}
	}
}

void codeIDLTYPE(TYPE *t, IDL *id) 
{	
	char buf[1000] = "";

	if (t != NULL && id != NULL) {

		codeIDLTYPE(t, id->next);

		if (strcmp(id->name, "_") != 0) {

			codeTYPEDEF();

			if (t->isArray) {

				codeTYPESP(t);
				fprintf(emitFILE, " ");
				fprintf(emitFILE, "%s", id->name);
				
				if (t->isArray) {
					codeARRAY(t, buf);
					strcat(buf, "\0");
					fprintf(emitFILE, "%s ", buf);
				}
			}
			else {

				codeTYPESP(t);
				fprintf(emitFILE, " ");
				fprintf(emitFILE, "%s", id->name);
			}

			codeSEMICOLON();
		}
	}
}

void codeFORMAL(FORMAL *f)
{	
	char buf[1000] = "";

	if (f != NULL) {

		if (f->next != NULL) {

			codeFORMAL(f->next);
			fprintf(emitFILE, ", ");
		}
		else {
			codeFORMAL(f->next);
		}

		if (strcmp(f->name, "_") != 0) {
			
			codeFORMALTYPE(f->type);
			fprintf(emitFILE, " ");
			codeIDE(f->name);

			if (f->type->isArray) {
				codeARRAY(f->type, buf);
				strcat(buf, "\0");
				fprintf(emitFILE, "%s", buf);
			}
		}	
		else {

			codeFORMALTYPE(f->type);
			fprintf(emitFILE, " golite_blank_");
		}
	}
}

void codeSTATEMENT(DECLARATION *d, STATEMENT *s, char* continue_end)
{	
	char ID[12];
	char label[50] = "DEFAULT";
	char end[50] = "END";
	char ptr[50] = "ptr";
	char case_id[50] = "case";
	char ctn[50] = "golite_end_loop";

	if (s != NULL) {

		switch (s->kind) {

			case k_statementBlock:
				codeLBRACKET();
				codeSTATEMENT(d, s->val.blockS.body, continue_end);
				codeRBRACKET();
				break;
			case k_statementExpression:
				codeSTATEMENTEXP(s->val.expressionS.exp);
				break;
			case k_statementAssignment:
				if (s->val.assignmentS.ids->isArray && s->val.assignmentS.exp) {
					codeASSGNARRAY(s->val.assignmentS.ids, s->val.assignmentS.exp);
				}
				else {
					codeTMP(s->val.assignmentS.exp);
					codeASSIGNMENT(s->val.assignmentS.ids, s->val.assignmentS.exp);
				}
				break;
			case k_statementDeclaration:
				codeDECLARATION(s->val.declarationS.decl);
				break;
			case k_statementIncrement:
				codeEXP(s->val.incrementS.exp);
				codeINC();
				codeSEMICOLON();
				break;
			case k_statementDecrement:
				codeEXP(s->val.decrementS.exp);
				codeDEC();
				codeSEMICOLON();
				break;
			case k_statementPrint:

				if (s->val.printS.printlist == NULL) {

					codePRINTF();
					codeLPAR();
					codeQUOTE();
					codeQUOTE();
					codeRPAR();
					codeSEMICOLON();
				}
				else {
					codePRINTF();
					codeLPAR();
					codeQUOTE();
					codePRINT_w(s->val.printS.printlist);
					codeQUOTE();
					codeCOMMA();
					codeSPACE();
					codeFEXP(s->val.printS.printlist);	
					codeRPAR();
					codeSEMICOLON();
				}
				break;
			case k_statementPrintln:

				if (s->val.printS.printlist == NULL) {
	
					codePRINTF();
					codeLPAR();
					codeQUOTE();
					fprintf(emitFILE, "\\n");
					codeQUOTE();
					codeRPAR();
					codeSEMICOLON();
				}
				else {
					codePRINTF();
					codeLPAR();
					codeQUOTE();
					
					codePRINT(s->val.printlnS.printlist);
					
					codeQUOTE();
					codeCOMMA();
					codeSPACE();
					
					codeFEXP(s->val.printlnS.printlist);
					
					codeRPAR();
					codeSEMICOLON();

					fprintf(emitFILE, "printf(\"\\n\")");
					codeSEMICOLON();
				}
				break;
			case k_statementReturn:

				if (d->kind == k_declarationFunction) {

					if (d->val.functionD.returnType != NULL) {

						if (d->val.functionD.returnType->isArray) {

							sprintf(ID, "%d", ptrID);
							strcat(ID, "\0");
							strcat(ptr, ID);
							strcat(ptr, "\0");
							ptrID++;

							codeTYPESP(d->val.functionD.returnType);
							codeSPACE();
							fprintf(emitFILE, "*%s = ", ptr);
							codeEXP(s->val.returnS.exp);
							codeSEMICOLON();

							codeRETURN();

							if (s->val.returnS.exp != NULL) {
								codeSPACE();
							}

							fprintf(emitFILE, "%s", ptr);
						}
						else {

							codeRETURN();

							if (s->val.returnS.exp != NULL) {
								codeSPACE();
							}

							codeEXP(s->val.returnS.exp);
						}
					}
					else {
						codeRETURN();
					}
				}

				codeSEMICOLON();

				break;
			case k_statementIf:
				codeIF();
				codeLPAR();
				codeEXP(s->val.ifS.condition);
				codeRPAR();
				codeLBRACKET();
				codeSTATEMENT(d, s->val.ifS.body, continue_end);
				codeRBRACKET();
				break;
			case k_statementIfElse:
				
				codeIF();
				codeLPAR();
				codeEXP(s->val.ifelseS.condition);
				codeRPAR();
				codeLBRACKET();
				codeSTATEMENT(d, s->val.ifelseS.thenPart, continue_end);
				codeRBRACKET();

				if (s->nextIf != NULL) {
					codeELSE();
					codeSTATEMENT(d, s->nextIf, NULL);
				}
				else {
					codeELSE();
					codeLBRACKET();
					codeSTATEMENT(d, s->val.ifelseS.elsePart, continue_end);
					codeRBRACKET();
				}

				break;
			case k_statementSwitch:

				// Switch ID
				sprintf(ID, "%d", globalID);

				// Default label
				strcat(label, ID);
				strcat(label, "\0");
    
		    	// End label
		    	strcat(end, ID);
				strcat(end, "\0");

				// Update global id
				globalID++;
			
				if (s->val.switchS.condition != NULL) {
						
					sprintf(ID, "%d", globalID);
					strcat(case_id, ID);
					strcat(case_id, "\0");
					globalID++;

					codeTYPEK(s->val.switchS.condition->typeK->base);
					fprintf(emitFILE, " %s = ", case_id);
					codeONEEXP(s->val.switchS.condition);
					codeSEMICOLON();

					codeSWITCHFUL(d, case_id, s->val.switchS.condition, s->val.switchS.clauses, label, end);
				}
				else {
					codeSWITCHLESS(d, s->val.switchS.clauses, label, end);
				}

				// Goto default
				fprintf(emitFILE, "%s:\n", label);
				codeDEFAULTLABEL(d, s->val.switchS.clauses);

				// Goto end
				fprintf(emitFILE, "%s: ;\n", end);

				break;
			case k_statementSwitchCase:
				// IGNORE
				break;
			case k_statementSwitchDefault:	
				// IGNORE
				break;
			case k_statementFor:

				sprintf(ID, "%d", globalID);
				strcat(ID, "\0");
				strcat(ctn, ID);
				strcat(ctn, "\0");
				globalID++;

				if (s->val.forS.infinitel) {
					codeWHILE();
					codeLPAR();
					codeINF();
					codeRPAR();
					codeLBRACKET();
					codeSTATEMENT(d, s->val.forS.body, ctn);
					fprintf(emitFILE, "%s: ;\n", ctn);
					codeRBRACKET();
				}
				else if (s->val.forS.whilel) {
					codeWHILE();
					codeLPAR();
					codeEXP(s->val.forS.condition);
					codeRPAR();
					codeLBRACKET();
					codeSTATEMENT(d, s->val.forS.body, ctn);
					fprintf(emitFILE, "%s: ;\n", ctn);
					codeRBRACKET();
				}
				else if (s->val.forS.threel) {
					
					codeWHILE();
					codeLPAR();
					codeEXP(s->val.forS.condition);
					codeRPAR();
					codeLBRACKET();
					codeSTATEMENT2(d, s->val.forS.initStmt);
					codeSEMICOLON();
					codeLBRACKET();
					codeSTATEMENT(d, s->val.forS.body, ctn);
					codeRBRACKET();
					fprintf(emitFILE, "%s: ;\n", ctn);
					codeSTATEMENT2(d, s->val.forS.postStmt);
					codeSEMICOLON();
					codeRBRACKET();

					/*
					codeFOR();
					codeLPAR();
					codeSTATEMENT2(d, s->val.forS.initStmt);
					codeSC();
					codeEXP(s->val.forS.condition);
					codeSC();
					codeSTATEMENT2(d, s->val.forS.postStmt);
					codeRPAR();
					codeLBRACKET();
					codeSTATEMENT(d, s->val.forS.body);
					codeRBRACKET();*/
				}

				break;
			case k_statementBreak:	
				codeBREAK();
				codeSEMICOLON();
				break;
			case k_statementContinue:
				if (continue_end != NULL) {
					fprintf(emitFILE, "goto %s", continue_end);
					codeSEMICOLON();
				}
				else {
					codeCONTINUE();
					codeSEMICOLON();
				}
				
				break;
			case k_statementSkip:
				codeSEMICOLON();
				break;
		}

		codeSTATEMENT(d, s->next, continue_end);
	}
}

void codeASSGNARRAY(EXP *left, EXP *right)
{
	if (left != NULL && right != NULL && left->isArray && right->isArray) {

		codeASSGNARRAY(left->next, right->next);

		fprintf(emitFILE, "memcpy(");
		codeONEEXP(left);
		fprintf(emitFILE,",");
		codeONEEXP(right);
		fprintf(emitFILE, ",");
		fprintf(emitFILE, "sizeof(");
		codeONEEXP(left);
		fprintf(emitFILE, ") / sizeof(");
		codeTYPEK(left->typeK->base);
		fprintf(emitFILE, "))");
		codeSEMICOLON();
	}
}

void codeSTATEMENT_switch(DECLARATION *d, STATEMENT *s, char* endlabel)
{	
	char ID[12];
	char label[50] = "DEFAULT";
	char end[50] = "END";
	char ptr[50] = "ptr";
	char case_id[50] = "case";

	if (s != NULL) {

		switch (s->kind) {

			case k_statementBlock:
				codeLBRACKET();
				codeSTATEMENT_switch(d, s->val.blockS.body, endlabel);
				codeRBRACKET();
				break;
			case k_statementExpression:
				codeSTATEMENTEXP(s->val.expressionS.exp);
				break;
			case k_statementAssignment:
				codeASSIGNMENT(s->val.assignmentS.ids, s->val.assignmentS.exp);
				break;
			case k_statementDeclaration:
				codeDECLARATION(s->val.declarationS.decl);
				break;
			case k_statementIncrement:
				codeEXP(s->val.incrementS.exp);
				codeINC();
				codeSEMICOLON();
				break;
			case k_statementDecrement:
				codeEXP(s->val.decrementS.exp);
				codeDEC();
				codeSEMICOLON();
				break;
			case k_statementPrint:
				if (s->val.printS.printlist == NULL) {
					codePRINTF();
					codeLPAR();
					codeQUOTE();
					codeQUOTE();
					codeRPAR();
					codeSEMICOLON();
				}
				else {
					codePRINTF();
					codeLPAR();
					codeQUOTE();
					codePRINT(s->val.printS.printlist);
					codeQUOTE();
					codeCOMMA();
					codeSPACE();
					codeFEXP(s->val.printS.printlist);	
					codeRPAR();
					codeSEMICOLON();
				}
				break;
			case k_statementPrintln:

				if (s->val.printS.printlist == NULL) {
					codePRINTF();
					codeLPAR();
					codeQUOTE();
					codeQUOTE();
					codeRPAR();
					codeSEMICOLON();
				}
				else {
					codePRINTF();
					codeLPAR();
					codeQUOTE();
					
					codePRINT(s->val.printlnS.printlist);
					
					codeQUOTE();
					codeCOMMA();
					codeSPACE();
					
					codeFEXP(s->val.printlnS.printlist);
					
					codeRPAR();
					codeSEMICOLON();

					fprintf(emitFILE, "printf(\"\\n\")");
					codeSEMICOLON();
				}
				break;
			case k_statementReturn:

				if (d->kind == k_declarationFunction) {

					if (d->val.functionD.returnType != NULL) {

						if (d->val.functionD.returnType->isArray) {

							sprintf(ID, "%d", ptrID);
							strcat(ID, "\0");
							strcat(ptr, ID);
							strcat(ptr, "\0");
							ptrID++;

							codeTYPESP(d->val.functionD.returnType);
							codeSPACE();
							fprintf(emitFILE, "*%s = ", ptr);
							codeEXP(s->val.returnS.exp);
							codeSEMICOLON();

							codeRETURN();

							if (s->val.returnS.exp != NULL) {
								codeSPACE();
							}

							fprintf(emitFILE, "%s", ptr);
						}
						else {

							codeRETURN();

							if (s->val.returnS.exp != NULL) {
								codeSPACE();
							}

							codeEXP(s->val.returnS.exp);
						}
					}
				}

				codeSEMICOLON();

				break;
			case k_statementIf:
				codeIF();
				codeLPAR();
				codeEXP(s->val.ifS.condition);
				codeRPAR();
				codeLBRACKET();
				codeSTATEMENT_switch(d, s->val.ifS.body, endlabel);
				codeRBRACKET();
				break;
			case k_statementIfElse:
				
				codeIF();
				codeLPAR();
				codeEXP(s->val.ifelseS.condition);
				codeRPAR();
				codeLBRACKET();
				codeSTATEMENT_switch(d, s->val.ifelseS.thenPart, endlabel);
				codeRBRACKET();

				if (s->nextIf != NULL) {
					codeELSE();
					codeSTATEMENT_switch(d, s->nextIf, endlabel);
				}
				else {
					codeELSE();
					codeLBRACKET();
					codeSTATEMENT_switch(d, s->val.ifelseS.elsePart, endlabel);
					codeRBRACKET();
				}

				break;
			case k_statementSwitch:

				// Switch ID
				sprintf(ID, "%d", globalID);

				// Default label
				strcat(label, ID);
				strcat(label, "\0");
    
		    	// End label
		    	strcat(end, ID);
				strcat(end, "\0");

				// Update global id
				globalID++;
			
				if (s->val.switchS.condition != NULL) {
						
					sprintf(ID, "%d", globalID);
					strcat(case_id, ID);
					strcat(case_id, "\0");
					globalID++;

					codeTYPEK(s->val.switchS.condition->typeK->base);
					fprintf(emitFILE, " %s = ", case_id);
					codeONEEXP(s->val.switchS.condition);
					codeSEMICOLON();

					codeSWITCHFUL(d, case_id, s->val.switchS.condition, s->val.switchS.clauses, label, end);
				}
				else {
					codeSWITCHLESS(d, s->val.switchS.clauses, label, end);
				}

				// Goto default
				fprintf(emitFILE, "%s:\n", label);
				codeDEFAULTLABEL(d, s->val.switchS.clauses);

				// Goto end
				fprintf(emitFILE, "%s: ;\n", end);

				break;
			case k_statementSwitchCase:
				// IGNORE
				break;
			case k_statementSwitchDefault:	
				// IGNORE
				break;
			case k_statementFor:

				if (s->val.forS.infinitel) {
					codeWHILE();
					codeLPAR();
					codeINF();
					codeRPAR();
					codeLBRACKET();
					codeSTATEMENT_switch(d, s->val.forS.body, endlabel);
					codeRBRACKET();
				}
				else if (s->val.forS.whilel) {
					codeWHILE();
					codeLPAR();
					codeEXP(s->val.forS.condition);
					codeRPAR();
					codeLBRACKET();
					codeSTATEMENT_switch(d, s->val.forS.body, endlabel);
					codeRBRACKET();
				}
				else if (s->val.forS.threel) {
					codeFOR();
					codeLPAR();
					codeSTATEMENT2(d, s->val.forS.initStmt);
					codeSC();
					codeEXP(s->val.forS.condition);
					codeSC();
					codeSTATEMENT2(d, s->val.forS.postStmt);
					codeRPAR();
					codeLBRACKET();
					codeSTATEMENT_switch(d, s->val.forS.body, endlabel);
					codeRBRACKET();
				}

				break;
			case k_statementBreak:	
				fprintf(emitFILE, "goto %s", endlabel);
				codeSEMICOLON();
				break;
			case k_statementContinue:
				codeCONTINUE();
				codeSEMICOLON();
				break;
			case k_statementSkip:
				codeSEMICOLON();
				break;
		}

		codeSTATEMENT_switch(d, s->next, endlabel);
	}
}

void codeSTATEMENT2(DECLARATION *d, STATEMENT *s)
{	
	char ID[12];
	char label[50] = "DEFAULT";
	char end[50] = "END";
	char case_id[50] = "case";

	if (s != NULL) {

		switch (s->kind) {

			case k_statementBlock:
				codeLBRACKET();
				codeSTATEMENT(d, s->val.blockS.body, NULL);
				codeRBRACKET();
				break;
			case k_statementExpression:
				codeSTATEMENTEXP(s->val.expressionS.exp);
				break;
			case k_statementAssignment:
				codeTMP(s->val.assignmentS.exp);
				codeASSIGNMENT(s->val.assignmentS.ids, s->val.assignmentS.exp);
				break;
			case k_statementDeclaration:
				codeDECLARATION(s->val.declarationS.decl);
				break;
			case k_statementIncrement:
				codeEXP(s->val.incrementS.exp);
				codeINC();
				break;
			case k_statementDecrement:
				codeEXP(s->val.decrementS.exp);
				codeDEC();
				break;
			case k_statementPrint:

				if (s->val.printS.printlist == NULL) {
					codePRINTF();
					codeLPAR();
					codeQUOTE();
					codeQUOTE();
					codeRPAR();
					codeSEMICOLON();
				}
				else {
					codePRINTF();
					codeLPAR();
					codeQUOTE();
					codePRINT(s->val.printS.printlist);
					codeQUOTE();
					codeCOMMA();
					codeSPACE();
					codeEXP(s->val.printS.printlist);	
					codeRPAR();
				}
				break;
			case k_statementPrintln:
				if (s->val.printS.printlist == NULL) {
					codePRINTF();
					codeLPAR();
					codeQUOTE();
					codeQUOTE();
					codeRPAR();
					codeSEMICOLON();
				}
				else {
					codePRINTF();
					codeLPAR();
					codeQUOTE();
					codePRINT(s->val.printlnS.printlist);
					codeQUOTE();
					codeCOMMA();
					codeSPACE();
					codeEXP(s->val.printlnS.printlist);	
					codeRPAR();

					fprintf(emitFILE, "printf(\"\\n\")");
				}
				break;
			case k_statementReturn:
				codeRETURN();

				if (s->val.returnS.exp != NULL) {
					codeSPACE();
				}

				codeEXP(s->val.returnS.exp);
				break;
			case k_statementIf:
				codeIF();
				codeLPAR();
				codeEXP(s->val.ifS.condition);
				codeRPAR();
				codeLBRACKET();
				codeSTATEMENT(d, s->val.ifS.body, NULL);
				codeRBRACKET();
				break;
			case k_statementIfElse:
				
				codeIF();
				codeLPAR();
				codeEXP(s->val.ifelseS.condition);
				codeRPAR();
				codeLBRACKET();
				codeSTATEMENT(d, s->val.ifelseS.thenPart, NULL);
				codeRBRACKET();

				if (s->nextIf != NULL) {
					codeELSE();
					codeSTATEMENT(d, s->nextIf, NULL);
				}
				else {
					codeELSE();
					codeLBRACKET();
					codeSTATEMENT(d, s->val.ifelseS.elsePart, NULL);
					codeRBRACKET();
				}

				break;
			case k_statementSwitch:

				// Switch ID
				sprintf(ID, "%d", globalID);

				// Default label
				strcat(label, ID);
				strcat(label, "\0");
    
		    	// End label
		    	strcat(end, ID);
				strcat(end, "\0");

				// Update global id
				globalID++;
			
				if (s->val.switchS.condition != NULL) {

					sprintf(ID, "%d", globalID);
					strcat(case_id, ID);
					strcat(case_id, "\0");
					globalID++;

					codeTYPEK(s->val.switchS.condition->typeK->base);
					fprintf(emitFILE, " %s = ", case_id);
					codeONEEXP(s->val.switchS.condition);
					codeSEMICOLON();

					codeSWITCHFUL(d, case_id, s->val.switchS.condition, s->val.switchS.clauses, label, end);
				}
				else {
					codeSWITCHLESS(d, s->val.switchS.clauses, label, end);
				}

				// Goto default
				fprintf(emitFILE, "%s:\n", label);
				codeDEFAULTLABEL(d, s->val.switchS.clauses);

				// Goto end
				fprintf(emitFILE, "%s: ;\n", end);

				break;
			case k_statementSwitchCase:
				// IGNORE
				break;
			case k_statementSwitchDefault:	
				// IGNORE
				break;
			case k_statementFor:

				if (s->val.forS.infinitel) {
					codeWHILE();
					codeLPAR();
					codeINF();
					codeRPAR();
					codeLBRACKET();
					codeSTATEMENT(d, s->val.forS.body, NULL);
					codeRBRACKET();
				}
				else if (s->val.forS.whilel) {
					codeWHILE();
					codeLPAR();
					codeEXP(s->val.forS.condition);
					codeRPAR();
					codeLBRACKET();
					codeSTATEMENT(d, s->val.forS.body, NULL);
					codeRBRACKET();
				}
				else if (s->val.forS.threel) {
					codeFOR();
					codeLPAR();
					codeSTATEMENT(d, s->val.forS.initStmt, NULL);
					codeSEMICOLON();
					codeSC();
					codeEXP(s->val.forS.condition);
					codeSC();
					codeSTATEMENT(d, s->val.forS.postStmt, NULL);
					codeRPAR();
					codeLBRACKET();
					codeSTATEMENT(d, s->val.forS.body, NULL);
					codeRBRACKET();
					codeSEMICOLON();
				}

				break;
			case k_statementBreak:	
				codeBREAK();
				break;
			case k_statementContinue:
				codeCONTINUE();
				break;
			case k_statementSkip:
				break;
		}

		codeSTATEMENT2(d, s->next);
	}
}

void codeEXP(EXP *e) 
{	
	char* arrayID;

	if (e != NULL) {

		if (e->next != NULL) {

			codeEXP(e->next);
			fprintf(emitFILE, ", ");
		}
		else {
			codeEXP(e->next);
		}

		switch (e->kind) {

			case k_expressionIdentifier:

				if (strcmp(e->val.identifierE.id, "_") != 0) {

					codeIDE(e->val.identifierE.id);
				}

				break;
			case k_expressionParenthesis:
				codeLPAR();
				codeEXP(e->val.parenthesisE.body);
				codeRPAR();
				break;
			case k_expressionInt:
				codeINT(e->val.intconst);
				break;
			case k_expressionFloat64:
				codeFLOAT64(e->val.floatconst);
				break;
			case k_expressionBool:
				codeBOOL(e->val.boolconst);
				break;
			case k_expressionString:
				codeSTRING(e->val.stringconst);
				break;
			case k_expressionRune:
				codeRUNE(e->val.runeconst);
				break;
			case k_expressionUplus:
				codeUPLUS();
				codeEXP(e->val.uplusE.body);
				break;
			case k_expressionUminus:
				codeUMINUS();
				codeEXP(e->val.uminusE.body);
				break;
			case k_expressionUnot:
				codeUNOT();
				codeEXP(e->val.unotE.body);
				break;
			case k_expressionUxor:
				codeUXOR();
				codeEXP(e->val.uxorE.body);
				break;
			case k_expressionTor:
				codeEXP(e->val.torE.left);
				codeTOR();
				codeEXP(e->val.torE.right);
				break;
			case k_expressionTand:
				codeEXP(e->val.tandE.left);
				codeTAND();
				codeEXP(e->val.tandE.right);
				break;
			case k_expressionEq:

				if (e->val.eqE.left->isArray && e->val.eqE.right->isArray) {
					fprintf(emitFILE, "golite_cmp_array(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ",");
					codeEXP(e->val.eqE.right);
					fprintf(emitFILE, ",sizeof(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ") / sizeof(");
					codeTYPEK(e->val.eqE.left->typeK->base);
					fprintf(emitFILE, ")");
					fprintf(emitFILE, ")? \"true\" : \"false\"");
				}
				else if (e->val.eqE.left->typeK != NULL && e->val.eqE.right->typeK != NULL && strcmp(e->val.eqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.eqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeEXP(e->val.eqE.right);
					fprintf(emitFILE, ") == 0");
				}
				else {
					codeEXP(e->val.eqE.left);
					codeEQ();
					codeEXP(e->val.eqE.right);
				}
				
				break;
			case k_expressionNeq: 

				if (e->val.neqE.left->isArray && e->val.neqE.right->isArray) {
					fprintf(emitFILE, "golite_cmp_array(");
					codeEXP(e->val.neqE.left);
					fprintf(emitFILE, ",");
					codeEXP(e->val.neqE.right);
					fprintf(emitFILE, ",sizeof(");
					codeEXP(e->val.neqE.left);
					fprintf(emitFILE, ") / sizeof(");
					codeTYPEK(e->val.neqE.left->typeK->base);
					fprintf(emitFILE, ")");
					fprintf(emitFILE, ")? \"false\" : \"true\"");
				}
				else if (e->val.neqE.left->typeK != NULL && e->val.neqE.right->typeK != NULL && strcmp(e->val.neqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.neqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeEXP(e->val.eqE.right);
					fprintf(emitFILE, ") != 0");
				}
				else {
					codeEXP(e->val.neqE.left);
					codeNEQ();
					codeEXP(e->val.neqE.right);
				}
				
				break;
			case k_expressionL:
				if (e->val.lE.left->typeK != NULL && e->val.lE.right->typeK != NULL && strcmp(e->val.lE.left->typeK->base->name, "string") == 0 && strcmp(e->val.lE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeEXP(e->val.eqE.right);
					fprintf(emitFILE, ") < 0");
				}
				else {
					codeEXP(e->val.lE.left);
					codeL();
					codeEXP(e->val.lE.right);
				}
				break;
			case k_expressionLeq:
				if (e->val.leqE.left->typeK != NULL && e->val.leqE.right->typeK != NULL && strcmp(e->val.leqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.leqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeEXP(e->val.eqE.right);
					fprintf(emitFILE, ") <= 0");
				}
				else {
					codeEXP(e->val.leqE.left);
					codeLEQ();
					codeEXP(e->val.leqE.right);
				}
				break;
			case k_expressionG:
				if (e->val.gE.left->typeK != NULL && e->val.gE.right->typeK != NULL && strcmp(e->val.gE.left->typeK->base->name, "string") == 0 && strcmp(e->val.gE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeEXP(e->val.eqE.right);
					fprintf(emitFILE, ") > 0");
				}
				else {
					codeEXP(e->val.gE.left);
					codeG();
					codeEXP(e->val.gE.right);
				}
				break;
			case k_expressionGeq:
				if (e->val.geqE.left->typeK != NULL && e->val.geqE.right->typeK != NULL && strcmp(e->val.geqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.geqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeEXP(e->val.eqE.right);
					fprintf(emitFILE, ") >= 0");
				}
				else {
					codeEXP(e->val.geqE.left);
					codeGEQ();
					codeEXP(e->val.geqE.right);
				}
				break;
			case k_expressionPlus:
				if (e->val.plusE.left->typeK != NULL && e->val.plusE.right->typeK != NULL && strcmp(e->val.plusE.left->typeK->base->name, "string") == 0 && strcmp(e->val.plusE.right->typeK->base->name, "string") == 0) {
					
					/*
					char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
      				strcpy(str3, str1);
      				strcat(str3, str2);
					*/

					fprintf(emitFILE, "(char *) malloc(1 + strlen(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ") + (");
					codeEXP(e->val.eqE.right);
					fprintf(emitFILE, ")");

					/*
					fprintf(emitFILE, "%s", "strcat(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeEXP(e->val.eqE.right);
					fprintf(emitFILE, ")");*/
				}
				else {
					codeEXP(e->val.plusE.left);
					codePLUS();
					codeEXP(e->val.plusE.right);
				}
				break;
			case k_expressionMinus:
				codeEXP(e->val.minusE.left);
				codeMINUS();
				codeEXP(e->val.minusE.right);
				break;
			case k_expressionOr:
				codeEXP(e->val.orE.left);
				codeOR();
				codeEXP(e->val.orE.right);
				break;
			case k_expressionXor:
				codeEXP(e->val.xorE.left);
				codeXOR();
				codeEXP(e->val.xorE.right);
				break;
			case k_expressionMultiply:
				codeEXP(e->val.multiplyE.left);
				codeMUL();
				codeEXP(e->val.multiplyE.right);
				break;
			case k_expressionDivide:
				codeEXP(e->val.divideE.left);
				codeDIV();
				codeEXP(e->val.divideE.right);
				break;
			case k_expressionMod:
				codeEXP(e->val.modE.left);
				codeMOD();
				codeEXP(e->val.modE.right);
				break;
			case k_expressionLshift:
				codeEXP(e->val.lshiftE.left);
				codeLSHIFT();
				codeEXP(e->val.lshiftE.right);
				break;
			case k_expressionRshift:
				codeEXP(e->val.rshiftE.left);
				codeRSHIFT();
				codeEXP(e->val.rshiftE.right);
				break;
			case k_expressionAnd:
				codeEXP(e->val.andE.left);
				codeAND();
				codeEXP(e->val.andE.right);
				break;
			case k_expressionNand:
				fprintf(emitFILE, "(");
				fprintf(emitFILE, "(");
				fprintf(emitFILE, "(");
				codeEXP(e->val.nandE.left);
				fprintf(emitFILE, ")");
				fprintf(emitFILE, "&");
				fprintf(emitFILE, "(");
				codeEXP(e->val.nandE.right);
				fprintf(emitFILE, ")");
				fprintf(emitFILE, ")");
				fprintf(emitFILE, "^");
				codeEXP(e->val.nandE.left);
				fprintf(emitFILE, ")");
				break;
			case k_expressionFunctionCall:

				if (e->isCast == 1) {
					codeEXP(e->val.funcE.explist);
				}
				else if (e->isCast == 2) {
					
				}
				else {
					
					codeIDE(e->val.funcE.id);
					codeLPAR();
					codeEXP(e->val.funcE.explist);
					codeRPAR();
				}
				
				break;
			case k_expressionArray:

				arrayID = findArrayName(e);

				if (strcmp(arrayID, "_") != 0) {

					codeEXP(e->val.arrayE.exp);
					codeLBRACE();
					codeEXP(e->val.arrayE.value);
					codeRBRACE();
				}
				break;
		}
	}
}

void codeFEXP(EXP *e) 
{	
	char* arrayID;

	if (e != NULL) {

		if (e->next != NULL) {

			codeFEXP(e->next);
			fprintf(emitFILE, ", ");
		}
		else {
			codeFEXP(e->next);
		}

		switch (e->kind) {

			case k_expressionIdentifier:

				if (strcmp(e->val.identifierE.id, "true") == 0 || strcmp(e->val.identifierE.id, "false") == 0) {
					goto BOOL_LABEL;
				}

				if (strcmp(e->val.identifierE.id, "_") != 0) {

					if (e->typeK != NULL && e->typeK->base != NULL && strcmp(e->typeK->base->name, "bool") == 0) {
						
						fprintf(emitFILE, "%s? \"true\" : \"false\"", e->val.identifierE.id);
					}
					else {
						codeIDE(e->val.identifierE.id);
					}
				}

				break;
			case k_expressionParenthesis:
				codeLPAR();
				codeFEXP(e->val.parenthesisE.body);
				codeRPAR();
				break;
			case k_expressionInt:
				
				codeINT(e->val.intconst);
				break;
			case k_expressionFloat64:
				
				//codeFLOAT64(e->val.floatconst);
				fprintf(emitFILE, "\"%s\"", e->val.floatconst);
				break;
			case k_expressionBool:
				
				//codeBOOL(e->val.boolconst);

				BOOL_LABEL:
				
				if (strcmp(e->val.identifierE.id, "true") == 0) {

					fprintf(emitFILE, "\"%s\"", "true");
				}
				else {

					fprintf(emitFILE, "\"%s\"", "false");
				}

				break;
			case k_expressionString:

				codeSTRING(e->val.stringconst);
				break;
			case k_expressionRune:

				codeRUNE(e->val.runeconst);
				break;
			case k_expressionUplus:
				codeUPLUS();
				codeFEXP(e->val.uplusE.body);
				break;
			case k_expressionUminus:
				codeUMINUS();
				codeFEXP(e->val.uminusE.body);
				break;
			case k_expressionUnot:
				codeUNOT();
				codeFEXP(e->val.unotE.body);
				break;
			case k_expressionUxor:
				codeUXOR();
				codeFEXP(e->val.uxorE.body);
				break;
			case k_expressionTor:
				codeFEXP(e->val.torE.left);
				codeTOR();
				codeFEXP(e->val.torE.right);
				break;
			case k_expressionTand:
				codeFEXP(e->val.tandE.left);
				codeTAND();
				codeFEXP(e->val.tandE.right);
				break;
			case k_expressionEq:

				if (e->val.eqE.left->isArray && e->val.eqE.left->isArray) {
					fprintf(emitFILE, "golite_cmp_array(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ",");
					codeEXP(e->val.eqE.right);
					fprintf(emitFILE, ",sizeof(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ") / sizeof(");
					codeTYPEK(e->val.eqE.left->typeK->base);
					fprintf(emitFILE, ")");
					fprintf(emitFILE, ")? \"true\" : \"false\"");
				}
				else if (e->val.eqE.left->typeK != NULL && e->val.eqE.right->typeK != NULL && strcmp(e->val.eqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.eqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "%s", "(strcmp(");
					codeFEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeFEXP(e->val.eqE.right);
					fprintf(emitFILE, ") == 0)? \"true\" : \"false\"");
				}
				else {
					codeFEXP(e->val.eqE.left);
					codeEQ();
					codeFEXP(e->val.eqE.right);
				}
				
				break;
			case k_expressionNeq: 

				if (e->val.neqE.left->isArray && e->val.neqE.left->isArray) {
					fprintf(emitFILE, "golite_cmp_array(");
					codeEXP(e->val.neqE.left);
					fprintf(emitFILE, ",");
					codeEXP(e->val.neqE.right);
					fprintf(emitFILE, ",sizeof(");
					codeEXP(e->val.neqE.left);
					fprintf(emitFILE, ") / sizeof(");
					codeTYPEK(e->val.neqE.left->typeK->base);
					fprintf(emitFILE, ")");
					fprintf(emitFILE, ")? \"false\" : \"true\"");
				}
				else if (e->val.neqE.left->typeK != NULL && e->val.neqE.right->typeK != NULL && strcmp(e->val.neqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.neqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "%s", "(strcmp(");
					codeFEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeFEXP(e->val.eqE.right);
					fprintf(emitFILE, ") != 0)? \"true\" : \"false\"");
				}
				else {
					codeFEXP(e->val.neqE.left);
					codeNEQ();
					codeFEXP(e->val.neqE.right);
				}
				
				break;
			case k_expressionL:
				if (e->val.lE.left->typeK != NULL && e->val.lE.right->typeK != NULL && strcmp(e->val.lE.left->typeK->base->name, "string") == 0 && strcmp(e->val.lE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "%s", "(strcmp(");
					codeFEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeFEXP(e->val.eqE.right);
					fprintf(emitFILE, ") < 0)? \"true\" : \"false\"");
				}
				else {
					codeFEXP(e->val.lE.left);
					codeL();
					codeFEXP(e->val.lE.right);
				}
				break;
			case k_expressionLeq:
				if (e->val.leqE.left->typeK != NULL && e->val.leqE.right->typeK != NULL && strcmp(e->val.leqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.leqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "%s", "(strcmp(");
					codeFEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeFEXP(e->val.eqE.right);
					fprintf(emitFILE, ") <= 0)? \"true\" : \"false\"");
				}
				else {
					codeFEXP(e->val.leqE.left);
					codeLEQ();
					codeFEXP(e->val.leqE.right);
				}
				break;
			case k_expressionG:
				if (e->val.gE.left->typeK != NULL && e->val.gE.right->typeK != NULL && strcmp(e->val.gE.left->typeK->base->name, "string") == 0 && strcmp(e->val.gE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "%s", "(strcmp(");
					codeFEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeFEXP(e->val.eqE.right);
					fprintf(emitFILE, ") > 0)? \"true\" : \"false\"");
				}
				else {
					codeFEXP(e->val.gE.left);
					codeG();
					codeFEXP(e->val.gE.right);
				}
				break;
			case k_expressionGeq:
				if (e->val.geqE.left->typeK != NULL && e->val.geqE.right->typeK != NULL && strcmp(e->val.geqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.geqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "%s", "(strcmp(");
					codeFEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeFEXP(e->val.eqE.right);
					fprintf(emitFILE, ") >= 0)? \"true\" : \"false\"");
				}
				else {
					codeFEXP(e->val.geqE.left);
					codeGEQ();
					codeFEXP(e->val.geqE.right);
				}
				break;
			case k_expressionPlus:
				
				if (e->val.plusE.left->typeK != NULL && e->val.plusE.right->typeK != NULL && strcmp(e->val.plusE.left->typeK->base->name, "string") == 0 && strcmp(e->val.plusE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "%s", "strcat(");
					codeFEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeFEXP(e->val.eqE.right);
					fprintf(emitFILE, ")");
				}
				else {
					codeFEXP(e->val.plusE.left);
					codePLUS();
					codeFEXP(e->val.plusE.right);
				}
				break;
			case k_expressionMinus:
				codeFEXP(e->val.minusE.left);
				codeMINUS();
				codeFEXP(e->val.minusE.right);
				break;
			case k_expressionOr:
				codeFEXP(e->val.orE.left);
				codeOR();
				codeFEXP(e->val.orE.right);
				break;
			case k_expressionXor:
				codeFEXP(e->val.xorE.left);
				codeXOR();
				codeFEXP(e->val.xorE.right);
				break;
			case k_expressionMultiply:
				codeFEXP(e->val.multiplyE.left);
				codeMUL();
				codeFEXP(e->val.multiplyE.right);
				break;
			case k_expressionDivide:
				codeFEXP(e->val.divideE.left);
				codeDIV();
				codeFEXP(e->val.divideE.right);
				break;
			case k_expressionMod:
				codeFEXP(e->val.modE.left);
				codeMOD();
				codeFEXP(e->val.modE.right);
				break;
			case k_expressionLshift:
				codeFEXP(e->val.lshiftE.left);
				codeLSHIFT();
				codeFEXP(e->val.lshiftE.right);
				break;
			case k_expressionRshift:
				codeFEXP(e->val.rshiftE.left);
				codeRSHIFT();
				codeFEXP(e->val.rshiftE.right);
				break;
			case k_expressionAnd:
				codeFEXP(e->val.andE.left);
				codeAND();
				codeFEXP(e->val.andE.right);
				break;
			case k_expressionNand:
				fprintf(emitFILE, "(");
				fprintf(emitFILE, "(");
				fprintf(emitFILE, "(");
				codeFEXP(e->val.nandE.left);
				fprintf(emitFILE, ")");
				fprintf(emitFILE, "&");
				fprintf(emitFILE, "(");
				codeFEXP(e->val.nandE.right);
				fprintf(emitFILE, ")");
				fprintf(emitFILE, ")");
				fprintf(emitFILE, "^");
				codeFEXP(e->val.nandE.left);
				fprintf(emitFILE, ")");
				break;
			case k_expressionFunctionCall:

				if (e->isCast) {
					codeFEXP(e->val.funcE.explist);
				}
				else {
					codeIDE(e->val.funcE.id);
					codeLPAR();
					codeFEXP(e->val.funcE.explist);
					codeRPAR();
				}
				
				break;
			case k_expressionArray:
				
				arrayID = findArrayName(e);

				if (strcmp(arrayID, "_") != 0) {

					codeFEXP(e->val.arrayE.exp);

					codeLBRACE();

					codeFEXP(e->val.arrayE.value);
					codeRBRACE();

				}
				break;
		}
	}
}

void codeTYPEK(TYPEK *tk) 
{
	if (tk != NULL) {

		if (strcmp(tk->name,"int") == 0) {
			fprintf(emitFILE, "int");
		}
		else if (strcmp(tk->name,"float64") == 0) {
			fprintf(emitFILE, "floatt");
		}
		else if (strcmp(tk->name,"string") == 0) {
			fprintf(emitFILE, "char*");
		}
		else if (strcmp(tk->name,"bool") == 0) {
			fprintf(emitFILE, "bool");
		}
		else if (strcmp(tk->name,"rune") == 0) {
			fprintf(emitFILE, "char");
		}
	}
}

// Special cases
void codeSWITCHFUL(DECLARATION *d, char* case_id, EXP *e, STATEMENT *s, char* label, char* end)
{	
	if (e != NULL && s != NULL) {

		if (s->kind == k_statementSwitchCase) {
			
			codeCASE_v(d, case_id, e, s->val.caseclauseS.explist, s->val.caseclauseS.content, label, end);
		}
		else if (s->kind == k_statementSwitchDefault) {
			
			codeDEFAULT(s->val.defaultclauseS.content, label, end);
		}

		codeSWITCHFUL(d, case_id, e, s->nextClause, label, end);
	}
}

void codeSWITCHLESS(DECLARATION *d, STATEMENT *s, char *label, char *end)
{
	if (s != NULL) {

		if (s->kind == k_statementSwitchCase) {
			
			codeCASE(d, s->val.caseclauseS.explist, s->val.caseclauseS.content, label, end);
		}
		else if (s->kind == k_statementSwitchDefault) {
			
			codeDEFAULT(s->val.defaultclauseS.content, label, end);
		}
		
		codeSWITCHLESS(d, s->nextClause, label, end);
	}
}

void codeCASE_v(DECLARATION *d, char* case_id, EXP *condition, EXP *e, STATEMENT *s, char *label, char *end)
{
	if (condition != NULL && e != NULL && s != NULL) {

		codeCASE_v(d, case_id, condition, e->next, s, label, end);

		codeIF();
		codeLPAR();
		fprintf(emitFILE, "%s", case_id);
		codeEQ();
		codeONEEXP(e);
		codeRPAR();
		codeLBRACKET();
		codeSTATEMENT_switch(d, s, end);
		codeGOTO();
		fprintf(emitFILE, "%s;\n", end);
		codeRBRACKET();
	}
}

void codeCASE(DECLARATION *d, EXP *e, STATEMENT *s, char *label, char *end)
{
	if (e != NULL && s != NULL) {

		codeCASE(d, e->next, s, label, end);

		codeIF();
		codeLPAR();
		codeONEEXP(e);
		codeRPAR();
		codeLBRACKET();
		codeSTATEMENT_switch(d, s, end);
		codeGOTO();
		fprintf(emitFILE, "%s;\n", end);
		codeRBRACKET();
	}
}

void codeDEFAULT(STATEMENT *s, char *label, char* end)
{
	if (s != NULL) {

		codeGOTO();
		fprintf(emitFILE, "%s;\n", label);
	}
}

void codeDEFAULTLABEL(DECLARATION *d, STATEMENT *s) 
{
	if (s != NULL) {

		if (s->kind == k_statementSwitchDefault) {

			codeSTATEMENT(d, s->val.defaultclauseS.content, NULL);
		}

		codeDEFAULTLABEL(d, s->nextClause);
	}
}

void codePRINT(EXP *e)
{
	if (e != NULL) {

		if (e->next != NULL) {
			codePRINT(e->next);
			fprintf(emitFILE, " ");
		}
		else {
			codePRINT(e->next);
		}

		if (strcmp(e->typeK->base->name, "int") == 0) {
			fprintf(emitFILE, "%%d");
		}
		else if (strcmp(e->typeK->base->name, "float64") == 0) {
			
			if (e->kind == k_expressionIdentifier) {
				fprintf(emitFILE, "%%f");
			}
			else if (e->kind == k_expressionFloat64) {
				fprintf(emitFILE, "%%s");
			}
			else {
				fprintf(emitFILE, "%%f");
			}
		}
		else if (strcmp(e->typeK->base->name, "rune") == 0) {
			fprintf(emitFILE, "%%d");
		}
		else if (strcmp(e->typeK->base->name, "bool") == 0) {
			fprintf(emitFILE, "%%s");
		}
		else if (strcmp(e->typeK->base->name, "string") == 0) {
			fprintf(emitFILE, "%%s");
		}
	}
}

void codePRINT_w(EXP *e)
{
	if (e != NULL) {

		if (e->next != NULL) {
			codePRINT(e->next);
			fprintf(emitFILE, "%s", "");
		}
		else {
			codePRINT(e->next);
		}

		if (strcmp(e->typeK->base->name, "int") == 0) {
			fprintf(emitFILE, "%%d");
		}
		else if (strcmp(e->typeK->base->name, "float64") == 0) {
			
			if (e->kind == k_expressionIdentifier) {
				fprintf(emitFILE, "%%f");
			}
			else if (e->kind == k_expressionFloat64) {
				fprintf(emitFILE, "%%s");
			}
			else {
				fprintf(emitFILE, "%%f");
			}
		}
		else if (strcmp(e->typeK->base->name, "rune") == 0) {
			fprintf(emitFILE, "%%d");
		}
		else if (strcmp(e->typeK->base->name, "bool") == 0) {
			fprintf(emitFILE, "%%s");
		}
		else if (strcmp(e->typeK->base->name, "string") == 0) {
			fprintf(emitFILE, "%%s");
		}
	}
}

void codeSTATEMENTEXP(EXP *e)
{
	if (e != NULL) {

		codeONEEXP(e);
		codeSEMICOLON();

		codeSTATEMENTEXP(e->next);
	}
}

void codeASSIGNMENT(EXP *id, EXP *e)
{	
	char ID[12];
	char tmp_id[50] = "TMP_";

	if (id != NULL && e != NULL) {

		sprintf(ID, "%d", globalID);
		strcat(tmp_id, ID);
		strcat(tmp_id, "\0");

		globalID++;
		codeASSIGNMENT(id->next, e->next);

		if (id->kind == k_expressionIdentifier) {

			if (strcmp(id->val.identifierE.id, "_") != 0) {

				codeONEEXP(id);
				codeEQUAL();
				fprintf(emitFILE, "%s", tmp_id);
				//codeONEEXP(e);
				codeSEMICOLON();
			}
		}
		else if (id->kind == k_expressionArray) {

			char *arrayID = findArrayName(id);

			if (strcmp(arrayID, "_") != 0) {
				codeONEEXP(id);
				codeEQUAL();
				fprintf(emitFILE, "%s", tmp_id);
				//codeONEEXP(e);
				codeSEMICOLON();
			}			
		}
	}
}

void codeTMP(EXP *e)
{	
	char ID[12];
	char tmp_id[50] = "TMP_";

	if (e != NULL) {

		sprintf(ID, "%d", globalID);
		strcat(tmp_id, ID);
		strcat(tmp_id, "\0");
		globalID++;

		codeTMP(e->next);

		codeTYPEK(e->typeK->base);
		codeSPACE();
		fprintf(emitFILE, "%s", tmp_id);
		codeEQUAL();
		codeONEEXP(e);
		codeSEMICOLON();
		globalID--;
	}
}

void codeASSIGNMENT2(EXP *id, EXP *e)
{
	if (id != NULL && e != NULL) {

		codeASSIGNMENT2(id->next, e->next);

		if (id->kind == k_expressionIdentifier) {

			if (strcmp(id->val.identifierE.id, "_") != 0) {

				codeONEEXP(id);
				codeEQUAL();
				codeONEEXP(e);
			}
		}
		else if (id->kind == k_expressionArray) {

			char *arrayID = findArrayName(id);

			if (strcmp(arrayID, "_") != 0) {
				codeONEEXP(id);
				codeEQUAL();
				codeONEEXP(e);
			}			
		}
	}
}

void codeLINE(TYPE *t, IDL *id, EXP *e)
{	
	char ID[12];
	char buf_init[50] = "golite_buffer";

	if (t != NULL && id != NULL && e != NULL) {

		codeLINE(t, id->next, e->next);
		
		if (strcmp(id->name, "_") != 0) {

			if (t->typeK != NULL && strcmp(t->typeK->base->name, "string") == 0 && e->kind == k_expressionPlus) {

				codeTYPE(t);
				fprintf(emitFILE, " ");
				codeIDE(id->name);
				codeEQUAL();
				codeCONCAT(e, id->name);

			}
			else if (e->kind == k_expressionFunctionCall) {

				sprintf(ID, "%d", globalID);
				strcat(buf_init, ID);
				strcat(buf_init, "\0");
				globalID++;

				fprintf(emitFILE, "char %s[50];\n", buf_init);

				if (e->isCast == 6) {

					if (e->val.funcE.explist->typeK != NULL && strcmp(e->val.funcE.explist->typeK->base->name, "int") == 0) {
						
						fprintf(emitFILE, "char golite_tmp = (char) ");
						codeONEEXP(e->val.funcE.explist);
						codeSEMICOLON();

						//codeCAST2(e);
						fprintf(emitFILE, "sprintf(%s, \"%%c\", golite_tmp);\n", buf_init);

						codeTYPE(t);
						codeIDE(id->name);
						codeEQUAL();
						fprintf(emitFILE, "%s;\n", buf_init);
					}
					else {
						codeCAST2(e, buf_init);

						codeTYPE(t);
						fprintf(emitFILE, " ");
						codeIDE(id->name);
						codeEQUAL();
						fprintf(emitFILE, "%s;\n", buf_init);
					}
				}
				else {
					codeTYPE(t);
					fprintf(emitFILE, " ");
					codeIDE(id->name);
					codeEQUAL();
					codeONEEXP(e);
					codeSEMICOLON();
				}
			}
			else {

				if (e->isCast) {

					fprintf(emitFILE, "%s ", e->val.funcE.id);
					codeIDE(id->name);
					codeEQUAL();
					codeONEEXP(e);
					codeSEMICOLON();
				}
				else {

					if (e->kind == k_expressionIdentifier) {
						if (strcmp(e->val.identifierE.id, id->name) == 0) {
							//
						}
						else {
							codeTYPE(t);
						}
					}
					else {
						codeTYPE(t);
					}
					
					fprintf(emitFILE, " ");
					codeIDE(id->name);
					codeEQUAL();
					codeONEEXP(e);
					codeSEMICOLON();
				}
			}
		}
	}
}

void codeCONCAT(EXP *e, char* id)
{
	if (e != NULL && e->kind == k_expressionPlus) {

		/*
		(char *) malloc(1 + strlen(str1)+ strlen(str2) );
      	strcpy(str3, str1);
      	strcat(str3, str2);
		*/

		fprintf(emitFILE, "(char *) malloc(1 + strlen(");
		codeONEEXP(e->val.plusE.left);
		fprintf(emitFILE, ") + strlen(");
		codeONEEXP(e->val.plusE.right);
		fprintf(emitFILE, "))");
		codeSEMICOLON();
		fprintf(emitFILE, "strcpy(");
		fprintf(emitFILE, "%s,", id);
		codeONEEXP(e->val.plusE.left);
		fprintf(emitFILE, ");");
		fprintf(emitFILE, "strcat(%s,", id);
		codeONEEXP(e->val.plusE.right);
		fprintf(emitFILE, ");");

	}
} 

void codeTOPDOWN(IDL *id, EXP *e)
{	
	char ID[12];
	char tmp_id[50] = "ARG_";


	if (id != NULL && e != NULL) {

		sprintf(ID, "%d", globalID);
		strcat(tmp_id, ID);
		strcat(tmp_id, "\0");

		globalID++;

		codeTOPDOWN(id->next, e->next);

		if (strcmp(id->name, "_") != 0) {


			if (e->isCast) {

				fprintf(emitFILE, "%s ", e->val.funcE.id);
				codeIDE(id->name);
				codeEQUAL();
				codeONEEXP(e);
				codeSEMICOLON();
			}
			else {
				
				if (e->kind == k_expressionIdentifier) {
					if (strcmp(e->val.identifierE.id, id->name) == 0) {
						//
					}
					else {
						inferTYPE(e);
					}
				}
				else {
					inferTYPE(e);
				}
				codeONEIDL(id);
				codeEQUAL();
				

				if (e->kind == k_expressionFunctionCall) {

					codeARG(e->val.funcE.explist);
					
					fprintf(emitFILE, "%s", e->val.funcE.id);
					codeLPAR();
					putARG(e->val.funcE.explist);
					codeRPAR();
					//fprintf(emitFILE, "%s", tmp_id);
					//codeONEEXP(e);
					codeSEMICOLON();

				}
				else {
					codeONEEXP(e);
				}

				codeSEMICOLON();
			}
		}
	}
}

void putARG(EXP *e)
{	
	char ID[12];
	char tmp_id[50] = "ARG_";

	if (e != NULL) {

		sprintf(ID, "%d", globalID);
		strcat(tmp_id, ID);
		strcat(tmp_id, "\0");

		globalID++;

		if (e->next != NULL) {
			putARG(e->next);
			printf(", ");
		}
		else {
			putARG(e->next);
		}

		fprintf(emitFILE, "%s", tmp_id);
	}
}

void codeARG(EXP *e)
{	
	char ID[12];
	char tmp_id[50] = "ARG_";

	if (e != NULL) {

		sprintf(ID, "%d", globalID);
		strcat(tmp_id, ID);
		strcat(tmp_id, "\0");
		globalID++;

		codeARG(e->next);

		codeTYPEK(e->typeK->base);
		codeSPACE();
		fprintf(emitFILE, "%s", tmp_id);
		codeEQUAL();
		codeONEEXP(e);
		codeSEMICOLON();

		globalID--;
	}
}

void inferTYPE(EXP *e)
{	
	if (e != NULL) {

		if (strcmp(e->typeK->name, "int") == 0) {
			fprintf(emitFILE, "int ");
		}
		else if (strcmp(e->typeK->name, "float64") == 0) {
			fprintf(emitFILE, "float ");
		}
		else if (strcmp(e->typeK->name, "rune") == 0) {
			fprintf(emitFILE, "char ");
		}
		else if (strcmp(e->typeK->name, "bool") == 0) {
			fprintf(emitFILE, "bool ");
		}
		else if (strcmp(e->typeK->name, "string") == 0) {
			fprintf(emitFILE, "char* ");
		}
		else {
			fprintf(emitFILE, "%s ", e->typeK->name);
		}
	}
} 

void codeONEIDL(IDL *id) 
{
	if (id != NULL) {

		fprintf(emitFILE, "%s", id->name);
	}
}

void codeONEEXP(EXP *e)
{
	char* arrayID;

	if (e != NULL) {

		switch (e->kind) {

			case k_expressionIdentifier:

				if (strcmp(e->val.identifierE.id, "_") != 0) {

					codeIDE(e->val.identifierE.id);
				}
		
				break;
			case k_expressionParenthesis:
				codeLPAR();
				codeONEEXP(e->val.parenthesisE.body);
				codeRPAR();
				break;
			case k_expressionInt:
				codeINT(e->val.intconst);
				break;
			case k_expressionFloat64:

				codeFLOAT64(e->val.floatconst);
				break;
			case k_expressionBool:
				codeBOOL(e->val.boolconst);
				break;
			case k_expressionString:
				//codeLQUOTE();
				codeSTRING(e->val.stringconst);
				//codeRQUOTE();
				break;
			case k_expressionRune:
				//codeSLQUOTE();
				codeRUNE(e->val.runeconst);
				//codeSRQUOTE();
				break;
			case k_expressionUplus:
				codeUPLUS();
				codeONEEXP(e->val.uplusE.body);
				break;
			case k_expressionUminus:
				codeUMINUS();
				codeONEEXP(e->val.uminusE.body);
				break;
			case k_expressionUnot:
				codeUNOT();
				codeONEEXP(e->val.unotE.body);
				break;
			case k_expressionUxor:
				codeUXOR();
				codeONEEXP(e->val.uxorE.body);
				break;
			case k_expressionTor:
				codeONEEXP(e->val.torE.left);
				codeTOR();
				codeONEEXP(e->val.torE.right);
				break;
			case k_expressionTand:
				codeONEEXP(e->val.tandE.left);
				codeTAND();
				codeONEEXP(e->val.tandE.right);
				break;
						case k_expressionEq:

				if (e->val.eqE.left->isArray && e->val.eqE.left->isArray) {
					fprintf(emitFILE, "golite_cmp_array(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ",");
					codeEXP(e->val.eqE.right);
					fprintf(emitFILE, ",sizeof(");
					codeEXP(e->val.eqE.left);
					fprintf(emitFILE, ") / sizeof(");
					codeTYPEK(e->val.eqE.left->typeK->base);
					fprintf(emitFILE, ")");
					fprintf(emitFILE, ")? \"true\" : \"false\"");
				}
				else if (e->val.eqE.left->typeK != NULL && e->val.eqE.right->typeK != NULL && strcmp(e->val.eqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.eqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeFEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeFEXP(e->val.eqE.right);
					fprintf(emitFILE, ") == 0");
				}
				else {
					codeFEXP(e->val.eqE.left);
					codeEQ();
					codeFEXP(e->val.eqE.right);
				}
				
				break;
			case k_expressionNeq: 

				if (e->val.neqE.left->isArray && e->val.neqE.left->isArray) {
					fprintf(emitFILE, "golite_cmp_array(");
					codeEXP(e->val.neqE.left);
					fprintf(emitFILE, ",");
					codeEXP(e->val.neqE.right);
					fprintf(emitFILE, ",sizeof(");
					codeEXP(e->val.neqE.left);
					fprintf(emitFILE, ") / sizeof(");
					codeTYPEK(e->val.neqE.left->typeK->base);
					fprintf(emitFILE, ")");
					fprintf(emitFILE, ")? \"false\" : \"true\"");
				}
				else if (e->val.neqE.left->typeK != NULL && e->val.neqE.right->typeK != NULL && strcmp(e->val.neqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.neqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeFEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeFEXP(e->val.eqE.right);
					fprintf(emitFILE, ") != 0");
				}
				else {
					codeFEXP(e->val.neqE.left);
					codeNEQ();
					codeFEXP(e->val.neqE.right);
				}
				
				break;
			case k_expressionL:
				if (e->val.lE.left->typeK != NULL && e->val.lE.right->typeK != NULL && strcmp(e->val.lE.left->typeK->base->name, "string") == 0 && strcmp(e->val.lE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeONEEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeONEEXP(e->val.eqE.right);
					fprintf(emitFILE, ") < 0");
				}
				else {
					codeONEEXP(e->val.lE.left);
					codeL();
					codeONEEXP(e->val.lE.right);
				}
				break;
			case k_expressionLeq:
				if (e->val.leqE.left->typeK != NULL && e->val.leqE.right->typeK != NULL && strcmp(e->val.leqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.leqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeONEEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeONEEXP(e->val.eqE.right);
					fprintf(emitFILE, ") <= 0");
				}
				else {
					codeONEEXP(e->val.leqE.left);
					codeLEQ();
					codeONEEXP(e->val.leqE.right);
				}
				break;
			case k_expressionG:
				if (e->val.gE.left->typeK != NULL && e->val.gE.right->typeK != NULL && strcmp(e->val.gE.left->typeK->base->name, "string") == 0 && strcmp(e->val.gE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeONEEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeONEEXP(e->val.eqE.right);
					fprintf(emitFILE, ") > 0");
				}
				else {
					codeONEEXP(e->val.gE.left);
					codeG();
					codeONEEXP(e->val.gE.right);
				}
				break;
			case k_expressionGeq:
				if (e->val.geqE.left->typeK != NULL && e->val.geqE.right->typeK != NULL && strcmp(e->val.geqE.left->typeK->base->name, "string") == 0 && strcmp(e->val.geqE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "strcmp(");
					codeONEEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeONEEXP(e->val.eqE.right);
					fprintf(emitFILE, ") >= 0");
				}
				else {
					codeONEEXP(e->val.geqE.left);
					codeGEQ();
					codeONEEXP(e->val.geqE.right);
				}
				break;
			case k_expressionPlus:
				if (e->val.plusE.left->typeK != NULL && e->val.plusE.right->typeK != NULL && strcmp(e->val.plusE.left->typeK->base->name, "string") == 0 && strcmp(e->val.plusE.right->typeK->base->name, "string") == 0) {
					fprintf(emitFILE, "%s", "strcat(");
					codeONEEXP(e->val.eqE.left);
					fprintf(emitFILE, ", ");
					codeONEEXP(e->val.eqE.right);
					fprintf(emitFILE, ")");
				}
				else {
					codeONEEXP(e->val.plusE.left);
					codePLUS();
					codeONEEXP(e->val.plusE.right);
				}
				break;
			case k_expressionMinus:
				codeONEEXP(e->val.minusE.left);
				codeMINUS();
				codeONEEXP(e->val.minusE.right);
				break;
			case k_expressionOr:
				codeONEEXP(e->val.orE.left);
				codeOR();
				codeONEEXP(e->val.orE.right);
				break;
			case k_expressionXor:
				codeONEEXP(e->val.xorE.left);
				codeXOR();
				codeONEEXP(e->val.xorE.right);
				break;
			case k_expressionMultiply:
				codeONEEXP(e->val.multiplyE.left);
				codeMUL();
				codeONEEXP(e->val.multiplyE.right);
				break;
			case k_expressionDivide:
				codeONEEXP(e->val.divideE.left);
				codeDIV();
				codeONEEXP(e->val.divideE.right);
				break;
			case k_expressionMod:
				codeONEEXP(e->val.modE.left);
				codeMOD();
				codeONEEXP(e->val.modE.right);
				break;
			case k_expressionLshift:
				codeONEEXP(e->val.lshiftE.left);
				codeLSHIFT();
				codeONEEXP(e->val.lshiftE.right);
				break;
			case k_expressionRshift:
				codeONEEXP(e->val.rshiftE.left);
				codeRSHIFT();
				codeONEEXP(e->val.rshiftE.right);
				break;
			case k_expressionAnd:
				codeONEEXP(e->val.andE.left);
				codeAND();
				codeONEEXP(e->val.andE.right);
				break;
			case k_expressionNand:
				fprintf(emitFILE, "(");
				fprintf(emitFILE, "(");
				fprintf(emitFILE, "(");
				codeONEEXP(e->val.nandE.left);
				fprintf(emitFILE, ")");
				fprintf(emitFILE, "&");
				fprintf(emitFILE, "(");
				codeONEEXP(e->val.nandE.right);
				fprintf(emitFILE, ")");
				fprintf(emitFILE, ")");
				fprintf(emitFILE, "^");
				codeONEEXP(e->val.nandE.left);
				fprintf(emitFILE, ")");
				break;
			case k_expressionFunctionCall:

				if (e->isCast == 1) {
					codeEXP(e->val.funcE.explist);
				}
				else if (e->isCast >= 2) {
					codeCAST(e);
				}
				else {
					codeIDE(e->val.funcE.id);
					codeLPAR();
					codeEXP(e->val.funcE.explist);
					codeRPAR();
				}
				break;
			case k_expressionArray:

				arrayID = findArrayName(e);

				if (strcmp(arrayID, "_") != 0) {

					codeONEEXP(e->val.arrayE.exp);
					codeLBRACE();
					codeONEEXP(e->val.arrayE.value);
					codeRBRACE();
				}
			
				break;
		}
	}
} 

void codeCAST(EXP *e)
{		
	if (e != NULL && e->kind == k_expressionFunctionCall) {

		if (e->isCast == 2) {
			// int
			fprintf(emitFILE, "(int)");
			codeLPAR();
			codeONEEXP(e->val.funcE.explist);
			codeRPAR();
		}
		else if (e->isCast == 3) {
			// float
			fprintf(emitFILE, "(float)");
			codeLPAR();
			codeONEEXP(e->val.funcE.explist);
			codeRPAR();
		}
		else if (e->isCast == 4) {
			// rune
			fprintf(emitFILE, "(char)");
			codeLPAR();
			codeONEEXP(e->val.funcE.explist);
			codeRPAR();
		}
		else if (e->isCast == 5) {
			// bool
			fprintf(emitFILE, "(bool)");
			codeLPAR();
			codeONEEXP(e->val.funcE.explist);
			codeRPAR();
		}
	}
}

void codeCAST2(EXP *e, char *buf)
{
	if (e != NULL) {
		if (e->val.funcE.explist->typeK != NULL && strcmp(e->val.funcE.explist->typeK->base->name, "int") == 0) {
			fprintf(emitFILE, "sprintf(%s, \"%%d\", ", buf);
			codeONEEXP(e->val.funcE.explist);
			fprintf(emitFILE, ");\n");
		}
		else if (e->val.funcE.explist->typeK != NULL && strcmp(e->val.funcE.explist->typeK->base->name, "float") == 0) {
			fprintf(emitFILE, "sprintf(%s, \"%%f\", ", buf);
			codeONEEXP(e->val.funcE.explist);
			fprintf(emitFILE, ");\n");
		}
		else if (e->val.funcE.explist->typeK != NULL && strcmp(e->val.funcE.explist->typeK->base->name, "rune") == 0) {
			fprintf(emitFILE, "sprintf(%s, \"%%c\", ", buf);
			codeONEEXP(e->val.funcE.explist);
			fprintf(emitFILE, ");\n");
		}
		else if (e->val.funcE.explist->typeK != NULL && strcmp(e->val.funcE.explist->typeK->base->name, "bool") == 0) {
			fprintf(emitFILE, "sprintf(%s, \"%%d\", ", buf);
			codeONEEXP(e->val.funcE.explist);
			fprintf(emitFILE, ");\n");
		}
		else if (e->val.funcE.explist->typeK != NULL && strcmp(e->val.funcE.explist->typeK->base->name, "string") == 0) {
			fprintf(emitFILE, "sprintf(%s, \"%%s\", ", buf);
			codeONEEXP(e->val.funcE.explist);
			fprintf(emitFILE, ");\n");
		}
	}
}

// Helpers

void codeFOR()
{
	fprintf(emitFILE, "for ");
}

void codeSC()
{
	fprintf(emitFILE, " ; ");
}

void codeINF()
{
	fprintf(emitFILE, "1");
}

void codeWHILE()
{
	fprintf(emitFILE, "while");
}

void codeBREAK()
{
	fprintf(emitFILE, "break");
}

void codeCONTINUE()
{
	fprintf(emitFILE, "continue");
}

void codeIF()
{
	fprintf(emitFILE, "if ");	
}

void codeELSE()
{
	fprintf(emitFILE, "else ");
}

void codeELSEIF()
{
	fprintf(emitFILE, "else if ");
}

void codeCOMMA()
{
	fprintf(emitFILE, ",");
}

void codeQUOTE()
{
	fprintf(emitFILE, "\"");
}

void codeSPACE()
{
	fprintf(emitFILE, " ");
}

void codePRINTF()
{
	fprintf(emitFILE, "printf");
}

void codeRETURN()
{
	fprintf(emitFILE, "return");
}

void codeINC()
{
	fprintf(emitFILE, "++");
}

void codeDEC()
{
	fprintf(emitFILE, "--");
}

void codeTYPEDEF()
{
	fprintf(emitFILE, "typedef ");
}

void codeLBRACE()
{
	fprintf(emitFILE, "[");
}

void codeRBRACE()
{
	fprintf(emitFILE, "]");
}

void codeLSHIFT()
{
	fprintf(emitFILE, " << ");
}

void codeRSHIFT()
{
	fprintf(emitFILE, " >> ");
}

void codeAND()
{
	fprintf(emitFILE, " & ");
}

void codeNAND()
{
	fprintf(emitFILE, " &^ ");
}

void codeMUL()
{
	fprintf(emitFILE, " * ");
}

void codeDIV()
{
	fprintf(emitFILE, " / ");
}

void codeMOD()
{
	fprintf(emitFILE, " %% ");
}

void codePLUS()
{
	fprintf(emitFILE, " + ");	
}

void codeMINUS()
{
	fprintf(emitFILE, " - ");	
}

void codeOR()
{
	fprintf(emitFILE, " | ");	
}

void codeXOR()
{
	fprintf(emitFILE, " ^ ");	
}

void codeL()
{
	fprintf(emitFILE, " < ");	
}

void codeLEQ()
{
	fprintf(emitFILE, " <= ");	
}

void codeG()
{
	fprintf(emitFILE, " > ");	
}

void codeGEQ()
{
	fprintf(emitFILE, " >= ");	
}

void codeTOR() 
{
	fprintf(emitFILE, " || ");	
}

void codeTAND()
{
	fprintf(emitFILE, " && ");	
}

void codeEQ()
{
	fprintf(emitFILE, " == ");	
}

void codeNEQ()
{
	fprintf(emitFILE, " != ");		
}

void codeUPLUS()
{
	fprintf(emitFILE, " + ");	
}

void codeUMINUS()
{
	fprintf(emitFILE, "-");	
}

void codeUNOT()
{
	fprintf(emitFILE, "!");	
}

void codeUXOR()
{
	fprintf(emitFILE, "^");	
}

void codeINT(int n)
{
	fprintf(emitFILE, "%d", n);
}

void codeFLOAT64(char *n)
{
	fprintf(emitFILE, "%s", n);
}

void codeBOOL(int n) 
{
	if (n == 1) {
		fprintf(emitFILE, "%s", "true");
	}
	else if (n == 0) {
		fprintf(emitFILE, "%s", "false");
	}
}

void codeSTRING(char *n)
{
	fprintf(emitFILE, "%s", n);
}

void codeRUNE(char *n)
{
	fprintf(emitFILE, "%s", n);
}

void codeLPAR() 
{
	fprintf(emitFILE, "(");
}

void codeRPAR() 
{
	fprintf(emitFILE, ")");
}

void codeLBRACKET()
{
	fprintf(emitFILE, " {\n");
}

void codeRBRACKET()
{
	fprintf(emitFILE, "}\n");
}

void codeSEMICOLON()
{
	fprintf(emitFILE, ";\n");
}

void codeEQUAL()
{
	fprintf(emitFILE, " = ");	
}

void codeIDE(char *id)
{
	fprintf(emitFILE, "%s", id);
}

void codeLQUOTE()
{
	fprintf(emitFILE, "\"");	
}

void codeRQUOTE()
{
	fprintf(emitFILE, "\"");	
}

void codeSLQUOTE()
{
	fprintf(emitFILE, "\'");
}

void codeSRQUOTE()
{
	fprintf(emitFILE, "\'");
}

void codeGOTO() 
{
	fprintf(emitFILE, "goto ");
}

char *findArrayName(EXP *e) {

	if (e != NULL) {

		if (e->kind == k_expressionIdentifier) {

			return e->val.identifierE.id;
		}
		
		if (e->kind == k_expressionArray) {

			return findArrayName(e->val.arrayE.exp);
		}
	}

	return "";
}

void golite_cmp_array_fct()
{
	fprintf(emitFILE, "int golite_cmp_array(int const *x, int const *y, size_t n) {\n");
	fprintf(emitFILE, "for (size_t i=0; i<n; i++) {\n");
	fprintf(emitFILE, "if (abs(x[i] - y[i]) > 0) { return 0; }\n } \nreturn 1;\n } \n\n");
}


