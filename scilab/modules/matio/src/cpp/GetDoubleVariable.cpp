/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
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
