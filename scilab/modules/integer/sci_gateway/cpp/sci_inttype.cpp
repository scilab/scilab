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

#include "alltypes.hxx"
#include "api_scilab.h"

using namespace types;

Function::ReturnValue sci_inttype(typed_list &in, int _piRetCount, typed_list &out)
{
    /* First check if there is only 1 input of type int */
    if (in.size() != 1)
    {
        return Function::Error;
    }

    InternalType *pitValue = in[0];

    if (!pitValue->isInt())
    {
        return Function::Error;
    }

    Double *pRetVal = NULL;
    switch(pitValue->getAsInt()->getIntType())
    {
    case Int::Type8 :
        pRetVal = new Double(SCI_INT8);
        break;
    case Int::TypeUnsigned8 :
        pRetVal = new Double(SCI_UINT8);
        break;
    case Int::Type16 :
        pRetVal = new Double(SCI_INT16);
        break;
    case Int::TypeUnsigned16 :
        pRetVal = new Double(SCI_UINT16);
        break;
    case Int::Type32 :
        pRetVal = new Double(SCI_INT32);
        break;
    case Int::TypeUnsigned32 :
        pRetVal = new Double(SCI_INT32);
        break;
    case Int::Type64 :
        pRetVal = new Double(SCI_INT64);
        break;
    case Int::TypeUnsigned64 :
        pRetVal = new Double(SCI_UINT64);
        break;
    }

    out.push_back(pRetVal);
    
    return Function::OK;
}
