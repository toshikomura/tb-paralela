#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#include "definicoes.h"
#include "io.h"

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
inline double Calcula_Uxy( malha *Grade, const int nx, const int ny, int pos, const double hx, const double hy) {

   // Verifica se é fronteiras topo
//    if ( Grade[ i][ j].y == MAX_Y)
    if (( pos + 1) / nx == 0)
        return ( Calcula_Fronteira_Topo( Grade[ pos].x));

    // Verifica se é outras fronteiras
//  if ( Grade[ i][ j].x == MAX_X || Grade[ i][ j].x == 0 || Grade[ i][ j].y == 0 )
    if ( ( pos + 1 % ny) == 0 || ( pos + 1 % nx) == 0 || ( pos + 1 / ny) == 0)
        return ( (double) 0);

    double stencil_Central = (1 / ( 2 / Quadrado( hx) + ( 2 / Quadrado( hy) ))) + Quadrado( K);
    // double fxy = Calcula_Fxy( Grade[ i][ j].x, Grade[ i][ j].y)
    double stencil_Desloc_X = (1 / ( 2 / Quadrado( hx))) * ( Grade[ pos - ny].valor + Grade[ pos + ny].valor);
    double stencil_Desloc_Y = (1 / ( 2 / Quadrado( hy))) * ( Grade[ pos - 1].valor + Grade[ pos + 1].valor);

    return ( (double) ( stencil_Central * ( Grade[ pos].fxy + stencil_Desloc_X + stencil_Desloc_Y)));

}
inline malha *Inicia_Grade( const int nx, const int ny, const double hx, const double hy) {

    //printf("%d/%d %f %d/%d %f\n", nx, MAX_X, hx, ny, MAX_Y, hy);

    int i, j;

    malha *Grade = malloc( sizeof(malha) * ( nx + 1) * ( ny + 1));

    #pragma omp parallel for shared( Grade) private( i, j)
        for (i = 0; i <= nx; i++) {
            for (j = 0; j <= ny; j++) {
                int pos = ( i * ny) + j;
                Grade[ pos].x = i * hx;
                Grade[ pos].y = j * hy;
                Grade[ pos].fxy = Calcula_Fxy( Grade[ pos].x, Grade[ pos].y);
                Grade[ pos].valor = 0;
            }
        }

    return ( Grade);
}

inline void Inverte_Grade( malha **End_Grade, malha **End_Grade_Solucao) {

    malha *tmp = *End_Grade;
    *End_Grade = *End_Grade_Solucao;
    *End_Grade_Solucao = tmp;

}
inline void Copia_Grade( malha *Grade_Origem, malha *Grade_Dest, const int nx, const int ny) {

    int i, j;

    for ( i = 0; i <= nx; i++) {
        for ( j = 0; j <= ny; j++) {
            int pos = ( i * ny) + j;
            Grade_Dest[ pos].valor = Grade_Origem[ pos].valor;
        }
    }

}
inline malha *Solucao_SL_Jacobbi( malha *Grade, const int nx, const int ny, const int iteracoes, const double hx, const double hy) {

    int n_Iteracao;
    malha *Grade_Solucao = Inicia_Grade( nx, ny, hx, hy);

    int i, j;

    // faz todas as iterações
    for ( n_Iteracao = 1; n_Iteracao <= iteracoes; n_Iteracao++) {

        #pragma omp parallel for shared( Grade) private( i, j)
            // percorre a grade
            for ( i = 0; i <= nx; i++) {
                for ( j = 0; j <= ny; j++) {
                    int pos = ( i * ny) + j;
                    Grade_Solucao[ pos].valor = Calcula_Uxy( Grade, nx, ny, pos, hx, hy);
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
inline malha *Solucao_SL_Red_Black_Gauss_Seidel( malha *Grade, const int nx, const int ny, const int iteracoes, const double hx, const double hy) {

    int n_Iteracao;
    int i, j;

    // faz todas as iterações
    for ( n_Iteracao = 1; n_Iteracao <= iteracoes; n_Iteracao++) {

        #pragma omp parallel for shared( Grade) private( i, j)
            // percorre a grade Red
            for ( i = 0; i <= nx; i++) {
                for ( j = 0; j <= ny; j = j + 2) {
                    int pos = ( i * ny) + j;
                    Grade[ pos].valor = Calcula_Uxy( Grade, nx, ny, pos, hx, hy);
                }
            }

        #pragma omp parallel for shared( Grade) private( i, j)
            // percorre a grade Black
            for ( i = 0; i <= nx; i++) {
                for ( j = 1; j <= ny; j = j + 2) {
                    int pos = ( i * ny) + j;
                    Grade[ pos].valor = Calcula_Uxy( Grade, nx, ny, pos, hx, hy);
                }
            }

        //Imprime_Grade( Grade_Solucao, nx, ny);
    }

    return ( Grade);

}


inline double residuo( malha *grade, int nx, int ny, double hx, double hy){
	 double valor_residuo=0;
	 int i,j;
	 #pragma omp parallel
	 {

		double fator;
		double inv_hx2, inv_hy2; // o inverso de hx^2 e hy^2
		fator=(2/(hx*hx))+(2/(hy*hy))+(2*M_PI*2*M_PI);
		inv_hx2=1/(hx*hx);
		inv_hy2=1/(hy*hy);
		double my_residue=0;
		double multiplicando;
		#pragma omp for private(j)
		for(i=1;i<ny;i++){
			for(j=1;j<nx;j++){
                int pos = ( i * ny) + j;
				printf ("%d\n", pos);
				multiplicando=abs(grade[pos].fxy+
							  inv_hx2*(grade[ pos - 1].valor+grade[ pos + 1].valor)+
							  inv_hy2*(grade[ pos - ny].valor+grade[ pos + ny].valor)-
							  (fator*grade[pos].valor));
			    my_residue+=multiplicando*multiplicando;
			}
		}
		#pragma omp critical
		{
				valor_residuo+=my_residue;
		}
	}
	return sqrt(valor_residuo);
}
