#include "../../routines/machine.h"
#include <malloc.h> 

/*************************************************************
  Example of an integer array created by C code
  converted to output Scilab variable by cintf
*************************************************************/

C2F(ccalc3) ( a,m,n) 
     int **a,*m,*n ;
{
  int i ;
  *n=5 ;
  *m=3 ;
  *a= ( int *) malloc( (unsigned) (*m)*(*n) *sizeof(int));
  for ( i= 0 ; i < (*m)*(*n) ; i++) (*a)[i] = i ;
}


