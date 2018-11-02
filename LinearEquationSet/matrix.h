#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

#define _isZero(x)   (fabs(x) < 1e-7)
#define _equal(x, y) (fabs((x)-(y)) < 1e-7)

typedef struct SMatrix {
    double **d;
    int h;
    int w;
} *Matrix;


// Matrix Construction
Matrix createMatrix(int h, int w);
Matrix copyMatrix(const Matrix m);
Matrix readMatrix(FILE *fp);
Matrix concatColumnMatrix(const Matrix m1, const Matrix m2);
Matrix concatRowMatrix(const Matrix m1, const Matrix m2);
Matrix subMatrix(const Matrix m, int start_row, int start_col, int h, int w);
void   deleteMatrix(Matrix m);

// Matrix Operation
void   printMatrix(const Matrix m);
int    isRowEchelonMatrix(const Matrix m);
int    isReducedRowEchelonMatrix(const Matrix m);
int    isZeroRow(const Matrix m, int row);
int    rowRankMatrix(const Matrix m);

void   mulRowMatrix(Matrix m, int row_i, double multi);
void   swpRowMatrix(Matrix m, int row_i, int row_j);
void   mulRow2RowMatrix(Matrix m, int row_i, double multi, int row_j);
void   toRowEchelonMatrix(Matrix m);
void   toReducedRowEchelonMatrix(Matrix m);

Matrix mulRowMatrixNew(const Matrix m, int row_i, double multi);
Matrix swpRowMatrixNew(const Matrix m, int row_i, int row_j);
Matrix mulRow2RowMatrixNew(const Matrix m, int row_i, double multi, int row_j);
Matrix toRowEchelonMatrixNew(const Matrix m);
Matrix toReducedRowEchelonMatrixNew(const Matrix m);

#endif /* ifndef MATRIX_H */


