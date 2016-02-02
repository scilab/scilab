/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009-2010 - Vincent COUVERT <vincent.couvert@scilab.org>
 * Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include "Xcos.hxx"
#include "GiwsException.hxx"
#include "loadStatus.hxx"

extern "C"
{
#include "gw_xcos.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "getScilabJavaVM.h"
#include "configvariable_interface.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos;

/*--------------------------------------------------------------------------*/
int sci_closeXcosFromScilab(char *fname, void* pvApiCtx)
{
    CheckRhs(0, 0);
    CheckLhs(0, 1);

    // only if xcos was already opened and with supported mode
    if ((getScilabMode() != SCILAB_NWNI) && get_loaded_status() == XCOS_CALLED)
    {
        try
        {
            Xcos::closeXcosFromScilab(getScilabJavaVM());
        }
        catch (GiwsException::JniCallMethodException &exception)
        {
            Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
            return 0;
        }
        catch (GiwsException::JniException &exception)
        {
            Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
            return 0;
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
