/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h"
MDialog SciMDialog = {{0},0,0,NULL,NULL,NULL,-1,0};  /** used to stored the mdialog data **/
#else
#include "men_scilab.h"
MDialog SciMDialog = {NULL,NULL,NULL,-1,0};  /** used to stored the mdialog data **/
#endif



/*************************************************     
 * a test function used in command.c
 **********************************************************/

int TestmDialogWindow(void)
{
  int rep,i;
  static char *labels = "LaBel";
  static char *pszTitle[] = {
    "first list entry",
    "second list entry",
    "third list entry",
    "fourth list entry",
    NULL
    };
  SciMDialog.labels = labels;
  SciMDialog.pszTitle = pszTitle;
  /** Warning pszName must be allocated because 
    MatricDialogWindow will reallc it to store result **/
  SciMDialog.pszName  = (char **) malloc((4+1)*sizeof(char *));
  if ( SciMDialog.pszName == ( char **) 0 ) return(FALSE);
  for ( i = 0 ; i < 4 ; i++ )
    {
      SciMDialog.pszName[i] = (char *) malloc(2*sizeof(char));
      if ( SciMDialog.pszName[i] == ( char *) 0 ) return(FALSE);
      sprintf(SciMDialog.pszName[i],"%1d",i);
    }
  SciMDialog.pszName[4]= (char*)0;
  SciMDialog.nv = 4;
  SciMDialog.ierr=0;
  rep=mDialogWindow();
  return(rep);
}

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
