
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intdgeqpf3)(char *fname, unsigned long fname_len);
extern int C2F(intzgeqpf3)(char *fname, unsigned long fname_len);
extern int C2F(intdgeqpf4)(char *fname, unsigned long fname_len);
extern int C2F(intzgeqpf4)(char *fname, unsigned long fname_len);
extern int C2F(doldqr)(double *tol, char *fname, unsigned long fname_len);
extern int C2F(zoldqr)(double *tol, char *fname, unsigned long fname_len);

/*--------------------------------------------------------------------------*/
int C2F(intqr)(char *fname, unsigned long fname_len)
{
    int *header1;
    int *header2;
    int Cmplx;
    int ret;
    double *snd;
    double tol;

    if (GetType(1) != sci_matrix)
    {
        OverLoad(1);
        return 0;
    }
    header1 = (int *) GetData(1);
    Cmplx = header1[3];

    if (header1[0] == 10)
    {
        Cmplx = 10;
    }

    if (Lhs == 4)
    {
        /* obsolete : [Q,R,rk,E]=qr(A) or = qr(A,tol)   */
        if (Rhs == 2)
        {
            if (GetType(2) == sci_matrix)
            {
                snd = (double *) GetData(2);
                tol = snd[2];
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"),
                         fname, 2);
                return 0;
            }
        }
        else
        {
            tol = -1;
            Rhs = 1;
        }

        switch (Cmplx)
        {
            case REAL :
                ret = C2F(doldqr)(&tol, "qr", 2L);
                break;
            case COMPLEX :
                ret = C2F(zoldqr)(&tol, "qr", 2L);
                break;
            default :
                Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                         fname, 1);
                return 0;
        }
        return 0;
    }

    switch (Rhs)
    {
        case 1:   /*   qr(A)   */
            switch (Cmplx)
            {
                case REAL :
                    ret = C2F(intdgeqpf3)("qr", 2L);
                    break;
                case COMPLEX :
                    ret = C2F(intzgeqpf3)("qr", 2L);
                    break;
                default :
                    Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                             fname, 1);
                    break;
            }
            break;

        case 2 :   /*   qr(A, something)   */
            header2 = (int *) GetData(2);
            switch (header2[0])
            {
                case STRING  :
                    /* Economy size:  ...=qr(A,"e")  */
                    switch (Cmplx)
                    {
                        case REAL :
                            ret = C2F(intdgeqpf4)("qr", 2L);
                            break;

                        case COMPLEX :
                            ret = C2F(intzgeqpf4)("qr", 2L);
                            break;

                        default :
                            Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                                     fname, 1);
                            break;
                    }
                    break;

                default:
                    Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                             fname, 2);
                    break;
            }
            return 0;
        default :
            Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                     fname, 1);
            break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
