/*-----------------------------------------------------------------------------------*/
/* Scilab function table */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashtable_core.h"
#include "stack-c.h"
#include "machine.h"
#include "Funtab.h"
/*-----------------------------------------------------------------------------------*/
int C2F(funtab)(int *id, int *fptr, int *job,char *namefunction, unsigned long namefunction_len) 
{
  switch ( *job) 
    {
    case 0 : /* before do a printf for What OBSOLETE */
    break;
		case 1 : /* @TODO : add comment : what is 1 ? */
      *fptr=0;
      action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_FIND);
      break;
    case 2 :  /* @TODO : add comment : what is 2 ? */
      action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_BACKSEARCH);
      break;
    case 3 :   /* @TODO : add comment : what is 3 ? */
		if ( (namefunction_len) && (strcmp(namefunction,"NULL_NAME")) )
		{
			action_hashtable_scilab_functions(id,namefunction,fptr,SCI_HFUNCTIONS_ENTER);
		}
		else
		{
			action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_ENTER);
		}
      break;
    case 4 :   /* @TODO : add comment : what is 4 ? */
      action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_DELETE);
      break;
    }
  return(0);
}
/*-----------------------------------------------------------------------------------*/  
