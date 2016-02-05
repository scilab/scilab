/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include "banner.hxx"
#include "scilabWrite.hxx"
#include "localization.hxx"

extern "C"
{
#include <string.h>
#include <math.h>
#include "version.h"
#include "charEncoding.h"
}
/*--------------------------------------------------------------------------*/
static const wchar_t *line = L"        ___________________________________________        ";
/*--------------------------------------------------------------------------*/
static void centerPrint(const wchar_t *str);

void banner(void)
{
    scilabForcedWriteW(line);
    scilabForcedWriteW(L"\n");

    centerPrint(SCI_VERSION_WIDE_STRING);
    scilabForcedWriteW(L"\n\n");

    centerPrint(_W("Scilab Enterprises\n").c_str());
    centerPrint(_W("Copyright (c) 2011-2016 (Scilab Enterprises)\n").c_str());
    centerPrint(_W("Copyright (c) 1989-2012 (INRIA)\n").c_str());
    centerPrint(_W("Copyright (c) 1989-2007 (ENPC)\n").c_str());

    scilabForcedWriteW(line);
    scilabForcedWriteW(L"\n");

    scilabForcedWriteW(L"\n");
    centerPrint(L"-*- This is the future version 6 of Scilab. Only for testing -*-");
    scilabForcedWriteW(L"\n");

    scilabForcedWriteW(line);
    scilabForcedWriteW(L"\n");
}

/*--------------------------------------------------------------------------*/
static void centerPrint(const wchar_t *str)
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
