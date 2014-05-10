#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define MAX_X 2
#define MAX_Y 1
#define PI 3.1415926535897932384626433832795
#define K 2*PI
#define NUM_CORRETO_PARAMETROS 6


typedef struct malha {
    long double x;
    long double y;
    long double valor;
} malha;


void Inicia_Programa() {
    printf("\n\n <=== Inciando Programa ===>\n\n");
}


void Finaliza_Programa() {
    printf("\n\n <=== Finalizando Programa ===>\n\n");
}


void Checa_Parametros(int num_Parametros, char metodo) {

    if (num_Parametros != NUM_CORRETO_PARAMETROS)
    {
        printf("\nNúmero de parametros incorreto\n");
        printf("O programa deve ser executado da seguinte forma:\n");
        printf("\n./rgbs nx ny t c [j|g]\n\n");
        printf("rgbs: é o nome do programa.\n");
        printf("'nx' e 'ny': número de intervalos na grade de discretização nas direções X e Y, respectivamente.\n");
        printf("t: número de threads\n");
        printf("c: número de iterações a serem executadas\n");
        printf("j ou c: utilizar o método de Jacobi (j) ou Red-black Gaussian (g)\n\n");
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


void Imprime_Parametros(int nx, int ny, int nthreads, int iteracoes, char metodo) {
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


long double Quadrado( long double n) {
    return (n * n);
}


long double Calcula_Fxy( long double x, long double y) {

    return (4 * Quadrado(PI) * sin( 2 * PI * x) * sinh( 2 * PI * y));

}


long double Calcula_Fronteira_Topo( long double x) {

    return (sin( 2 * PI * x) * sinh( 2 * PI));

}


long double Valor_Fronteira_Lados_e_Embaixo() {

    return (0);

}


malha **Inicia_Grade( int nx, int ny) {

    long double hx = (long double)MAX_X / nx;
    long double hy = (long double)MAX_Y / ny;

    printf("%d/%d %Lf %d/%d %Lf\n", nx, MAX_X, hx, ny, MAX_Y, hy);

    long double valor_x = hx / 2;
    long double valor_y = hy / 2;
    int i, j;

    malha **Grade = malloc( sizeof(malha) * nx);
    for (i = 0; i < nx; i++) {
        Grade[i] = malloc( sizeof(malha) * ny);
    }

    for (i = 0; i < nx; i++) {
        for (j = 0; j < ny; j++) {
            Grade[i][j].x = valor_x;
            Grade[i][j].y = valor_y;
            Grade[i][j].valor = 0;
            // atualiza valor de y
            valor_y = valor_y + hy;
        }

        // atualiza valor de x
        valor_x = valor_x + hx;
    }

    return Grade;
}


void Imprime_Grade( malha **Grade, int nx, int ny) {

    int i, j;

    for (i = 0; i < nx; i++) {
        for (j = 0; j < ny; j++) {
            printf("| %Lf ", Grade[i][j].x);
            printf("%Lf ", Grade[i][j].y);
            printf("%Lf |", Grade[i][j].valor);
        }
        printf("\n");
    }

}


int main (int argc, char **argv)
{
    Inicia_Programa();
    Checa_Parametros( argc, *argv[5]);

    int nx = atoi( argv[1]);
    int ny = atoi( argv[2]);
    int nthreads = atoi( argv[3]);
    int iteracoes = atoi( argv[4]);
    char metodo = *argv[5];

    Imprime_Parametros(nx, ny, nthreads, iteracoes, metodo);

    malha **Grade = Inicia_Grade( nx, ny);

    Imprime_Grade( Grade, nx, ny);

    omp_set_num_threads(nthreads);

    int max = nthreads * 3;

    int i;
    #pragma omp parallel for private(i)
        for (i = 0; i < max; i++)
        {
            int id = omp_get_thread_num();
            printf("thread %d executando %d total threads %d\n", id, i, omp_get_num_threads());
        }

    Finaliza_Programa();

    return (0);

}
