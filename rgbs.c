#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#include "definicoes.h"
#include "io.h"
#include "matematica.h"

void Executa_Metodo_Escolhido(const int nx, const int ny, const int iteracoes, const double hx, const double hy, const char metodo);

int main (int argc, char **argv) {

 //   Inicia_Programa();

    Checa_Num_Parametros( argc);

    const int nx = atoi( argv[1]);
    const int ny = atoi( argv[2]);
    const double hx = ( double) MAX_X / nx;
    const double hy = ( double) MAX_Y / ny;
    const int nthreads = atoi( argv[3]);
    const int iteracoes = atoi( argv[4]);
    const char metodo = *argv[5];

    Checa_Valor_Parametros( nx, ny, nthreads, iteracoes, metodo);

//    Imprime_Parametros(nx, ny, nthreads, iteracoes, metodo);

    omp_set_num_threads(nthreads);

    //Imprime_Grade( Grade, nx, ny);

    Executa_Metodo_Escolhido( nx, ny, iteracoes, hx, hy, metodo);

//    Finaliza_Programa();

    return (0);
}

void Executa_Metodo_Escolhido(const int nx, const int ny, const int iteracoes, const double hx, const double hy, const char metodo) {

    double start_time, run_time=0;

    malha **Grade;
    malha **Grade_Solucao;

//    #pragma omp parallel
//    {
//            printf("Eu sou a thread nº%d\n", omp_get_thread_num());
//    }

    switch ( metodo) {
        case 'j' :
        case 'J' :
            start_time = omp_get_wtime();

            Grade = Inicia_Grade( nx, ny, hx, hy);
            Grade_Solucao = Solucao_SL_Jacobbi( Grade, nx, ny, iteracoes, hx, hy);

            run_time = omp_get_wtime() - start_time;

            break;

        case 'g' :
        case 'G' :
            start_time = omp_get_wtime();

            Grade = Inicia_Grade( nx, ny, hx, hy);
            Grade_Solucao = Solucao_SL_Red_Black_Gauss_Seidel( Grade, nx, ny, iteracoes, hx, hy);

            run_time = omp_get_wtime() - start_time;

            break;

        default :
            Grade_Solucao = Inicia_Grade( nx, ny, hx, hy);
    }

    printf("%lf\n", run_time );

	printf("Final residuo: %lf\n", residuo( Grade_Solucao, nx, ny, hx, hy) ); // seg fault?

    //Imprime_Grade( Grade_Solucao, nx, ny);

    Escreve_Grade_Arquivo( Grade_Solucao, nx, ny);
}
