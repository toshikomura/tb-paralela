#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#include "definicoes.h"
#include "io.h"
#include "matematica.h"

inline double Quadrado( double n) {

    return ( n * n);

}
inline double Calcula_Fxy( double x, double y) {

    return ( M4QUAD_PI * sin( M2PI * x) * sinh( M2PI * y));

}
double Calcula_Fronteira_Topo( double x) {

    return ( sin( M2PI * x) * sinh( M2PI));

}
inline double Calcula_Uxy( malha **Grade, const int nx, const int ny, int i, int j, const double stencil_Central, const double quad_hx, const double quad_hy) {

    return ( (Grade[ i][ j].fxy + (( Grade[ i - 1][ j].valor + Grade[ i + 1][ j].valor) / quad_hx) +
            (( Grade[ i][ j -1].valor + Grade[ i][ j + 1].valor) / quad_hy)) /
            ( stencil_Central));

}
malha **Inicia_Grade( const int nx, const int ny, const double hx, const double hy) {

    int i, j;

    malha **Grade = malloc( sizeof(malha) * ( ny + 1));
    for (i = 0; i <= ny; i++) {
        Grade[i] = malloc( sizeof(malha) * ( nx + 1));
    }

    #pragma omp parallel for shared( Grade) private( i, j)
        for (i = 0; i <= ny; i++) {
            for (j = 0; j <= nx; j++) {

                Grade[ i][ j].fxy = Calcula_Fxy( j * hx, i * hy);

                // Verifica se é fronteiras topo
                if ( i == ny)
                    Grade[ i][ j].valor = Calcula_Fronteira_Topo( j * hx);

                // Verifica se é outras fronteiras
                if ( i == 0 || j == 0 || j == nx)
                    Grade[ i][ j].valor = 0;
            }
        }

    return ( Grade);

}
void Inverte_Grade( malha ***End_Grade, malha ***End_Grade_Solucao) {

    malha **tmp = *End_Grade;
    *End_Grade = *End_Grade_Solucao;
    *End_Grade_Solucao = tmp;

}
malha **Solucao_SL_Jacobbi( malha **Grade, const int nx, const int ny, const int iteracoes, const double hx, const double hy) {

    int n_Iteracao;
    malha **Grade_Solucao = Inicia_Grade( nx, ny, hx, hy);

    int i, j;
    double quad_hx = Quadrado( hx);
    double quad_hy = Quadrado( hy);
	double stencil_Central = (( 2 / quad_hx) + ( 2/ quad_hy) + QUAD_K);

    // faz todas as iterações
    for ( n_Iteracao = 1; n_Iteracao <= iteracoes; n_Iteracao++) {

        #pragma omp parallel for shared( Grade, Grade_Solucao) private( i, j)
            // percorre a grade
            for ( i = 1; i < ny; i++) {
                for ( j = 1; j < nx; j++) {
                    Grade_Solucao[ i][ j].valor = Calcula_Uxy( Grade, nx, ny, i, j, stencil_Central, quad_hx, quad_hy);
                }
            }

        // Grade recebe a Grade Solução
        // e a Grade Solução fica livre para calcular uma nova solução
        Inverte_Grade( &Grade, &Grade_Solucao);
    }

    return ( Grade);

}
malha **Solucao_SL_Red_Black_Gauss_Seidel( malha **Grade, const int nx, const int ny, const int iteracoes, const double hx, const double hy) {

    int n_Iteracao;

    int i, j;
    double quad_hx = Quadrado( hx);
    double quad_hy = Quadrado( hy);
	double stencil_Central = (( 2 / quad_hx) + ( 2/ quad_hy) + QUAD_K);

    // faz todas as iterações
    for ( n_Iteracao = 1; n_Iteracao <= iteracoes; n_Iteracao++) {

        #pragma omp parallel for shared( Grade) private( i, j)
            // percorre a grade Red
            for ( i = 1; i < ny; i++) {
                for ( j = i % 2; j <= nx; j = j + 2) {
                    Grade[ i][ j].valor = Calcula_Uxy( Grade, nx, ny, i, j, stencil_Central, quad_hx, quad_hy);
                }
            }

        #pragma omp parallel for shared( Grade) private( i, j)
            // percorre a grade Black
            for ( i = 1; i < ny; i++) {
                for ( j = ( i + 1) % 2; j <= nx; j = j + 2) {
                    Grade[ i][ j].valor = Calcula_Uxy( Grade, nx, ny, i, j, stencil_Central, quad_hx, quad_hy);
                }
            }

    }

    return ( Grade);

}


double residuo( malha **Grade, int nx, int ny, double hx, double hy){

    int i, j;
    double quad_hx = Quadrado( hx);
    double quad_hy = Quadrado( hy);
	double stencil_Central = (( 2 / quad_hx) + ( 2/ quad_hy) + QUAD_K);
	double meu_Residuo = 0;

	#pragma omp parallel for private( i, j) reduction ( +: meu_Residuo)
        for( i = 1; i < ny; i++){
	        for( j = 1; j < nx; j++){
                meu_Residuo += Quadrado( Grade[ i][ j].fxy +
                                    (( Grade[ i - 1][ j].valor + Grade[ i + 1][ j].valor) / quad_hx) +
                                    (( Grade[ i][ j -1].valor + Grade[ i][ j + 1].valor) / quad_hy) -
                                    ( stencil_Central * Grade[ i][ j].valor));
            }
        }

	return sqrt( meu_Residuo);

}
