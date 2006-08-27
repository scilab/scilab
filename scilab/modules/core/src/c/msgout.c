/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
#include <string.h>
/*-----------------------------------------------------------------------------------*/ 
extern integer C2F(msgstore)();
extern int C2F(basout)();
/*-----------------------------------------------------------------------------------*/ 
int C2F(msgout)(integer *io,integer *lunit,char *str,long int str_len)
{
	static integer ierr;
	int len=strlen(str);

    C2F(basout)(io, lunit, str, str_len);
    ierr = C2F(msgstore)(str, &len,str_len);
    
    return 0;
}
/*-----------------------------------------------------------------------------------*/ 
