/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (java version)
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
