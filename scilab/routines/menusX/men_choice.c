/* Copyright ENPC */
/********************************************************
 * choice.c
 * All the generic stuff for dealing with choice widgets.
 * (from xgc) 
 ***********************************************************/

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

SciStuff **Everything = NULL;

/*************************************************
 * Interface to work with Scilab 
 ************************************************/

void C2F(xchoices)(int *descla, int *ptrdescla, int *nvla, int *desc, int *ptrdesc, int *nv, int *defval, int *nitems, int *rep)
{
  int ierr,i,sepc=0;
  char **items,*label;
  ierr=0;
  ScilabMStr2C(descla,nvla,ptrdescla,&label,&ierr);
  if ( ierr == 1) return;
  ScilabMStr2CM(desc,nv,ptrdesc,&items,&ierr);
  if ( ierr == 1) return;
  for (i = 0 ; i < *nv ; i++ ) 
    if ( strcmp(items[i],"[--sep--]")==0) 
      {
	sepc++;
	FREE(items[i]);
	items[i]= (char *) NULL;
      };
  if ( sepc != *nitems-1) 
    {
      sciprint("x_choices : the choice vector is too small\r\n");
      return;
    }
  *rep=SciChoice(label,items,defval,*nitems);
  for (i=0;i< *nv;i++) FREE(items[i]); FREE(items);
  FREE(label);
}

/*************************************************
 *
 * Main function 
 *if test == 1
 *a test is used whitout using items, defval, nitems 
 *see choice-test.c
 ************************************************/

int SciChoice(char *label, char **items, int *defval, int nitems)
{
  int rep;
  rep =  SciChoiceCreate(items,defval,nitems);
  if ( rep == 0 )
    {
      sciprint("x_choices : No more place\r\n");
      return(-1);
    }
  else if ( rep == -1 )
    {
      sciprint("Only one toggle menu at a time please \n");
      return(-1);
    }
  rep=SciChoiceI(label,defval,nitems);
  SciChoiceFree(nitems);
  return(rep);
}

