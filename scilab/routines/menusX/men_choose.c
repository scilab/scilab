/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h" 
#else
#include "men_scilab.h"
#endif

int     ExposeChooseWindow(ChooseMenu *);


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




