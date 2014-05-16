#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#include "definicoes.h"
#include "io.h"

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


