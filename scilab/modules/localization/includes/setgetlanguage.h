/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SETGETLANGUAGE_H__
#define __SETGETLANGUAGE_H__

#include "machine.h"


BOOL setlanguage(char *lang);
char *getlanguage(void);
char *getlanguagealias(void);
int getcurrentlanguagecode(void);
char *getlanguagefromcode(int code);
int getcodefromlanguage(char *language);

#endif /* __SETGETLANGUAGE_H__ */
/*-----------------------------------------------------------------------------------*/ 