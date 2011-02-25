/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "fullpath.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
}
/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_fullpath(typed_list &in, int _iRetCount, typed_list &out)
{
    if(in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d expected.\n"), L"fullpath" , 1);
        return Function::Error;
    }

    if(_iRetCount != 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"fullpath", 1);
        return Function::Error;
    }

    if(in[0]->isString() == false || in[0]->getAs<String>()->getSize() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A String expected.\n"), L"fullpath", 1);
        return Function::Error;
    }

    wchar_t *relPath = in[0]->getAs<String>()->get(0);
    wchar_t fullpath[PATH_MAX*4];

    if( get_full_pathW( fullpath, relPath, PATH_MAX*4 ) != NULL )
    {
        out.push_back(new String(fullpath));
        return Function::OK;
    }
    else
    {
        ScierrorW(999,_W("%ls: Wrong value for input argument #%d: '%ls' is an invalid path.\n"), L"fullpath", 1 , relPath);
        return Function::Error;
    }
}
/*--------------------------------------------------------------------------*/
