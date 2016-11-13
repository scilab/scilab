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

extern "C"
{
#include <string.h>
#include <math.h>
#include "version.h"
#include "charEncoding.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
static const char *line = "        ___________________________________________        ";
/*--------------------------------------------------------------------------*/
static void centerPrint(const char* str);

void banner(void)
{
    scilabForcedWrite(line);
    scilabForcedWrite("\n");

    centerPrint(SCI_VERSION_STRING);
    scilabForcedWrite("\n\n");

    centerPrint(_("Scilab Enterprises\n"));
    centerPrint(_("Copyright (c) 2011-2016 (Scilab Enterprises)\n"));
    centerPrint(_("Copyright (c) 1989-2012 (INRIA)\n"));
    centerPrint(_("Copyright (c) 1989-2007 (ENPC)\n"));

    scilabForcedWrite(line);
    scilabForcedWrite("\n");

    scilabForcedWrite("\n");
    centerPrint("-*- This is the future version 6 of Scilab. Only for testing -*-");
    scilabForcedWrite("\n");

    scilabForcedWrite(line);
    scilabForcedWrite("\n");
}

/*--------------------------------------------------------------------------*/
static void centerPrint(const char *str)
{
    int i = 0;
    int startVersion = (int)(floor((double)(strlen(line) / 2)) - floor((double)(strlen(str) / 2)));

    /* To center the version name */
    for (i = 0 ; i < startVersion ; i++ )
    {
        scilabForcedWrite(" ");
    }
    scilabForcedWrite(str);
}
