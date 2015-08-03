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

#include <string.h>
#include <stdlib.h>

#include "gatewaystruct.hxx"
#include "double.hxx"
#include "int.hxx"
#include "context.hxx"

extern "C"
{
#include "CreateMatlabVariable.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
}

using namespace types;

int CreateHyperMatrixVariable(void *pvApiCtx, int iVar, int type, int *iscomplex, int * rank, int *dims, matvar_t *matVariable, int * parent, int item_position)
{
    GatewayStruct* pStr = (GatewayStruct*)pvApiCtx;
    typed_list in = *pStr->m_pIn;
    InternalType** out = pStr->m_pOut;

    int rhs = iVar - *getNbInputArgument(pvApiCtx);

    switch (type)
    {
        case MAT_C_DOUBLE: /* 6 */
        case MAT_C_SINGLE: /* 7 */
        {
            Double* pDbl = new Double(rank[0], dims, (bool)(iscomplex[0] != 0));
            if (iscomplex[0])
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

            out[rhs - 1] = pDbl;
        }
        break;
        case MAT_C_INT8: /* 8 */
        {
            Int8* pInt8 = new Int8(rank[0], dims);

            pInt8->set((char*)matVariable->data);

            out[rhs - 1] = pInt8;
        }
        break;
        case MAT_C_UINT8: /* 9 */
        {
            UInt8* pUInt8 = new UInt8(rank[0], dims);

            pUInt8->set((unsigned char*)matVariable->data);

            out[rhs - 1] = pUInt8;
        }
        break;
        case MAT_C_INT16: /* 10 */
        {
            Int16* pInt16 = new Int16(rank[0], dims);

            pInt16->set((short*)matVariable->data);

            out[rhs - 1] = pInt16;
        }
        break;
        case MAT_C_UINT16: /* 11 */
        {
            UInt16* pUInt16 = new UInt16(rank[0], dims);

            pUInt16->set((unsigned short*)matVariable->data);

            out[rhs - 1] = pUInt16;
        }
        break;
        case MAT_C_INT32: /* 12 */
        {
            Int32* pInt32 = new Int32(rank[0], dims);

            pInt32->set((int*)matVariable->data);

            out[rhs - 1] = pInt32;
        }
        break;
        case MAT_C_UINT32: /* 13 */
        {
            UInt32* pUInt32 = new UInt32(rank[0], dims);

            pUInt32->set((unsigned int*)matVariable->data);

            out[rhs - 1] = pUInt32;
        }
        break;
#ifdef __SCILAB_INT64__
        case MAT_C_INT64: /* 14 */
        {
            Int64* pInt64 = new Int64(rank[0], dims);

            pInt64->set((long long*)matVariable->data);

            out[rhs - 1] = pInt64;
        }
        break;
        case MAT_C_UINT64: /* 15 */
        {
            UInt64* pUInt64 = new UInt64(rank[0], dims);

            pUInt64->set((unsigned long long*)matVariable->data);

            out[rhs - 1] = pUInt64;
        }
        break;
#else
        case MAT_C_INT64: /* 14 */
        case MAT_C_UINT64: /* 15 */
#endif
        case MAT_C_OBJECT: /* 3 to be written */
        case MAT_C_CELL: /* 1 */
        case MAT_C_STRUCT: /* 2 */
        case MAT_C_CHAR: /* 4 */
        case MAT_C_SPARSE: /* 5 */
        case MAT_C_FUNCTION: /* 16 to be written */
        default:
        {
            out[rhs - 1] = Double::Empty();
            return FALSE;
        }
    }

    return TRUE;
}

