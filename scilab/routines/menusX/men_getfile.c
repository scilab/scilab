/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h"
#else
#include "men_scilab.h"
#endif

/*************************************************     
 * test function 
 **********************************************************/

#define MAXSTRGETF 256

int TestGetFile(void)
{
  int ierr=0;
  static char *init ="*.sci";
  char *res;
  return(GetFileWindow(init,&res,".",0,&ierr,"Title"));
}

/****************************************************
 * Scilab getfile Menu
 * interface with scilab 
 * res is dynamically allocated in GetFileWindow 
 *     and the routines which use xgetfile must 
 *     clear the memory ( see xawelm.f ) 
 **********************************************************/
     
void C2F(xgetfile)(char *filemask, char *dirname, char **res, integer *ires, integer *ierr, integer *idir, integer *desc, integer *ptrdesc, integer *nd)
{
  int flag=0,rep;
  char *description;
  *ierr=0;
  ScilabMStr2C(desc,nd,ptrdesc,&description,ierr);
  if ( *ierr == 1) return;
  *ierr = 0;
  if ( *idir == 1) flag =1 ;
  rep = GetFileWindow(filemask,res,dirname,flag,ierr,description);
  FREE(description);
  if ( *ierr >= 1 || rep == FALSE )
    {
      *ires = 0 ;
      return;
    }
  else 
    {
      *ires=strlen(*res);
    }
}



