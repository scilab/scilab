/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#include <stdlib.h>

#include "getScilabPreference.h"
#include "os_string.h"
#include "scicurdir.h"
#include "sci_malloc.h"

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
