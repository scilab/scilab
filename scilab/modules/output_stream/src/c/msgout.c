/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
#include "msgout.h"
#include "msgstore.h"
#include "basout.h"
/*--------------------------------------------------------------------------*/ 
int C2F(msgout)(integer *io,integer *lunit,char *str,long int str_len)
{
	static integer ierr;
	integer i__1=str_len;

    C2F(basout)(io, lunit, str, str_len);
    ierr = C2F(msgstore)(str, &i__1);

    return 0;
}
/*--------------------------------------------------------------------------*/ 
