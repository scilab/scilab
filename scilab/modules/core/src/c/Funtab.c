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
	case SCI_HFUNCTIONS_FIND : 
		*fptr=0;
		action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_FIND);
    break;

    case SCI_HFUNCTIONS_BACKSEARCH : 
		action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_BACKSEARCH);
    break;

    case SCI_HFUNCTIONS_ENTER :
		if ( (namefunction_len) && (strcmp(namefunction,"NULL_NAME")) )
		{
			action_hashtable_scilab_functions(id,namefunction,fptr,SCI_HFUNCTIONS_ENTER);
		}
		else
		{
			action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_ENTER);
		}
      break;

    case SCI_HFUNCTIONS_DELETE :  
		action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_DELETE);
    break;

	default :
	break;

  }
  return(0);
}
/*-----------------------------------------------------------------------------------*/  
