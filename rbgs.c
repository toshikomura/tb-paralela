#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#define MAX_X 2.0
#define MAX_Y 1.0
#define PI 3.14
#define K 2*PI
#define NUM_CORRETO_PARAMETROS 6


typedef struct malha {
    double x;
    double y;
    double valor;
} malha;


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


malha **Inicia_Grade( const int nx, const int ny, const double hx, const double hy) {

    //printf("%d/%d %f %d/%d %f\n", nx, MAX_X, hx, ny, MAX_Y, hy);

    double valor_x = 0;
    double valor_y = 0;
    int i, j;

    malha **Grade = malloc( sizeof(malha) * ( nx + 1));
    for (i = 0; i <= nx; i++) {
        Grade[i] = malloc( sizeof(malha) * ( ny + 1));
    }

    for (i = 0; i <= nx; i++) {
        for (j = 0; j <= ny; j++) {
            Grade[ i][ j].x = valor_x;
            Grade[ i][ j].y = valor_y;
            Grade[ i][ j].valor = 0;
            // atualiza valor de y
            valor_y = valor_y + hy;
        }

        // atualiza valor de x
        valor_x = valor_x + hx;
        valor_y = 0;
    }

    // Por causa do arredondamento do valor das bordas
    // causado pelos intervalos, foi necessário inserir
    // os valores máximos no braço
    /*
    for (i = 0; i <= nx; i++)
        Grade[ i][ ny].y = MAX_Y;

    for (j = 0; j <= ny; j++)
        Grade[ nx][ j].x = MAX_X;
    */

    return ( Grade);
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


double Quadrado( double n) {

    return ( n * n);

}


double Calcula_Fxy( double x, double y) {

//    printf("x %f y %f r %f\n", x, y, ( 4 * Quadrado( PI) * sin( 2 * PI * x) * sinh( 2 * PI * y)));

    return ( 4 * Quadrado( PI) * sin( 2 * PI * x) * sinh( 2 * PI * y));

}


double Calcula_Fronteira_Topo( double x) {

//    printf("\n\nTopo x %f r %f\n\n", x, ( sin( 2 * PI * x) * sinh( 2 * PI)));

    return ( sin( 2 * PI * x) * sinh( 2 * PI));

}



double Calcula_Uxy( malha **Grade, const int nx, const int ny, int i, int j, const double hx, const double hy) {

   // Verifica se é fronteiras topo
//    if ( Grade[ i][ j].y == MAX_Y)
    if ( j == nx)
        return ( Calcula_Fronteira_Topo( Grade[ i][ j].x));

    // Verifica se é outras fronteiras
//  if ( Grade[ i][ j].x == MAX_X || Grade[ i][ j].x == 0 || Grade[ i][ j].y == 0 )
    if ( i == 0 || j == 0 || i == ny)
        return ( (double) 0);

    double stencil_Central = (1 / ( 2 / Quadrado( hx) + ( 2 / Quadrado( hy) ))) + Quadrado( K);
    double fxy = Calcula_Fxy( Grade[ i][ j].x, Grade[ i][ j].y);
    double stencil_Desloc_X = (1 / ( 2 / Quadrado( hx))) * ( Grade[ i - 1][ j].valor + Grade[ i + 1][ j].valor);
    double stencil_Desloc_Y = (1 / ( 2 / Quadrado( hy))) * ( Grade[ i][ j - 1].valor + Grade[ i][ j + 1].valor);

    return ( (double) ( stencil_Central * ( fxy + stencil_Desloc_X + stencil_Desloc_Y)));

}


void Inverte_Grade( malha ***End_Grade, malha ***End_Grade_Solucao) {

    malha **tmp = *End_Grade;
    *End_Grade = *End_Grade_Solucao;
    *End_Grade_Solucao = tmp;

}


void Copia_Grade( malha **Grade_Origem, malha **Grade_Dest, const int nx, const int ny) {

    int i, j;

    for ( i = 0; i <= nx; i++) {
        for ( j = 0; j <= ny; j++) {
            Grade_Dest[ i][ j].valor = Grade_Origem[ i][ j].valor;
        }
    }

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


malha **Solucao_SL_Jacobbi( malha **Grade, const int nx, const int ny, const int iteracoes, const double hx, const double hy) {

    int n_Iteracao;
    malha **Grade_Solucao = Inicia_Grade( nx, ny, hx, hy);

    int i, j;

    // faz todas as iterações
    for ( n_Iteracao = 1; n_Iteracao <= iteracoes; n_Iteracao++) {

        #pragma omp parallel for shared( Grade, Grade_Solucao) private( i, j)
            // percorre a grade
            for ( i = 0; i <= nx; i++) {
                for ( j = 0; j <= ny; j++) {
                    Grade_Solucao[ i][ j].valor = Calcula_Uxy( Grade, nx, ny, i, j, hx, hy);
                }
            }

        //Imprime_Grade( Grade_Solucao, nx, ny);

        // Grade recebe a Grade Solução
        // e a Grade Solução fica livre para calcular uma nova solução
        Inverte_Grade( &Grade, &Grade_Solucao);
        //Copia_Grade( Grade_Solucao, Grade, nx, ny);
    }

    return ( Grade);

}


malha **Solucao_SL_Red_Black_Gauss_Seidel( malha **Grade, const int nx, const int ny, const int iteracoes, const double hx, const double hy) {

    int n_Iteracao;

    int i, j;

    // faz todas as iterações
    for ( n_Iteracao = 1; n_Iteracao <= iteracoes; n_Iteracao++) {

        #pragma omp parallel for shared( Grade) private( i, j)
            // percorre a grade Red
            for ( i = 0; i <= nx; i++) {
                // se i é par
                if ( i % 2 == 0) {
                    for ( j = 0; j <= ny; j = j + 2) {
                        Grade[ i][ j].valor = Calcula_Uxy( Grade, nx, ny, i, j, hx, hy);
                    }
                }
                else {
                    for ( j = 1; j <= ny; j = j + 2) {
                        Grade[ i][ j].valor = Calcula_Uxy( Grade, nx, ny, i, j, hx, hy);
                    }
                }
            }

        #pragma omp parallel for shared( Grade) private( i, j)
            // percorre a grade Black
            for ( i = 0; i <= nx; i++) {
                // se i é par
                if ( i % 2 == 0) {
                    for ( j = 1; j <= ny; j = j + 2) {
                        Grade[ i][ j].valor = Calcula_Uxy( Grade, nx, ny, i, j, hx, hy);
                    }
                }
                else {
                    for ( j = 0; j <= ny; j = j + 2) {
                        Grade[ i][ j].valor = Calcula_Uxy( Grade, nx, ny, i, j, hx, hy);
                    }
                }
            }

        //Imprime_Grade( Grade_Solucao, nx, ny);
    }

    return ( Grade);

}


void Executa_Metodo_Escolhido(malha **Grade, const int nx, const int ny, const int iteracoes, const double hx, const double hy, const char metodo) {

    double start_time, run_time;

    malha **Grade_Solucao;

    #pragma omp parallel
    {
            printf("num threads %d\n", omp_get_num_threads());
    }

    switch ( metodo) {
        case 'j' :
        case 'J' :
            start_time = omp_get_wtime();

            Grade_Solucao = Solucao_SL_Jacobbi( Grade, nx, ny, iteracoes, hx, hy);

            run_time = omp_get_wtime() - start_time;

            break;

        case 'g' :
        case 'G' :
            start_time = omp_get_wtime();

            Grade_Solucao = Solucao_SL_Red_Black_Gauss_Seidel( Grade, nx, ny, iteracoes, hx, hy);

            run_time = omp_get_wtime() - start_time;

            break;

        default :
            Grade_Solucao = Inicia_Grade( nx, ny, hx, hy);
    }

    printf("\n\nExecutou em %lf segundos\n\n", run_time);

    //Imprime_Grade( Grade_Solucao, nx, ny);

    Escreve_Grade_Arquivo( Grade_Solucao, nx, ny);

}


int main (int argc, char **argv)
{

    Inicia_Programa();

    Checa_Num_Parametros( argc);

    const int nx = atoi( argv[1]);
    const int ny = atoi( argv[2]);
    const double hx = ( double) MAX_X / nx;
    const double hy = ( double) MAX_Y / ny;
    const int nthreads = atoi( argv[3]);
    const int iteracoes = atoi( argv[4]);
    const char metodo = *argv[5];

    Checa_Valor_Parametros( nx, ny, nthreads, iteracoes, metodo);

    Imprime_Parametros(nx, ny, nthreads, iteracoes, metodo);

    omp_set_num_threads(nthreads);

    malha **Grade = Inicia_Grade( nx, ny, hx, hy);

    //Imprime_Grade( Grade, nx, ny);

    Executa_Metodo_Escolhido( Grade, nx, ny, iteracoes, hx, hy, metodo);

    Finaliza_Programa();

    return (0);

}
