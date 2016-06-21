%{
void yyerror (char *s);
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
extern FILE* yyin;
%}


%start injection
%token  TEXT LOGICAL END ALT_ENCODING PROCEDURE COMPARATOR UNION WHERE N_INSTRUCTION D_INSTRUCTION FOLLOWER APOSTROPHE SEMICOLON QUOTATION STRING NUMBER

%%


injection : tautology END {printf("tautology\n");}
		| illegal END {printf("illegal\n");}
		| union END {printf("union\n");}
		| piggy_backed END {printf("piggy_backed\n");}
		| stored_procedure END {printf("stored_procedure\n");}
		| alternate_encoding END {printf("alternate_encoding\n");}
		;
		
instruction :  N_INSTRUCTION FOLLOWER TEXT
		| N_INSTRUCTION FOLLOWER TEXT FOLLOWER
		| N_INSTRUCTION TEXT FOLLOWER
		| N_INSTRUCTION TEXT FOLLOWER TEXT
		| N_INSTRUCTION TEXT FOLLOWER TEXT FOLLOWER
		| N_INSTRUCTION FOLLOWER TEXT where_clause
		| D_INSTRUCTION FOLLOWER TEXT
		| D_INSTRUCTION FOLLOWER TEXT FOLLOWER
		;
		
tautology : TEXT APOSTROPHE
		| TEXT APOSTROPHE LOGICAL expression
		;
illegal : APOSTROPHE APOSTROPHE
		| QUOTATION QUOTATION
		| TEXT QUOTATION instruction
		| TEXT APOSTROPHE instruction
		| instruction
		;
union : TEXT APOSTROPHE union_select
		| union_select
		;
union_select : UNION N_INSTRUCTION TEXT FOLLOWER TEXT WHERE TEXT
		| UNION N_INSTRUCTION TEXT FOLLOWER TEXT WHERE unfinished_expression
		;
piggy_backed : TEXT APOSTROPHE SEMICOLON instruction
		| APOSTROPHE SEMICOLON instruction
		;
stored_procedure : TEXT APOSTROPHE SEMICOLON procedure_exp
		| APOSTROPHE SEMICOLON procedure_exp
		| TEXT APOSTROPHE SEMICOLON procedure_exp '-' '-'
		| APOSTROPHE SEMICOLON procedure_exp '-' '-'
		;
alternate_encoding : TEXT APOSTROPHE SEMICOLON alt_encoding_exp
		| APOSTROPHE SEMICOLON alt_encoding_exp
		;
		
alt_encoding_exp : ALT_ENCODING
		| ALT_ENCODING SEMICOLON
		| ALT_ENCODING '-' '-'
		| ALT_ENCODING SEMICOLON '-' '-'
		| ALT_ENCODING '-' '-' SEMICOLON
		;
		
		
procedure_exp : PROCEDURE SEMICOLON 
		| N_INSTRUCTION SEMICOLON
		| D_INSTRUCTION SEMICOLON
		;
expression : STRING COMPARATOR STRING
		| NUMBER COMPARATOR NUMBER
		| APOSTROPHE TEXT APOSTROPHE COMPARATOR APOSTROPHE TEXT APOSTROPHE
		;
unfinished_expression : APOSTROPHE TEXT APOSTROPHE COMPARATOR APOSTROPHE TEXT
		;
		
where_clause : WHERE expression
		| WHERE unfinished_expression
		| WHERE expression LOGICAL expression
		| WHERE expression LOGICAL unfinished_expression
		;
		
%%                     /* C code */


int main (int argc, char** argv) {
	yyin=fopen(argv[1],"r");
	yyparse();
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 

