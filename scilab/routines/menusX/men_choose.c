/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h" 
#else
#include "men_scilab.h"
#endif

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
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

  for (i=0 ; i < Ch.nstrings ; i++ )
  {
	  if (Ch.strings[i])
	  {
		  FREE(Ch.strings[i]);
		  Ch.strings[i]=NULL;
	  }
  }
  if (Ch.strings) {FREE(Ch.strings); Ch.strings=NULL;}
  if (Ch.description) {FREE(Ch.description);Ch.description=NULL;}

  for (i=0 ; i < Ch.nb ; i++ )
  {
	  if (Ch.buttonname[i])
	  {
		  FREE(Ch.buttonname[i]);
		  Ch.buttonname[i]=NULL;
	  }
  }
 
  if (Ch.buttonname) {FREE(Ch.buttonname);Ch.buttonname=NULL;}
  
  if ( Rep == TRUE )
  {
    *nrep=(1+ Ch.choice);
  }
  else
  {
	  *nrep=0;
  }
}




