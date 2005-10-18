#include "../stack-c.h"


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>



#define EPSILON -HUGE_VAL

extern int Howard _PARAMS((int *ij, double *A,int nnodes,int narcs,double *chi,double *v,int *pi,int *NIterations,int *NComponents,int verbosemode));

extern int Semi_Howard _PARAMS((int *ij, double *A,double *t,int nnodes,int narcs,double *chi,double *v,int *pi,int *NIterations,int *NComponents,int verbosemode));


  int narcs=0;
  int *ij=NULL;
  double *a=NULL;
  double *tSTEPH=NULL; 
  double *v=NULL;
  double *chi=NULL;
  int *pi=NULL;
  int *NIterations=NULL;
  int *NComponents=NULL;
  int entry=0;

int iHoward(fname)
  char* fname;
{ 
  int l1, l2, l3, l4, l5, l6, l7,l8,I;
  int m1, n1, m2, n2, m3, n3, nnodes,ncolumns,verbosemode,errorflag;


  static int minlhs=5, minrhs=3, maxlhs=5, maxrhs=3;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "i", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "i", &m3, &n3, &l3);
  nnodes=*istk(l3);
  narcs=m2;
  ij=istk(l1);
  /* in Howard... matrices are numbered starting from zero */
  for (I=0;I<2*narcs; I++)
    {
      ij[I]=ij[I]-1;
    }
  a=stk(l2);
  ncolumns=1;
  CreateVar(4, "d", &nnodes, &ncolumns, &l4);
  CreateVar(5, "d", &nnodes, &ncolumns, &l5);
  CreateVar(6, "i", &nnodes, &ncolumns, &l6);
  CreateVar(7, "i", &ncolumns, &ncolumns,&l7);
  CreateVar(8, "i", &ncolumns, &ncolumns,&l8);
  chi=stk(l4);
  v=stk(l5);
  pi=istk(l6);
  NIterations=istk(l7);
  NComponents=istk(l8);
  verbosemode=0;
  errorflag =Howard(ij, a, nnodes, narcs, chi, stk(l5), pi, NIterations,NComponents,verbosemode);
  switch (errorflag)
	 {
	 case 1:
	     sciprint("Error in Howard: input matrix has one empty row");
	     Error(999);
	     return(0);
	 case 3:
  	     sciprint("Error in Howard: number of nodes must be a positive integer");
	     Error(999);
	     return(0);
	 case 4:
  	     sciprint("Error in Howard: number of arcs must be a positive integer");
	     Error(999);
	     return(0);
	 case 5:
	     sciprint("AARRGH... error in Howard: maximal number of iterations is reached\n");
	     Error(999);
	     return(0);
	   }
  LhsVar(1)=4;
  LhsVar(2)=5;
  LhsVar(3)=6;
  LhsVar(4)=8;
  LhsVar(5)=7;
  /* restoring the matrix */
  for (I=0;I<2*narcs; I++)
    {
      ij[I]=ij[I]+1;
    }
  PutLhsVar();
  return(0);
}

/* HOWARD ALGORITHM FOR DETERMINISTIC SEMI MARKOV PROCESSES */
int iSemi_Howard(fname)
  char* fname;
{ 
  int l1, l2, l3, l4, l5, l6, l7,l8,l9,I;
  int m1, n1, m2, n2, m3, n3, m4,n4,nnodes,ncolumns,verbosemode,errorflag;


  static int minlhs=5, minrhs=4, maxlhs=5, maxrhs=4;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "i", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "i", &m3, &n3, &l3);
  GetRhsVar(4, "d", &m4, &n4, &l4);
  nnodes=*istk(l3);
  narcs=m2;
  ij=istk(l1);
  /* in Howard... matrices are numbered starting from zero */
  for (I=0;I<2*narcs; I++)
    {
      ij[I]=ij[I]-1;
    }
  a=stk(l2);
  tSTEPH=stk(l4);
  ncolumns=1;
  CreateVar(5, "d", &nnodes, &ncolumns, &l5);
  CreateVar(6, "d", &nnodes, &ncolumns, &l6);
  CreateVar(7, "i", &nnodes, &ncolumns, &l7);
  CreateVar(8, "i", &ncolumns, &ncolumns,&l8);
  CreateVar(9, "i", &ncolumns, &ncolumns,&l9);
  chi=stk(l5);
  v=stk(l6);
  pi=istk(l7);
  NIterations=istk(l8);
  NComponents=istk(l9);
  verbosemode=0;
  errorflag =Semi_Howard(ij, a, tSTEPH, nnodes, narcs, chi, stk(l6), pi, NIterations,NComponents,verbosemode);
  switch (errorflag)
	 {
	 case 1:
	     sciprint("Error in Semi Howard: input matrix has one empty row");
	     Error(999);
	     return(0);
         case 2: 
	     sciprint("Error in Semi Howard: weight matrix has a circuit with non positive weight");
	     Error(999);
	     return(0);
	 case 3:
  	     sciprint("Error in Semi Howard: number of nodes must be a positive integer");
	     Error(999);
	     return(0);
	 case 4:
  	     sciprint("Error in Semi Howard: number of arcs must be a positive integer");
	     Error(999);
	     return(0);
	 case 5:
	     sciprint("AARRGH... Semi error in Howard: maximal number of iterations is reached\n");
	     Error(999);
	     return(0);
	   }
  LhsVar(1)=5;
  LhsVar(2)=6;
  LhsVar(3)=7;
  LhsVar(4)=9;
  LhsVar(5)=8;
  /* restoring the matrix */
  for (I=0;I<2*narcs; I++)
    {
      ij[I]=ij[I]+1;
    }
  PutLhsVar();
  return(0);
}

/* KARP  */
extern void Karp _PARAMS((int *ij,double *A,int nnodes, int narcs, int entry, double *u));

int iKarp(fname)
  char* fname;
{ 
  int l1, l2, l3, l4, l5, I;
  int m1, n1, m2, n2, m3, n3, m4,n4, nnodes,ncolumns,entry;


  static int minlhs=1, minrhs=4, maxlhs=1, maxrhs=4;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "i", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "i", &m3, &n3, &l3);
  GetRhsVar(4, "i", &m4, &n4, &l4);
  nnodes=*istk(l3);
  entry=*istk(l4)-1;
  narcs=m2;
  ij=istk(l1);
  /* ... matrices are numbered starting from zero */
  for (I=0;I<2*narcs; I++)
    {
      ij[I]=ij[I]-1;
    }
  a=stk(l2);
  ncolumns=1;
  CreateVar(5, "d", &ncolumns, &ncolumns, &l5);
  Karp(ij, a, nnodes, narcs, entry,stk(l5));
  LhsVar(1)=5;
  /* restoring the matrix */
  for (I=0;I<2*narcs; I++)
    {
      ij[I]=ij[I]+1;
    }
  PutLhsVar();
   return(0);
}

/* sparse2full */
int isparse2full(fname)
  char* fname;
{ 
  int l1, l2, l3, l4, I;
  int m1, n1, m2, n2, m3, n3,  nnodes,ncolumns;

  static int minlhs=1, minrhs=3, maxlhs=1, maxrhs=3;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "i", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "i", &m3, &n3, &l3);
  nnodes=*istk(l3);
  narcs=m2;
  ij=istk(l1);
  a=stk(l2);
  ncolumns=1;
  CreateVar(4, "d", &nnodes, &nnodes, &l4);
  for (I=0; I<nnodes*nnodes; I++ )
    {
      stk(l4)[I]=-HUGE_VAL;
    }
  for (I=0; I<narcs; I++)
    {
      stk(l4)[(ij[2*I]-1)+ (ij[2*I+1]-1)*nnodes]=a[I];
    }
  LhsVar(1)=4;
  PutLhsVar();
  return(0);
}

/* in_span */
extern int in_span _PARAMS((double *A,int n, int p, double *b, double precision));

int iin_span(fname)
  char* fname;
{ 
  int l1, l2, l3, l4;
  int m1, n1, m2, n2, m3, n3;
  int size;
  static int minlhs=1, minrhs=3, maxlhs=1, maxrhs=3;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  size=1;
  CreateVar(4, "i", &size, &size, &l4);
  *istk(l4)=in_span(stk(l1),m1,n1,stk(l2),*stk(l3));
  LhsVar(1)=4;
  PutLhsVar();
  return(0);
}

/* weakbasis */
extern void weakbasis _PARAMS((double *A,int n,int p, double **B, int *q, double precision));

int iweakbasis(fname)
  char* fname;
{ 
  int l1, l2, l3;
  int m1, n1, m2, n2;
  int q,i;
  double **B;
  static int minlhs=1, minrhs=2, maxlhs=1, maxrhs=2;
  Nbvars = 0;
  B=(double **)calloc(1, sizeof(double*));
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  weakbasis(stk(l1),m1,n1,B,&q,*stk(l2));
  CreateVar(3, "d", &m1, &q, &l3);
  for (i=0; i<m1*q; i++)
    {
      stk(l3)[i]=(*B)[i];
    }
  LhsVar(1)=3;
  PutLhsVar();
  free(*B); /* BUG TO BE CORRECTED */
  free(B);
  return(0);
}
/* weakbasis */
extern void weakbasis2 _PARAMS((double *A,int n,int p, double **B, int *q, double precision));

int iweakbasis2(fname)
  char* fname;
{ 
  int l1, l2, l3;
  int m1, n1, m2, n2;
  int q,i;
  double **B;
  static int minlhs=1, minrhs=2, maxlhs=1, maxrhs=2;
  Nbvars = 0;
  B=(double **)calloc(1, sizeof(double*));
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  weakbasis2(stk(l1),m1,n1,B,&q,*stk(l2));
  CreateVar(3, "d", &m1, &q, &l3);
  for (i=0; i<m1*q; i++)
    {
      stk(l3)[i]=(*B)[i];
    }
  LhsVar(1)=3;
  PutLhsVar();
  free(*B); /* BUG TO BE CORRECTED */
  free(B);
  return(0);
}
/* include_span */
extern int include_span _PARAMS((double *A,int n, int p, double *B, int q, double precision));

int iinclude_span(fname)
  char* fname;
{ 
  int l1, l2, l3,l4;
  int m1, n1, m2, n2,m3,n3;
  int size;
  static int minlhs=1, minrhs=3, maxlhs=1, maxrhs=3;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  size=1;
  CreateVar(4, "i", &size, &size, &l4);
  if (m1 != m2)
    {
      cerro("ERROR cannot compare column spaces since number of rows differ\n");
    }
  *istk(l4)=include_span(stk(l1),m1,n1,stk(l2),n2,*stk(l3));
  LhsVar(1)=4;
  PutLhsVar();
  return(0);
}

extern void STproduct _PARAMS((double *A, int n, int p, double *B, int q, double *C));

int iproduct(fname)
  char* fname;
{ 
  int l1, l2, l3;
  int m1, n1, m2, n2;
  static int minlhs=1, minrhs=2, maxlhs=1, maxrhs=2;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  if (n1!= m2)
    {
      cerro("ERROR : incompatible dimension in matrix product");
    }
  CreateVar(3, "d", &m1, &n2, &l3);
  STproduct(stk(l1),m1,n1,stk(l2),n2,stk(l3));
  LhsVar(1)=3;
  PutLhsVar();
  return(0);
}

extern void rowbasis _PARAMS((double *A,int n,double *B,double **U, int *q, double precision));

int irowbasis(fname)
  char* fname;
{ 
  int l1, l2, l3,l4;
  int m1, n1, m2, n2,m3,n3;
  int q,i;
  double **U;
  static int minlhs=1, minrhs=3, maxlhs=1, maxrhs=3;
  Nbvars = 0;
  U=(double **)calloc(1, sizeof(double*));
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  rowbasis(stk(l1),n1,stk(l2),U,&q,*stk(l3));
    CreateVar(4, "d", &n1, &q, &l4);
  if (q>0)
    {
      for (i=0; i<n1*q; i++)
	{
	  stk(l4)[i]=(*U)[i];
	}
    }
  LhsVar(1)=4;
  PutLhsVar();
  /*free(*U);*/ /* BUG TO BE CORRECTED */
  free(*U);
  free(U);
  return(0);
}

/* weakbasis */

extern int solve2 _PARAMS((double *A, int n, int p, double *B,double **U,double precision));

extern print_matrix(double *A, int n, int p);

int isolve2(fname)
  char* fname;
{ 
  int l1, l2, l3,l4;
  int m1, n1, m2, n2,m3,n3;
  int q,i;
  double *B;
  static int minlhs=1, minrhs=3, maxlhs=1, maxrhs=3;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  B=(double *)calloc(1, sizeof(double));
  q=solve2(stk(l1),m1,n1,stk(l2),&B,*stk(l3));
  /* printf("OKOK\n");
  printf(" VALEURS %g",(B)[0]);
  print_matrix(B,n1,q);
  printf("OKOKOK\n");*/
  CreateVar(4, "d", &n1, &q, &l4);
  for (i=0; i<n1*q; i++)
    {
      stk(l4)[i]=B[i];
    }
  LhsVar(1)=4;
  PutLhsVar();
  free(B);
  return(0);
}
extern int solve3 _PARAMS((double *A, int n, int p, double *B,double **U,double precision));


int isolve3(fname)
  char* fname;
{ 
  int l1, l2, l3,l4;
  int m1, n1, m2, n2,m3,n3;
  int q,i;
  double *B;
  static int minlhs=1, minrhs=3, maxlhs=1, maxrhs=3;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  B=(double *)calloc(1, sizeof(double));
  q=solve3(stk(l1),m1,n1,stk(l2),&B,*stk(l3));
  CreateVar(4, "d", &n1, &q, &l4);
  for (i=0; i<n1*q; i++)
    {
      stk(l4)[i]=B[i];
    }
  LhsVar(1)=4;
  PutLhsVar();
  free(B);
  return(0);
}

extern void matrix_star _PARAMS((double *B, int n, double *C));

int istar(fname)
  char* fname;
{ 
  int l1, l2;
  int m1, n1;
  static int minlhs=1, minrhs=1, maxlhs=1, maxrhs=1;
  Nbvars = 0;
/*  printf("in istar/1\n");*/
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
/*  printf("in istar/2\n");*/
  GetRhsVar(1, "d", &m1, &n1, &l1);
/*  printf("in istar/3\n");*/
  if (m1!= n1)
    { 
      sciprint("Error in star: number of rows and columns differ");
      Error(999);
      return(0);
    }
/*  printf("in istar/4\n");*/
  CreateVar(2, "d", &m1, &m1, &l2);
/*  printf("in istar/5\n");*/
  matrix_star(stk(l1),m1,stk(l2));
/*  printf("in istar/6\n");*/
  LhsVar(1)=2;
/*  printf("in istar/7\n");*/
  PutLhsVar();
/*  printf("in istar/8\n");*/
  return(0);
}

extern void matrix_plus _PARAMS((double *B, int n, double *C));

int iplus(fname)
  char* fname;
{ 
  int l1, l2;
  int m1, n1;
  static int minlhs=1, minrhs=1, maxlhs=1, maxrhs=1;
  Nbvars = 0;
/*  printf("in istar/1\n");*/
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
/*  printf("in istar/2\n");*/
  GetRhsVar(1, "d", &m1, &n1, &l1);
/*  printf("in istar/3\n");*/
  if (m1!= n1)
    { 
      sciprint("Error in star: number of rows and columns differ");
      Error(999);
      return(0);
    }
/*  printf("in istar/4\n");*/
  CreateVar(2, "d", &m1, &m1, &l2);
/*  printf("in istar/5\n");*/
  matrix_plus(stk(l1),m1,stk(l2));
/*  printf("in istar/6\n");*/
  LhsVar(1)=2;
/*  printf("in istar/7\n");*/
  PutLhsVar();
/*  printf("in istar/8\n");*/
  return(0);
}

extern int FordBellman _PARAMS((int *ij, double *A, int nnodes, int narcs, int entry, double *u,int *policy, int *niterations));


int iFordBellman(fname)
  char* fname;
{ 
  int l1, l2, l3, l4, l5, l6, l7, l8, I, i;
  int m1, n1, m2, n2, m3, n3, m4, n4, ncolumns, nnodes, narcs, errorflag;

  static int minlhs=1, minrhs=4, maxlhs=3, maxrhs=4;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1, "i", &m1, &n1, &l1); /* tail head matrix */
  GetRhsVar(2, "d", &m2, &n2, &l2); /* weight vector */
  GetRhsVar(3, "i", &m3, &n3, &l3); /* nnodes */
  GetRhsVar(4, "i", &m4, &n4, &l4); /* entry */
  nnodes=*istk(l3);
  narcs=n2;
  /*  ij=istk(l1); */
  /*  a=stk(l2); */
  ncolumns=1;
  entry=*istk(l4);
  CreateVar(5, "d", &nnodes, &ncolumns, &l5);
  CreateVar(6, "i", &nnodes, &ncolumns, &l6);
  CreateVar(7, "i", &ncolumns, &ncolumns, &l7);
  /*  pi=istk(l6); */
  for (i=0;i<narcs*2; i++)
    {
      istk(l1)[i]=istk(l1)[i]-1;
    }
    errorflag =FordBellman(istk(l1),stk(l2),nnodes,narcs,entry-1,stk(l5),istk(l6),istk(l7)); 
    /* restoring the indices */    
   for (i=0;i<narcs*2; i++)
    {
      istk(l1)[i]=istk(l1)[i]+1;
    }    
   for (i=0;i<nnodes; i++)
    {
      istk(l6)[i]=istk(l6)[i]+1;
      } 
   switch (errorflag)
	 {
	 case 2:
	     sciprint("Error in FordBellman: initial point is out of range");
	 case 1:
	     sciprint("Error in FordBellman: the digraph has a circuit with positive weight");
	     Error(999);
	     return(0);
	   }
  LhsVar(1)=5;
  LhsVar(2)=6;
  LhsVar(3)=7;
  PutLhsVar();
  return(0);
}




static TabF Tab[]={ 
  {iHoward, "Howard"},   {iSemi_Howard, "Semi_Howard"}, {iKarp, "Karp"}, {isparse2full, "Max_Full"}, {iin_span, "in_span"}, {iweakbasis, "weakbasis"},  {iinclude_span, "include_span"}, {iproduct, "product"}, {irowbasis, "rowbasis"}, {isolve2, "solve2"}, {isolve3, "solve3"}, {iweakbasis2, "weakbasis2"}, {istar, "star"}, {iplus, "plus"}, {iFordBellman, "hFordBellman"}
};

int C2F(maxplusc)()
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name);
  return 0;
}




