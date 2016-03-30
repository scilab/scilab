/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Antoine ELIAS
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
