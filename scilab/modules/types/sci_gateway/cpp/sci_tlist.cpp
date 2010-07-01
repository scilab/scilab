/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#include "alltypes.hxx"
#include "funcmanager.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

using namespace types;

Function::ReturnValue sci_tlist(typed_list &in, int _piRetCount, typed_list &out)
{
    TList* pRetVal = NULL;

    //check input parameters
    if(in.size() < 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: At least %d expected.\n"), "tlist" ,1);        
        return Function::Error;
    }

    if(in[0]->getType() != InternalType::RealString)
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), "tlist", 1);
        return Function::Error;
    }

    //check uniqueness of fields name
    String* pS = in[0]->getAsString();

    //first string is the tlist type
    list<string> fieldNames;
    for(int i = 1 ; i < pS->size_get() ; i++)
    {
        list<string>::iterator it;
        for(it = fieldNames.begin() ; it != fieldNames.end() ; it++)
        {
            if(*it == string(pS->string_get(i)))
            {
                Scierror(999, _("%s : Fields names must be unique"), "tlist");
                return Function::Error;
            }
        }
        fieldNames.push_back(pS->string_get(i));
    }

    pRetVal = new TList();
    for(int i = 0 ; i < in.size() ; i++)
    {
        pRetVal->append(in[i]);
    }

    //fill empty field with []
    while(pRetVal->size_get() < pS->size_get())
    {
        pRetVal->append(new Double(0,0));
    }

    out.push_back(pRetVal);
    return Function::OK;
}
