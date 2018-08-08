#ifndef PRETTY_H
#define PRETTY_H

#include "tree.h"

/* NOTE: 
 * The following are the pretty print function declarations
 */

void prettyPROGRAM(PROGRAM *p);
void prettyPACKAGE(PACKAGE *p);
void prettyDECLARATION(DECLARATION *d);
void prettyIDL(IDL *i);
void prettyTYPE(TYPE *t);
void prettyFORMAL(FORMAL *f);
void prettySTATEMENT(STATEMENT *s, int extratTab, int nLine);
void prettyEXP(EXP *e);

#endif

