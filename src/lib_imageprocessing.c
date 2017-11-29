
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "imageprocessing.h"
#include <FreeImage.h>

int resto_linhas, resto_colunas, contador=0, aux_lc;
int linhas_por_thread, colunas_por_thread, nthreadscopia;
float intensidadecopia;
char opecopia;

pthread_mutex_t trava; //DEfine a existência de uma trava pra ser usada no mutex


imagem abrir_imagem(char *nome_do_arquivo) {
  FIBITMAP *bitmapIn;
  int x, y, i, j;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
  } else {
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = malloc(sizeof(float) * x * y);
  I.g = malloc(sizeof(float) * x * y);
  I.b = malloc(sizeof(float) * x * y);

   for (i=0; i<x; i++) {
     for (j=0; j <y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;
    }
   }
  return I;

}

//FUNÇÃO QUE LIBERA A IMAGEM------------------------------------------------------------------------------

void liberar_imagem(imagem *I) {
  free(I->r);
  free(I->g);
  free(I->b);
}


//FUNÇÃO QUE SALVA A IMAGEM------------------------------------------------------------------------------

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;
  int i, j;

  bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);

   for (i=0; i<I->width; i++) {
     for (j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      color.rgbRed = I->r[idx];
      color.rgbGreen = I->g[idx];
      color.rgbBlue = I->b[idx];

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
}



//FUNÇÃO QUE APLICA O BRILHO NA IMAGEM-----------------------------------------------

void brilho (imagem I, float intensidade, char ope, char *nome_do_arquivo, int tipo, int lc, int nthreads){
   int x, y;
   int i, j, idx;

   opecopia = ope;
   intensidadecopia = intensidade;
   nthreadscopia = nthreads;

   /*Aqui, pegamos as matrizes das cores através de alocação dinâmica e definimos o # de colunas e # de linhas da imagem */
	//abrir_imagem(nome_do_arquivo);

   x = I.width;
   y = I.height;
  
	if(tipo == 0){ //Escolha foi que a varredura será serial (e não por thread)

		if(lc == 0){ //A varredura será feita por LINHAS
			//Se for aumentar o brilho, multiplica
		   if(ope == '*'){
			   for (i=0; i<y; i++) {
     				for (j=0; j<x; j++) {

     					idx = i + (j*y); //É utilizado esse idx pois o computador entende uma matriz com um vetor linha apenas. Aí tenho que somar tantos elementos pra ele entender que quero ir pra próxima coluna

					    I.r[idx] = I.r[idx] * intensidade;
					    I.g[idx] = I.g[idx] * intensidade;
					    I.b[idx] = I.b[idx] * intensidade;

						if(I.r[idx] > 255)
							I.r[idx] = 255;
						if(I.g[idx] > 255)
							I.g[idx] = 255;
						if(I.b[idx] > 255)
							I.b[idx] = 255;
				    }	
				}	
		    }

		   //Se for diminuir o brilho, divide
		   if(ope == '/'){
			   for (i=0; i<y; i++) {
     				for (j=0; j<x; j++) {

     					idx = i + (j*y); //É utilizado esse idx pois o computador entende uma matriz com um vetor linha apenas. Aí tenho que somar tantos elementos pra ele entender que quero ir pra próxima coluna

					    I.r[idx] = I.r[idx] / intensidade;
					    I.g[idx] = I.g[idx] / intensidade;
					    I.b[idx] = I.b[idx] / intensidade;


				    }	
				}

		   }

	    }else{ //A varredura será feita por COLUNAS
	    	if(ope == '*'){
			   for (i=0; i<x; i++) {
     				for (j=0; j<y; j++) {

     					idx = i + (j*x); //É utilizado esse idx pois o computador entende uma matriz com um vetor linha apenas. Aí tenho que somar tantos elementos pra ele entender que quero ir pra próxima coluna

					    I.r[idx] = I.r[idx] * intensidade;
					    I.g[idx] = I.g[idx] * intensidade;
					    I.b[idx] = I.b[idx] * intensidade;

						if(I.r[idx] > 255)
							I.r[idx] = 255;
						if(I.g[idx] > 255)
							I.g[idx] = 255;
						if(I.b[idx] > 255)
							I.b[idx] = 255;
				    }	
				}
		    }

		   //Se for diminuir o brilho, divide
		   if(ope == '/'){
			   for (i=0; i<x; i++) {
     				for (j=0; j<y; j++) {

     					idx = i + (j*x); //É utilizado esse idx pois o computador entende uma matriz com um vetor linha apenas. Aí tenho que somar tantos elementos pra ele entender que quero ir pra próxima coluna

					    I.r[idx] = I.r[idx] / intensidade;
					    I.g[idx] = I.g[idx] / intensidade;
					    I.b[idx] = I.b[idx] / intensidade;


				    }	
				}
		    }

	    }

    }else if (tipo == 1){ //Aplicar brilho por THREADS
    		pthread_t ID_threads[nthreads]; //Vetor que salva o ID de todas as threads 

    		aux_lc = lc;

    		//Determinação do número de linhas/colunas que cada thread vai aplicar o brilho
    		if(lc==0){ //Varredura será feita por linhas
    			linhas_por_thread = y / nthreads; //Número de linhas que cada thread vai varrer = número total de linhas dividido pelo número de threads solicitado
    			resto_linhas = y - nthreads*linhas_por_thread; //O resto existe quando o número de threads não é múlitplo do número de linhas. Aí sobra umas linhas que nenhuma thread executaria
    		} else if(lc==1){ //Varredura será feita por colunas
    			colunas_por_thread = x / nthreads; //Número de coluans que cada thread vai varrer = número total de colunas dividido pelo número de threads solicitado
    			resto_colunas = x - nthreads*colunas_por_thread; //O resto existe quando o número de threads não é múlitplo do número de colunas. Aí sobra umas colunas que nenhuma thread executaria

    		}

    		//Criando as threads
    		for(i=0; i<nthreads; i++){
    			pthread_create(&(ID_threads[i]), NULL, (void *) &aplicar_brilho_por_threads, (void *) (&I));
    		}

    		//Esperando todas as threads terminarem para realmente encerrar
    		for (i=0; i <nthreads; i++) {
    			pthread_join(ID_threads[i], NULL);
  			}
    }
}

//FUNÇÃO QUE APLICA O BRILHO POR THREADS-------------------------------------------------------------------------

//Preciso passar o endereço da onde está aquela função para que a thread possa acessá-la porque thread funciona assim
void * aplicar_brilho_por_threads(imagem *I){

	int NT, i, j, x, y, idx; //Identificador do Número da Thread (NT) (ou seja, terei thread 0, thread 1, thread 2... até thread nthread)
	int inicio = 0, final = 0;

	x = (*I).width;
  	y = (*I).height;

	pthread_mutex_lock(&trava); //Trava para que apenas uma thread, por vez, possa calcular o seu identificador (NT)
	NT = contador; 
	contador++;
	pthread_mutex_unlock(&trava);

	//Se a varredura for por linhas
	if(aux_lc == 0){
		//Determinação do nº da linha inicial e do nº da linha final que cada threadNT vai varrer
		inicio = NT*linhas_por_thread;
		
		if(NT == nthreadscopia-1){ //Se estou na última thread e tenho um resto, quero que essa thread execute todas as outras linhas até o final. Isso ocorre quadno tenho número de threads não múltiplo do número de linhas/colunas
			final = y;
		}else{
			final = (NT+1)*linhas_por_thread-1;

		}

		//Se for aumentar o brilho, multiplica
		if(opecopia == '*'){
	    	//printf("Entrei por linhas na thread!");
			   for (i=inicio; i<final; i++) {
     				for (j=0; j<x; j++) { //x é o número de colunas

     					idx = i + (j*y); //É utilizado esse idx pois o computador entende uma matriz com um vetor linha apenas. Aí tenho que somar tantos elementos pra ele entender que quero ir pra próxima coluna

					    (*I).r[idx] = (*I).r[idx] * intensidadecopia;
					    (*I).g[idx] = (*I).g[idx] * intensidadecopia;
					    (*I).b[idx] = (*I).b[idx] * intensidadecopia;

						if((*I).r[idx] > 255)
							(*I).r[idx] = 255;
						if((*I).g[idx] > 255)
							(*I).g[idx] = 255;
						if((*I).b[idx] > 255)
							(*I).b[idx] = 255;
				    }	
				}
		    }

		   //Se for diminuir o brilho, divide
		   if(opecopia == '/'){
			   for (i=inicio; i<final; i++) {
     				for (j=0; j<x; j++) {

     					idx = i + (j*y); //É utilizado esse idx pois o computador entende uma matriz com um vetor linha apenas. Aí tenho que somar tantos elementos pra ele entender que quero ir pra próxima coluna

					    (*I).r[idx] = (*I).r[idx] / intensidadecopia;
					    (*I).g[idx] = (*I).g[idx] / intensidadecopia;
					    (*I).b[idx] = (*I).b[idx] / intensidadecopia;


				    }	
				}
		    }


	}else if(aux_lc == 1){ //Se a varredura for feita por colunas

		//Determinação do nº da linha inicial e do nº da linha final que cada threadNT vai varrer
		inicio = NT*colunas_por_thread;

		if(NT == nthreadscopia-1){ //Se estou na última thread e tenho um resto, quero que essa thread execute todas as outras linhas até o final. Isso ocorre quadno tenho número de threads não múltiplo do número de linhas/colunas
			final = x;
		}else{
			final = (NT+1)*colunas_por_thread-1;

		}

		//Se for aumentar o brilho, multiplica
		if(opecopia == '*'){
	    	//printf("Entrei por colunas na thread!\n");
			   for (i=inicio; i<final; i++) {
     				for (j=0; j<y; j++) { //x é o número de colunas

     					idx = i + (j*x); //É utilizado esse idx pois o computador entende uma matriz com um vetor linha apenas. Aí tenho que somar tantos elementos pra ele entender que quero ir pra próxima coluna

					    (*I).r[idx] = (*I).r[idx] * intensidadecopia;
					    (*I).g[idx] = (*I).g[idx] * intensidadecopia;
					    (*I).b[idx] = (*I).b[idx] * intensidadecopia;

						if((*I).r[idx] > 255)
							(*I).r[idx] = 255;
						if((*I).g[idx] > 255)
							(*I).g[idx] = 255;
						if((*I).b[idx] > 255)
							(*I).b[idx] = 255;
				    }	
				}
		    }

		   //Se for diminuir o brilho, divide
		   if(opecopia == '/'){
			   for (i=inicio; i<final; i++) {
     				for (j=0; j<y; j++) {

     					idx = i + (j*x); //É utilizado esse idx pois o computador entende uma matriz com um vetor linha apenas. Aí tenho que somar tantos elementos pra ele entender que quero ir pra próxima coluna

					    (*I).r[idx] = (*I).r[idx] / intensidadecopia;
					    (*I).g[idx] = (*I).g[idx] / intensidadecopia;
					    (*I).b[idx] = (*I).b[idx] / intensidadecopia;


				    }	
				}
		    }
	}
}




































//NÃO VAI USAR ESSE NEGÓCIO
void busca (imagem I, char *nome_do_arquivo){
   float x, y;
   int i, j, max=0, soma;

   abrir_imagem(nome_do_arquivo);
   x = I.width;
   y = I.height;

   for( j = 0; j < x*y; j++){
	   soma = I.r[j] + I.g[j] + I.b[j]; 
		if (soma > max){
		   max = soma;
		}
   }
	printf("Pixel maximo: %d\n", max);
}
