/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConvertSciVarToMatVar.hxx"
#include "GetMatlabVariable.hxx"

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
}

using namespace types;

matvar_t *ConvertSciVarToMatVar(InternalType* pIT, const char *name, int matfile_version)
{
    int Dims = pIT->getAs<GenericType>()->getDims();
    int* pDims = pIT->getAs<GenericType>()->getDimsArray();
    int isize = pIT->getAs<GenericType>()->getSize();
    size_t* psize_t = (size_t*)MALLOC(Dims * sizeof(size_t));
    struct mat_complex_split_t mat5ComplexData;

    for (int i = 0; i < Dims; i++)
    {
        psize_t[i] = (int)pDims[i];
    }

    switch (pIT->getType())
    {
        case GenericType::ScilabDouble:
        {
            return GetDoubleMatVar(pIT->getAs<Double>(), name, matfile_version);
        }
        break;
        case GenericType::ScilabInt8:
        case GenericType::ScilabUInt8:
        case GenericType::ScilabInt16:
        case GenericType::ScilabUInt16:
        case GenericType::ScilabInt32:
        case GenericType::ScilabUInt32:
#ifdef __SCILAB_INT64__
        case GenericType::ScilabInt64:
        case GenericType::ScilabUInt64:
#endif
        {
            return GetIntegerMatVar(pIT, name);
        }
        break;
        case GenericType::ScilabString:
        {
            return GetCharMatVar(pIT->getAs<String>(), name);
        }
        break;
        case GenericType::ScilabSparse:
        {
            return GetSparseMatVar(pIT->getAs<Sparse>(), name);
        }
        break;
        case GenericType::ScilabCell:
        {
            matvar_t **cellEntries = NULL;

            cellEntries = (matvar_t **)MALLOC(sizeof(matvar_t*) * isize);
            if (cellEntries == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "ConvertSciVarToMatVar");
                return NULL;
            }

            types::InternalType** ppIT = pIT->getAs<Cell>()->get();

            for (int K = 0; K < isize; K++)
            {
                cellEntries[K] = ConvertSciVarToMatVar(ppIT[K], name, matfile_version);
                if (cellEntries[K] == NULL)
                {
                    FREE(cellEntries);
                    FREE(psize_t);
                    return NULL;
                }
            }

            return Mat_VarCreate(name, MAT_C_CELL, MAT_T_CELL, Dims, psize_t, cellEntries, 0);
        }
        break;
        case GenericType::ScilabStruct:
        {
            matvar_t **structEntries = NULL;
            String* pFieldNames = pIT->getAs<Struct>()->getFieldNames();
            wchar_t** ppwchFieldNames = pFieldNames->get();
            int isizeFieldNames = pFieldNames->getSize();

            structEntries = (matvar_t **)MALLOC(sizeof(matvar_t*) * isize * isizeFieldNames + 1);
            if (structEntries == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "ConvertSciVarToMatVar");
                return NULL;
            }

            for (int K = 0; K < isize * isizeFieldNames + 1; K++)
            {
                structEntries[K] = NULL;
            }

            SingleStruct** ppSingleStruct = pIT->getAs<Struct>()->get();
            for (int i = 0; i < isize; i++)
            {
                for (int j = 0; j < isizeFieldNames; j++)
                {
                    structEntries[i * isizeFieldNames + j] = ConvertSciVarToMatVar(ppSingleStruct[i]->get(pFieldNames->get(j)), wide_string_to_UTF8(pFieldNames->get(j)), matfile_version);
                    if (structEntries[i * isizeFieldNames + j] == NULL)
                    {
                        FREE(structEntries);
                        FREE(psize_t);
                        return NULL;
                    }
                }
            }

            return Mat_VarCreate(name, MAT_C_STRUCT, MAT_T_STRUCT, isize * isizeFieldNames, psize_t, structEntries, 0);
        }
        break;
        default:
            break;
    }
    Scierror(999, "Do not known how to get variable of type %d\n", pIT->getType());
    return NULL;
}
