/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include <stdarg.h>
#include "Scierror.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "msgstore.h"
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#define vsnprintf _vsnprintf
#endif
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(error)  __PARAMS((integer *n));
extern int C2F(errmgr)  __PARAMS((integer *n, integer *errtyp));
extern int C2F(errmsg)  __PARAMS((integer *n, integer *errtyp));
extern int C2F(errmds)  __PARAMS((integer *num, integer *imess, integer *imode));
extern int C2F(depfun)  __PARAMS((integer *lunit, int *trace, int *first));
extern int C2F(depexec)  __PARAMS((integer *lunit, int *trace, int *first, int *pflag));
extern int C2F(errloc)  __PARAMS((integer *n));

static int Scierror_internal __PARAMS((integer *n,char *buffer));
/*-----------------------------------------------------------------------------------*/ 
/* Scilab Error at C level */
/*-----------------------------------------------------------------------------------*/ 
/* 
* as sciprint but with an added first argument 
* which is ignored (used in do_printf) 
* @param iv
* @param fmt
* @return
*/
int  Scierror(int iv,char *fmt,...) 
{
	int retval;
	integer lstr;
	va_list ap;
	char s_buf[bsiz];
	va_start(ap,fmt);

#if defined (vsnprintf) || defined (linux)
{
	retval= vsnprintf(s_buf,bsiz-1, fmt, ap );
}
#else
	retval= vsprintf(s_buf,fmt, ap );
#endif
if (retval == -1)
{
	 s_buf[bsiz-1]='\0';
}

lstr=(integer)strlen(s_buf);
va_end(ap);
Scierror_internal(&iv,s_buf);
return retval;
}
/*-----------------------------------------------------------------------------------*/ 
static int Scierror_internal(integer *n,char *buffer)
{
	integer imode;
	int trace;
	integer imess;
	integer errtyp, num, lct1, len;

	C2F(errmds)(&num, &imess, &imode);
	trace = ! ((num < 0 || num == *n) && imess != 0);
	/*     de-activate output control */
	lct1 = C2F(iop).lct[0];
	C2F(iop).lct[0] = 0;
	errtyp = 0;
	if (C2F(errgst).err1 == 0 && C2F(errgst).err2 == 0) 
	{
		/*     . locate the error in the current statement */
		if (trace) 
			C2F(errloc)(n);
		/*     . output error message */
		else 
			C2F(iop).lct[0] = -1;
		/* XXX  C2F(errmsg)(n, &errtyp); */
		errtyp=0;
		C2F(freemsgtable)();
		C2F(errstore)(n);
		len=(integer)strlen(buffer);
		C2F(msgstore)(buffer,&len);
		if (C2F(iop).lct[0] != -1)
		{
			sciprint(buffer);
		}
		C2F(iop).lct[0] = 0;
	}
	/*     handle the error */
	C2F(errmgr)(n, &errtyp);
	/*     re-activate output control */
	C2F(iop).lct[0] = lct1;
	return 0;
} 
/*-----------------------------------------------------------------------------------*/ 

