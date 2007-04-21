/* Copyright ENPC */
#ifdef _MSC_VER 
#include "wmen_scilab.h"
#else
#include "men_scilab.h"
#endif
#include "../../string/includes/men_Sutils.h" /* ScilabMStr2C */


#include "Os_specific.h" /* isanan */ 
#include "MALLOC.h"
#include "cluni0.h"

/****************************************************
 * Scilab xgetfile command used to get a file name 
 * through a menu 
 * res is dynamically allocated in GetFileWindow 
 *     and the routines which use xgetfile must 
 *     clear the memory ( see xawelm.f ) 
 * dirname is <<expanded>> using cluni0 
 **********************************************************/

#define MAX_PATH_STR 1024
     
void C2F(xgetfile)(char *filemask, char *dirname, char **res,
		   integer *ires, integer *ierr, integer *idir,
		   integer *desc, integer *ptrdesc, integer *nd)
{
  static char dir_expanded[MAX_PATH_STR+1];
  int flag=0,rep,out_n;
  char *description;
  *ierr=0;
  ScilabMStr2C(desc,nd,ptrdesc,&description,ierr);
  if ( *ierr == 1) return;
  *ierr = 0;
  if ( *idir == 1) flag =1 ;
  C2F(cluni0)(dirname,dir_expanded,&out_n,strlen(dirname),MAX_PATH_STR);

  rep = GetFileWindow(filemask,res,dir_expanded,flag,ierr,description);
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



