/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*---------------------------------------------------------------------------*/
#include "dynamiclibrary.h"
#include "dynamic_link.h"
#include "dl_genErrorMessage.h"
#include "Scierror.h"
#include "localization.h"
/*---------------------------------------------------------------------------*/
void dl_genErrorMessage(wchar_t* _pwstCallerName, int _iErr, wchar_t* _pwstLibraryName)
{
    switch (_iErr)
    {
    case -1:
        ScierrorW(236, _W("%ls: The shared archive was not loaded: %ls\n"), _pwstCallerName, GetLastDynLibError());
        break;

    case -2:
        //never occurs now
        break;

    case -3:
        ScierrorW(999, _W("%ls: Shared lib %ls does not exist.\n") ,_pwstCallerName, _pwstLibraryName);
        break;

    case -4:
        ScierrorW(999, _W("%ls: Already loaded from library %ls\n"),_pwstCallerName, _pwstLibraryName);
        break;
    case -5:
        ScierrorW(235, _W("%ls: problem with one of the entry point.\n"), _pwstCallerName, GetLastDynLibError());
        break;
    default:
        ScierrorW(999, _W("%ls: An error occurred: %ls\n"), _pwstCallerName, GetLastDynLibError());
        break;
    }
}
/*---------------------------------------------------------------------------*/
