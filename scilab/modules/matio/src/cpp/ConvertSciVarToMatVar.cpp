/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

#include "ConvertSciVarToMatVar.hxx"
#include "GetMatlabVariable.hxx"

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
}

matvar_t *ConvertSciVarToMatVar(types::InternalType* pIT, const char *name, int matfile_version)
{
    switch (pIT->getType())
    {
        case types::GenericType::ScilabDouble:
        {
            return GetDoubleMatVar(pIT->getAs<types::Double>(), name, matfile_version);
        }
        break;
        case types::InternalType::ScilabInt8:
        case types::InternalType::ScilabUInt8:
        case types::InternalType::ScilabInt16:
        case types::InternalType::ScilabUInt16:
        case types::InternalType::ScilabInt32:
        case types::InternalType::ScilabUInt32:
#ifdef __SCILAB_INT64__
        case types::InternalType::ScilabInt64:
        case types::InternalType::ScilabUInt64:
#endif
        {
            return GetIntegerMatVar(pIT, name);
        }
        break;
        case types::InternalType::ScilabString:
        {
            return GetCharMatVar(pIT->getAs<types::String>(), name);
        }
        break;
        case types::InternalType::ScilabSparse:
        {
            return GetSparseMatVar(pIT->getAs<types::Sparse>(), name);
        }
        break;
        case types::InternalType::ScilabCell:
        {
            return GetCellMatVar(pIT->getAs<types::Cell>(), name, matfile_version);
        }
        break;
        case types::InternalType::ScilabStruct:
        {
            return GetStructMatVar(pIT->getAs<types::Struct>(), name, matfile_version);
        }
        break;
        case types::InternalType::ScilabMList:
        {
            return GetMListMatVar(pIT->getAs<types::MList>(), name, matfile_version);
        }
        break;
        default:
            break;
    }
    Scierror(999, "Do not known how to get variable of type %d\n", pIT->getType());
    return NULL;
}
