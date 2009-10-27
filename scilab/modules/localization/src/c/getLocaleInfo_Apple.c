/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#include <CoreFoundation/CoreFoundation.h>

char *getLocaleUserInfo(void)
{
  char *cUserLanguage = NULL;
  CFLocaleRef userLocaleRef = CFLocaleCopyCurrent();
  CFStringRef userLanguage = CFLocaleGetIdentifier(userLocaleRef);

  cUserLanguage = malloc(((int) CFStringgetLength(userLanguage)) * sizeof(char) + 1);
  CFStringGetCString(userLanguage, cUserLanguage, sizeof(cUserLanguage), kCFStringEncodinfUTF8);

  return cUserLanguage;
}
