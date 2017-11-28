%{
#include <stdio.h>
#include "imageprocessing.h"
#include <FreeImage.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

void yyerror(char *c);
int yylex(void);
clock_t ct0, ct1, dct;  // medida de tempo baseada no clock da CPU
struct timeval rt0,rt1,drt;   // tempo baseada em tempo real

%}
%union {
  char    strval[50];
  char    cval;
  int     ival, inteiro;
  float   valor;
}
%token <strval> STRING STRINGX 
%token <ival> VAR IGUAL EOL ASPA
%token <valor> NUM
%token <cval> CONTA
%token <inteiro> INTEIRO
%left SOMA

%%

PROGRAMA:
        PROGRAMA EXPRESSAO EOL
        |
        ;

EXPRESSAO:
    | STRING IGUAL STRING {
        printf("Copiando %s para %s\n", $3, $1);
        imagem I = abrir_imagem($3);
        printf("Li imagem %d por %d\n", I.width, I.height);
        salvar_imagem($1, &I);
        liberar_imagem(&I);
                          }

    //(Nome da imagem nova) (=) (nome da imagem a ser alterada) (*/) (brilho) (serial=0; thread=1) (linha=0; coluna=1) (número de threads desejado)
    | STRING IGUAL STRING CONTA NUM INTEIRO INTEIRO INTEIRO{  
        double mean_clock = 0.0, mean_real_sec = 0.0, mean_real_usec = 0.0;

        imagem I = abrir_imagem($3);
        
        gettimeofday(&rt0, NULL);
        ct0 = clock();
        brilho(I, $5, $4, $3, $6, $7, $8);        
        ct1 = clock();
        gettimeofday(&rt1, NULL);
        
        salvar_imagem($1, &I);
        liberar_imagem(&I);

        timersub(&rt1, &rt0, &drt);
        mean_real_sec = drt.tv_sec;    // media em segundos
        mean_real_usec = drt.tv_usec;    // media em  micro segundos
        mean_clock = (double) (ct1 - ct0)/CLOCKS_PER_SEC;

        if($6 == 0){
          
          printf("    Tempo real: %.6lf segundos.\tUtilizando Varredura Sequencial\n",mean_real_sec+mean_real_usec/1000000);
          printf("    Tempo user: %f segundos.\tUtilizando Varredura Sequencial\n",mean_clock);  
        }else{
          printf("    Tempo real: %.6lf segundos.\tUtilizando %d Threads\n",mean_real_sec+mean_real_usec/1000000, $8);
          printf("    Tempo user: %f segundos.\tUtilizando %d Threads\n",mean_clock, $8);
        }

        return 0;
                                                                    }
   | STRINGX {
        imagem I = abrir_imagem($1);
        printf("Achando o valor máximo de %s\n", $1);
        busca(I, $1);
              }
   ;
%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
  FreeImage_Initialise(0);
  yyparse();
  return 0;

}
