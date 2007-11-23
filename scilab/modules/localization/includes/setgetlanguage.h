/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __SETGETLANGUAGE_H__
#define __SETGETLANGUAGE_H__

#include "machine.h"

/** TODO
 * change language
 * @param lang
 */
BOOL setlanguage(char *lang,BOOL updateHelpIndex,BOOL updateMenus);


/** TODO
 * returns current language
 * @return
 */
char *getlanguage(void);

/** TODO
 * returns current alias 
 * example : en_US --> en
 * @return
 */
char *getlanguagealias(void);

/** TODO
 * return internal code associated with a language
 * @return
 */
int getcurrentlanguagecode(void);

/** TODO
 * returns language of code
 * @param code
 * @return
 */
char *getlanguagefromcode(int code);

/** TODO
 * get code of a language
 * @param language
 * @return
 */
int getcodefromlanguage(char *language);

/** TODO
* compare 2 languages same or not ?
* @param language
* @return
*/
int comparelanguages(char *language1,char *language2);

/** TODO
* test if we need to change language
* @param language
* @return
*/
BOOL needtochangelanguage(char *language);


/** TODO
* try to convert strlanguage (alias) to a good language string
* @param language
* @return
*/
char *convertlanguagealias(char *strlanguage);

/** TODO
* test if lang is defined language
* @param lang
* @return TRUE or FALSE
*/
BOOL LanguageIsOK(char *lang);


#endif /* __SETGETLANGUAGE_H__ */
/*--------------------------------------------------------------------------*/ 
