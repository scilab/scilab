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

#include "CreateMatlabTreeVariable.hxx"

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
}

using namespace types;

matvar_t *ConvertSciVarToMatVar(InternalType* pIT, const char *name)
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
            Double* pDouble = pIT->getAs<Double>();
            if (pDouble->isComplex())
            {
                mat5ComplexData.Re = pDouble->get();
                mat5ComplexData.Im = pDouble->getImg();
                return Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, Dims, psize_t, &mat5ComplexData, MAT_F_COMPLEX);
            }
            else
            {
                return Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, Dims, psize_t, pDouble->get(), 0);
            }
        }
        break;
        case GenericType::ScilabInt8:
        {
            Int8* pInt8 = pIT->getAs<Int8>();
            return Mat_VarCreate(name, MAT_C_INT8, MAT_T_INT8, Dims, psize_t, pInt8->get(), 0);
        }
        break;
        case GenericType::ScilabUInt8:
        {
            UInt8* pUInt8 = pIT->getAs<UInt8>();
            return Mat_VarCreate(name, MAT_C_INT8, MAT_T_INT8, Dims, psize_t, pUInt8->get(), 0);
        }
        break;
        case GenericType::ScilabInt16:
        {
            Int16* pInt16 = pIT->getAs<Int16>();
            return Mat_VarCreate(name, MAT_C_INT16, MAT_T_INT16, Dims, psize_t, pInt16->get(), 0);
        }
        break;
        case GenericType::ScilabUInt16:
        {
            UInt16* pUInt16 = pIT->getAs<UInt16>();
            return Mat_VarCreate(name, MAT_C_INT16, MAT_T_INT16, Dims, psize_t, pUInt16->get(), 0);
        }
        break;
        case GenericType::ScilabInt32:
        {
            Int32* pInt32 = pIT->getAs<Int32>();
            return Mat_VarCreate(name, MAT_C_INT32, MAT_T_INT32, Dims, psize_t, pInt32->get(), 0);
        }
        break;
        case GenericType::ScilabUInt32:
        {
            UInt32* pUInt32 = pIT->getAs<UInt32>();
            return Mat_VarCreate(name, MAT_C_INT32, MAT_T_INT32, Dims, psize_t, pUInt32->get(), 0);
        }
        break;
#ifdef __SCILAB_INT64__
        case GenericType::ScilabInt64:
        {
            Int64* pInt64 = pIT->getAs<Int64>();
            return Mat_VarCreate(name, MAT_C_INT64, MAT_T_INT64, Dims, psize_t, pInt64->get(), 0);
        }
        break;
        case GenericType::ScilabUInt64:
        {
            UInt64* pUInt64 = pIT->getAs<UInt64>();
            return Mat_VarCreate(name, MAT_C_INT64, MAT_T_INT64, Dims, psize_t, pUInt64->get(), 0);
        }
#endif
        case GenericType::ScilabString:
        {
            String* pUInt32 = pIT->getAs<String>();
            return Mat_VarCreate(name, MAT_C_CHAR, MAT_T_UINT8, Dims, psize_t, pUInt32->get(), 0);
        }
        break;
        case GenericType::ScilabSparse:
        {
            Sparse* pSparse = pIT->getAs<Sparse>();
            if (pSparse->isComplex())
            {
                double* pdReal = new double[isize];
                double* pdImg = new double[isize];
                std::complex<double> dbl;
                for (int i = 0; i < isize; i++)
                {
                    dbl = pSparse->getImg(i);
                    pdReal[i] = dbl.real();
                    pdImg[i] = dbl.imag();
                }
                mat5ComplexData.Re = pdReal;
                mat5ComplexData.Im = pdImg;
                return Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, Dims, psize_t, &mat5ComplexData, MAT_F_COMPLEX);
            }
            else
            {
                double* pdReal = new double[isize];
                for (int i = 0; i < isize; i++)
                {
                    pdReal[i] = pSparse->get(i);
                }
                return Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, Dims, psize_t, pdReal, 0);
            }
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
                cellEntries[K] = ConvertSciVarToMatVar(ppIT[K], name);
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
                    structEntries[i * isizeFieldNames + j] = ConvertSciVarToMatVar(ppSingleStruct[i]->get(pFieldNames->get(j)), wide_string_to_UTF8(pFieldNames->get(j)));
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
