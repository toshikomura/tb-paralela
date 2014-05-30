#ifndef H_DEFINICOES_H
#define H_DEFINICOES_H

#define MAX_X 2.0
#define MAX_Y 1.0
#define PI M_PI
#define M2PI 2*PI
#define QUAD_PI PI*PI
#define M4QUAD_PI 4*QUAD_PI
#define K 2*PI
#define QUAD_K K*K
#define NUM_CORRETO_PARAMETROS 6

typedef struct malha {
    double x;
    double y;
    double fxy;
    double valor;
} malha;

#endif
