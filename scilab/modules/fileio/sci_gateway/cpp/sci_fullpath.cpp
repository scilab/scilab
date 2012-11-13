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
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "fullpath" , 1);
        return Function::Error;
    }

    if(_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "fullpath", 1);
        return Function::Error;
    }

/*
    if(in[0]->isString() == false || in[0]->getAs<String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "fullpath", 1);
        return Function::Error;
    }
*/

    wchar_t fullpath[PATH_MAX*4];
    String* pIn = in[0]->getAs<String>();
    String* pOut = new String(pIn->getDims(), pIn->getDimsArray());

    for(int i = 0 ; i < pIn->getSize() ; i++)
    {
        wchar_t *relPath = pIn->get(i);
        if( get_full_pathW( fullpath, relPath, PATH_MAX*4 ) != NULL )
        {
            pOut->set(i, fullpath);
        }
        else
        {
            pOut->set(i, relPath);
        }
        fullpath[0] = L'\0';
    }

    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
