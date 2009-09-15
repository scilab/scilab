/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2007 - INRIA - Allan CORNET
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

#include "dynlib_localization.h"
#include "BOOL.h"

/** 
 * change language
 * @param lang
 */
LOCALIZATION_IMPEXP BOOL setlanguage(char *lang);


/** 
 * returns current language
 * @return
 */
LOCALIZATION_IMPEXP char *getlanguage(void);

/** 
 * returns current alias 
 * example : en_US --> en
 * @return
 */
LOCALIZATION_IMPEXP char *getlanguagealias(void);

/** 
 * return internal code associated with a language
 * @return
 */
LOCALIZATION_IMPEXP int getcurrentlanguagecode(void);

/** 
 * returns language of code
 * @param code
 * @return
 */
LOCALIZATION_IMPEXP char *getlanguagefromcode(int code);

/** 
 * get code of a language
 * @param language
 * @return
 */
LOCALIZATION_IMPEXP int getcodefromlanguage(char *language);

/** 
* try to convert strlanguage (alias) to a good language string
* @param language
* @return
*/
LOCALIZATION_IMPEXP char *convertlanguagealias(char *strlanguage);

/** 
* test if lang is defined language
* @param lang
* @return TRUE or FALSE
*/
LOCALIZATION_IMPEXP BOOL LanguageIsOK(char *lang);

/**
 * Check if the current language is the same we are trying to switch to
 *
 * @param language The language we want to switch to
 * @return If we need to change the language or not
 */
LOCALIZATION_IMPEXP BOOL needtochangelanguage(char *language);


/**
 * Export the locale to the system
 *
 * @param the locale we want to export
 * @return if is worked
 */
LOCALIZATION_IMPEXP BOOL exportLocaleToSystem(char *locale);

#endif /* __SETGETLANGUAGE_H__ */
/*--------------------------------------------------------------------------*/ 
