/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SETGETLANGUAGE_H__
#define __SETGETLANGUAGE_H__

#include "machine.h"

/** TODO
 * @param lang
 */
BOOL setlanguage(char *lang);


/** TODO
 * @return
 */
char *getlanguage(void);

/** TODO
 * @return
 */
char *getlanguagealias(void);

/** TODO
 * @return
 */
int getcurrentlanguagecode(void);

/** TODO
 * @param code
 * @return
 */
char *getlanguagefromcode(int code);

/** TODO
 * @param language
 * @return
 */
int getcodefromlanguage(char *language);

#endif /* __SETGETLANGUAGE_H__ */
/*-----------------------------------------------------------------------------------*/ 
