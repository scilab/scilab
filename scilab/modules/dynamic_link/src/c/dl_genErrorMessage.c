/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*---------------------------------------------------------------------------*/
#include "dynamiclibrary.h"
#include "dynamic_link.h"
#include "dl_genErrorMessage.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
/*---------------------------------------------------------------------------*/
void dl_genErrorMessage(wchar_t* _pwstCallerName, int _iErr, wchar_t* _pwstLibraryName)
{
    char* pstCaller = wide_string_to_UTF8(_pwstCallerName);
    char* pstLibName = wide_string_to_UTF8(_pwstLibraryName);
    switch (_iErr)
    {
        case -1:
            Scierror(236, _("%s: The shared archive was not loaded: %s\n"), pstCaller, GetLastDynLibError());
            break;
        case -2:
            //never occurs now
            break;
        case -3:
            Scierror(999, _("%s: Shared lib %s does not exist.\n") , pstCaller, pstLibName);
            break;
        case -4:
            Scierror(999, _("%s: Already loaded from library %s\n"), pstCaller, pstLibName);
            break;
        case -5:
            Scierror(235, _("%s: problem with one of the entry point.\n"), pstCaller);
            break;
        default:
            Scierror(999, _("%s: An error occurred: %s\n"), pstCaller, GetLastDynLibError());
            break;
    }

    FREE(pstLibName);
    FREE(pstCaller);
}
/*---------------------------------------------------------------------------*/
