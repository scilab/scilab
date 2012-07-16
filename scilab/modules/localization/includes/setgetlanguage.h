/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2011 - 2011 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SETGETLANGUAGE_H__
#define __SETGETLANGUAGE_H__

#include <wchar.h>
#include "BOOL.h"
#include "dynlib_localization.h"

/**
 * change language
 * @param lang
 */
LOCALIZATION_IMPEXP BOOL setlanguage(wchar_t *lang);


/**
 * returns current language
 * @return
 */
wchar_t *getlanguage(void);

/**
 * returns current alias
 * example : en_US --> en
 * @return
 */
wchar_t *getlanguagealias(void);

/**
 * return internal code associated with a language
 * @return
 */
int getcurrentlanguagecode(void);

/**
 * returns language of code
 * @param code
 * @return
 */
wchar_t *getlanguagefromcode(int code);

/**
 * get code of a language
 * @param language
 * @return
 */
int getcodefromlanguage(wchar_t *language);

/**
* try to convert strlanguage (alias) to a good language string
* @param language
* @return
*/
LOCALIZATION_IMPEXP wchar_t *convertlanguagealias(wchar_t *strlanguage);

/**
* test if lang is defined language
* @param lang
* @return TRUE or FALSE
*/
LOCALIZATION_IMPEXP BOOL LanguageIsOK(wchar_t *lang);

/**
 * Check if the current language is the same we are trying to switch to
 *
 * @param language The language we want to switch to
 * @return If we need to change the language or not
 */
BOOL needtochangelanguage(wchar_t *language);


/**
 * Export the locale to the system
 *
 * @param the locale we want to export
 * @return if is worked
 */
BOOL exportLocaleToSystem(wchar_t *locale);

/**
 * Set the default language and save it in the registry (Windows)
 * @param lang the new language
 */
void setdefaultlanguage(char * lang);

/**
 * Get the default language from the registry (Windows)
 * @return the default language
 */
char * getdefaultlanguage();


#endif /* __SETGETLANGUAGE_H__ */
/*--------------------------------------------------------------------------*/
