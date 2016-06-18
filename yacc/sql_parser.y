%{
void yyerror (char *s);
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
extern FILE* yyin;
%}


%start injections
%token TEXT LOGICAL COMPARATOR N_INSTRUCTION D_INSTRUCTION FOLLOWER APOSTROPHE SEMICOLON QUOTATION STRING NUMBER

%%

injections : injection
		| injection injections
		;
injection : tautology
		| illegal
		| union
		| piggy_backed
		| stored_procedure
		| alternate_encoding
		;
		
tautology : TEXT APOSTROPHE
		| TEXT APOSTROPHE LOGICAL expression
		;
illegal : APOSTROPHE APOSTROPHE
		| QUOTATION QUOTATION
		;
union : TEXT APOSTROPHE
		| TEXT APOSTROPHE LOGICAL expression
		;
piggy_backed : TEXT APOSTROPHE
		| TEXT APOSTROPHE LOGICAL expression
		;
stored_procedure : TEXT APOSTROPHE
		| TEXT APOSTROPHE LOGICAL expression
		;
alternate_encoding : TEXT APOSTROPHE
		| TEXT APOSTROPHE LOGICAL expression
		;
		
expression : STRING COMPARATOR STRING
		| NUMBER COMPARATOR NUMBER
		;

		
%%                     /* C code */


int main (int argc, char** argv) {
	yyin=fopen(argv[1],"r");
	yyparse();
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 

