#include "matrix.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

Matrix createMatrix(int h, int w) {
    Matrix m = (Matrix) malloc (sizeof(struct SMatrix));
    m->d = (double **) malloc (sizeof(double *) * h);
    int i;
    for (i = 0; i < h; i++)
        m->d[i] = (double *) calloc (w, sizeof(double));
    m->h = h;
    m->w = w;
    return m;
}

Matrix copyMatrix(const Matrix m) {
    Matrix res = createMatrix(m->h, m->w);
    int i;
    for (i = 0; i < res->h; i++)
        memcpy(res->d[i], m->d[i], sizeof(double) * m->w);
    return res;
}

Matrix readMatrix(FILE *fp) {
    if (!fp) return NULL;
    int h, w, i, j;
    double val;
    if (fscanf(fp, "%d%d", &h, &w) < 2) return NULL;
    Matrix m = createMatrix(h, w);
    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++) {
            if (!fscanf(fp, "%lf", &val)) {
                deleteMatrix(m);
                return NULL;
            }
            m->d[i][j] = val;
        }
    return m;
}

Matrix concatColumnMatrix(const Matrix m1, const Matrix m2) {
    if (!m1 || !m2 || m1->h != m2->h) return NULL;
    Matrix m = createMatrix(m1->h, m1->w + m2->w);
    int i;
    for (i = 0; i < m->h; i++) {
        memcpy(m->d[i], m1->d[i], sizeof(double) * m1->w);
        memcpy(m->d[i] + m1->w, m2->d[i], sizeof(double) * m2->w);
    }
    return m;
}

Matrix concatRowMatrix(const Matrix m1, const Matrix m2) {
    if (!m1 || !m2 || m1->w != m2->w) return NULL;
    Matrix m = createMatrix(m1->h + m2->h, m1->w);
    int i;
    for (i = 0; i < m1->h; i++)
        memcpy(m->d[i], m1->d[i], sizeof(double) * m->w);
    for (i = 0; i < m2->h; i++)
        memcpy(m->d[m1->h + i], m2->d[i], sizeof(double) * m->w);
    return m;
}

Matrix subMatrix(const Matrix m, int start_row, int start_col, int h, int w) {
    if (!m || h < 0 || w < 0 || start_row < 0 || start_col < 0) return NULL;
    if (start_row + h > m->h || start_col + w > m->w) return NULL;
    Matrix res = createMatrix(h, w);
    int i;
    for (i = 0; i < h; i++)
        memcpy(res->d[i], m->d[start_row+i]+start_col, sizeof(double) * w);
    return res;
}

Matrix transposeMatrix(const Matrix m) {
    Matrix res = createMatrix(m->w, m->h);
    int i, j;
    for (i = 0; i < m->h; i++)
        for (j = 0; j < m->w; j++)
            res->d[j][i] = m->d[i][j];
    return res;
}

void deleteMatrix(Matrix m) {
    if (!m) return ;
    int i;
    for (i = 0; i < m->h; i++)
        free(m->d[i]);
    free(m->d);
    free(m);
}





void printMatrix(const Matrix m) {
    if (!m) {
        printf("*Empty matrix*\n");
        return ;
    }
    int i, j;
    for (i = 0; i < m->h; i++) {
        for (j = 0; j < m->w; j++)
            printf("%10.4g", _toZero(m->d[i][j]));
        putchar('\n');
    }
}

int isRowEchelonMatrix(const Matrix m) {
    int ih=0, iw=0, j;
    while (ih < m->h && iw < m->w) {
        if (_isZero(m->d[ih][iw])) {
            iw++;
            continue;
        }
        for (j = ih+1; j < m->h && _isZero(m->d[j][iw]); j++) ;
        if (j < m->h) return 0;
        ih++;
        iw++;
    }
    return 1;
}

int isReducedRowEchelonMatrix(const Matrix m) {
    int ih=0, iw=0, j;
    while (ih < m->h && iw < m->w) {
        if (_isZero(m->d[ih][iw])) {
            iw++;
            continue;
        }
        if (!_equal(m->d[ih][iw], 1)) return 0;
        for (j = 0; j < m->h; j++) {
            if (j == ih) continue;
            if (!_isZero(m->d[j][iw])) return 0;
        }
        ih++;
        iw++;
    }
    return 1;
}

int isZeroRow(const Matrix m, int row) {
    if (!m || row < 0 || row > m->h-1) return -1;
    int i;
    for (i = 0; i < m->w; i++)
        if (!_isZero(m->d[row][i]))
            return 0;
    return 1;
}

int rowRankMatrix(const Matrix m) {
    if (!m) return 0;
    int is_echelon = isRowEchelonMatrix(m);
    Matrix mat = is_echelon ? m : toRowEchelonMatrixNew(m);
    int rank = m->h;
    for (; rank > 0; rank--)
        if (!isZeroRow(mat, rank-1)) break;
    if (!is_echelon) deleteMatrix(mat);
    return rank;
}





void mulRowMatrix(Matrix m, int row_i, double multi) {
    if (!m || row_i < 0 || row_i > m->h-1) return ;
    int i;
    if (_isZero(multi))
        memset(m->d[row_i], 0, m->w * sizeof(double));
    else for (i = 0; i < m->w; i++)
        m->d[row_i][i] *= multi;
}

void swpRowMatrix(Matrix m, int row_i, int row_j) {
    if (!m || row_j < 0 || row_j > m->h-1 || row_i < 0 || row_i > m->h-1) return ;
    if (row_i == row_j) return ;
    double *tmp = m->d[row_i];
    m->d[row_i] = m->d[row_j];
    m->d[row_j] = tmp;
}

void mulRow2RowMatrix(Matrix m, int row_i, double multi, int row_j) {
    if (!m || row_j < 0 || row_j > m->h-1 || row_i < 0 || row_i > m->h-1) return ;
    if (row_i == row_j) return mulRowMatrix(m, row_i, multi+1);
    int i;
    for (i = 0; i < m->w; i++)
        m->d[row_j][i] += multi * m->d[row_i][i];
}

void toRowEchelonMatrix(Matrix m) {
    if (!m) return ;
    int ih = 0, iw = 0, j; // Init main at (0, 0)
    while (ih < m->h && iw < m->w) {
        if (_isZero(m->d[ih][iw])) {
            j = ih+1;
            while (j < m->h && _isZero(m->d[j][iw])) j++;
            if (j == m->h) {
                iw++;
                continue;
            }
            swpRowMatrix(m, ih, j);
        }
        if (ih == m->h-1 && _isZero(m->d[ih][iw])) break;

        mulRowMatrix(m, ih, 1/(m->d[ih][iw]));

        for (j = ih+1; j < m->h; j++)
            mulRow2RowMatrix(m, ih, -m->d[j][iw], j);
        ih++;
        iw++;
    }
}

void toReducedRowEchelonMatrix(Matrix m) {
    if (!m) return ;
    toRowEchelonMatrix(m);
    int ih=rowRankMatrix(m)-1, iw=0, j;
    for (; ih > 0; ih--) {
        for (iw = 0; iw < m->w && _isZero(m->d[ih][iw]); iw++) ;
        for (j = 0; j < ih; j++)
            mulRow2RowMatrix(m, ih, -m->d[j][iw], j);
    }
}





Matrix mulRowMatrixNew(const Matrix m, int row_i, double multi) {
    if (!m || row_i < 0 || row_i > m->h-1) return NULL;
    Matrix res = copyMatrix(m);
    mulRowMatrix(res, row_i, multi);
    return res;
}

Matrix swpRowMatrixNew(const Matrix m, int row_i, int row_j) {
    if (!m || row_j < 0 || row_j > m->h-1 || row_i < 0 || row_i > m->h-1) return NULL;
    Matrix res = copyMatrix(m);
    swpRowMatrix(res, row_i, row_j);
    return res;
}

Matrix mulRow2RowMatrixNew(const Matrix m, int row_i, double multi, int row_j) {
    if (!m || row_j < 0 || row_j > m->h-1 || row_i < 0 || row_i > m->h-1) return NULL;
    Matrix res = copyMatrix(m);
    mulRow2RowMatrix(res, row_i, multi, row_j);
    return res;
}

Matrix toRowEchelonMatrixNew(const Matrix m) {
    if (!m) return NULL;
    Matrix res = copyMatrix(m);
    toRowEchelonMatrix(res);
    return res;
}

Matrix toReducedRowEchelonMatrixNew(const Matrix m) {
    if (!m) return NULL;
    Matrix res = copyMatrix(m);
    toReducedRowEchelonMatrix(res);
    return res;
}
