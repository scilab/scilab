/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be
 * still available and supported in Scilab 6.
 */

extern "C"
{
#include "sci_malloc.h"
#include <stdio.h>
#include "api_scilab.h"
#include "sciprint.h"
#include "Scierror.h"
#include "os_string.h"
#include "localization.h"
#include "configvariable_interface.h"
#include "api_internal_common.h"
}

int addErrorMessage(SciErr* _psciErr, int _iErr, const char* _pstMsg, ...)
{
    int iRet = 0;
    char pstMsg[bsiz];
    va_list ap;

    va_start(ap, _pstMsg);
#if defined (vsnprintf) || defined (linux)
    iRet = vsnprintf(pstMsg, bsiz - 1, _pstMsg, ap );
#else
    iRet = vsprintf(pstMsg, _pstMsg, ap );
#endif
    va_end(ap);

    if (_psciErr->iMsgCount >= MESSAGE_STACK_SIZE)
    {
        // no more space, shift error messages
        FREE(_psciErr->pstMsg[0]);
        for (int i = 1; i < MESSAGE_STACK_SIZE; i++)
        {
            _psciErr->pstMsg[i - 1] = _psciErr->pstMsg[i];
        }
        _psciErr->pstMsg[MESSAGE_STACK_SIZE - 1] = os_strdup(pstMsg);
    }
    else
    {
        _psciErr->pstMsg[_psciErr->iMsgCount++] = os_strdup(pstMsg);
    }

    _psciErr->iErr = _iErr;
    return iRet;
}

int printError(SciErr* _psciErr, int _iLastMsg)
{
    if (_psciErr->iErr == 0)
    {
        return 0;
    }

    SciStoreError(_psciErr->iErr);

    if (isPrintOutput() == 1 && isSilentError() == 0)
    {
        if (_iLastMsg)
        {
            sciprint(_("API Error:\n"));
            sciprint(_("\tin %s\n"), _psciErr->pstMsg[0]);
        }
        else
        {
            sciprint(_("API Error:\n"));

            for (int i = _psciErr->iMsgCount - 1; i >= 0; i--)
            {
                sciprint(_("\tin %s\n"), _psciErr->pstMsg[i]);
            }
        }
    }

    sciErrClean(_psciErr);
    return 0;
}

char* getErrorMessage(SciErr _sciErr)
{
    if (_sciErr.iErr == 0)
    {
        return NULL;
    }

    return _sciErr.pstMsg[0];
}

