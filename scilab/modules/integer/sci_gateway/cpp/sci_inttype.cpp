/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

using namespace types;

Function::ReturnValue sci_inttype(typed_list &in, int _piRetCount, typed_list &out)
{
    /* First check if there is only 1 input of type int */
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "inttype", 1);
        return Function::Error;
    }

    InternalType *pitValue = in[0];

    if (pitValue->isInt() == false && pitValue->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : int or double expected.\n"), "inttype", 1);
        return Function::Error;
    }

    Double *pRetVal = NULL;
    switch (pitValue->getType())
    {
        case InternalType::RealDouble :
            pRetVal = new Double(0);
            break;
        case InternalType::RealInt8 :
            pRetVal = new Double(SCI_INT8);
            break;
        case InternalType::RealUInt8 :
            pRetVal = new Double(SCI_UINT8);
            break;
        case InternalType::RealInt16 :
            pRetVal = new Double(SCI_INT16);
            break;
        case InternalType::RealUInt16 :
            pRetVal = new Double(SCI_UINT16);
            break;
        case InternalType::RealInt32 :
            pRetVal = new Double(SCI_INT32);
            break;
        case InternalType::RealUInt32 :
            pRetVal = new Double(SCI_UINT32);
            break;
        case InternalType::RealInt64 :
            pRetVal = new Double(SCI_INT64);
            break;
        case InternalType::RealUInt64 :
            pRetVal = new Double(SCI_UINT64);
            break;
    }

    out.push_back(pRetVal);

    return Function::OK;
}
