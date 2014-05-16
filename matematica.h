#include "definicoes.h"
#ifndef H_MATEMATICA_H
#define H_MATEMATICA_H

malha **Inicia_Grade( const int nx, const int ny, const double hx, const double hy);
double Quadrado( double n);
double Calcula_Fxy( double x, double y);
double Calcula_Fronteira_Topo( double x);
double Calcula_Uxy( malha **Grade, const int nx, const int ny, int i, int j, const double hx, const double hy);
void Inverte_Grade( malha ***End_Grade, malha ***End_Grade_Solucao);
void Copia_Grade( malha **Grade_Origem, malha **Grade_Dest, const int nx, const int ny);
malha **Solucao_SL_Jacobbi( malha **Grade, const int nx, const int ny, const int iteracoes, const double hx, const double hy);
malha **Solucao_SL_Red_Black_Gauss_Seidel( malha **Grade, const int nx, const int ny, const int iteracoes, const double hx, const double hy);

#endif
