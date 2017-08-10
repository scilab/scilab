/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include "integer_gw.hxx"
#include "function.hxx"
#include "double.hxx"

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_inttype(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    /* First check if there is only 1 input of type int */
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "inttype", 1);
        return types::Function::Error;
    }

    types::InternalType *pitValue = in[0];

    if (pitValue->isInt() == false && pitValue->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : int or double expected.\n"), "inttype", 1);
        return types::Function::Error;
    }

    types::Double *pRetVal = NULL;
    switch (pitValue->getType())
    {
        case types::InternalType::ScilabDouble :
            pRetVal = new types::Double(0);
            break;
        case types::InternalType::ScilabInt8 :
            pRetVal = new types::Double(SCI_INT8);
            break;
        case types::InternalType::ScilabUInt8 :
            pRetVal = new types::Double(SCI_UINT8);
            break;
        case types::InternalType::ScilabInt16 :
            pRetVal = new types::Double(SCI_INT16);
            break;
        case types::InternalType::ScilabUInt16 :
            pRetVal = new types::Double(SCI_UINT16);
            break;
        case types::InternalType::ScilabInt32 :
            pRetVal = new types::Double(SCI_INT32);
            break;
        case types::InternalType::ScilabUInt32 :
            pRetVal = new types::Double(SCI_UINT32);
            break;
        case types::InternalType::ScilabInt64 :
            pRetVal = new types::Double(SCI_INT64);
            break;
        case types::InternalType::ScilabUInt64 :
            pRetVal = new types::Double(SCI_UINT64);
            break;
        default:
            pRetVal = NULL;
    }

    out.push_back(pRetVal);

    return types::Function::OK;
}
