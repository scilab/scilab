#include "f2c.h"

/*
 * subroutine getarg(k, c)
 * returns the kth unix command argument in fortran character
 * variable argument c
*/

/** JPC 1998 : pour faire un dll **/
int xargc;
char **xargv;

#ifdef KR_headers
VOID send_xarg(xxargc,xxargv) int xxargc; char **xxargv;
#else
void send_xarg(int xxargc, char **xxargv)
#endif
{
  xargc=xxargc;
  xargv=xxargv;
}

#ifdef KR_headers
VOID getarg_(n, s, ls) ftnint *n; register char *s; ftnlen ls;
#else
void getarg_(ftnint *n, register char *s, ftnlen ls)
#endif
{
register char *t;
register int i;

if(*n>=0 && *n<xargc)
	t = xargv[*n];
else
	t = "";
for(i = 0; i<ls && *t!='\0' ; ++i)
	*s++ = *t++;
for( ; i<ls ; ++i)
	*s++ = ' ';
}
