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
    int i;
    EqSet eq = (EqSet) malloc (sizeof(struct SEqSet));
    eq->coef = coef ? copyMatrix(coef) : createMatrix(param->h, 1);
    eq->param = copyMatrix(param);
    eq->vars = (NameTag *) calloc (param->w, sizeof(NameTag));
    for (i = 0; i < eq->param->w; i++)
        sprintf(eq->vars[i], "X%d", i+1);
    return eq;
}

void setEqVars(EqSet eq, const NameTag *vars) {
    if (!_isValidEqSet(eq)) return ;
        int i;
    if (!vars) {
        free(eq->vars);
        eq->vars = (NameTag *) calloc (eq->param->w, sizeof(NameTag));
        for (i = 0; i < eq->param->w; i++)
            sprintf(eq->vars[i], "X%d", i+1);
    } else {
        for (i = 0; vars[i][0]; i++) ;
        if (i != eq->param->w) return ;
        free(eq->vars);
        eq->vars = (NameTag *) calloc (eq->param->w, sizeof(NameTag));
        for (i = 0; vars[i][0]; i++)
            strncpy(eq->vars[i], vars[i], NAMETAG_LEN);
    }
}

EqSet copyEqSet(const EqSet eq) {
    if (!_isValidEqSet(eq)) return NULL;
    EqSet res = createEqSet(eq->param, eq->coef);
    memcpy(res->vars, eq->vars, eq->param->w * sizeof(NameTag));
    return res;
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
    memcpy(eq->vars, eq1->vars, eq->param->w * sizeof(NameTag));
    deleteMatrix(new_param);
    deleteMatrix(new_coef);
    return eq;
}

void deleteEqSet(EqSet eq) {
    if (!eq) return ;
    free(eq->vars);
    deleteMatrix(eq->param);
    deleteMatrix(eq->coef);
    free(eq);
}





// EqSet Operation
void printEqSet(const EqSet eq) {
    if (!_isValidEqSet(eq)) return ;
    int chr_p = (10 * eq->param->w - 6)/2 + 6, chr_dash = 8 + 10 * (eq->param->w+1), i, j;
    NameTag *strv = eq->vars;
    for (i = 0; i < chr_dash; i++) putchar('-'); putchar('\n');
    printf("  Eq  |%*s%-*s| %6s   \n", chr_p, "Params", 10*eq->param->w-chr_p, "", "Coef");
    // Print X_n
    printf("      |");
    for (i = 0; i < eq->param->w; i++)
        printf("%10s", strv[i]);
    printf("|          \n");
    for (i = 0; i < chr_dash; i++) putchar('-'); putchar('\n');
    for (i = 0; i < eq->coef->h; i++) {
        printf("Eq.%.2d |", i+1);
        for (j = 0; j < eq->param->w; j++)
            printf("%10.4g", _toZero(eq->param->d[i][j]));
        putchar('|');
        printf("%10.4g", _toZero(eq->coef->d[i][0]));
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





int _hasSolution(const EqSet solved_eq) {
    if (!_isValidEqSet(solved_eq)) return 0;
    int i;
    const EqSet eq = solved_eq;
    for (i = eq->param->h-1; i >= 0; i--)
        if (isZeroRow(eq->param, i) && !_isZero(eq->coef->d[i][0]))
            return 0;
    return 1;
}

EqSol solveEqSet(const EqSet eq_set) {
    if (!_isValidEqSet(eq_set)) return NULL;
    EqSet eq = copyEqSet(eq_set);
    toReducedRowEchelonEqSet(eq);
    if (!_hasSolution(eq)) {
        deleteEqSet(eq);
        return NULL;
    }
    int eqRank = rowRankEqSet(eq), ih=0, iw=0, j, n_free=0;
    EqSol s = (EqSol) malloc (sizeof(struct SEqSol));
    s->n_free = eq->param->w - eqRank;
    s->vars = (NameTag *) calloc (eq->param->w, sizeof(NameTag));
    memcpy(s->vars, eq->vars, eq->param->w * sizeof(NameTag));
    if (s->n_free) s->free_vars = (int *) malloc (sizeof(int) * s->n_free);
    else s->free_vars = NULL;
    Matrix vars_sol = createMatrix(eqRank, s->n_free);
    while (ih < eq->param->h && iw < eq->param->w) {
        if (_isZero(eq->param->d[ih][iw])) {
            if (!s->n_free) printf("BUG!!!\n");
            s->free_vars[n_free] = iw;
            for (j = 0; j < eq->param->h; j++)
                vars_sol->d[j][n_free] = -eq->param->d[j][iw];
            n_free++;
            iw++;
            continue;
        }
        iw++;
        ih++;
    }
    while (iw < eq->param->w) {
        s->free_vars[n_free] = iw;
        for (j = 0; j < eq->param->h; j++)
            vars_sol->d[j][n_free] = -eq->param->d[j][iw];
        n_free++;
        iw++;
    }
    Matrix sub_coef = subMatrix(eq->coef, 0, 0, eqRank, 1);
    s->sol = concatColumnMatrix(sub_coef, vars_sol);
    deleteMatrix(sub_coef);
    deleteMatrix(vars_sol);
    deleteEqSet(eq);
    return s;
}

#define BUFSIZE 32
typedef char BufItem[BUFSIZE];

int _prePrintArr(BufItem *dest, const double *val, int n_val) {
    int i, len, max_len = 0, max_i = 0;
    for (i = 0; i < n_val; i++) {
        len = sprintf(dest[i], "%.4g", _toZero(val[i]));
        if (len > max_len) {
            max_len = len;
            max_i   = i;
        }
    }
    return max_len;
}

void printEqSetFunc(const EqSet eq) {
    if (!_isValidEqSet(eq)) {
        printf("*No equations*");
        return ;
    }
    int i, j, isPrinted;
    NameTag *strv = eq->vars;
    for (i = 0; i < eq->param->h; i++) {
        isPrinted = 0;
        for (j = 0; j < eq->param->w; j++) {
            if (_isZero(eq->param->d[i][j])) continue;
            if (_equal(eq->param->d[i][j], 1) || _equal(eq->param->d[i][j], -1))
                printf("%s", eq->param->d[i][j]>0? (isPrinted ? "+" : ""):"-");
            else
                printf(isPrinted ? "%.4g":"%+.4g", eq->param->d[i][j]);
            isPrinted = 1;
            printf("%s", strv[j]);
        }
        if (!isPrinted) printf("0");
        printf("=%.4g\n", eq->coef->d[i][0]);
    }
}

void printEqSol(const EqSol s, int mode) {
    if (!s) {
        printf("*No solution*\n");
        return ;
    }
    int n_vars = s->sol->h, n_free = s->n_free, i, j, k;
    int *fidx = (int *) malloc (sizeof(int) * (n_vars + n_free));  // -1 for free, >=0 for not free.
    if (!n_free)
        for (i = 0; i < n_vars + n_free; i++)
            fidx[i] = i;
    else
        for (i = j = k = 0; i < n_vars + n_free; i++)
            if (j < n_vars && i == s->free_vars[j]) {
                fidx[i] = -1;
                j++;
            } else fidx[i] = k++;

    NameTag *strv = s->vars;
    NameTag *strf;
    if (!n_free) strf = NULL;
    else {
        strf = (NameTag *) calloc (n_free, sizeof(NameTag));
        if (n_free == 1) sprintf(strf[0], "C");
        else for (i = 0; i < n_free; i++)
            sprintf(strf[i], i > 9 ? "C%d" : "C%d", i);
    }

    switch (mode) {
        case 1:
            for (i = 0; i < n_vars + n_free; i++) {
                if (fidx[i] == -1) {
                    for (j = 0; j < n_free; j++)
                        if (s->free_vars[j] == i) break;
                    printf("%*s = %s\n", NAMETAG_LEN, strv[i], strf[j]);
                    continue;
                }
                printf("%*s = ", NAMETAG_LEN, strv[i]);
                printf("%.4g", _toZero(s->sol->d[fidx[i]][0]));
                for (j = 0; j < n_free; j++) {
                    if (_isZero(s->sol->d[fidx[i]][j+1])) continue;
                    if (_equal(s->sol->d[fidx[i]][j+1], -1)) putchar('-');
                    else if (_equal(s->sol->d[fidx[i]][j+1], 1)) {}
                    else printf("%+.4g*", _toZero(s->sol->d[fidx[i]][j+1]));
                    printf("%s", strf[j]);
                }
                putchar('\n');
            }
            break;
        case 2:
            {
                printf(" Free variables: ");
                for (i = 0; i < n_free; i++)
                    printf("%s = %s  ", strv[s->free_vars[i]], strf[i]);
                putchar('\n');
                Matrix trs = transposeMatrix(s->sol); // Transposed solution
                BufItem buf[s->sol->h];
                int strv_maxlen = 0, main_row, strf_lens[n_free], first_row, last_row;
                for (i = 0; i < n_free + n_vars; i++) {
                    if (fidx[i] == -1) continue;
                    int len = strlen(strv[i]);
                    if (len > strv_maxlen) strv_maxlen = len;
                }
                // Get row info
                for (last_row = n_vars+n_free-1; last_row>0 && fidx[last_row]==-1; last_row--);
                for (first_row = 0; first_row < n_vars+n_free && fidx[first_row]==-1; first_row++)
                for (main_row = first_row, k=0; main_row < last_row && k < n_vars/2; main_row++) {
                    if (fidx[main_row] == -1) continue;
                    k++;
                }
                for (i = 0; i < n_free; i++)
                    strf_lens[i] = strlen(strf[i]);
                int nmaxs[s->n_free + 1]; // Number of max num length.
                for (i = 0; i < s->n_free+1; i++)
                    nmaxs[i] = _prePrintArr(buf, trs->d[i], s->sol->h);

                for (i = 0; i < n_vars + n_free; i++) {
                    if (fidx[i] == -1) continue;
                    const char *st = (i == first_row ? "┌" : (i == last_row ? "└" : "│")),
                               *et = (i == first_row ? "┐" : (i == last_row ? "┘" : "│"));

                    printf("%s %*s %s %c ", st, strv_maxlen, strv[i], et, i == main_row ? '=' : ' ');
                    printf("%s %*.4g %s", st, nmaxs[0], _toZero(s->sol->d[fidx[i]][0]), et);
                    for (j = 0; j < n_free; j++) {
                        /* printf("\n%d : %d : isZero: %s\n", j, j+1, isZeroRow(trs, j+1) ? "True" : "False"); */
                        if (isZeroRow(trs, j+1)) continue;
                        printf(" %c %s %*.4g %s%*s", (i == main_row ? '+' : ' '), st, nmaxs[j+1],
                                                     _toZero(s->sol->d[fidx[i]][j+1]), et,
                                                     strf_lens[j], (i == main_row ? strf[j] : ""));
                    }

                    putchar('\n');
                }
                deleteMatrix(trs);
            }

            // ┌ X1 ┐
            // │ X2 │
            // └ X3 ┘

            break;
        default:
            printf(" Free variables: ");
            for (i = 0; i < n_free; i++)
                printf("%s = %s  ", strv[s->free_vars[i]], strf[i]);
            printf("\nOther variables: ");
            for (i = 0; i < n_vars + n_free; i++) {
                if (fidx[i] == -1) continue;
                printf("%s  ", strv[i]);
            }
            printf("\nSolution matrix: (Coef & free vars)\n");
            printMatrix(s->sol);
    }

    free(fidx);
    if (n_free) free(strf);
}

void deleteEqSol(EqSol sol) {
    if (!sol) return ;
    free(sol->vars);
    if (sol->free_vars) free(sol->free_vars);
    deleteMatrix(sol->sol);
    free(sol);
}

