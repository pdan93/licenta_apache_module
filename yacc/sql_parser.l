%{
#include "y.tab.h"
%}
%%
\"[a-zA-Z0-9_ -]+\" 	{return STRING;}
[1-9]+[0-9]* 	{return NUMBER;}
"OR"|"AND"|"ANY"|"BETWEEN"|"EXISTS"|"IN"|"LIKE"|"NOT"|"IS NULL"|"UNIQUE"	{return LOGICAL;}
(=|!=|<>|>|<|>=|<=|!<|!>) 	{return COMPARATOR;}

"SELECT"|"UPDATE"|"DELETE"|"INSERT"				   {return N_INSTRUCTION;}
"DROP"|"CREATE"|"ALTER"|"TRUNCATE"				{return D_INSTRUCTION;}
"WHERE"|"TABLE"|"BETWEEN"|"DATABASE"|"INDEX"|"VIEW"|"EXISTS"|"GROUP BY"|"HAVING"|"IN"|"INTO"|"JOIN"|"LIKE" {return FOLLOWER;}
'						{return APOSTROPHE;}
;						{return SEMICOLON;}
\"						{return QUOTATION;}
[a-zA-Z0-9_ -]*			{return TEXT;}

[ \t\n]                ;
[-+=]           	   		{return yytext[0];}
"("|")"          	   			{return yytext[0];}
\[|\]          	   			{return yytext[0];}
.                      {ECHO; yyerror ("unexpected character");}

%%
int yywrap (void) {return 1;}
