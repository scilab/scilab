/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h"
char *dialog_str = (char *) 0;
#else
#include "men_scilab.h"
char *dialog_str = (char *) 0;
#endif

SciDialog ScilabDialog;

/**********************************************************
 * Test function 
 **********************************************************/

int TestDialog() 
{
  int rep;
  static char *description = "Dialog test\nTexte";
#ifdef WIN32 
  static char *init ="Initial\r\nvalue";
#else 
  static char *init ="Initial\nvalue";
#endif
  static char *pButName[] = {
    "LabelOK",
    "LabelCancel",
    NULL
    };
  ScilabDialog.description = description ;
  ScilabDialog.init = init;
  ScilabDialog.pButName = pButName;
  ScilabDialog.nb = 2;
  rep = DialogWindow();
  sciprint("reponse[%d] %s\n",rep,dialog_str) ;
  return(rep);
}


/*************************************************     
 * interface with scilab 
 **********************************************************/

void C2F(xdialg)(value,ptrv,nv,desc,ptrdesc,nd,btn,ptrbtn,nb,res,ptrres,nr,ierr)
     int *value,*ptrv,*nv,*desc,*ptrdesc,*nd,*btn,*nb;
     int *ptrbtn,*res,*ptrres,*nr,*ierr;
{
  int maxlines,maxchars,i,rep;
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
}


/*********************************************************
 * to open a dialog in a procedure 
 *********************************************************/

void xdialg1(description,valueinit,pButName,value,ok)
     char *description,*valueinit,**pButName,*value;
     int *ok;
{
  int rep;
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
}

