#include "machine.h"

#ifdef __STDC__ 
#include <stdlib.h> 
#else 
#include <malloc.h> 
#endif 

extern void sciprint __PARAMS((char *fmt, ...));

/*************************************************************
 * Example of an integer array created by C code
 * converted to output Scilab variable by cintf
 *************************************************************/

int C2F(ccalc8) ( a,m,n,err) 
     int **a,*m,*n,*err ;
{
  int i ;
  *n=5 ;
  *m=3 ;
  *a= ( int *) malloc( (unsigned) (*m)*(*n) *sizeof(int));
  if ( *a == (int *) 0 ) 
    {
      *err=1;
      sciprint("No more space\r\n");
    }
  *err=0;
  for ( i= 0 ; i < (*m)*(*n) ; i++) (*a)[i] = i ;
  return 0;
}


