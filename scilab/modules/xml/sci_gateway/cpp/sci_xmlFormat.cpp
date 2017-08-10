/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
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

extern "C"
{
#include <string.h>
#include "gw_xml.h"
#include "api_scilab.h"
}

/*--------------------------------------------------------------------------*/
int sci_xmlFormat(char *fname, void* pvApiCtx)
{
    CheckRhs(1, 1);
    CheckLhs(1, 1);

    OverLoad(1);

    return 0;
}
