#include "eq_set.h"
#include <stdio.h>

NameTag vars[] = {
    "x1",
    "x2",
    "x3",
    "x4",
    "x5",
    ""
};

EqSet sample1() {
    FILE *fp_param = fopen("sample.param", "r");
    FILE *fp_coef  = fopen("sample.coef", "r");
    Matrix param = readMatrix(fp_param);
    Matrix coef = readMatrix(fp_coef);
    fclose(fp_param);
    fclose(fp_coef);
    EqSet eq = createEqSet(param, coef);
    setEqVars(eq, vars);
    deleteMatrix(param);
    deleteMatrix(coef);
    return eq;
}

EqSet sample2() {
    FILE *fp_eq_set = fopen("sample2.eqset", "r");
    EqSet eq = readEqSet(fp_eq_set);
    eq->param = concatColumnMatrix(createMatrix(eq->param->h, 2), eq->param);
    setEqVars(eq, NULL);
    fclose(fp_eq_set);
    return eq;
}

void printSolution(EqSet eq) {
    printf("\nRaw equation:\n");
    printEqSetFunc(eq); putchar('\n');

    printf("Reduced equations set below:\n");
    EqSet reduced_eq_set = toReducedRowEchelonEqSetNew(eq);
    printEqSet(reduced_eq_set); putchar('\n');
    deleteEqSet(reduced_eq_set);

    EqSol s = solveEqSet(eq);

    printf("Solution: (Style 1)\n");
    printEqSol(s, 1); putchar('\n');

    printf("Solution: (Style 2)\n");
    printEqSol(s, 2); putchar('\n');


    deleteEqSol(s);
}

int main(void) {
    EqSet eq = sample2();
    printSolution(eq);
    deleteEqSet(eq);
    return 0;
}

