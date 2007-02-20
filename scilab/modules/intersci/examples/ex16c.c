
#include "../../routines/machine.h"

/*******************************************
 * simple example with sparse matrix 
 * Display of a 
 *******************************************/

int F2C(ext16ca)(ar,m,n)
     int *ar;
     int *n,*m;
{ 
  int i;
  for ( i=0; i < (*m)*(*n) ; i++) ar[i]= (ar[i]==1)? 0:1;
}

/*******************************************
 * simple example with sparse matrix 
 * 2*a copied into b 
 *******************************************/

int F2C(ext16cb)(ar1,ar2,m,n)
     int *ar1,*ar2;
     int *m,*n;
{
  int i;
  for ( i = 0; i < *m*(*n) ; i++) ar2[i] =  (ar1[i]==1)? 0:1;
}


/*******************************************
 * a is an external 
 *******************************************/


int F2C(ext16ce)(ar1,m,n,err)
     int  **ar1;
     int *m,*n,*err;
{
  int i,j;
  *n=2;
  *m=3;
  *ar1 = (int *) malloc((unsigned) (*m)*(*n)*sizeof(int));
  if ( *ar1 == (int*) 0) 
    {
      *err=1;
      sciprint("No more space\r\n");
      return;
    }
  *err=0;
  for ( i = 0; i < (*n) ; i++) 
    for ( j = 0; j < (*m) ; j++) 
      (*ar1)[i+(*n)*j] = i ;
}
