#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "core_math.h"

extern int C2F(dgesv)(int*, int*, double*, int*, int*, double*, int*, int*);
extern double C2F(dlamch)(char*);
typedef void (*resfunc)(double*, double*, double*, double*, int*, double*, int*);

int C2F(pjac1)(resfunc res, int *ires, int *nequations, double *tOld, double *actual, double *actualP,
               double *rewt, double *savr, double *wk, double *h, double *cj, double *wp, int *iwp,
               int *ier, double *rpar, int *ipar)
{
    int i = 0;
    int j = 0;
    int nrow = 0;
    double tx = 0;
    double del = 0;
    double delinv = 0;
    double ysave = 0;
    double ypsave = 0;
    double* e = NULL;
    int neq = *nequations;
    double SQuround = sqrt(C2F(dlamch)("P"));

    tx = *tOld;

    e = (double*)calloc(neq, sizeof(double));

    for (i = 0 ; i < neq ; ++i)
    {
        del = Max(SQuround * Max(fabs(actual[i]), fabs(*h * actualP[i])), 1. / rewt[i]);
        del *= (*h * actualP[i] >= 0) ? 1 : -1;
        ysave   =  actual[i];
        ypsave  =  actualP[i];
        actual[i]  += del;
        actualP[i] += *cj * del;
        res(&tx, actual, actualP, e, ires, rpar, ipar);

        if (ires < 0)
        {
            return 0;
        }

        delinv = 1. / del;
        for (j = 0 ; j < neq ; j++)
        {
            wp[nrow + j] = (e[j] - savr[j]) * delinv;
            iwp[nrow + j] = nrow + j + 1;
            iwp[nrow + j + neq * neq] = nrow + j + 1;
        }
        nrow       += neq;
        actual[i]  =  ysave;
        actualP[i] =  ypsave;
    }

    free(e);

    return (*ier);
}

int C2F(psol1)(int *nequations, double *tOld, double *actual, double *actualP,
               double *savr, double *wk, double *cj, double *wght, double *wp,
               int *iwp, double *b, double *eplin, int *ier, double *dummy1, int *dummy2)
{
    int nColB = 1;
    int info = 0;
    int *ipiv = NULL;

    ipiv = (int *) malloc(*nequations * sizeof(int));

    C2F(dgesv) (nequations, &nColB, wp, nequations, ipiv, b, nequations, &info);

    free(ipiv);
    return 0;
}

