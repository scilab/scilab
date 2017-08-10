/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
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

#include "CreateMatlabTreeVariable.hxx"
#include "cell.hxx"
#include "struct.hxx"
#include "string.hxx"
#include "sparse.hxx"
#include "int.hxx"

extern "C"
{
#include "CreateMatlabVariable.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
}

types::InternalType* CreateMatlabTreeVariable(matvar_t *matVariable)
{
    types::InternalType* pOut = NULL;
    /* To be sure isComplex is 0 or 1 */
    bool isComplex = (matVariable->isComplex != 0);
    int* piDims = NULL;

    int iRank = matVariable->rank;
    if (iRank != 0)
    {
        if (iRank == 1)
        {
            iRank = 2;
            piDims = (int *)MALLOC(iRank * sizeof(int));
            piDims[0] = (int)matVariable->dims[0];
            piDims[1] = 1;
        }
        else
        {
            piDims = (int *)MALLOC(iRank * sizeof(int));

            for (int i = 0; i < iRank; ++i)
            {
                piDims[i] = (int)matVariable->dims[i];
            }
        }
    }

    if (!piDims)
    {
        return types::Double::Empty();
    }

    switch (matVariable->class_type)
    {
        case MAT_C_CELL: /* 1 */
        {

            if ((iRank == 2) && ((piDims[0] * piDims[1]) <= 0))
            {
                pOut = new types::Cell();
            }
            else
            {
                pOut = new types::Cell(iRank, piDims);

                matvar_t** allData = (matvar_t**)(matVariable->data);
                int iSize = pOut->getAs<types::Cell>()->getSize();
                types::InternalType** ppIT = new types::InternalType*[iSize];
                for (int i = 0; i < iSize; i++)
                {
                    ppIT[i] = CreateMatlabTreeVariable(allData[i]);
                }

                pOut->getAs<types::Cell>()->set(ppIT);
            }

        }
        break;
        case MAT_C_STRUCT: /* 2 */
        {
            if (matVariable->data == NULL)
            {
                pOut = new types::Struct();
            }
            else
            {
                pOut = new types::Struct(iRank, piDims);
                int iSizeStruct = Mat_VarGetNumberOfFields(matVariable);
                wchar_t* temp;
                matvar_t** allData = (matvar_t**)(matVariable->data);

                bool bSearchSizeStruck = false;
                for (int i = 0; (allData[i] != NULL) && (bSearchSizeStruck == false) && (i < iSizeStruct); i++)
                {
                    for (int j = (i - 1); j >= 0; j--)
                    {
                        if (strcmp((char*)allData[i]->name, (char*)allData[j]->name) == 0)
                        {
                            bSearchSizeStruck = true;
                            break;
                        }
                    }
                    temp = to_wide_string((char*)allData[i]->name);
                    std::wstring wstField(temp);
                    FREE(temp);
                    pOut->getAs<types::Struct>()->addField(wstField);
                }

                for (int i = 0; i < pOut->getAs<types::Struct>()->getSize(); i++)
                {
                    for (int j = 0; j < iSizeStruct; j++)
                    {
                        temp = to_wide_string((char*)allData[j]->name);
                        std::wstring wstField(temp);
                        FREE(temp);
                        pOut->getAs<types::Struct>()->get(i)->set(wstField, CreateMatlabTreeVariable(allData[i * iSizeStruct + j]));
                    }
                }
            }
        }

        break;
        case MAT_C_CHAR: /* 4 */
        {
            char* pChar = (char*)MALLOC(sizeof(char) * (piDims[1] + 1));
            pChar[0] = '\0';
            strncat(pChar, (char*)matVariable->data, piDims[1]);
            types::String* pString = new types::String(pChar);
            pOut = pString;
            FREE(pChar);
        }
        break;
        case MAT_C_SPARSE: /* 5 */
        {
            mat_sparse_t *sparseData = (mat_sparse_t*)matVariable->data;

            types::Sparse* pSparse = new types::Sparse(piDims[0], piDims[1], isComplex);
            int *colIndexes = NULL;
            int *rowIndexes = NULL;

            /* Computes column indexes from Matlab indexes */
            if (sparseData->njc > 1)
            {
                colIndexes = (int*)MALLOC(sizeof(int) *  (sparseData->njc - 1));
                if (colIndexes == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "CreateMatlabTreeVariable");
                    delete pSparse;
                    FREE(piDims);
                    return NULL;
                }

                for (int K = 0; K < sparseData->njc - 1; K++)
                {
                    colIndexes[K] = sparseData->jc[K + 1] - sparseData->jc[K];
                }
            }

            /* Computes row indexes from Matlab indexes */
            rowIndexes = (int*)MALLOC(sizeof(int) * sparseData->nir);
            if (rowIndexes == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "CreateMatlabTreeVariable");
                delete pSparse;
                FREE(colIndexes);
                FREE(piDims);
                return NULL;
            }

            for (int K = 0; K < sparseData->nir; K++)
            {
                rowIndexes[K] = sparseData->ir[K] + 1;
            }


            if (isComplex)
            {

                mat_complex_split_t *mat5ComplexData = NULL;
                mat5ComplexData = (mat_complex_split_t*)sparseData->data;

                double* pReal = (double*)mat5ComplexData->Re;
                double* pIm = (double*)mat5ComplexData->Im;

                int i = 0;
                for (int j = 0; j < (sparseData->njc - 1); j++)
                {
                    for (int k = 0; k < colIndexes[j]; k++)
                    {
                        std::complex<double> dbl(pReal[i], pIm[i]);
                        pSparse->set((rowIndexes[i] - 1), j, dbl);
                        i++;
                    }
                }

            }
            else
            {
                double* pReal = (double*)sparseData->data;
                int i = 0;
                for (int j = 0; j < (sparseData->njc - 1); j++)
                {
                    for (int k = 0; k < colIndexes[j]; k++)
                    {
                        pSparse->set((rowIndexes[i] - 1), j, pReal[i]);
                        i++;
                    }
                }
            }
            pOut = pSparse;
            FREE(rowIndexes);
            FREE(colIndexes);
        }
        break;
        case MAT_C_DOUBLE: /* 6 */
        case MAT_C_SINGLE: /* 7 */
        {
            types::Double* pDbl = new types::Double(iRank, piDims, isComplex);
            if (isComplex)
            {
                mat_complex_split_t *mat5ComplexData = NULL;
                mat5ComplexData = (mat_complex_split_t*)matVariable->data;

                pDbl->set((double*)mat5ComplexData->Re);
                pDbl->setImg((double*)mat5ComplexData->Im);
            }
            else
            {
                pDbl->set((double*)matVariable->data);
            }

            pOut = pDbl;
        }
        break;
        case MAT_C_INT8: /* 8 */
        {
            types::Int8* pInt8 = new types::Int8(iRank, piDims);

            pInt8->set((char*)matVariable->data);

            pOut = pInt8;
        }
        break;
        case MAT_C_UINT8: /* 9 */
        {
            types::UInt8* pUInt8 = new types::UInt8(iRank, piDims);

            pUInt8->set((unsigned char*)matVariable->data);

            pOut = pUInt8;
        }
        break;
        case MAT_C_INT16: /* 10 */
        {
            types::Int16* pInt16 = new types::Int16(iRank, piDims);

            pInt16->set((short*)matVariable->data);

            pOut = pInt16;
        }
        break;
        case MAT_C_UINT16: /* 11 */
        {
            types::UInt16* pUInt16 = new types::UInt16(iRank, piDims);

            pUInt16->set((unsigned short*)matVariable->data);

            pOut = pUInt16;
        }
        break;
        case MAT_C_INT32: /* 12 */
        {
            types::Int32* pInt32 = new types::Int32(iRank, piDims);

            pInt32->set((int*)matVariable->data);

            pOut = pInt32;
        }
        break;
        case MAT_C_UINT32: /* 13 */
        {
            types::UInt32* pUInt32 = new types::UInt32(iRank, piDims);

            pUInt32->set((unsigned int*)matVariable->data);

            pOut = pUInt32;
        }
        break;
#ifdef __SCILAB_INT64__
        case MAT_C_INT64: /* 14 */
        {
            types::Int64* pInt64 = new types::Int64(iRank, piDims);

            pInt64->set((long long*)matVariable->data);

            pOut = pInt64;
        }
        break;
        case MAT_C_UINT64: /* 15 */
        {
            types::UInt64* pUInt64 = new types::UInt64(iRank, piDims);

            pUInt64->set((unsigned long long*)matVariable->data);

            pOut = pUInt64;
        }
        break;
#else
        case MAT_C_INT64: /* 14 */
        case MAT_C_UINT64: /* 15 */
#endif
        case MAT_C_OBJECT: /* 3 to be written */
        case MAT_C_FUNCTION: /* 16 to be written */
        default:
            /* Empty matrix returned */
            FREE(piDims);
            return types::Double::Empty();
    }
    FREE(piDims);
    return pOut;
}
