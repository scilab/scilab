/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_io.h"
#include "PATH_MAX.h"
#include "stack-c.h"
#include "fileinfo.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "Scierror.h"
#include "transposeMatrix.h"
/*--------------------------------------------------------------------------*/
int sci_fileinfo(char *fname, unsigned long fname_len)
{
    if (VarType(1) == sci_strings)
    {
        char **InputString_Parameter = NULL;
        int m = 0, n = 0;
        int mn = 0;

        CheckRhs(1, 1);
        CheckLhs(1, 2);

        GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &m, &n, &InputString_Parameter);
        mn = m * n;

        if (mn == 0)
        {
            Scierror(999, "%s: Memory allocation error.\n", fname);
            return 0;
        }

        if (mn == 1)
        {
            int result = 0;
            double *infos = fileinfo(InputString_Parameter[0], &result);
            freeArrayOfString(InputString_Parameter, mn);

            if (infos)
            {
                int m_out = 1;
                int n_out = FILEINFO_ARRAY_SIZE;
                CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m_out, &n_out, &infos);
                LhsVar(1) = Rhs + 1;
                FREE(infos);
            }
            else
            {
                int m_out = 0;
                int n_out = 0;
                int l_out = 0;

                /* returns [] */

                CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m_out, &n_out, &l_out);
                LhsVar(1) = Rhs + 1;
            }


            if (Lhs == 2)
            {
                int m_out = 1;
                int n_out = 1;
                int l_out = 0;
                CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &m_out, &n_out, &l_out);
                *stk(l_out) = (double) result;
                LhsVar(2) = Rhs + 2;
            }

            PutLhsVar();
            return 0;
        }
        else
        {
            if (m != 1 && n == 1) //column vector only
            {
                int *results = (int*)MALLOC(sizeof(int) * mn);
                double *infos = filesinfo(InputString_Parameter, mn, results);
                freeArrayOfString(InputString_Parameter, mn);
                if (infos)
                {
                    int m_out = 0;
                    int n_out = 0;
                    double *transposeInfos = NULL;

                    transposeInfos = transposeMatrixDouble(FILEINFO_ARRAY_SIZE, m, infos);
                    FREE(infos);

                    n_out = FILEINFO_ARRAY_SIZE;
                    m_out = m;

                    CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m_out, &n_out, &transposeInfos);
                    LhsVar(1) = Rhs + 1;
                    FREE(transposeInfos);
                }
                else
                {
                    int m_out = 0;
                    int n_out = 0;
                    int l_out = 0;

                    /* returns [] */
                    FREE(results);
                    CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m_out, &n_out, &l_out);
                    LhsVar(1) = Rhs + 1;
                    if (Lhs == 2)
                    {
                        CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &m_out, &n_out, &l_out);
                        LhsVar(2) = Rhs + 2;
                    }

                    PutLhsVar();
                    return 0;
                }

                if (Lhs == 2)
                {
                    int i = 0;
                    int m_out = m;
                    int n_out = n;
                    int l_out = 0;

                    CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &m_out, &n_out, &l_out);
                    for (i = 0; i < mn; i++)
                    {
                        *stk(l_out + i) = (double) results[i];
                    }
                    LhsVar(2) = Rhs + 2;
                }

                FREE(results);
                PutLhsVar();
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A m-by-1 array expected.\n"), fname, 1);
                return 0;
            }
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument: A string expected.\n"), fname);
        return 0;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/

