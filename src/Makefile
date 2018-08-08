CC = gcc

CFLAGS = -Wall

main: parser.tab.o lex.yy.o main.o tree.h tree.o weed.h weed.o pretty.h pretty.o symbol.h symbol.o type.h type.o code.h code.o
	$(CC) lex.yy.o parser.tab.o tree.o weed.o pretty.o symbol.o type.o code.o main.o -o golite -lfl

parser.tab.c parser.tab.h: golite.y
	bison --yacc --defines=parser.tab.c --output=parser.tab.h golite.y

lex.yy.c: golite.l parser.tab.h
	flex golite.l
clean:
	rm parser.tab.* lex.yy.*; rm *.o; rm golite
