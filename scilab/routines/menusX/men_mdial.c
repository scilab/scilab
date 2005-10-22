/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h"
MDialog SciMDialog = {{0},0,0,NULL,NULL,NULL,-1,0};  /** used to stored the mdialog data **/
#else
#include "men_scilab.h"
MDialog SciMDialog = {NULL,NULL,NULL,-1,0};  /** used to stored the mdialog data **/
#endif

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

/*************************************************     
 * interface with scilab 
 **********************************************************/

void C2F(xmdial)(int *label, int *ptrlab, int *nlab, int *value, int *ptrv, int *desc, int *ptrdesc, int *nv, int *res, int *ptrres, int *ierr)
{
  int i,rep;
  int  maxchars= *ierr;
  *ierr=0;

  /* first test that no one is using xmdial */ 
  if (SciMDialog.nv >= 0) 
    {
      sciprint("Only one mdialog at a time please \r\n");
      return ;
    }
  /* conversion of scilab characters into strings */
  ScilabMStr2C(label,nlab,ptrlab,&(SciMDialog.labels),ierr);
  if ( *ierr == 1) return;
  ScilabMStr2CM(desc,nv,ptrdesc,&(SciMDialog.pszTitle),ierr);
  if ( *ierr == 1) return;
  ScilabMStr2CM(value,nv,ptrv,&(SciMDialog.pszName),ierr);
  if ( *ierr == 1) return;
  SciMDialog.nv = *nv;
  if ( SciMDialog.nv >  NPAGESMAX*NITEMMAXPAGE )
    {
      sciprint("Sorry : x_mdialog limited to %d items on Windows\n",
	        NPAGESMAX*NITEMMAXPAGE );
      *ierr = 1;
      SciMDialog.nv = -1;
      return;
    }
  SciMDialog.ierr = 0;
  rep = mDialogWindow();
  if ( SciMDialog.ierr == 0 )
    {
      /** No errors **/
      if ( rep == FALSE ) 
	*nv=0;  /** cancel **/
      else 
	ScilabCM2MStr(SciMDialog.pszName,*nv,res,ptrres,maxchars,ierr);
    }
  FREE(SciMDialog.labels);
  for (i=0; i< *nv;i++) FREE(SciMDialog.pszName[i]); 
  FREE(SciMDialog.pszName);
  for (i=0; i< *nv;i++) FREE(SciMDialog.pszTitle[i]); 
  FREE(SciMDialog.pszTitle);
  SciMDialog.nv = -1;
}
