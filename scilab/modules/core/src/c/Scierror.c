/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "Scierror.h"

#if defined  (__STDC__) || defined  (_MSC_VER)
	#include <stdarg.h>
#else
	#include <varargs.h>
#endif

#include "MALLOC.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
	#define vsnprintf _vsnprintf
	extern char *QueryStringError(char *Tag);
#endif
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(error)  __PARAMS((integer *n));
extern int C2F(errmgr)  __PARAMS((integer *n, integer *errtyp));
extern int C2F(errmsg)  __PARAMS((integer *n, integer *errtyp));
extern int C2F(errmds)  __PARAMS((integer *num, integer *imess, integer *imode));
extern int C2F(depfun)  __PARAMS((integer *lunit, int *trace, int *first));
extern int C2F(depexec)  __PARAMS((integer *lunit, int *trace, int *first, int *pflag));
extern int C2F(errloc)  __PARAMS((integer *n));
extern void C2F(freemsgtable)();
extern int C2F(msgstore)(char *str,int *n);
extern int C2F(errstore)(int *n);

static int Scierror_internal __PARAMS((integer *n,char *buffer));
/*-----------------------------------------------------------------------------------*/ 
/* Scilab Error at C level */
/*-----------------------------------------------------------------------------------*/ 
/* 
* as sciprint but with an added first argument 
* which is ignored (used in do_printf) 
*/
#if defined (__STDC__) || defined (_MSC_VER)
	int  Scierror(int iv,char *fmt,...) 
#else
	int Scierror(va_alist) va_dcl
#endif 
{
	int retval;
	integer lstr;
	va_list ap;
	char s_buf[bsiz];
#if defined (__STDC__) || defined (_MSC_VER)
	va_start(ap,fmt);
#else
	int iv;
	char *fmt;
	va_start(ap);
	iv = va_arg(ap,int);
	fmt = va_arg(ap, char *);
#endif

#if defined (vsnprintf) || defined (linux)
{
	#if _MSC_VER
		char *LocalizedString=QueryStringError(fmt);
		if (LocalizedString)
		{
			retval= vsnprintf(s_buf,bsiz-1, LocalizedString, ap );
			if (LocalizedString) {FREE(LocalizedString);LocalizedString=NULL;}
		}
		else
		{
			retval= vsnprintf(s_buf,bsiz-1, fmt, ap );
		}
	#else
		retval= vsnprintf(s_buf,bsiz-1, fmt, ap );
	#endif
	
}
#else
	retval= vsprintf(s_buf,fmt, ap );
#endif
if (retval == -1)
{
	 s_buf[bsiz-1]='\0';
}

lstr=strlen(s_buf);
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
		len=strlen(buffer);
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

