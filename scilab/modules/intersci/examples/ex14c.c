
#include "stack-c.h"

/*******************************************
 * simple example with sparse matrix 
 * Display of a 
 *******************************************/

int F2C(ext14ca)(it,m,n,nel,mnel,icol,ar,ai)
     int *n,*m,*it,*nel,*mnel,*icol;
     double *ar,*ai;
{ 
  int i,iof = 0, j;
  for ( i = 0; i < *m ; i++) 
    {
      sciprint("Row %d, %d non zero elements\r\n",i+1,mnel[i]);
      for ( j = 0; j < mnel[i]; j++) 
	{
	  if (*it == 1) 
	      sciprint("(%d,%d) -> %f +i%f\r\n",i+1,icol[iof+j],
		       ar[iof+j],ai[iof+j]);
	  else 
	      sciprint("(%d,%d) -> %f \r\n",i+1,icol[iof+j],
		       ar[iof+j]);
	}
      iof += mnel[i];
    }
  return(0);
}

/*******************************************
 * simple example with sparse matrix 
 * 2*a copied into b 
 *******************************************/

int F2C(ext14cb)(it1,m1,n1,nel1,mnel1,icol1,ar1,ai1,
		 it2,m2,n2,nel2,mnel2,icol2,ar2,ai2)
     int *n1,*m1,*it1,*nel1,*mnel1,*icol1;
     double *ar1,*ai1;
     int *it2,*m2,*n2,*nel2,*mnel2,*icol2;
     double *ar2,*ai2;
{
  int i,iof = 0, j;
  for ( i = 0; i < *m1 ; i++) 
    {
      for ( j = 0; j < mnel1[i]; j++) 
	{
	  if (*it1 == 1) 
	    {
	      ai2[iof+j] = 2*ai1[iof+j];
	    }
	  ar2[iof+j] = 2*ar1[iof+j];
	}
      iof += mnel1[i];
    }
  for ( i = 0 ; i < *m1 ; i++ ) 
    mnel2[i]= mnel1[i];
  for ( i = 0 ; i < *nel1 ; i++) 
    icol2[i] = icol1[i] ;
  return(0);
}

/*******************************************
 * same as ext14ca but ar and ai are integers 
 *******************************************/

int F2C(ext14cc)(it,m,n,nel,mnel,icol,ar,ai)
     int *n,*m,*it,*nel,*mnel,*icol;
     int *ar,*ai;
{ 
  int i,iof = 0, j;
  for ( i = 0; i < *m ; i++) 
    {
      sciprint("Row %d, %d non zero elements\r\n",i+1,mnel[i]);
      for ( j = 0; j < mnel[i]; j++) 
	{
	  if (*it == 1) 
	      sciprint("(%d,%d) -> %d +i%d\r\n",i+1,icol[iof+j],
		       ar[iof+j],ai[iof+j]);
	  else 
	      sciprint("(%d,%d) -> %d \r\n",i+1,icol[iof+j],
		       ar[iof+j]);
	}
      iof += mnel[i];
    }
  return(0);
}


/*******************************************
 * same as ext14cb but ar2 and ai2 are integers 
 * 
 *******************************************/


int F2C(ext14cd)(it1,m1,n1,nel1,mnel1,icol1,ar1,ai1,
		 it2,m2,n2,nel2,mnel2,icol2,ar2,ai2)
     int *n1,*m1,*it1,*nel1,*mnel1,*icol1;
     double *ar1,*ai1;
     int *it2,*m2,*n2,*nel2,*mnel2,*icol2;
     int *ar2,*ai2;
{
  int i,iof = 0, j;
  for ( i = 0; i < *m1 ; i++) 
    {
      for ( j = 0; j < mnel1[i]; j++) 
	{
	  if (*it1 == 1) 
	    {
	      ai2[iof+j] = 2*ai1[iof+j];
	    }
	  ar2[iof+j] = 2*ar1[iof+j];
	}
      iof += mnel1[i];
    }
  for ( i = 0 ; i < *m1 ; i++ ) 
    mnel2[i]= mnel1[i];
  for ( i = 0 ; i < *nel1 ; i++) 
    icol2[i] = icol1[i] ;
  return(0);
}




/*******************************************
 * Test for intersci with external sparse (spt10 in ex14fi.desc) 
 * 
 * sparse.h defines a Sparse structure which can be used 
 * for storing external Sparse matrices which can be 
 * copied back to scilab sparse matrices ( by csparsef ) 
 * 
 * if x is a Sparse object 
 *     x.m : number of rows of x 
 *     x,n : number of columns of x 
 *     x.nel : number of non zero elements in x 
 *     x.it : 0 or 1 i.e. real or complex matrix 
 *     x.mnel[i],i=0,m-1 number of non zero elements in line i+1
 *
 *     x.icol[j],j=0,nel-1 : column of (j+1)th non zero element of the matrix 
 *                           ( stored by rows ) 
 *     x.xr[j],  .....     : real value of the (j+1)th non zero element 
 *     x.xi[j],  .....     : if x.it ==1 imaginary value of the (j+1)th 
 *                                       non zero element 
 *
 * NewSparse can be used to allocate a new Sparse object 
 *
 * The Scilab internal coding is very similar 
 * and the following function copies a Scilab Sparse (it,m,n,nel,mnel,icol,ar,ai) 
 * into an allocated Sparse object 
 * and return the allocated Sparse and its dimensions 
 *
 *
 * the routine C2F(csparsef) (file routines/intersci/sparse.c) 
 * is used for back converstion from Sparse to Scilab sparse and 
 * and the Sparse object is freed in csparsef 
 *
 *******************************************/

#include "../../routines/intersci/sparse.h"

int F2C(ext14ce)(it,m,n,nel,mnel,icol,ar,ai,x,mx,nx,nelx,itx,err)
     int *n,*m,*it,*nel,*mnel,*icol;
     double *ar,*ai;
     SciSparse **x;
     int *mx,*nx,*nelx,*itx;
     int *err;
{ 
  int i,iof = 0, j;
  *x = NewSparse(it,m,n,nel);
  if ( *x == (SciSparse *) 0) 
    {
      sciprint("No more space\r\n");
      *err = 1;return;
    }
  *mx = *m ;
  *nx = *n ; 
  *nelx = *nel;
  *itx = *it;
  *err=0;
  for ( i = 0; i < *m ; i++) 
    {
      for ( j = 0; j < mnel[i]; j++) 
	{
	  if (*it == 1) 
	    {
	      (*x)->I[iof+j] = 2*ai[iof+j];
	    }
	  (*x)->R[iof+j] = 2*ar[iof+j];
	}
      iof += mnel[i];
    }
  for ( i = 0 ; i < *m ; i++ ) 
    (*x)->mnel[i]= mnel[i];
  for ( i = 0 ; i < *nel ; i++) 
    (*x)->icol[i] = icol[i] ;
  return(0);
}


