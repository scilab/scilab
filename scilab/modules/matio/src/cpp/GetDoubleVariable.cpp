/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "GetMatlabVariable.hxx"

extern "C"
{
#include "sci_types.h"
#include "api_scilab.h"
}

matvar_t * GetDoubleVariable(void *pvApiCtx, int iVar, const char* name, int matfile_version, int * parent, int item_position)
{
    types::GatewayStruct* pStr = (types::GatewayStruct*)pvApiCtx;
    types::typed_list in = *pStr->m_pIn;

    if (in[iVar - 1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for first input argument: Double matrix expected.\n"), "GetDoubleVariable");
        return NULL;
    }

    types::Double* pDbl = in[iVar - 1]->getAs<types::Double>();

    return GetDoubleMatVar(pDbl, name, matfile_version);
}

matvar_t * GetDoubleMatVar(types::Double* pDblIn, const char* name, int matfile_version)
{
    int Dims = pDblIn->getDims();
    int* pDims = pDblIn->getDimsArray();
    size_t* psize_t = (size_t*)MALLOC(Dims * sizeof(size_t));
    matvar_t * pMatVarOut = NULL;

    for (int i = 0; i < Dims; i++)
    {
        psize_t[i] = (int)pDims[i];
    }


    if (pDblIn->isComplex())
    {
        if (matfile_version == MAT_FT_MAT4) /* MATLAB4: data is a table of value */
        {
            pMatVarOut = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, Dims, psize_t, pDblIn->get(), MAT_F_COMPLEX);
        }
        else
        {
            struct mat_complex_split_t mat5ComplexData;
            mat5ComplexData.Re = pDblIn->get();
            mat5ComplexData.Im = pDblIn->getImg();
            pMatVarOut = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, Dims, psize_t, &mat5ComplexData, MAT_F_COMPLEX);
        }
    }
    else
    {
        pMatVarOut = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, Dims, psize_t, pDblIn->get(), 0);
    }

    FREE(psize_t);
    return pMatVarOut;
}
