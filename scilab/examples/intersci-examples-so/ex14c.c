#include "stack-c.h"

/*******************************************
 * simple example with sparse matrix 
 * Display of a 
 *******************************************/
#ifdef __STDC__
extern void sciprint(char *, ...);
#endif    
int F2C(ext14ca)(S)
     SciSparse *S;
{ 
  int i,iof = 0, j;
  for ( i = 0; i < S->m ; i++) 
    {
      sciprint("Row %d, %d non nul elements\r\n",i+1,S->mnel[i]);
      for ( j = 0; j < S->mnel[i]; j++) 
	{
	  if (S->it == 1) 
	      sciprint("(%d,%d) -> %f +i%f\r\n",i+1,S->icol[iof+j],
		       S->R[iof+j],S->I[iof+j]);
	  else 
	      sciprint("(%d,%d) -> %f \r\n",i+1,S->icol[iof+j],
		       S->R[iof+j]);
	}
      iof += S->mnel[i];
    }
  return(0);
}

/*******************************************
 * simple example with sparse matrix 
 * 2*a copied into b 
 *******************************************/

int F2C(ext14cb)(S1,S2) 
     SciSparse *S1,*S2;
{
  int i,iof = 0, j;
  for ( i = 0; i < S1->m ; i++) 
    {
      for ( j = 0; j < S1->mnel[i]; j++) 
	{
	  if ( S1->it == 1) 
	    {
	      S2->I[iof+j] = 2*S1->I[iof+j];
	    }
	  S2->R[iof+j] = 2*S1->R[iof+j];
	}
      iof += S1->mnel[i];
    }
  for ( i = 0 ; i < S1->m ; i++ ) 
    S2->mnel[i]= S1->mnel[i];
  for ( i = 0 ; i < S1->nel ; i++) 
    S2->icol[i] = S1->icol[i] ;
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
 *     x.nel : number of non nul elements in x 
 *     x.it : 0 or 1 i.e. real or complex matrix 
 *     x.mnel[i],i=0,m-1 number of non nul elements in line i+1
 *
 *     x.icol[j],j=0,nel-1 : column of (j+1)th non nul element of the matrix 
 *                           ( stored by rows ) 
 *     x.xr[j],  .....     : real value of the (j+1)th non nul element 
 *     x.xi[j],  .....     : if x.it ==1 imaginary value of the (j+1)th non nul element 
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

#include "stack-c.h"

int F2C(ext14ce)(S1,S2,err)
     SciSparse *S1,**S2;
     int *err;
{ 
  int i,j,iof=0;
  if ((*S2 = NewSparse(&S1->it,&S1->m,&S1->n,&S1->nel)) == (SciSparse *) 0) 
    {
      sciprint("No more space\r\n");
      *err = 1;return 0;
    }
  *err=0;
  for ( i = 0; i < S1->m ; i++) 
    {
      for ( j = 0; j < S1->mnel[i]; j++) 
	{
	  if (S1->it == 1) 
	    {
	      (*S2)->I[iof+j] = 2*S1->I[iof+j];
	    }
	  (*S2)->R[iof+j] = 2*S1->R[iof+j];
	}
      iof += S1->mnel[i];
    }
  for ( i = 0 ; i < S1->m ; i++ ) 
    (*S2)->mnel[i]= S1->mnel[i];
  for ( i = 0 ; i < S1->nel ; i++) 
    (*S2)->icol[i] = S1->icol[i] ;
  return(0);
}


