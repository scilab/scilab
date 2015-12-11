/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>

#include "CoverModule.hxx"

#include "coverage_gw.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
#include "configvariable.hxx"
#include "context.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_covMerge(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "covMerge" , 2);
        return types::Function::Error;
    }

    if (!in[0]->isString())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of strings expected.\n"), "covWrite" , 1);
        return types::Function::Error;
    }

    if (!in[1]->isString() || in[1]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar string expected.\n"), "covWrite" , 2);
        return types::Function::Error;
    }    

    types::String * strs = in[0]->getAs<types::String>();
    const unsigned int size = strs->getSize();
    std::vector<std::wstring> paths;
    paths.reserve(size);
    
    for (unsigned int i = 0; i < size; ++i)
    {
	paths.emplace_back(strs->get(i));
    }

    coverage::CoverModule::merge(paths, in[1]->getAs<types::String>()->get(0));
    
    return types::Function::OK;
}
