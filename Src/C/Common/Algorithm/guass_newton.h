#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

typedef double(*e_func)(double ** data, double ** param);


typedef struct Matrix_{
	uint32_t m;
	uint32_t n;
	int8_t Transposed;
	double ** data;
} Matrix;

int32_t InitMatrix(Matrix * M, uint32_t m, uint32_t n);

int32_t ReleaseMatrix(Matrix * M);


/*
double * MatrixValue(Matrix * M, uint32_t i, uint32_t j)
{
return (M->Transposed) ? (&(M->data[(M->m * j + i)])) : (&(M->data[(M->m * i + j)]));
}
*/
//#define MatrixValue(M, i, j) (((M)->Transposed) ? ((M)->data + (M)->n * (j) + (i)) : ((M)->data + (M)->n * (i) + (j)))
#define MatrixValue(M, i, j) (((M)->Transposed) ? ((M)->data[(j)] + (i)) : ((M)->data[(i)] + (j)))

/*
uint32_t Row(Matrix * M)
{
return (M->Transposed) ? (M->n) : (M->m);
}
*/
#define Row(M) (((M)->Transposed) ? ((M)->n) : ((M)->m))


/*
uint32_t Col(Matrix * M)
{
return (M->Transposed) ? (M->m) : (M->n);
}
*/
#define Col(M) (((M)->Transposed) ? ((M)->m) : ((M)->n))

#define IS_ZERO(f) (!IS_NOT_ZERO(f)) 
#define IS_NOT_ZERO(f) ((f) < -1.0e-5 || (f) > 1.0e-5)

int32_t ShallowCopyMatrix(Matrix * Dest, Matrix * Src);

int32_t MatrixSetAll(Matrix * M, double value);

int32_t MatrixTrans(Matrix * M, Matrix * TM);

int32_t MatrixAdd(Matrix * M, Matrix * N);

int32_t MatrixSub(Matrix * M, Matrix * N);

int32_t MatrixMult(Matrix * M, Matrix * N, Matrix * R);

int32_t MatrixMultScale(Matrix * M, double scale);

int32_t MatrixDivScale(Matrix * M, double scale);

int32_t MatrixFunc(e_func * F, Matrix * M, Matrix * P, Matrix * D);

double MatrixDet(Matrix * M);

int32_t MatrixCoFactor(Matrix * M, Matrix * R);

int32_t MatrixInverse(Matrix * M, Matrix * R);

double VectorNorm2(Matrix * v);

#define MATRIX_NO_COPY	0
#define MATRIX_COPY		1

int32_t MatrixInverseGE(Matrix * M, Matrix * R, uint8_t opt);

void guass_newton(e_func E, e_func * J, e_func * H, uint32_t p_dim, double * data, uint32_t data_dim, uint32_t data_N, double * p);
