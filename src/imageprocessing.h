
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H


typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I);
void liberar_imagem(imagem *i);
void brilho (imagem I, float intensidade, char ope, char *nome_do_arquivo, int tipo, int lc, int nthreads);
void * aplicar_brilho_por_threads(imagem *I);
void busca (imagem I, char *nome_do_arquivo);
#endif

