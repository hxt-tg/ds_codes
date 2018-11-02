#include "eq_set.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int _isValidEqSet(EqSet eq) {
    return (eq && eq->coef && eq->param && eq->coef->h == eq->param->h && eq->coef->w == 1) ;
}

// EqSet Construction
EqSet createEqSet(const Matrix param, const Matrix coef) {
    if (!param || (coef && (coef->h != param->h || coef->w != 1))) return NULL;
    EqSet eq = (EqSet) malloc (sizeof(struct SEqSet));
    eq->coef = coef ? copyMatrix(coef) : createMatrix(param->h, 1);
    eq->param = copyMatrix(param);
    eq->vars = NULL;
    return eq;
}

void setEqVars(EqSet eq, NameTag *vars) {
    if (!_isValidEqSet(eq)) return ;
    int i;
    for (i = 0; vars[i][0]; i++) ;
    if (i != eq->param->w) return ;
    if (eq->vars) free(eq->vars);
    eq->vars = (NameTag *) calloc (eq->param->w, sizeof(NameTag));
    for (i = 0; vars[i][0]; i++)
        strncpy(eq->vars[i], vars[i], NAMETAG_LEN);
}

EqSet copyEqSet(const EqSet eq) {
    if (!_isValidEqSet(eq)) return NULL;
    return createEqSet(eq->param, eq->coef);
}

EqSet readEqSet(FILE *fp) {
    Matrix m = readMatrix(fp);
    if (!m) return NULL;
    if (m->w < 2) {
        deleteMatrix(m);
        return NULL;
    }
    Matrix param = subMatrix(m, 0, 0, m->h, m->w-1);
    Matrix coef = subMatrix(m, 0, m->w-1, m->h, 1);
    EqSet eq = createEqSet(param, coef);
    deleteMatrix(param);
    deleteMatrix(coef);
    deleteMatrix(m);
    return eq;
}

EqSet concatEqSet(const EqSet eq1, const EqSet eq2) {
    if (!_isValidEqSet(eq1) || !_isValidEqSet(eq2) || eq1->param->w != eq2->param->w) return NULL;
    Matrix new_param = concatRowMatrix(eq1->param, eq2->param);
    Matrix new_coef  = concatRowMatrix(eq1->coef,  eq2->coef );
    EqSet eq = createEqSet(new_param, new_coef);
    deleteMatrix(new_param);
    deleteMatrix(new_coef);
    return eq;
}

void deleteEqSet(EqSet eq) {
    if (eq->vars) free(eq->vars);
    deleteMatrix(eq->param);
    deleteMatrix(eq->coef);
    free(eq);
}





// EqSet Operation
void printEqSet(const EqSet eq) {
    if (!_isValidEqSet(eq)) return ;
    int chr_p = (10 * eq->param->w - 5)/2 + 5, chr_dash = 8 + 10 * (eq->param->w+1), i, j;
    for (i = 0; i < chr_dash; i++) putchar('-'); putchar('\n');
    printf("  Eq  |%*s%-*s| %6s   \n", chr_p, "Params", 10*eq->param->w-chr_p, "", "Coef");
    if (eq->vars) {
        printf("      |");
        for (i = 0; i < eq->param->w; i++)
            printf("%10s", eq->vars[i]);
        printf("|          \n");
    }
    for (i = 0; i < chr_dash; i++) putchar('-'); putchar('\n');
    for (i = 0; i < eq->coef->h; i++) {
        printf("Eq.%.2d |", i+1);
        for (j = 0; j < eq->param->w; j++)
            printf("%10.5g", eq->param->d[i][j]);
        putchar('|');
        printf("%10.5g", eq->coef->d[i][0]);
        putchar('\n');
    }
    for (i = 0; i < chr_dash; i++) putchar('-'); putchar('\n');
}

int isRowEchelonEqSet(const EqSet eq) {
    if (!_isValidEqSet(eq)) return 0;
    return isRowEchelonMatrix(eq->param);
}

int isReducedRowEchelonEqSet(const EqSet eq) {
    if (!_isValidEqSet(eq)) return 0;
    return isReducedRowEchelonMatrix(eq->param);
}

int rowRankEqSet(const EqSet eq) {
    if (!_isValidEqSet(eq)) return 0;
    return rowRankMatrix(eq->param);
}

void mulRowEqSet(EqSet eq, int row_i, double multi) {
    if (!_isValidEqSet(eq) || row_i < 0 || row_i > eq->param->h-1) return ;
    mulRowMatrix(eq->param, row_i, multi);
    mulRowMatrix(eq->coef,  row_i, multi);
}

void swpRowEqSet(EqSet eq, int row_i, int row_j) {
    if (!_isValidEqSet(eq) || row_j < 0 || row_j > eq->param->h-1
                           || row_i < 0 || row_i > eq->param->h-1) return ;
    swpRowMatrix(eq->param, row_i, row_j);
    swpRowMatrix(eq->coef,  row_i, row_j);
}

void mulRow2RowEqSet(EqSet eq, int row_i, double multi, int row_j) {
    if (!_isValidEqSet(eq) || row_j < 0 || row_j > eq->param->h-1
                           || row_i < 0 || row_i > eq->param->h-1) return ;
    mulRow2RowMatrix(eq->param, row_i, multi, row_j);
    mulRow2RowMatrix(eq->coef,  row_i, multi, row_j);
}

void toRowEchelonEqSet(EqSet eq) {
    if (!_isValidEqSet(eq)) return ;
    int ih = 0, iw = 0, j; // Init main at (0, 0)
    while (ih < eq->param->h && iw < eq->param->w) {
        if (_isZero(eq->param->d[ih][iw])) {
            j = ih+1;
            while (j < eq->param->h && _isZero(eq->param->d[j][iw])) j++;
            if (j == eq->param->h) {
                iw++;
                continue;
            }
            swpRowEqSet(eq, ih, j);
        }
        if (ih == eq->param->h-1 && _isZero(eq->param->d[ih][iw])) break;

        mulRowEqSet(eq, ih, 1/(eq->param->d[ih][iw]));

        for (j = ih+1; j < eq->param->h; j++)
            mulRow2RowEqSet(eq, ih, -eq->param->d[j][iw], j);
        ih++;
        iw++;
    }
}

void toReducedRowEchelonEqSet(EqSet eq) {
    if (!_isValidEqSet(eq)) return ;
    toRowEchelonEqSet(eq);
    int ih=rowRankEqSet(eq)-1, iw=0, j;
    for (; ih > 0; ih--) {
        for (iw = 0; iw < eq->param->w && _isZero(eq->param->d[ih][iw]); iw++) ;
        for (j = 0; j < ih; j++)
            mulRow2RowEqSet(eq, ih, -eq->param->d[j][iw], j);
    }
}





EqSet mulRowEqSetNew(const EqSet eq, int row_i, double multi) {
    EqSet res = copyEqSet(eq);
    mulRowEqSet(res, row_i, multi);
    return res;
}

EqSet swpRowEqSetNew(const EqSet eq, int row_i, int row_j) {
    EqSet res = copyEqSet(eq);
    swpRowEqSet(res, row_i, row_j);
    return res;
}

EqSet mulRow2RowEqSetNew(const EqSet eq, int row_i, double multi, int row_j) {
    EqSet res = copyEqSet(eq);
    mulRow2RowEqSet(res, row_i, multi, row_j);
    return res;
}

EqSet toRowEchelonEqSetNew(const EqSet eq) {
    EqSet res = copyEqSet(eq);
    toRowEchelonEqSet(res);
    return res;
}

EqSet toReducedRowEchelonEqSetNew(const EqSet eq) {
    EqSet res = copyEqSet(eq);
    toReducedRowEchelonEqSet(res);
    return res;
}





Matrix solveEqSet(const EqSet eq) {
    if (!_isValidEqSet(eq)) return NULL;
    printEqSet(eq);
    /* toReducedRowEchelonEqSet(eq); */
    return eq->coef;
}
