/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008-2010 - DIGITEO - Allan CORNET
* Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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
#ifndef __LANGUAGEPREFERENCES_WINDOWS_H__
#define __LANGUAGEPREFERENCES_WINDOWS_H__

#include "BOOL.h"

/**
* Get default language saved in registry
* @return language
*/
wchar_t *getLanguagePreferences(void);

/**
* Set default language saved in registry
* @return TRUE or FALSE
*/
BOOL setLanguagePreferences(void);

/**
* Set language from Command line (wscilex.exe -l lang)
* @param[in] language string
* @return TRUE or FALSE
*/
BOOL setLanguageFromCommandLine(wchar_t *lang);

/**
* check if it is valid language format
* xx_XX
* @param[in] language string
* @return TRUE or FALSE
*/
BOOL isValidLanguage(wchar_t *lang);

#endif /* __LANGUAGEPREFERENCES_WINDOWS_H__ */
/*--------------------------------------------------------------------------*/
