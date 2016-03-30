/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
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

#include <CoreFoundation/CoreFoundation.h>
#include <stdlib.h>
#include "getLocaleInfo_Apple.h"
#include "charEncoding.h"

wchar_t *getLocaleUserInfo(void)
{
    char *cUserLanguage = NULL;
    CFLocaleRef userLocaleRef = CFLocaleCopyCurrent();
    CFStringRef userLanguage = CFLocaleGetIdentifier(userLocaleRef);

    if (getenv( "LANG" ))
    {
        /* Mac OS X does not respect the LANG variable. We do it ourself. */
        return to_wide_string(getenv("LANG"));
    }
    else
    {
        cUserLanguage = (char *) malloc(((int) CFStringGetLength(userLanguage) + 1) * sizeof(char));
        CFStringGetCString(userLanguage, cUserLanguage, ((int) CFStringGetLength(userLanguage)) + 1, kCFStringEncodingUTF8);
    }
    return to_wide_string(cUserLanguage);
}
