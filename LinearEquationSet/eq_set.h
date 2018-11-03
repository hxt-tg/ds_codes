#ifndef EQ_SET_H
#define EQ_SET_H

#include "matrix.h"
#include <stdio.h>

#define NAMETAG_LEN 7

typedef char NameTag[NAMETAG_LEN+1];

typedef struct SEqSet {
    Matrix param;
    Matrix coef;
    NameTag *vars;
} *EqSet;

typedef struct SEqSol {
    Matrix sol;
    NameTag *vars;
    int n_free;        /* Free variables number */
    int *free_vars;
} *EqSol;


// EqSet Construction
EqSet createEqSet(const Matrix param, const Matrix coef);
void setEqVars(EqSet eq, const NameTag *vars);
EqSet copyEqSet(const EqSet eq);
EqSet readEqSet(FILE *fp);
EqSet concatEqSet(const EqSet eq1, const EqSet eq2);
void  deleteEqSet(EqSet eq);

// EqSet Operation
void  printEqSet(const EqSet eq);
int   isRowEchelonEqSet(const EqSet eq);
int   isReducedRowEchelonEqSet(const EqSet eq);
int   rowRankEqSet(const EqSet eq);

void  mulRowEqSet(EqSet eq, int row_i, double multi);
void  swpRowEqSet(EqSet eq, int row_i, int row_j);
void  mulRow2RowEqSet(EqSet eq, int row_i, double multi, int row_j);
void  toRowEchelonEqSet(EqSet eq);
void  toReducedRowEchelonEqSet(EqSet eq);

EqSet mulRowEqSetNew(const EqSet eq, int row_i, double multi);
EqSet swpRowEqSetNew(const EqSet eq, int row_i, int row_j);
EqSet mulRow2RowEqSetNew(const EqSet eq, int row_i, double multi, int row_j);
EqSet toRowEchelonEqSetNew(const EqSet eq);
EqSet toReducedRowEchelonEqSetNew(const EqSet eq);

// Equation Set Solve
EqSol solveEqSet(const EqSet eq);
void  printEqSetFunc(const EqSet eq);
void  printEqSol(const EqSol sol, int mode); // 1 for row and 2 for column
void  deleteEqSol(EqSol sol);

#endif /* ifndef EQ_SET_H */


