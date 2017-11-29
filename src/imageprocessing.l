%{
#include <stdlib.h>
#include <string.h>
void yyerror(char *);

#include "y.tab.h"
%}



%%

[a-z]  {
        yylval.ival = yytext[0] - 'a';
        return VAR;
       }

[0-9]+[.][0-9]*  {
	int i;
        for (i = 0; i < yyleng; i++ )
          yylval.strval[i] = yytext[i];

        yylval.strval[yyleng] = '\0';

	      yylval.valor = atof(yytext);
        return NUM;
       }

[0-9]+  {

  yylval.inteiro = atof(yytext);
        return INTEIRO;
       }


[a-zA-Z0-9\.]+  {
	int i;
        for (i = 0; i < yyleng; i++ )
          yylval.strval[i] = yytext[i];

        yylval.strval[yyleng] = '\0';
        return STRING;
       }


(\[[a-zA-Z0-9\.]+\])  {
	int i;
        for (i = 1; i < yyleng -1; i++ ){
        	yylval.strval[i-1] = yytext[i];
	}
	yylval.strval[yyleng-1] = '\0';   
        return STRINGX;
       }


[=]     {
          return IGUAL;
        }


[*/]     {
	yylval.cval = (char) yytext[0];	
          return CONTA;
        }


\n   {
         return EOL;
       }


[ \t]+ ;

.     {printf("**%s**\n", yytext);
yyerror("Token nao reconhecido!\n");}

%%

int  yywrap() {
  return 1;
}
