/*---------------------------------------------
 * calling fortran iargc and getarg 
 * special case for win32 
 *--------------------------------------------*/

#include "../graphics/Math.h"

extern int sci_iargc(void);
extern int sci_getarg(int *,char *,long int ln);
extern int iargc_(void);
extern int getarg_(int *,char *,long int ln);

int C2F(sciiargc)() 
{
  int val;
#ifdef WIN32
  val=sci_iargc(); /* see wsci/winmain.c */
#else
  val=iargc_();
#endif 
  return Max(val,0);
}

int C2F(scigetarg)(int *n,char *str,long int ln)
{
#ifdef WIN32
  sci_getarg(n,str,ln); /* see wsci/winmain.c */
#else 
  getarg_(n,str,ln);
#endif
  return 0;
}

