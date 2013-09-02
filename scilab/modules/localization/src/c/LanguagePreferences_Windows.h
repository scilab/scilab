/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008-2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
char *getLanguagePreferences(void);

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
BOOL setLanguageFromCommandLine(char *lang);

/**
* check if it is valid language format
* xx_XX
* @param[in] language string
* @return TRUE or FALSE
*/
BOOL isValidLanguage(char *lang);

#endif /* __LANGUAGEPREFERENCES_WINDOWS_H__ */
/*--------------------------------------------------------------------------*/
