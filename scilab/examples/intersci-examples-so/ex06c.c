
#include "machine.h"
#include <string.h> 
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h> 

/*************************************************************
 * Example of a dynamically allocated array of character created here.
 * Its size is sent back as an output.
 * It is converted into Scilab variable in the interface program.
 * The allocated array is freed in the interface.
 * 
 * the Scilab variable is of type stringmat and is not in the argument 
 * list of the scilab function. The  Fortran type is Cstringv 
 * 
 * NOTE: The string array allocated here must be NULL terminated 
 *       (this is used in the free process)
 *************************************************************/

int C2F(ccalc6b)(a,m,n,err)
     char ***a;
     int *m,*n,*err;
{
  int i,nstring;
  *m=3;
  *n=2;
  nstring= (*m)*(*n); 
  /* add 1 for last null string */
  *a =(char **) malloc((unsigned) ((nstring+1) * sizeof(char *)));
  if ( *a ==0) 
    {
      sciprint("No more space\r\n");
      *err=1;
      return 0;
    }
  for ( i=0  ; i< nstring ; i++) 
    {
      (*a)[i] = (char *) malloc ((8)*sizeof(char));
      sprintf((*a)[i],"char %d",i);
      *err=0;
    }
  (*a)[nstring]= NULL; /* null terminated */
  return 0;
}


/*************************************************************
 * This example shows how a Scilab argument of type stringmat 
 * (which is in the calling list of the Scilab function )
 * is transmited to a C program. 
 * a is a stringmat Scilab argument and a Cstringv Fortran argument 
 * a is allocated in the interface 
 *************************************************************/

int C2F(ccalc6a)(a,m,n) 
     char ***a;
     int *m,*n;
{
  int i,j;
  for ( i = 0 ; i < *m*(*n) ; i++) 
    {
      char *loc = (*a)[i];
      for ( j = 0 ; j < strlen(loc); j++) 
	if ( loc[j] =='a' ) loc[j] ='o';
    }
  return 0;
}









