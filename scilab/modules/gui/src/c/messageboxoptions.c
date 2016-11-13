/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (java version)
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

#include "messageboxoptions.h"
#include "os_string.h"

/*--------------------------------------------------------------------------*/

int isModalOption(char *option)
{
    return (!os_stricmp(option, "modal") || !os_stricmp(option, "non-modal"));
}

/*--------------------------------------------------------------------------*/

int isIconName(char *name)
{
    return (os_stricmp(name, "scilab")
            || os_stricmp(name, "error")
            || os_stricmp(name, "hourglass")
            || os_stricmp(name, "info")
            || os_stricmp(name, "passwd")
            || os_stricmp(name, "question")
            || os_stricmp(name, "warning"));
}

/*--------------------------------------------------------------------------*/
