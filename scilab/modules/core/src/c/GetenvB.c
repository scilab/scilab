/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/ 

#include "GetenvB.h"

void GetenvB(char *name, char *env, int len)
{
  int ierr,un=1;
  C2F(getenvc)(&ierr,name,env,&len,&un);
  if( ierr == 0) 
    {
      char *last = &env[len-1];
      while ( *last == ' ' ) { last = '\0' ; } 
	  last--;
    }
  else 
    {
      env[0] = '\0' ;
    }  
}
