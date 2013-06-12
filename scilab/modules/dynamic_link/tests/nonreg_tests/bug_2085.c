/* ============================================== */
/* Allan CORNET - bug 2085 */
/* ============================================== */
#include <string.h>
#include <stdio.h>
#define __USE_DEPRECATED_STACK_FUNCTIONS__
#include "stack-c.h"
/* ============================================== */
static void f99(double *ar, double *ac, int *ita, int *ma, int *na);
/* ============================================== */
int intex2c(char *fname)
{
    static int lr1, lc1, it1, m1, n1;
    int minlhs = 1, minrhs = 1, maxlhs = 1, maxrhs = 1;

    CheckRhs(minrhs, maxrhs) ;
    CheckLhs(minlhs, maxlhs) ;

    GetRhsCVar(1, "d", &it1, &m1, &n1, &lr1, &lc1);

    f99(stk(lr1), stk(lc1), &it1, &m1, &n1);

    LhsVar(1) = 1;
    return 0;
}
/* ============================================== */
static void f99(double *ar, double *ac, int *ita, int *ma, int *na)
{
    int i = 0;
    for ( i = 0 ; i < (*ma) * (*na) ; i++)
    {
        ar[i] = 2 * ar[i] ;
    }
    if ( *ita == 1)
    {
        for ( i = 0 ; i < (*ma) * (*na) ; i++)
        {
            ac[i] = 3 * ac[i] ;
        }
    }
}
/* ============================================== */
