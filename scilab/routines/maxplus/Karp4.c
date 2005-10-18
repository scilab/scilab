#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define max(M,N) (M > N ? M : N)
#define min(M,N) (M > N ? N : M)

#define EPSILON -HUGE_VAL
#define INF HUGE_VAL

/*  Computes the entry ``entry'' of the cycle time
    of the sparse matrix (ij,A,nnodes,narcs)
    using Karp's algorithm. Result in u 
    
    Requires : O(nnodes^2) SPACE
           O(nnodes*narcs) TIME
	  
	   The input must be a max-plus matrix with at
	   least one finite entry per row.
*/

void Karp(int *ij,double *A,int nnodes, int narcs, int entry, double *u)
{  
  double *V;
  int i,j,k;
  double v;
  int SIZE,SMALLSIZE,jnnodes,newjnnodes;
  SMALLSIZE=nnodes*nnodes;
  SIZE=SMALLSIZE+nnodes;
    V=(double *)calloc(SIZE,sizeof(double));
    if (V==NULL)
    {
      printf("Error in Karp... memory allocation failed...\n");
      printf("You may try Karp_Big, which is twice slower\n");
      printf("but only requires a linear space\n");
      exit(1);
    }
    for (i=0; i<SIZE; i++)
	{
	  V[i]=EPSILON;  
	}
    V[entry]=0;
    jnnodes=0;
    for (j=2;j<=nnodes+1;j++)
	{ 
	  newjnnodes=jnnodes+nnodes;
	  for (i=0;i<narcs; i++)
	    {
	       V[ij[i*2+1]+ newjnnodes]=max(A[i]+V[ij[i*2]+jnnodes],V[ij[i*2+1]+newjnnodes]);
	    }
	  jnnodes=newjnnodes;
	}
  *u=EPSILON;
  for (i=0;i<nnodes;i++)
    { if (V[SMALLSIZE+i] != EPSILON) 
	{ v=INF;
      	  for (k=0; k<nnodes; k++)
	    {
		  v=min(v,(V[SMALLSIZE+ i]-V[nnodes*k+i])/(nnodes-k));
	    }
      *u=max(*u,v);
	}
    }
  free(V);
}



/* Karp algorithm for very large matrices. Twice slower than
the original Karp's algorithm above, but requires only O(nnodes) space */

void Karp_Big(int *ij,double *A,int nnodes, int narcs, int entry, double *u)
{  
  double *V,*W,*LAST,*TABLE;
  int i,j;
    V=(double *)calloc(nnodes,sizeof(double));
    W=(double *)calloc(nnodes,sizeof(double));
    LAST=(double *)calloc(nnodes,sizeof(double));
    TABLE=(double *)calloc(nnodes,sizeof(double));
    if ( (V==NULL) || (W==NULL) || (LAST==NULL) || (TABLE==NULL))
    {
      printf("Error in Karp_Big... memory allocation failed...\n");
      exit(1);
    }
    for (i=0; i<nnodes; i++)
	{
	  V[i]=EPSILON;  
	  W[i]=EPSILON;
	}
    V[entry]=0;
    for (j=0;j<nnodes;j++)
	{ 
	  for (i=0;i<narcs; i++)
	    {
	     W[ij[i*2+1]]=max(A[i]+V[ij[i*2]],W[ij[i*2+1]]);
	    }
	  /* copying the new value */
	  for (i=0; i<nnodes; i++)
	    {
	      V[i]=W[i];
	      W[i]=EPSILON;
  	    }
	}
    /* yA^n has been computed */
    /* We initialize the table */ 
    for (i=0; i<nnodes; i++)
	    {
	      LAST[i]=V[i];
	      V[i]=EPSILON;
	      TABLE[i]=INF;
	    }
    V[entry]=0;
    if (LAST[entry] != EPSILON)
      TABLE[entry] =LAST[entry]/nnodes;
    /* we compute the sequence yA^k, k< =n a second time, and
       generate the vector of differences min_k (yA^k-yA^n) on the fly.
       This is the price to pay (in time) 
       to perform the computations in linear space
       */
    for (j=1;j<nnodes;j++)
	{ 
	  for (i=0;i<narcs; i++)
	    {
	       W[ij[i*2+1]]=max(A[i]+V[ij[i*2]],W[ij[i*2+1]]);
	    }
	  /* copying the new value */
	  for (i=0; i<nnodes; i++)
	    {
	      V[i]=W[i];
	      W[i]=EPSILON;
	      if (LAST[i] !=EPSILON)
		{
		  TABLE[i]=min(TABLE[i],(LAST[i]-V[i])/(nnodes -j));
		}
	    }
	}

  *u=EPSILON;
  for (i=0;i<nnodes;i++)
    {
      if (LAST[i] !=EPSILON)
	{
	  *u=max(*u,TABLE[i]);
	}
    }
  free(TABLE);
  free(LAST);
  free(W);
  free(V);
}





