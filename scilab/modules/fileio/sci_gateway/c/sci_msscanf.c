/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include "MALLOC.h"
#include "stack-c.h"
#include "do_xxprintf.h"
#include "do_xxscanf.h"
#include "scanf_functions.h"
#include "scistrtostr.h"
#include "StringConvert.h"
#include "gw_fileio.h"
#include "Scierror.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
int sci_sscanf(char *fname, unsigned long fname_len)
{
    static int l1 = 0, m1 = 0, n1 = 0, l2 = 0, m2 = 0, n2 = 0, iarg = 0, maxrow = 0, nrow = 0, rowcount = 0, ncol = 0;
    int args = 0, retval = 0, retval_s = 0, err = 0, n_count = 0, lw = 0, il1 = 0, ild1 = 0, skip = 0;
    int k = 0;

    entry *data = NULL;
    rec_entry buf[MAXSCAN];
    sfdir type[MAXSCAN], type_s[MAXSCAN];
    char *str = NULL;

    Nbvars = 0;
    CheckRhs(2, 3);

    if (Rhs == 3)
    {
        GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
        if (m1 * n1 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Scalar expected.\n"), fname, 1);
            return 0;
        }

        iarg = 2;
        maxrow = *istk(l1);
    }
    else
    {
        iarg = 1;
        maxrow = 1;
    }

    lw = iarg + Top - Rhs;      /* Scilab string vector */
    if (!C2F(getwsmat) ("sscanf", &Top, &lw, &m1, &n1, &il1, &ild1, 6L))
        return 0;
    GetRhsVar(iarg + 1, STRING_DATATYPE, &m2, &n2, &l2);    /* Format */
    n_count = StringConvert(cstk(l2)) + 1;  /* conversion */

    if ((maxrow >= 0) && (maxrow * n_count > m1 * n1))
    {
        Scierror(999, _("%s: An error occurred: %s\n"), fname, _("Not enough entries."));
        return 0;
    }

    k = 0;
    nrow = maxrow;
    rowcount = -1;              /* number-1 of result lines already got */
    while (1)
    {
        rowcount++;
        if ((maxrow >= 0) && (rowcount >= maxrow))
            break;
        if (k >= m1 * n1)
            break;
        skip = *istk(ild1 + k) - 1;
        SciStrtoStr(istk(il1 + skip), &n_count, istk(ild1 + k), &str);
        k += n_count;

        args = Rhs;             /* args set to Rhs on entry */
        err = do_xxscanf("sscanf", (FILE *) 0, cstk(l2), &args, str, &retval, buf, type);
        FREE(str);
        if (err < 0)
            return 0;

        if ((err = Store_Scan(&nrow, &ncol, type_s, type, &retval, &retval_s, buf, &data, rowcount, args)) < 0)
        {
            switch (err)
            {
            case MISMATCH:
                if (maxrow >= 0)
                {
                    Free_Scan(rowcount, ncol, type_s, &data);
                    Scierror(999, _("%s: Data mismatch.\n"), fname);
                    return 0;
                }
                break;

            case MEM_LACK:
                Free_Scan(rowcount, ncol, type_s, &data);
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            if (err == MISMATCH)
                break;
        }
    }                           /* while */

    /* create Scilab variables with each column of data */
    err = Sci_Store(rowcount, ncol, data, type_s, retval_s);
    Free_Scan(rowcount, ncol, type_s, &data);
    if (err == MEM_LACK)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
