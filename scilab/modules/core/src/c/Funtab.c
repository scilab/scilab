/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
/* Scilab function table */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashtable_core.h"
#include "stack-c.h"
#include "Funtab.h"
/*--------------------------------------------------------------------------*/
int C2F(funtab)(int *id, int *fptr, int *job,char *namefunction, unsigned long namefunction_len) 
{
  switch ( *job) 
  {
	case SCI_HFUNCTIONS_FIND : 
		*fptr = 0;
		action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_FIND);
    break;

    case SCI_HFUNCTIONS_BACKSEARCH : 
		action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_BACKSEARCH);
    break;

    case SCI_HFUNCTIONS_ENTER :
		if ( (namefunction) && (namefunction_len) )
		{
			if (strcmp(namefunction,"NULL_NAME") == 0)
			{
				/* we add function referenced only by id */
				/* this function has no name */
				action_hashtable_scilab_functions(id,NULL,fptr,SCI_HFUNCTIONS_ENTER);
			}
			else
			{
				/* we have a function named */
				action_hashtable_scilab_functions(id,namefunction,fptr,SCI_HFUNCTIONS_ENTER);
			}
		}
		else
		{
			/* namefunction == NULL or namefunction = 0 */
			/* we add function referenced only by id */
			/* this function has no name */
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
/*--------------------------------------------------------------------------*/  
