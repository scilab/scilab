/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h"
char *dialog_str = (char *) 0;
#else
#include "men_scilab.h"
char *dialog_str = (char *) 0;
#endif

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

SciDialog ScilabDialog = { NULL,NULL,NULL,-1,0}; 


/*************************************************     
 * interface with scilab 
 **********************************************************/

void C2F(xdialg)(int *value, int *ptrv, int *nv, int *desc, int *ptrdesc, int *nd, int *btn, int *ptrbtn, int *nb, int *res, int *ptrres, int *nr, int *ierr)
{
  int maxlines,maxchars,i,rep;
  if (ScilabDialog.nb >= 0) 
    {
      sciprint("Only one dialog at a time please \r\n");
      return ;
    }
  maxlines= *nr;
  maxchars= *ierr;
  *ierr=0;
  ScilabMStr2C(desc,nd,ptrdesc,&(ScilabDialog.description),ierr);
  if ( *ierr == 1) return;
  ScilabMStr2C(value,nv,ptrv,&(ScilabDialog.init),ierr);
  if ( *ierr == 1) return;
  ScilabMStr2CM(btn,nb,ptrbtn,&(ScilabDialog.pButName),ierr);
  if ( *ierr == 1) return;
  ScilabDialog.nb = *nb ;
  ScilabDialog.ierr = 0;
  rep = DialogWindow();
  FREE(ScilabDialog.description);
  FREE(ScilabDialog.init);
  for (i=0 ; i < *nb ; i++ )FREE(ScilabDialog.pButName[i]);
  FREE(ScilabDialog.pButName);
  if ( rep == FALSE)
    {
      /** cancel or memory exhausted **/
      *nr=0;
    }
  else {
    ScilabC2MStr2(res,nr,ptrres,dialog_str,ierr,maxchars,maxlines);
    FREE(dialog_str);/** allocated in DialogOK **/
    dialog_str = NULL;
  }
  ScilabDialog.nb = -1;
}


/*********************************************************
 * to open a dialog in a procedure 
 *********************************************************/

void xdialg1(char *description, char *valueinit, char **pButName, char *value, int *ok)
{
  int rep;
  if (ScilabDialog.nb >= 0) 
    {
      sciprint("Only one dialog at a time please \r\n");
      return ;
    }
  ScilabDialog.description = description ;
  ScilabDialog.init = valueinit;
  ScilabDialog.pButName = pButName;
  ScilabDialog.nb = 2; /** XXX pas clair **/
  rep = DialogWindow();
  if (rep == FALSE )
    *ok=0;
  else {
    *ok=1;
    strcpy(value,dialog_str);
    FREE(dialog_str);/** allocated in DialogOK **/
    dialog_str = NULL;
  }
  ScilabDialog.nb = -1;
}

