/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SETGETLANGUAGE_H__
#define __SETGETLANGUAGE_H__

#include "machine.h"


BOOL setlanguage(char *lang);
char *getlanguage(void);
int getcurrentlanguagecode(void);
char *getlanguagealias(void);

#endif /* __SETGETLANGUAGE_H__ */
/*-----------------------------------------------------------------------------------*/ 