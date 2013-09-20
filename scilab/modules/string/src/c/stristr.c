/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <ctype.h>
#include <string.h>
#include "stristr.h"
#include "stricmp.h"
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
