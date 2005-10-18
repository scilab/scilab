#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#ifndef RAND_MAX
#define RAND_MAX 2147483647
#endif

void Make_Rand_Sparse_Matrix(int nnodes, int N,int *ij,double *A)
{
  int i,j;
  int k=0;
  double v;
    for (i=0;i<nnodes; i++)
    {
      for (j=0;j<N; j++)
	{
	  ij[k*2]=i;
	  v=rand();
	  ij[k*2+1]=(int)(nnodes*v/(RAND_MAX+1.0));
	  v=rand();
	  /* 	  A[k]=floor(v*2/(RAND_MAX+1.0));*/
	  A[k]=(v*100/(RAND_MAX+1.0));
	  k++;
	}
    }
}
void Semi_Make_Rand_Sparse_Matrix(int nnodes, int N,int *ij,double *A, double *t)
{
  int i,j;
  int k=0;
  double v;
    for (i=0;i<nnodes; i++)
    {
      for (j=0;j<N; j++)
	{
	  ij[k*2]=i;
	  v=rand();
	  ij[k*2+1]=(int)(nnodes*v/(RAND_MAX+1.0));
	  v=rand();
	  /* 	  A[k]=floor(v*2/(RAND_MAX+1.0));*/
	  A[k]=(v*100/(RAND_MAX+1.0));
	  v=rand();
	  /* 	  A[k]=floor(v*2/(RAND_MAX+1.0));*/
	  /* t[k]= 2;*/
	  t[k]=(v+1)*100/(RAND_MAX+1.0);
/*	  printf("time =%g v=%g RAND_MAX=%g\n",t[k],v,RAND_MAX);*/
	  k++;
	}
    }
}

void Make_Rand_Min_Max(int nnodes, int NPOLICIESPERNODE,int NARCSPERNODEANDPOLICY,int *ij,double *A,int *P,int *U)
{
  int i,j,l;
  int k=0;
  double v;
    for (i=0;i<nnodes; i++)
    {
      U[i]=NPOLICIESPERNODE;
      for (l=0;l<NPOLICIESPERNODE; l++)
	{
	for (j=0;j<NARCSPERNODEANDPOLICY; j++)
	  {
	    ij[k*2]=i;
	    v=rand();
	    ij[k*2+1]=(int)(nnodes*v/(RAND_MAX+1.0));
	    v=rand();
	    A[k]=floor((v*2/(RAND_MAX+1.0)));
	    P[k]=l;
	    k++;
	  }
	}
    }
}
void Generate_lp_file(int narcs, int *ij,double *A)
{
  int i;
  printf("PROBLEM DESCRIPTION\n");
  printf("t-z;\n");
  for (i=0;i<narcs; i++)
      {
	printf("%f +x%d-y%d< z-t + x%d-y%d;\n",A[i],ij[i*2+1],ij[i*2+1],ij[i*2],ij[i*2]);
      }
}

