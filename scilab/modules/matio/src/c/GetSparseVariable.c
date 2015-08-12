/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "GetMatlabVariable.h"
#include "stack-c.h"
#include "api_scilab.h"

matvar_t *GetSparseVariable(void *pvApiCtx, int iVar, const char *name, int * parent, int item_position)
{
    int K = 0;
    int rank = 0;
    size_t* pszDims = NULL;
    int *dims = NULL;
    double *data = NULL;
    matvar_t *createdVar = NULL;
    mat_sparse_t *sparseData = NULL;
    SciSparse scilabSparse;
    int *colIndexes = NULL;
    int *rowIndexes = NULL;
    int * var_addr = NULL;
    int * item_addr = NULL;
    int var_type;
    SciErr sciErr;

    if (parent == NULL)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getVarType(pvApiCtx, var_addr, &var_type);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    else
    {
        sciErr = getListItemAddress(pvApiCtx, parent, item_position, &item_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getVarType(pvApiCtx, item_addr, &var_type);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }

    if (var_type == sci_sparse)
    {
        sparseData = (mat_sparse_t*) MALLOC(sizeof(mat_sparse_t));
        if (sparseData == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
            return FALSE;
        }

        rank = 2;
        if ((dims = (int*) MALLOC (sizeof(int) * rank)) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
            return NULL;
        }

        if (isVarComplex(pvApiCtx, var_addr))
        {
            if (parent == NULL)
            {
                getAllocatedComplexSparseMatrix(pvApiCtx, var_addr, &dims[1], &dims[0],
                                                &scilabSparse.nel, &scilabSparse.mnel,
                                                &scilabSparse.icol, &scilabSparse.R,
                                                &scilabSparse.I);
            }
            else
            {
                sciErr = getSparseMatrixInList(pvApiCtx, parent, item_position, &dims[1], &dims[0],
                                               &scilabSparse.nel, NULL, NULL, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                scilabSparse.mnel = (int *)MALLOC(dims[1] * sizeof(int));
                if (scilabSparse.mnel == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");

                    FREE(scilabSparse.mnel);

                    return FALSE;
                }
                scilabSparse.icol = (int *)MALLOC(scilabSparse.nel * sizeof(int));
                if (scilabSparse.icol == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");

                    FREE(scilabSparse.mnel);
                    FREE(scilabSparse.icol);

                    return FALSE;
                }
                scilabSparse.R    = (double *)MALLOC(scilabSparse.nel * sizeof(double));
                if (scilabSparse.R == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");

                    FREE(scilabSparse.mnel);
                    FREE(scilabSparse.icol);
                    FREE(scilabSparse.R);

                    return FALSE;
                }
                scilabSparse.I    = (double *)MALLOC(scilabSparse.nel * sizeof(double));
                if (scilabSparse.I == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");

                    FREE(scilabSparse.mnel);
                    FREE(scilabSparse.icol);
                    FREE(scilabSparse.R);
                    FREE(scilabSparse.I);

                    return FALSE;
                }
                sciErr = getComplexSparseMatrixInList(pvApiCtx, parent, item_position, &dims[1], &dims[0],
                                                      &scilabSparse.nel, &scilabSparse.mnel,
                                                      &scilabSparse.icol, &scilabSparse.R, &scilabSparse.I);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }

            scilabSparse.it = 1;

        }
        else
        {
            if (parent == NULL)
            {
                getAllocatedSparseMatrix(pvApiCtx, var_addr, &dims[1], &dims[0],
                                         &scilabSparse.nel, &scilabSparse.mnel,
                                         &scilabSparse.icol, &scilabSparse.R);
            }
            else
            {
                sciErr = getSparseMatrixInList(pvApiCtx, parent, item_position, &dims[1], &dims[0],
                                               &scilabSparse.nel, NULL, NULL, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                scilabSparse.mnel = (int *)MALLOC(dims[1] * sizeof(int));
                if (scilabSparse.mnel == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");

                    FREE(scilabSparse.mnel);

                    return FALSE;
                }
                scilabSparse.icol = (int *)MALLOC(scilabSparse.nel * sizeof(int));
                if (scilabSparse.icol == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");

                    FREE(scilabSparse.mnel);
                    FREE(scilabSparse.icol);

                    return FALSE;
                }
                scilabSparse.R    = (double *)MALLOC(scilabSparse.nel * sizeof(double));
                if (scilabSparse.R == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");

                    FREE(scilabSparse.mnel);
                    FREE(scilabSparse.icol);
                    FREE(scilabSparse.R);

                    return FALSE;
                }
                sciErr = getSparseMatrixInList(pvApiCtx, parent, item_position, &dims[1], &dims[0],
                                               &scilabSparse.nel, &scilabSparse.mnel,
                                               &scilabSparse.icol, &scilabSparse.R);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }

            scilabSparse.it = 0;
        }

        scilabSparse.m = dims[1];
        scilabSparse.n = dims[0];

        /* colIndexes = (int*) MALLOC(sizeof(int) *  (scilabSparse.nel + 1));  */
        colIndexes = (int*) MALLOC(sizeof(int) *  (scilabSparse.m + 1));
        if (colIndexes == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");

            if (scilabSparse.it)
            {
                freeAllocatedComplexSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R, scilabSparse.I);
            }
            else
            {
                freeAllocatedSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R);
            }

            return FALSE;
        }

        colIndexes[0] = 0;
        /* for (K=0; K<scilabSparse.nel; K++) */
        for (K = 0; K < scilabSparse.m; K++)
        {
            colIndexes[K + 1] = colIndexes[K] + scilabSparse.mnel[K];
        }

        rowIndexes = (int*) MALLOC(sizeof(int) *  scilabSparse.nel);
        if (rowIndexes == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");

            if (scilabSparse.it)
            {
                freeAllocatedComplexSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R, scilabSparse.I);
            }
            else
            {
                freeAllocatedSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R);
            }

            return FALSE;
        }

        for (K = 0; K < scilabSparse.nel; K++)
        {
            rowIndexes[K] = scilabSparse.icol[K] - 1;
        }

        if (scilabSparse.it == 0) /* Real sparse */
        {
            if ((data = (double*) MALLOC(sizeof(double) * scilabSparse.nel)) == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
                FREE(rowIndexes);
                FREE(colIndexes);
                if (scilabSparse.it)
                {
                    freeAllocatedComplexSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R, scilabSparse.I);
                }
                else
                {
                    freeAllocatedSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R);
                }

                return 0;
            }

            for (K = 0; K < scilabSparse.nel; K++)
            {
                data[K] = scilabSparse.R[K];
            }
        }
        else
        {
            if ((data = (double*) MALLOC(2 * sizeof(double) * scilabSparse.nel)) == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
                FREE(rowIndexes);
                FREE(colIndexes);
                if (scilabSparse.it)
                {
                    freeAllocatedComplexSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R, scilabSparse.I);
                }
                else
                {
                    freeAllocatedSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R);
                }

                return FALSE;
            }

            for (K = 0; K < scilabSparse.nel; K++)
            {
                data[K] = scilabSparse.R[K];
            }

            for (K = 0; K < scilabSparse.nel; K++)
            {
                data[K + scilabSparse.nel] = scilabSparse.I[K];
            }
        }

        /* Create Matlab Sparse matrix data */
        sparseData->nzmax = scilabSparse.nel;
        sparseData->nir   = scilabSparse.nel;
        sparseData->ir    = rowIndexes;
        /* sparseData->njc   = scilabSparse.nel + 1; */
        sparseData->njc   = scilabSparse.m + 1;
        sparseData->jc    = colIndexes;
        sparseData->ndata = scilabSparse.nel;
        sparseData->data  = (void*) data;

        pszDims = (size_t*) MALLOC(rank * sizeof(size_t));
        if (pszDims == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");

            if (scilabSparse.it)
            {
                freeAllocatedComplexSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R, scilabSparse.I);
            }
            else
            {
                freeAllocatedSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R);
            }

            return 0;
        }

        for (K = 0; K < rank; K++)
        {
            pszDims[K] = dims[K];
        }

        if (scilabSparse.it == 0)
        {
            createdVar = Mat_VarCreate(name, MAT_C_SPARSE, MAT_T_DOUBLE, rank, pszDims, sparseData, 0 | MAT_F_DONT_COPY_DATA);
        }
        else
        {
            createdVar = Mat_VarCreate(name, MAT_C_SPARSE, MAT_T_DOUBLE, rank, pszDims, sparseData, MAT_F_COMPLEX | MAT_F_DONT_COPY_DATA);

            if (data)
            {
                FREE(data);
            }
        }

        if (dims)
        {
            FREE(dims);
        }

        if (pszDims)
        {
            FREE(pszDims);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for first input argument: Sparse matrix expected.\n"), "GetSparseVariable");
    }

    if (scilabSparse.it)
    {
        freeAllocatedComplexSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R, scilabSparse.I);
    }
    else
    {
        freeAllocatedSparseMatrix(scilabSparse.mnel, scilabSparse.icol, scilabSparse.R);
    }

    return createdVar;
}
