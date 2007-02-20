
#include "../../routines/machine.h"

/*******************************************
 * simple example with sparse matrix 
 * Display of a 
 *******************************************/

int F2C(ext15ca)(ar,m,n)
     double *ar;
     int *n,*m;
{ 

}

/*******************************************
 * simple example with sparse matrix 
 * 2*a copied into b 
 *******************************************/

int F2C(ext15cb)(ar1,ar2,m,n)
     double *ar1,*ar2;
     int *m,*n;
{
  int i;
  for ( i = 0; i < *m*(*n) ; i++) 
    ar2[i] = 2*ar1[i];
}

/*******************************************
 * same as ext15ca but ar and ai are integers 
 *******************************************/

int F2C(ext15cc)(ar,m,n)
     int *ar,*m,*n;
{ 

}


/*******************************************
 * same as ext15cb but ar2 and ai2 are integers 
 * 
 *******************************************/


int F2C(ext15cd)(ar1,ar2,m,n)
     int  *ar1,*ar2;
     int *m,*n;

{
  int i;
  for ( i = 0; i < *m*(*n) ; i++) 
    ar2[i] = 2*ar1[i];
}




/*******************************************
 * a is an external 
 *******************************************/


int F2C(ext15ce)(ar1,m,n,err)
     int  **ar1;
     int *m,*n,*err;
{
  int i;
  *n=1;
  *m=10;
  *ar1 = (int *) malloc((unsigned) (*m)*sizeof(int));
  if ( *ar1 == (int*) 0) 
    {
      *err=1;
      sciprint("No more space\r\n");
      return;
    }
  *err=0;
  for ( i = 0; i < *m*(*n) ; i++) 
    (*ar1)[i] = i;
}
