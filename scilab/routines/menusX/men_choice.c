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

SciStuff **Everything = NULL;

/*************************************************
 * Interface to work with Scilab 
 ************************************************/

void C2F(xchoices)(descla,ptrdescla,nvla,desc,ptrdesc,nv,defval,nitems,rep)
     int *desc,*ptrdesc,*nv,*defval,*nitems;
     int *descla,*ptrdescla,*nvla,*rep;
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
 * test function 
 ************************************************/
int TestChoice()
{
  static char* items[] = {
    "Label1",    "choice1",    "a choice2",
    NULL,
    "colors",    "1","4","5","2","3","7","6",
    NULL,
    "Label2",    "poo", "foo","goo",
    NULL,
    "Label3",    "Blue",    "train",
    NULL,
    };
  static int defch[]={1,3,3,2};
  static int nitems= 4;
  return(SciChoice("test",items,defch,nitems));
}


/*************************************************
 *
 * Main function 
 *if test == 1
 *a test is used whitout using items, defval, nitems 
 *see choice-test.c
 ************************************************/

int SciChoice(label,items,defval,nitems)
     char **items,*label;
     int defval[], nitems;

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

