/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
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
#include "ConsoleRead.hxx"

/*--------------------------------------------------------------------------*/
#include "CallScilabBridge.hxx"

#include "os_string.h"

using namespace  org_scilab_modules_gui_bridge;
/*--------------------------------------------------------------------------*/
static char *line = NULL;
/*--------------------------------------------------------------------------*/
char *ConsoleRead(void)
{
    if (line)
    {
        delete line;
        line = NULL;
    }
    line = CallScilabBridge::readLine(getScilabJavaVM());
    return os_strdup(line);
}
/*--------------------------------------------------------------------------*/
