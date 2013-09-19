/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2012 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "BOOL.h"
#include "vect_or.h"
/*--------------------------------------------------------------------------*/
void vect_or(const int *v, int m, int n, int *r, int opt)
{
    switch (opt)
    {
        case OR_ON_ALL:
        {
            int k = 0;
            r[0] = FALSE;
            for (k = 0; k < m * n; k++)
            {
                if (v[k])
                {
                    r[0] = TRUE;
                    break;
                }
            }
        }
        break;
        case OR_BY_ROWS:
        {
            int k = 0;
            for (k = 0; k < n; k++)
            {
                int l = 0;
                int i = k * m;
                r[k] = FALSE;
                for (l = 0; l < m; l++)
                {
                    if (v[i++])
                    {
                        r[k] = TRUE;
                        break;
                    }
                }
            }
        }
        break;
        case OR_BY_COLUMNS:
        {
            int l = 0;
            for (l = 0; l < m; l++)
            {
                int k = 0;
                int i = l;
                r[l] = FALSE;
                for (k = 0; k < n; k++)
                {
                    if (v[i])
                    {
                        r[l] = TRUE;
                        break;
                    }
                    i += m;
                }
            }
        }
        break;
    }
}
/*--------------------------------------------------------------------------*/
