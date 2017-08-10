/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "GetMatlabVariable.hxx"

extern "C"
{
#include "api_scilab.h"
#include "scisparse.h"
}

matvar_t *GetSparseVariable(void *pvApiCtx, int iVar, const char *name, int * parent, int item_position)
{

    types::GatewayStruct* pGS = (types::GatewayStruct*)pvApiCtx;
    types::typed_list in = *pGS->m_pIn;

    if (in[iVar - 1]->isSparse() == false)
    {
        Scierror(999, _("%s: Wrong type for first input argument: Sparse matrix expected.\n"), "GetSparseVariable");
        return NULL;
    }

    types::Sparse* pSparse = in[iVar - 1]->getAs<types::Sparse>();

    return GetSparseMatVar(pSparse, name);
}

matvar_t* GetSparseMatVar(types::Sparse* pSparse, const char *name)
{
    int Dims = pSparse->getDims();
    int* pDims = pSparse->getDimsArray();
    int isize = pSparse->getSize();
    size_t* psize_t = NULL;
    matvar_t * pMatVarOut = NULL;

    if (pSparse->getDims() > 2)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetSparseMatVar");
        return NULL;
    }

    mat_sparse_t *sparseData = NULL;
    sparseData = (mat_sparse_t*)MALLOC(sizeof(mat_sparse_t));
    if (sparseData == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "GetSparseMatVar");
        return NULL;
    }

    int nonZeros = pSparse->nonZeros();
    int* colPos = new int[nonZeros];
    int* itemsRow = new int[pSparse->getRows()];
    pSparse->getNbItemByRow(itemsRow);

    int* colIndexes = (int*)MALLOC(sizeof(int) *  (pSparse->getRows() + 1));
    if (colIndexes == NULL)
    {
        FREE(sparseData);
        Scierror(999, _("%s: No more memory.\n"), "GetSparseMatVar");
        delete[] colPos;
        delete[] itemsRow;
        return NULL;
    }

    colIndexes[0] = 0;

    for (int K = 0; K < pSparse->getRows(); ++K)
    {
        colIndexes[K + 1] = colIndexes[K] + itemsRow[K];
    }

    int* rowIndexes = (int*)MALLOC(sizeof(int) *  nonZeros);
    if (rowIndexes == NULL)
    {
        FREE(sparseData);
        FREE(colIndexes);
        delete[] colPos;
        delete[] itemsRow;
        Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
        return NULL;
    }

    pSparse->getColPos(colPos);
    for (int K = 0; K < nonZeros; ++K)
    {
        rowIndexes[K] = colPos[K] - 1;
    }


    /* Create Matlab Sparse matrix data */
    sparseData->nzmax = nonZeros;
    sparseData->nir = nonZeros;
    sparseData->ir = rowIndexes;
    /* sparseData->njc   = scilabSparse.nel + 1; */
    sparseData->njc = pSparse->getRows() + 1;
    sparseData->jc = colIndexes;
    sparseData->ndata = nonZeros;
    /* get position data*/
    int* iPositVal = new int[nonZeros];

    int idx = 0;
    for (int i = 0; i < pSparse->getRows(); i++)
    {
        for (int j = 0; j < itemsRow[i]; j++)
        {
            iPositVal[idx] = (colPos[idx] - 1) * pSparse->getRows() + i;
            ++idx;
        }
    }

    psize_t = (size_t*)MALLOC(Dims * sizeof(size_t));
    if (psize_t == NULL)
    {
        FREE(sparseData);
        FREE(rowIndexes);
        FREE(colIndexes);
        delete[] itemsRow;
        delete[] colPos;
        delete[] iPositVal;
        Scierror(999, _("%s: No more memory.\n"), "GetSparseVariable");
        return NULL;
    }
    psize_t[0] = (int)pDims[1];
    psize_t[1] = (int)pDims[0];

    if (pSparse->isComplex())
    {
        struct mat_complex_split_t* data;
        double* dataReal = NULL;
        double* dataImg = NULL;
        if ((dataReal = (double*)MALLOC(sizeof(double) * nonZeros)) == NULL)
        {
            FREE(psize_t);
            FREE(sparseData);
            FREE(colIndexes);
            FREE(rowIndexes);
            delete[] itemsRow;
            delete[] colPos;
            delete[] iPositVal;
            Scierror(999, _("%s: No more memory.\n"), "GetSparseMatVar");
            return NULL;
        }

        if ((dataImg = (double*)MALLOC(sizeof(double) * nonZeros)) == NULL)
        {
            FREE(dataReal);
            FREE(psize_t);
            FREE(sparseData);
            FREE(colIndexes);
            FREE(rowIndexes);
            delete[] itemsRow;
            delete[] colPos;
            delete[] iPositVal;
            Scierror(999, _("%s: No more memory.\n"), "GetSparseMatVar");
            return NULL;
        }

        if ((data = (mat_complex_split_t*)MALLOC(sizeof(mat_complex_split_t))) == NULL)
        {
            FREE(dataImg);
            FREE(dataReal);
            FREE(psize_t);
            FREE(sparseData);
            FREE(colIndexes);
            FREE(rowIndexes);
            delete[] itemsRow;
            delete[] colPos;
            delete[] iPositVal;
            Scierror(999, _("%s: No more memory.\n"), "GetSparseMatVar");
            return NULL;
        }

        std::complex<double> complexData;
        for (int K = 0; K < nonZeros; ++K)
        {
            complexData = pSparse->getImg(iPositVal[K]);
            dataReal[K] = complexData.real();
            dataImg[K] = (-1 * complexData.imag());

        }
        data->Re = dataReal;
        data->Im = dataImg;

        sparseData->data = (void*)data;

        pMatVarOut = Mat_VarCreate(name, MAT_C_SPARSE, MAT_T_DOUBLE, Dims, psize_t, sparseData, MAT_F_COMPLEX | MAT_F_DONT_COPY_DATA);
    }
    else
    {
        double* data = NULL;

        if ((data = (double*)MALLOC(sizeof(double) * nonZeros)) == NULL)
        {
            FREE(psize_t);
            FREE(sparseData);
            FREE(colIndexes);
            FREE(rowIndexes);
            delete[] itemsRow;
            delete[] colPos;
            delete[] iPositVal;
            Scierror(999, _("%s: No more memory.\n"), "GetSparseMatVar");
            return NULL;
        }

        for (int K = 0; K < nonZeros; ++K)
        {
            data[K] = pSparse->getReal(iPositVal[K]);
        }


        sparseData->data = (void*)data;

        pMatVarOut = Mat_VarCreate(name, MAT_C_SPARSE, MAT_T_DOUBLE, Dims, psize_t, sparseData, 0 | MAT_F_DONT_COPY_DATA);
    }

    FREE(psize_t);
    delete[] iPositVal;
    delete[] colPos;
    delete[] itemsRow;
    return pMatVarOut;
}
