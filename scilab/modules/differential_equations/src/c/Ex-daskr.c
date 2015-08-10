#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "core_math.h"
#include "Ex-daskr.h"
#include "numericconstants_interface.h"

void pjac1( resfunc res, int *ires, int *nequations, double *tOld, double *actual, double *actualP,
            double *rewt, double *savr, double *wk, double *h, double *cj, double *wp, int *iwp,
            int *ier, double *rpar, int *ipar)
{
    int i = 0;
    int j = 0;
    int info = 0;
    int nrow = 0;
    double tx = 0;
    double del = 0;
    double delinv = 0;
    double ysave = 0;
    double ypsave = 0;
    double * e = NULL;

    int neq = *nequations;
    double SQuround = sqrt(nc_eps_machine());

    tx = *tOld;

    e = (double *) calloc(neq, sizeof(double));

    for (i = 0 ; i < neq ; ++i)
    {
        del =  Max(SQuround * Max(fabs(actual[i]), fabs(*h * actualP[i])), 1. / rewt[i]);
        del *= (*h * actualP[i] >= 0) ? 1 : -1;
        del =  (actual[i] + del) - actual[i];
        ysave  = actual[i];
        ypsave = actualP[i];
        actual[i]  += del;
        actualP[i] += *cj * del;
        res(&tx, actual, actualP, e, ires, rpar, ipar);

        if (*ires < 0)
        {
            *ier = -1;
            free(e);
            return;
        }

        delinv = 1. / del;
        for (j = 0 ; j < neq ; j++)
        {
            wp[nrow + j] = (e[j] - savr[j]) * delinv;

            /* NaN test */
            if (ISNAN(wp[nrow + j]))
            {
                *ier = -1;
                free(e);
                return;
            }
        }
        nrow       += neq;
        actual[i]  =  ysave;
        actualP[i] =  ypsave;
    }

    /* Proceed to LU factorization of P. */
    C2F(dgefa) (wp, nequations, nequations, iwp, &info);
    if (info != 0)
    {
        *ier = -1;
    }

    free(e);
}

void psol1( int *nequations, double *tOld, double *actual, double *actualP,
            double *savr, double *wk, double *cj, double *wght, double *wp,
            int *iwp, double *b, double *eplin, int *ier, double *dummy1, int *dummy2)
{
    int i = 0, job = 0;

    C2F(dgesl) (wp, nequations, nequations, iwp, b, &job);

    /* NaN test */
    for (i = 0; i < *nequations; ++i)
    {
        if (ISNAN(b[i]))
        {
            /* Indicate a recoverable error, meaning that the step will be retried with the same step size,
               but with a call to 'jacpsol' to update necessary data, unless the Jacobian data is current,
               if (b[i] - b[i] != 0) in which case the step will be retried with a smaller step size. */
            *ier = -1;
            return;
        }
    }
}
