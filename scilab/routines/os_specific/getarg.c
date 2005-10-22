/*---------------------------------------------
 * calling fortran iargc and getarg 
 * special case for win32 
 *--------------------------------------------*/

#include "../graphics/Math.h"
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

extern int sci_iargc(void);
extern int sci_getarg(int *,char *,long int ln);
extern int C2F(iargc)(void);
extern int C2F(getarg)(int *,char *,long int ln);

int C2F(sciiargc)() 
{
  int val=0;
#ifdef WIN32
  val=sci_iargc(); /* see wsci/winmain.c */
#else
#ifdef G95_FORTRAN
  val = _gfortran_iargc();
#else 
  val=C2F(iargc)();
#endif 
#endif 
  return Max(val,0);
}

int C2F(scigetarg)(int *n,char *str,long int ln)
{
#ifdef WIN32
  sci_getarg(n,str,ln); /* see wsci/winmain.c */
#else 
#ifdef G95_FORTRAN
  _gfortran_getarg_i4(n,str,ln);
#else 
  C2F(getarg)(n,str,ln);
#endif 
#endif
  return 0;
}

