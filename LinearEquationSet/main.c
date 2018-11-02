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

int main(void) {
    FILE *fp_param = fopen("sample.param", "r");
    FILE *fp_coef  = fopen("sample.coef", "r");
    Matrix param = readMatrix(fp_param);
    Matrix coef = readMatrix(fp_coef);
    fclose(fp_param);
    fclose(fp_coef);
    EqSet eq = createEqSet(param, coef);
    setEqVars(eq, vars);
    printEqSet(eq);
    deleteMatrix(param);
    deleteMatrix(coef);
    deleteEqSet(eq);
    return 0;
}
