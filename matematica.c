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

//    printf("x %f y %f r %f\n", x, y, ( 4 * Quadrado( PI) * sin( 2 * PI * x) * sinh( 2 * PI * y)));

    return ( 4 * Quadrado( PI) * sin( 2 * PI * x) * sinh( 2 * PI * y));

}
inline double Calcula_Fronteira_Topo( double x) {

//    printf("\n\nTopo x %f r %f\n\n", x, ( sin( 2 * PI * x) * sinh( 2 * PI)));

    return ( sin( 2 * PI * x) * sinh( 2 * PI));

}
inline double Calcula_Uxy( malha **Grade, const int nx, const int ny, int i, int j, const double hx, const double hy) {

   // Verifica se é fronteiras topo
//    if ( Grade[ i][ j].y == MAX_Y)
    if ( j == nx)
        return ( Calcula_Fronteira_Topo( Grade[ i][ j].x));

    // Verifica se é outras fronteiras
//  if ( Grade[ i][ j].x == MAX_X || Grade[ i][ j].x == 0 || Grade[ i][ j].y == 0 )
    if ( i == 0 || j == 0 || i == ny)
        return ( (double) 0);

    // printf( "-%lf\n", Grade[ i][ j].fxy);
    double stencil_Central = ( 2 / Quadrado( hx) + ( 2 / Quadrado( hy) )) + Quadrado( K);
    // double fxy = Calcula_Fxy( Grade[ i][ j].x, Grade[ i][ j].y)
    double stencil_Desloc_X = (1 / ( 2 / Quadrado( hx))) * ( Grade[ i - 1][ j].valor + Grade[ i + 1][ j].valor);
    double stencil_Desloc_Y = (1 / ( 2 / Quadrado( hy))) * ( Grade[ i][ j - 1].valor + Grade[ i][ j + 1].valor);
    // printf("%lf\n", ( (double) ( stencil_Central * ( Grade[ i][ j].fxy + stencil_Desloc_X + stencil_Desloc_Y))));
    return ( (double) ( ( Grade[ i][ j].fxy + stencil_Desloc_X + stencil_Desloc_Y) / stencil_Central));

}
inline malha **Inicia_Grade( const int nx, const int ny, const double hx, const double hy) {

    //printf("%d/%d %f %d/%d %f\n", nx, MAX_X, hx, ny, MAX_Y, hy);

    int i, j;

    malha **Grade = malloc( sizeof(malha) * ( nx + 1));
    for (i = 0; i <= nx; i++) {
        Grade[i] = malloc( sizeof(malha) * ( ny + 1));
    }

    #pragma omp parallel for shared( Grade) private( i, j)
        for (i = 0; i <= nx; i++) {
            for (j = 0; j <= ny; j++) {
                Grade[ i][ j].x = i * hx;
                Grade[ i][ j].y = j * hy;
                Grade[ i][ j].fxy = Calcula_Fxy( Grade[ i][ j].x, Grade[ i][ j].y);
                Grade[ i][ j].valor = 0;
            }
        }

    // Imprime_Grade( Grade, nx, ny);

    return ( Grade);
}

inline void Inverte_Grade( malha ***End_Grade, malha ***End_Grade_Solucao) {

    malha **tmp = *End_Grade;
    *End_Grade = *End_Grade_Solucao;
    *End_Grade_Solucao = tmp;

}
inline void Copia_Grade( malha **Grade_Origem, malha **Grade_Dest, const int nx, const int ny) {

    int i, j;

    for ( i = 0; i <= nx; i++) {
        for ( j = 0; j <= ny; j++) {
            Grade_Dest[ i][ j].valor = Grade_Origem[ i][ j].valor;
        }
    }

}
inline malha **Solucao_SL_Jacobbi( malha **Grade, const int nx, const int ny, const int iteracoes, const double hx, const double hy) {

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
inline malha **Solucao_SL_Red_Black_Gauss_Seidel( malha **Grade, const int nx, const int ny, const int iteracoes, const double hx, const double hy) {

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
        // printf("\nresiduo: %lf\n", residuo( Grade, nx, ny, hx, hy) );
        // Imprime_Grade( Grade, nx, ny);
        //Imprime_Grade( Grade_Solucao, nx, ny);
    }

    return ( Grade);

}


double residuo( malha **grade, int nx, int ny, double hx, double hy){

	 double valor_Residuo = 0;
	 int i, j;

	 #pragma omp parallel
	 {

		double fator;
		double inv_hx2, inv_hy2; // o inverso de hx^2 e hy^2

		fator = ( 2 / ( hx * hx)) + ( 2/ ( hy * hy)) + ( 2 * M_PI * 2 * M_PI);
		inv_hx2 = 1/ ( hx * hx);
		inv_hy2 = 1/ ( hy * hy);

		double meu_Residuo = 0;
		double multiplicando;

		#pragma omp for private( i, j)
		    for( i = 1; i < ny; i++){
			    for( j = 1; j < nx; j++){
				    multiplicando = abs( grade[ i][ j].fxy +
					    		  inv_hx2 * ( grade[ i][ j - 1].valor + grade[ i][ j + 1].valor) +
						    	  inv_hy2 * ( grade[ i - 1][ j].valor + grade[ i + 1][ j].valor) -
							      ( fator * grade[ i][ j].valor));
			        meu_Residuo += multiplicando * multiplicando;
			    }
    		    #pragma omp critical
	    	    {
		            valor_Residuo += meu_Residuo;
		        }
		    }

	}

	return sqrt( valor_Residuo);

}
