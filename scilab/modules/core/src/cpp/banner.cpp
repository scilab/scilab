/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "banner.hxx"
#include "scilabWrite.hxx"

extern "C"
{
#include <string.h>
#include <math.h>
#include "version.h"
#include "localization.h"
#include "charEncoding.h"
}
/*--------------------------------------------------------------------------*/
static wchar_t *line = L"        ___________________________________________        ";
/*--------------------------------------------------------------------------*/
static void centerPrint(wchar_t *str);

void banner(void)
{
    scilabForcedWriteW(line);
    scilabForcedWriteW(L"\n");

    centerPrint(SCI_VERSION_WIDE_STRING);
    scilabForcedWriteW(L"\n\n");

    centerPrint(_W("Scilab Enterprises\n"));
    centerPrint(_W("Consortium Scilab (DIGITEO)\n"));
    centerPrint(_W("Copyright (c) 2011-2012 (Scilab Enterprises)\n"));
    centerPrint(_W("Copyright (c) 1989-2012 (INRIA)\n"));
    centerPrint(_W("Copyright (c) 1989-2007 (ENPC)\n"));

    scilabForcedWriteW(line);
    scilabForcedWriteW(L"\n");

    scilabForcedWriteW(L"\n");
    centerPrint(L"-*- This is the future version 6 of Scilab. Only for testing -*-");
    scilabForcedWriteW(L"\n");

    scilabForcedWriteW(line);
    scilabForcedWriteW(L"\n");
}

/*--------------------------------------------------------------------------*/
static void centerPrint(wchar_t *str)
{
    int i = 0;
    int startVersion = (int)(floor((double)(wcslen(line) / 2)) - floor((double)(wcslen(str) / 2)));

    /* To center the version name */
    for (i = 0 ; i < startVersion ; i++ )
    {
        scilabForcedWriteW(L" ");
    }
    scilabForcedWriteW(str);
}
