#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#include "definicoes.h"
#include "matematica.h"

void Inicia_Programa() {
    printf("\n\n <=== Inciando Programa ===>\n\n");
}
void Finaliza_Programa() {
    printf("\n\n <=== Finalizando Programa ===>\n\n");
}
void Checa_Num_Parametros(const int num_Parametros) {

    if (num_Parametros != NUM_CORRETO_PARAMETROS)
    {
        printf("\nNúmero de parametros incorreto\n");
        printf("O programa deve ser executado da seguinte forma:\n");
        printf("\n./rgbs nx ny t c [j|g]\n\n");
        printf("rgbs: é o nome do programa.\n");
        printf("'nx' e 'ny': número de intervalos na grade de discretização nas direções X e Y, respectivamente.\n");
        printf("t: número de threads\n");
        printf("c: número de iterações a serem executadas\n");
        printf("j ou g: utilizar o método de Jacobi (j) ou Red-black Gaussian (g)\n\n");
        Finaliza_Programa();
        exit(-1);
    }

}
void Checa_Valor_Parametros( const int nx, const int ny, const int nthreads, const int iteracoes, const char metodo) {

    if ( nx < 1) {
        printf("O número de nx não pode ser %d, valor deve ser maior que '0'\n\n", nx);
        Finaliza_Programa();
        exit(-1);
    }

    if ( ny < 1) {
        printf("O número de ny não pode ser %d, valor deve ser maior que '0'\n\n", nx);
        Finaliza_Programa();
        exit(-1);
    }

    if ( nthreads < 1) {
        printf("O número de threads não pode ser %d, valor deve ser maior que '0'\n\n", nthreads);
        Finaliza_Programa();
        exit(-1);
    }

    if ( iteracoes < 1) {
        printf("O número de interações não pode ser %d, valor deve ser maior que '0'\n\n", iteracoes);
        Finaliza_Programa();
        exit(-1);
    }

    if ( ( metodo != 'j' && metodo != 'J' && metodo != 'g' && metodo != 'G')) {
        printf("\nNenhum método para (%c)\n", metodo);
        printf("O método a ser utilizado só pode ser de Jacobi (j) ou Red-black Gaussian (g)\n\n");
        Finaliza_Programa();
        exit(-1);
    }
}
void Imprime_Parametros(const int nx, const int ny, const int nthreads, const int iteracoes, const char metodo) {
    printf("\nImprimindo parametros\n");
    printf("nx = %d ny %d\n", nx, ny);
    printf("threads = %d iterações = %d\n", nthreads, iteracoes);
    printf("método = ");
    switch(metodo){
        case 'j' :
        case 'J' :
            printf("Jacobi\n");
            break;
        case 'g' :
        case 'G' :
            printf("Red-Black Gaussian\n");
            break;
        default :
            printf("método não encontrado\n");
    }
    printf("\n\n");
}
void Escreve_Grade_Arquivo( malha **Grade_Solucao, const int nx, const int ny) {

    int i, j;
    FILE *f_Solucao;

    f_Solucao = fopen("solution.txt", "w+");

    fprintf( f_Solucao, "# Plotando solução da equação diferencial parcial\n");
    fprintf( f_Solucao, "# f(x, y) = 4 * PI^2 * sin( 2 * PI * x) * sinh( 2 * PI * y)\n");
    fprintf( f_Solucao, "# x      y        valor\n\n");

    for (i = 0; i <= nx; i++) {
        for (j = 0; j <= ny; j++) {
            fprintf( f_Solucao, "%f %f %f\n", Grade_Solucao[ i][ j].x, Grade_Solucao[ i][ j].y, Grade_Solucao[ i][ j].valor);
        }
    }

    fclose( f_Solucao);
}
void Imprime_Grade( malha **Grade, const int nx, const int ny) {

    int i, j;

    for (i = 0; i <= nx; i++) {
//        printf("x = %f -|", Grade[ i][ 0].x);
        for (j = 0; j <= ny; j++) {
//            printf("| %Lf ", Grade[ i][ j].x);
//            printf("%Lf ", Grade[ i][ j].y);
            printf("| %e |", Grade[ i][ j].valor);
        }
        printf("\n");
    }

}
