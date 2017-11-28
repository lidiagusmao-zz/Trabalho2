#include <stdio.h>
#include <stdlib.h>

int main(){
	int i, j, k;
	FILE *script;

	printf("\033[0;34m     OPERANDO NO MODO SEQUENCIAL\033[0m\n\n");

/*---------------------------------LOOP PARA AS OPERAÇÕES SEQUENCIAIS LINHAS---------------------------------*/
	
	printf("\033[0;33m\tVARREDURA POR LINHAS\033[0m\n\n");

	if((script = popen("./main >> tempo_serial_linhas.txt | mv tempo_serial_linhas.txt Tempos_Serial", "w")) == NULL){
		perror("popen");
		exit(1);
	}
	fprintf(script, "serial0.jpg = demo.jpg * 3.0 0 0 0");
	pclose(script);

	if((script = popen(" cat serial0.jpg | mv serial0.jpg Imagens_Serial", "w")) == NULL){
		perror("popen");
		exit(1);
	}
	pclose(script);

/*---------------------------------LOOP PARA AS OPERAÇÕES SEQUENCIAIS COLUNA---------------------------------*/

	printf("\033[0;33m\tVARREDURA POR COLUNAS\033[0m\n\n\n");

	if((script = popen("./main >> tempo_serial_colunas.txt | mv tempo_serial_colunas.txt Tempos_Serial", "w")) == NULL){
		perror("popen");
		exit(1);
	}
	fprintf(script, "serial1.jpg = demo.jpg * 3.0 0 1 0");
	pclose(script);

	if((script = popen(" cat serial1.jpg | mv serial1.jpg Imagens_Serial", "w")) == NULL){
		perror("popen");
		exit(1);
	}
	pclose(script);

/*---------------------------------LOOP PARA AS OPERAÇÕES COM THREADS---------------------------------*/

	printf("\033[0;34m      OPERANDO NO MODO THREADS\033[0m\n\n");

/*---------------------------------LOOP PARA AS OPERAÇÕES THREADS LINHAS---------------------------------*/

	printf("\033[0;33m\tVARREDURA POR LINHAS\033[0m\n\n");
	
	for(k = 1; k <=10; k++){
		printf("\033[0;36m\tUtilizando %d Threads\033[0m\n", k);
			
		if((script = popen("./main >> tempo_threads_linhas.txt", "w")) == NULL){
			perror("popen");
			exit(1);
		}
		fprintf(script, "lthreads%d.jpg = demo.jpg * 3.0 1 0 %d ", k, k);
		pclose(script);
	}

	if((script = popen("grep user | sort -n tempo_threads_linhas.txt >> tempo_threads_linhas_finais.txt | mv tempo_threads_linhas_finais.txt Tempos_Thread", "w")) == NULL){
		perror("popen");
		exit(1);
	}
	pclose(script);

	if((script = popen("rm tempo_threads_linhas.txt", "w")) == NULL){
		perror("popen");
		exit(1);
	}
	pclose(script);

	if((script = popen("for i in $(seq 1 10); do cat lthreads$i.jpg | mv lthreads$i.jpg Imagens_Thread; done", "w")) == NULL){
		perror("popen");
		exit(1);
	}
	pclose(script);


/*---------------------------------LOOP PARA AS OPERAÇÕES THREADS COLUNA---------------------------------*/

	printf("\n\033[0;33m\tVARREDURA POR COLUNAS\033[0m\n\n");

	for(k = 1; k <=10; k++){
		printf("\033[0;36m\tUtilizando %d Threads\033[0m\n", k);
			
		if((script = popen("./main >> tempo_threads_colunas.txt", "w")) == NULL){
			perror("popen");
			exit(1);
		}
		fprintf(script, "cthreads%d.jpg = demo.jpg * 3.0 1 1 %d ", k, k);
		pclose(script);
	}

			
	if((script = popen("grep user | sort -n tempo_threads_colunas.txt >> tempo_threads_colunas_finais.txt | mv tempo_threads_colunas_finais.txt Tempos_Thread", "w")) == NULL){
		perror("popen");
		exit(1);
	}
	pclose(script);

	if((script = popen("rm tempo_threads_colunas.txt", "w")) == NULL){
		perror("popen");
		exit(1);
	}
	pclose(script);

	if((script = popen("for i in $(seq 1 10); do cat cthreads$i.jpg | mv cthreads$i.jpg Imagens_Thread; done", "w")) == NULL){
		perror("popen");
		exit(1);
	}
	pclose(script);
	  

	return 0;
}