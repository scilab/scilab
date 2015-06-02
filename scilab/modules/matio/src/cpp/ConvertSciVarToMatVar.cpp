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
            return GetCellMatVar(pIT->getAs<Cell>(), name, matfile_version);
        }
        break;
        case GenericType::ScilabStruct:
        {
            return GetStructMatVar(pIT->getAs<Struct>(), name, matfile_version);
        }
        break;
        case GenericType::ScilabMList:
        {
            return GetMListMatVar(pIT->getAs<MList>(), name, matfile_version);
        }
        break;
        default:
            break;
    }
    Scierror(999, "Do not known how to get variable of type %d\n", pIT->getType());
    return NULL;
}
