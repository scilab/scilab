/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h" 
#else
#include "men_scilab.h"
#endif

int     ExposeChooseWindow(ChooseMenu *);

/*****************************************
 * TestFunction 
 *****************************************/

int TestChoose(void)
{
  ChooseMenu Ch;
  int Rep;
  int nrep;
  static char *butn[]= { "But 1"," But 2",NULL};
  static char *strings[]= { "Str 1"," Str 2","Str 3",NULL};
  Ch.nstrings = 3;
  Ch.nb = 2; 
  Ch.choice = 0;
  Ch.description = " Title";
  Ch.buttonname = butn;
  Ch.strings = strings;
  Rep = ExposeChooseWindow(&Ch);
  if ( Rep == TRUE )
    nrep=(1+ Ch.choice);
  else nrep=0;
  return (nrep);
}

/*****************************************
 * Interface with a Scilab ``structure'' 
 *****************************************/

void C2F(xchoose)(int *desc, int *ptrdesc, int *nd, int *basstrings, int *ptrstrings, int *nstring, int *btn, int *ptrbtn, int *nb, int *nrep, int *ierr)
{
  ChooseMenu Ch;
  int Rep,i;
  *ierr=0;
  Ch.nstrings = *nstring;
  Ch.nb = *nb;
  Ch.choice = 0;
  ScilabMStr2C(desc,nd,ptrdesc,&(Ch.description),ierr);
  if ( *ierr == 1) return;
  ScilabMStr2CM(btn,nb,ptrbtn,&(Ch.buttonname),ierr);
  if ( *ierr == 1) return;
  ScilabMStr2CM(basstrings,&(Ch.nstrings),ptrstrings,&(Ch.strings),ierr);
  Rep = ExposeChooseWindow(&Ch);
  for (i=0 ; i < Ch.nstrings ; i++ ) FREE(Ch.strings[i]);
  FREE(Ch.strings); 
  FREE(Ch.description);
  for (i=0 ; i < Ch.nb ; i++ )FREE(Ch.buttonname[i]);
  FREE(Ch.buttonname);
  if ( Rep == TRUE )
    *nrep=(1+ Ch.choice);
  else *nrep=0;
}




