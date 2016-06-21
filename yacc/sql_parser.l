%{
#include "y.tab.h"
%}
%%
\"[a-zA-Z0-9_ -]+\" 	{printf("STRING %s\n",yytext);return STRING;}
[1-9]+[0-9]* 	{printf("NUMBER %s\n",yytext);return NUMBER;}
"OR"|"AND"|"ANY"|"BETWEEN"|"EXISTS"|"IN"|"LIKE"|"NOT"|"IS NULL"|"UNIQUE"	{printf("LOGICAL %s\n",yytext);return LOGICAL;}
(=|!=|<>|>|<|>=|<=|!<|!>) 	{printf("COMPARATOR %s\n",yytext);return COMPARATOR;}
"UNION" 				{printf("UNION %s\n",yytext);return UNION;}
"WHERE"					{printf("WHERE %s\n",yytext);return WHERE;}
"SHUTDOWN"					{printf("PROCEDURE %s\n",yytext);return PROCEDURE;}
"exec(0x"[a-zA-Z0-9]+")"    {printf("ALT_ENCODING %s\n",yytext);return ALT_ENCODING;}
"SELECT"|"UPDATE"|"DELETE"|"INSERT"				   {printf("N_INSTRUCTION %s\n",yytext);return N_INSTRUCTION;}
"DROP"|"CREATE"|"ALTER"|"TRUNCATE"				{printf("D_INSTRUCTION %s\n",yytext);return D_INSTRUCTION;}
"FROM"|"TABLE"|"BETWEEN"|"DATABASE"|"INDEX"|"VIEW"|"EXISTS"|"GROUP BY"|"HAVING"|"IN"|"INTO"|"JOIN"|"LIKE" {printf("FOLLOWER %s\n",yytext);return FOLLOWER;}
'						{printf("APOSTROPHE %s\n",yytext);return APOSTROPHE;}
;						{printf("SEMICOLON %s\n",yytext);return SEMICOLON;}
\"						{printf("QUOTATION %s\n",yytext);return QUOTATION;}
[a-zA-Z0-9_*]*			{printf("TEXT %s\n",yytext);return TEXT;}

\n					{printf("END %s\n",yytext);return END;}
[ \t]                ;
[-+=]           	   		{return yytext[0];}
"("|")"          	   			{return yytext[0];}
\[|\]          	   			{return yytext[0];}
.                      ;

%%
int yywrap (void) {return 1;}
