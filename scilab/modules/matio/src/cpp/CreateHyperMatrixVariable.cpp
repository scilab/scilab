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

int CreateHyperMatrixVariable(void *pvApiCtx, int iVar, int type, int *iscomplex, int * rank, int *dims, matvar_t *matVariable, int * parent, int item_position)
{
    types::GatewayStruct* pStr = (types::GatewayStruct*)pvApiCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;

    int rhs = iVar - *getNbInputArgument(pvApiCtx);

    switch (type)
    {
        case MAT_C_DOUBLE: /* 6 */
        case MAT_C_SINGLE: /* 7 */
        {
            types::Double* pDbl = new types::Double(rank[0], dims, (bool)(iscomplex[0] != 0));
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
            types::Int8* pInt8 = new types::Int8(rank[0], dims);

            pInt8->set((char*)matVariable->data);

            out[rhs - 1] = pInt8;
        }
        break;
        case MAT_C_UINT8: /* 9 */
        {
            types::UInt8* pUInt8 = new types::UInt8(rank[0], dims);

            pUInt8->set((unsigned char*)matVariable->data);

            out[rhs - 1] = pUInt8;
        }
        break;
        case MAT_C_INT16: /* 10 */
        {
            types::Int16* pInt16 = new types::Int16(rank[0], dims);

            pInt16->set((short*)matVariable->data);

            out[rhs - 1] = pInt16;
        }
        break;
        case MAT_C_UINT16: /* 11 */
        {
            types::UInt16* pUInt16 = new types::UInt16(rank[0], dims);

            pUInt16->set((unsigned short*)matVariable->data);

            out[rhs - 1] = pUInt16;
        }
        break;
        case MAT_C_INT32: /* 12 */
        {
            types::Int32* pInt32 = new types::Int32(rank[0], dims);

            pInt32->set((int*)matVariable->data);

            out[rhs - 1] = pInt32;
        }
        break;
        case MAT_C_UINT32: /* 13 */
        {
            types::UInt32* pUInt32 = new types::UInt32(rank[0], dims);

            pUInt32->set((unsigned int*)matVariable->data);

            out[rhs - 1] = pUInt32;
        }
        break;
#ifdef __SCILAB_INT64__
        case MAT_C_INT64: /* 14 */
        {
            types::Int64* pInt64 = new types::Int64(rank[0], dims);

            pInt64->set((long long*)matVariable->data);

            out[rhs - 1] = pInt64;
        }
        break;
        case MAT_C_UINT64: /* 15 */
        {
            types::UInt64* pUInt64 = new types::UInt64(rank[0], dims);

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
            out[rhs - 1] = types::Double::Empty();
            return FALSE;
        }
    }

    return TRUE;
}

