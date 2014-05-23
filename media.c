#include <stdio.h>		// inclui diretivas de entrada-saída
#include <math.h>		// inclui diretivas das funções matemáticas
#include <stdlib.h>		// Usa recursos do sistema
#include <time.h>		// Permite o uso do tempo do sistema
#include <string.h>		// Bibliotecas de comandos com strings
#include <limits.h>		// Limites da linguagem c

int main(int argc, char **argv){
	FILE *arq = fopen(argv[1], "r");
	if (!arq) {
		fprintf(stderr, "Não deu pra abrir o arquivo!!!\n");
		exit(1);
	}

	double acumulador=0;
	double tmp;
	int i=0;

	while (fscanf(arq, "%lf\n", &tmp) == 1) {
		acumulador += tmp;
		i+=1;
	}

	acumulador = acumulador/i;

	printf ("Media: %lf\n",i,acumulador);
	return 0;
}






