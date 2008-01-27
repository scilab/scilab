/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/ 
#ifndef __SETGETLANGUAGE_H__
#define __SETGETLANGUAGE_H__

#include "machine.h"

/** 
 * change language
 * @param lang
 */
BOOL setlanguage(char *lang,BOOL updateHelpIndex,BOOL updateMenus);


/** 
 * returns current language
 * @return
 */
char *getlanguage(void);

/** 
 * returns current alias 
 * example : en_US --> en
 * @return
 */
char *getlanguagealias(void);

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
char *getlanguagefromcode(int code);

/** 
 * get code of a language
 * @param language
 * @return
 */
int getcodefromlanguage(char *language);

/** 
* try to convert strlanguage (alias) to a good language string
* @param language
* @return
*/
char *convertlanguagealias(char *strlanguage);

/** 
* test if lang is defined language
* @param lang
* @return TRUE or FALSE
*/
BOOL LanguageIsOK(char *lang);

/**
 * Check if the current language is the same we are trying to switch to
 *
 * @param language The language we want to switch to
 * @return If we need to change the language or not
 */
BOOL needtochangelanguage(char *language);

#endif /* __SETGETLANGUAGE_H__ */
/*--------------------------------------------------------------------------*/ 
