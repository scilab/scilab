/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#include <ctype.h>
#include <string.h>
#include "stristr.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
char *stristr(const char *psz, const char *tofind)
{
    const char *ptr = psz;
    const char *ptr2;

    if (strlen(tofind) > 0)
    {
        while (1)
        {
            ptr = strchr(psz, toupper(*tofind));
            ptr2 = strchr(psz, tolower(*tofind));

            if (!ptr)
            {
                ptr = ptr2; /* was ptr2 = ptr.  Big bug fixed 10/22/99 */
            }

            if (!ptr)
            {
                break;
            }

            if (ptr2 && (ptr2 < ptr))
            {
                ptr = ptr2;
            }

            if (!strnicmp(ptr, tofind, strlen(tofind)))
            {
                return (char *) ptr;
            }

            psz = ptr + 1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
