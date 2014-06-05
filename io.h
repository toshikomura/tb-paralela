#include "definicoes.h"
#ifndef H_IO_H
#define H_IO_H

void Inicia_Programa();
void Finaliza_Programa();
void Checa_Num_Parametros(const int num_Parametros);
void Checa_Valor_Parametros( const int nx, const int ny, const int nthreads, const int iteracoes, const char metodo);
void Imprime_Parametros(const int nx, const int ny, const int nthreads, const int iteracoes, const char metodo);
void Escreve_Grade_Arquivo( malha **Grade_Solucao, const int nx, const int ny, const double hx, const double hy);
void Imprime_Grade( malha **Grade, const int nx, const int ny);

#endif
