#include <stdio.h>

void Display_Policy(int *pi,double *c,int nnodes)
{
int i;
printf("DISPLAYING POLICY nnodes=%d\n",nnodes);
for (i=0; i<nnodes; i++)
  { 
    printf("pi(%d)=%d; c(%d)=%g\n",i,pi[i],i,c[i]);
  }
printf("END OF DISPLAY POLICY\n");
}

void Display_Semi_Policy(int *pi,double *c,double *tau, int nnodes)
{
int i;
printf("DISPLAYING POLICY nnodes=%d\n",nnodes);
for (i=0; i<nnodes; i++)
  { 
    printf("pi(%d)=%d; c(%d)=%g, tau(%d)=%g \n",i,pi[i],i,c[i],i,tau[i]);
  }
printf("END OF DISPLAY POLICY\n");
}


void Display_Max_Policy(int *pi,int nnodes)
{
int i;
for (i=0; i<nnodes; i++)
  { 
    printf("pi(%d)=%d\n",i,pi[i]);
  }
}



void Display_Vector(int nnodes, double *v)
{
int i;
for (i=0; i<nnodes; i++)
  {
    printf("value(%d)= %g\n",i,v[i]);
  }
}

void Display_Vector_String(int nnodes, double *v,char *s)
{
int i;
for (i=0; i<nnodes; i++)
  {
    printf(s);
    printf("(%d)= %g\n",i,v[i]);
  }
}

void Display_Sparse_Matrix(int narcs, int *ij,double *A)
{
  int i;
  for (i=0;i<narcs; i++)
    {
      printf("A[%d,%d]=%g\n",ij[i*2],ij[i*2+1],A[i]);
    }
}

void Display_Semi_Sparse_Matrix(int narcs, int *ij,double *A, double *t)
{
  int i;
  for (i=0;i<narcs; i++)
    {
      printf("A[%d,%d]=%g tau=%g\n",ij[i*2],ij[i*2+1],A[i],t[i]);
    }
}

void Display_Min_Max_Function(int narcs, int *ij,double *A,int *P)
{
  int i;
  printf("DISPLAYING THE MINMAX FUNCTION\n");
  for (i=0;i<narcs; i++)
    {
      printf("A[%d,%d]=%g Policy=%d\n",ij[i*2],ij[i*2+1],A[i],P[i]);
    }
  printf("DONE\n");
}




