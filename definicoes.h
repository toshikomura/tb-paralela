#ifndef H_DEFINICOES_H
#define H_DEFINICOES_H

#define MAX_X 2.0
#define MAX_Y 1.0
#define PI 3.14
#define K 2*PI
#define NUM_CORRETO_PARAMETROS 6

typedef struct malha {
    double x;
    double y;
    double fxy;
    double valor;
} malha;

#endif
