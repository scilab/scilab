/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
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

/*---------------------------------------------------------------------------*/
#include "dynamiclibrary.h"
#include "dynamic_link.h"
#include "dl_genErrorMessage.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
/*---------------------------------------------------------------------------*/
void dl_genErrorMessage(const char* _pstCallerName, int _iErr, const char * _pstLibraryName)
{
    switch (_iErr)
    {
        case -1:
            Scierror(236, _("%s: The shared archive was not loaded: %s\n"), _pstCallerName, GetLastDynLibError());
            break;
        case -2:
            //never occurs now
            break;
        case -3:
            Scierror(999, _("%s: Shared lib %s does not exist.\n"), _pstCallerName, _pstLibraryName);
            break;
        case -4:
            Scierror(999, _("%s: Already loaded from library %s\n"), _pstCallerName, _pstLibraryName);
            break;
        case -5:
            Scierror(235, _("%s: problem with one of the entry point.\n"), _pstCallerName);
            break;
        default:
            Scierror(999, _("%s: An error occurred: %s\n"), _pstCallerName, GetLastDynLibError());
            break;
    }
}
/*---------------------------------------------------------------------------*/
