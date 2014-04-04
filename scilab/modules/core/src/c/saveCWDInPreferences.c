/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <stdlib.h>

#include "getScilabPreference.h"
#include "stricmp.h"
#include "scicurdir.h"
#include "MALLOC.h"

void saveCWDInPreferences()
{
    const char * keys[] = {"use", "previous"};
    char ** values = getPrefAttributesValues("//general/body/startup", keys, sizeof(keys) / sizeof(const char *));

    if (values)
    {
        if (values[0] && stricmp(values[0], "previous") == 0)
        {
            int err;
            char * cwd = scigetcwd(&err);

            if (!err && cwd && values[1] && strcmp(cwd, values[1]))
            {
                const char * kv[] = {"previous", cwd};
                setPrefAttributesValues("//general/body/startup", kv, sizeof(kv) / sizeof(const char *));
            }

            if (cwd)
            {
                FREE(cwd);
            }
        }
        if (values[0])
        {
            FREE(values[0]);
        }
        if (values[1])
        {
            FREE(values[1]);
        }
        FREE(values);
    }
}
