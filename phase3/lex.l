%{
	#include "y.tab.h"
	#include <bits/stdc++.h>
	int lineCounter = 1;

	extern int printLineNumber(int num);
%}
letter  [a-zA-Z]
digit [0-9]
digits {digit}+
id {letter}({letter}|{digit})*
num "-"?{digits}
floatnum "-"?{digits}.{digits}("E"{digits}+)?
relop "=="|"!="|">"|">="|"<"|"<="
assign "="
addop "+"|"-"
mulop "*"|"/"|"%"

%%
"System.out.println"	{return SYSTEM_OUT;}
"if"	{return IF_KEY;}
"else"	{return ELSE_KEY;}
"while"	{return WHILE;}
"int"	{return INT_KEY;}
"float"	{return FLOAT_KEY;}
{addop}	{yylval.stringsunion = strdup(yytext); return ADDOP;}
{mulop}	{yylval.stringsunion = strdup(yytext); return MULOP;}
{num}	{yylval.intunion = atoi(yytext); return INT;}
{floatnum}	{yylval.floatunion = atof(yytext); return FLOAT;}
{relop} {yylval.stringsunion = strdup(yytext); return RELOP;}
{id}	{yylval.stringsunion = strdup(yytext);return ID;}
{assign}    {yylval.stringsunion = strdup(yytext);return ASSIGN;}
";" { return SEMI_COLON;}
"(" { return BRACKET_OPEN;}
")" {return BRACKET_CLOSE;}
"{" {return CURLY_OPEN;}
"}" {return CURLY_CLOSE;}
\n	{++lineCounter;	printLineNumber(lineCounter);}
%%


int yywrap() {
   // open next reference or source file and start scanning
   return -1;
}
