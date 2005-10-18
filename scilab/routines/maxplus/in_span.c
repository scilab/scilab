/* returns true if b is in the column span of A */
/* n= number of rows of A */
/* p= number of columns of A */
/* precision: equality test are performed up to the "precision" value  */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#define EPSILON -HUGE_VAL
#define INF HUGE_VAL

#define min(M,N) (M > N ? N : M)
#define max(M,N) (M > N ? M : N)
#define notzero(x) (x>testepsilon)

void print_matrix(double *B,int n,int q)
{
  int k,l;
  for (k=0;k<n;k++)
    {
      for (l=0;l<q; l++)
	{ printf("%g\t",B[k+ n*l]);
	}
      printf("\n");
    }
  }

void  print_vector(double *b,int n)
  {
    int k;
    for (k=0; k<n; k++)
    printf("%g\t",b[k]);
    printf("\n");
  }


int in_span(double *A, int n, int p, double *b, double precision)
{
int *saturated;
int i,j;
int J=0;
int answer=1;
double colmin;
double testepsilon=EPSILON+precision;

saturated=(int *)calloc(n, sizeof(int));
if (saturated==NULL)
    {
      printf("error in in_span... cannot allocate vector of size %d\n",n);
      exit(1);
    }

for (i=0;i<n; i++)
  {
    if (b[i]<=EPSILON+precision)
      saturated[i]=1;
  }

for (j=0;j<p; j++)
  {
    colmin=INF;
    for (i=0; i<n; i++)
      {
	if ( notzero(A[J+i]) && (b[i]-A[J+i] <= colmin ))
	  {
	    colmin=(b[i]-A[J+i]);
	  }
      }
    if (colmin!=INF)/* otherwise the column J of A must be zero */
      {
	for (i=0; i<n; i++)
	  {
	    if (b[i]-A[J+i] <= colmin +precision)
	      {
		saturated[i]=1;
	      }
	  }
      }
    J=J+n;
  }
for (i=0;i<n; i++)
  {
    if (saturated[i]==0)
      answer=0;
  }
free(saturated);
return(answer);
}

/* return a minimal generating family of the column space of A */
/* A input matrix */
/* n number of rows, p number of columns */
/* q=number of generators=number of columns of B */
/* result in B */
void weakbasis(double *A,int n,int p, double **B, int *q, double precision)
{
double *C;
double *b;
double m;
int Q,R,i,j,k,J,K,LAST,S;
  if (p<1)
    {
      printf("error in weakbasis, announced number of columns must be at least 1\n");
     exit(1);
    }
C=(double *)calloc(n*p, sizeof(double));
b=(double *)calloc(n, sizeof(double));
if ((C==NULL) || (b==NULL))
    {
      printf("error in weakbasis... cannot allocate memory\n");
      exit(1);
    }
/* begin first pass */
printf("entering weakbasis\n");
for (i=0;i<n; i++)
  {
    C[i]=A[i];
  }
Q=1;
J=n;
K=n;
for (j=1; j<p; j++)
  {
    for (i=0;i<n; i++)
      {
	b[i]=A[J+i];
      }
    if (in_span(A,n,Q,b,precision) ==0)
      {
	Q++;
        for (k=0; k<n;k++)
	  {
	    C[K+k]=A[J+k];
	  }
	K=K+n;	
      }
    J=J+n;
  }
printf("end of first pass in weakbasis: n generators<= %d\n",Q);
/* end of first pass */
/* we know that the number of generators is at most Q */
/* and that the last generator is not a combination  */
/* of the other ones */
LAST=K-n;
S=Q-1;
/* column 1---> b, last column --> column 1 */
/*printf("STARTING SECOND PASS\n");*/
for (i=0; i<n; i++)
      { 
	b[i]=C[i];
	C[i]=C[LAST+i];
      }
R=n;
j=1;
while (j<=S)
  { 
    /*
    printf("current column j=%d R=%d end S=%d\n",j,R,S); printf("b= ");
    for (i=0; i<n; i++)
      {
	printf("%g ",b[i]);
      }
    printf("\n");
    printf("S=%d, C= \n", S);
    {int k,l;
    for (k=0;k<S; k++)
      { 
	for (l=0; l<n; l++)
	  { printf("%g ", C[n*k+l]);
	  }
        printf("\n");
      }
    }
    */
    if (in_span(C,n,S, b,precision)==0)
      { 
	j++;
 	/* swap column j (starting at R) and the column b  */
	    for (i=0; i<n; i++)
	      { m=C[R+i];
	        C[R+i]=b[i];
		b[i]=m;
	      }
	R=R+n;
      }
    else
      { 	    	
	/*	printf("eliminating column %d\n",j); */
	S--;
	LAST=LAST-n;
	for (i=0; i<n; i++)
	  { 
	    b[i]=C[R+i];
	    C[R+i]=C[LAST+i];
	  }
      }
  }
printf("end of second pass in weakbasis: n generators =%d\n",S+1);
/* it remains to generate the output */
*B=(double *)calloc(n*(S+1), sizeof(double));
if (*B==NULL)
    {
      printf("error in weakbasis... cannot allocate memory for the output\n");
      exit(1);
    }
for (i=0; i<n*(S+1); i++)
  {
       (*B)[i]=C[i];
  }
*q=S+1;
free(b);
free(C);
}

/* return a minimal generating family of the column space of A */
/* A input matrix */
/* n number of rows, p number of columns */
/* q=number of generators=number of columns of B */
/* result in B */
void weakbasis2(double *A,int n,int p, double **B, int *q, double precision)
{
double *C;
double *b;
double m;
 int Q,R,i,j,J,K,LAST,jj,ii;
  if (p<1)
    {
      printf("error in weakbasis, announced number of columns must be at least 1\n");
     exit(1);
    }
C=(double *)calloc(n*p, sizeof(double));
b=(double *)calloc(n, sizeof(double));
if ((C==NULL) || (b==NULL))
    {
      printf("error in weakbasis... cannot allocate memory\n");
      exit(1);
    }
printf("entering weakbasis2\n");
for (i=0;i<n; i++)
  {
    C[i]=A[i];
  }
Q=1;
J=n;
K=n;
for (j=1; j<p; j++)
  {
    for (i=0;i<n; i++)
      {
	b[i]=A[J+i];
      }
    if (in_span(A,n,Q,b,precision) ==0)
      {
        for (i=0; i<n;i++)
	  {
	    b[i]=C[i];
	    C[i]=A[J+i];
	  }
	R=n;
	jj=1;
	LAST=Q*n;
	/*	printf("step %d/%d, n generators=%d\n",j,p,Q);*/
	while (jj<=Q)
	  { 
	    if (in_span(C,n,Q, b,precision)==0)
	      { 
		jj++;
		/* swap column j (starting at R) and the column b  */
		for (ii=0; ii<n; ii++)
		  { m=C[R+ii];
		  C[R+ii]=b[ii];
		  b[ii]=m;
		  }
		R=R+n;
	      }
	    else
	      { 	    	
		/*	printf("eliminating column %d\n",j); */
		Q--;
		LAST=LAST-n;
		for (ii=0; ii<n; ii++)
		  { 
		    b[ii]=C[R+ii];
		    C[R+ii]=C[LAST+ii];
		  }
	      }
	  }
	Q++;
	K=K+n;	
      }
    J=J+n;
  }
/* it remains to generate the output */
*B=(double *)calloc(n*(Q), sizeof(double));
if (*B==NULL)
    {
      printf("error in weakbasis... cannot allocate memory for the output\n");
      exit(1);
    }
for (i=0; i<n*(Q); i++)
  {
       (*B)[i]=C[i];
  }
*q=Q;
free(b);
free(C);
}


/* returns 1 if Im B is included Im A */
int include_span(double *A,int n, int p, double *B, int q, double precision)
{
double *b;
int i,j,flag;
b=(double *)calloc(n, sizeof(double));
if ((b==NULL))
    {
      printf("error in include_span... cannot allocate memory\n");
      exit(1);
    }
flag=1;
i=0;
while (flag && i<q)
  { for (j=0; j<n; j++)
      b[j]=B[n*i+j];
    if (in_span(A,n,p,b,precision)==0)
      {
	flag=0;
	return(flag);
      /*      printf("col %d of RHS not included in LHS\n", j);
      for (j=0; j<n; j++)
	printf("%g ",b[j]);
      printf("\n");*/
      }
    i++;
  }
return(flag);
}
/*
bbasis3:=proc(A,B,U)
local n,p,a,b,aa,bb,V,UU,UUU,AA,BB;
# finds {Ux | Ax=Bx} 
n:=linalg[rowdim](A);
p:=linalg[coldim](A);
a:=linalg[submatrix](A,1..1,1..p);
b:=linalg[submatrix](B,1..1,1..p);
aa:=evald(a&p U);
bb:=evald(b&p U);
V:=basis3(aa,bb);
if nops(V)=0 then RETURN([]) fi;
UU:=evald(U &p V);
print(`entering weakbasis `.(linalg[rowdim](UU)).` x `.(linalg[coldim](UU)));
uu:=weakbasis(UU);
print(`card basis=`.(nops(uu)));
UUU:=linalg[submatrix](UU,1..(linalg[rowdim](UU)),uu);
if n=1 then
	RETURN(op(UUU));
fi;
AA:=linalg[submatrix](A,2..n,1..p);
BB:=linalg[submatrix](B,2..n,1..p);
print(``.`invoking bbasis3 `.(nops(uu)).` generators`);
bbasis3(AA,BB,UUU);
end:
*/

void rowbasis(double *A,int n,double *B,double **U, int *q, double precision)
{ double *V;
  int i,j,k,R,Q;
  V=(double *)calloc(n*n*n, sizeof(double));
  if ((V==NULL))
    {
      printf("error in weakbasis... cannot allocate memory\n");
      exit(1);
    }
  R=0;
  Q=0;
  /*  printf("\n");*/
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      { 
	/* printf("i=%d j=%d \n",i,j);*/
	if ((B[i]<=A[i]+precision) && (A[j]<=B[j]+precision))
	  { /*	   printf("generator\n ");*/
	    Q++;
	    for (k=0; k<n; k++)
	      {
		V[R+k]=EPSILON;
	      }
	    V[R+i]=B[j];
	    V[R+j]=A[i];
	    R=R+n;
	  }
      }
if (Q>0)
  {
    *U=(double *)calloc(n*Q,sizeof(double));
    if (*U==NULL)
    {
      printf("error in rowbasis... cannot allocate memory for the output\n");
      exit(1);
    }
    for (i=0;i<n*Q; i++)
      (*U)[i]=V[i];
  }
*q=Q;
free(V);
}
/* returns the number of generators */
int rowbasis2(double *A,int n,double *B,double **U, double precision)
{ double *V;
  int i,j,k,R,Q;
  V=(double *)calloc(n*n*n, sizeof(double));
  if ((V==NULL))
    {
      printf("error in weakbasis... cannot allocate memory\n");
      exit(1);
    }
  R=0;
  Q=0;
  /*  printf("\n");*/
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      { 
	/* printf("i=%d j=%d \n",i,j);*/
	if ((B[i]<=A[i]+precision) && (A[j]<=B[j]+precision))
	  { /*	   printf("generator\n ");*/
	    Q++;
	    for (k=0; k<n; k++)
	      {
		V[R+k]=EPSILON;
	      }
	    V[R+i]=B[j];
	    V[R+j]=A[i];
	    R=R+n;
	  }
      }
  /*if (Q>0)
  {  
       weakbasis(V,n,Q,U,&Q,precision);
  }*/
  /*free(V);*/
*U=V;
return(Q);
}

int rowbasis3(double *A,int n,double *B,int p,double *U, double **UU, double precision)
{ double *V;
  int i,j,k,R,Q;
  V=(double *)calloc(p*n*n, sizeof(double));
  if ((V==NULL))
    {
      printf("error in row basis... cannot allocate memory\n");
      exit(1);
    }
  R=0;
  Q=0;
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      { 
	if ((B[i]<=A[i]+precision) && (A[j]<=B[j]+precision) )
	  { 
	    Q++;
	    if (A[i]>EPSILON+precision)
		  {
		    for (k=0; k<p; k++)
		    V[R+k]=max(U[k+i*p]+ B[j]-A[i], U[k+j*p]);
		  }
	    else
	      {
		for (k=0; k<p; k++)
		    V[R+k]=U[k+i*p]+ B[j];
	      }
	    R=R+p;
	  }
      }
*UU=V;
return(Q);
}

void STproduct(double *A, int n, int p, double *B, int q, double *C)
{ int i,I,j,k,K,N;
  double m;
  for (i=0; i<n; i++)
    {K=0;N=0;
    for (j=0; j<q;j++)
      {
	I=i;
	m=A[I]+B[K];
	for (k=1;k<p; k++)
	  { I=I+n;
	    if (A[I]+B[K+k]>m)
	      m=A[I]+B[K+k];
	  }
	C[i+N]=m;
	/*	printf("C[%d,%d]=%g\n",i,j,m);*/
	K=K+p;
	N=N+n;
      }
    }
}

double scalar_star(double a)
{
  if (a<=0)
    {
      return(0);
    }
  else
    {
      return(INF);
    }
}

double secure_max_add(double a,double b,double c,double d)
{
/* max(a,b+c+d) with infinite handling */
 if (b<=EPSILON)
   {
     return(a);
   }
 else
   {
     if (c<=EPSILON)
       {
	 return(a);
       }
     else
       {
	 if(d<=EPSILON)
	   {
	     return(a);
	   }
	 else
	   {
	     if(b>=INF)
	       {
	       return(INF);
	     }
	     else
	       {
		 if (c>=INF)
		   {
		     return(INF);
		   }
		 else
		   {
		     if (d>=INF)
		       {
		       return(INF);
		     }
		     else
		       {
			 return(max(a,b+c+d));
		       }
	       }
	       }
	   }
       }
   }
}

void matrix_star(double *b, int n, double *c)
{ int k,i,j;
  double t;
  for (k=0;k<n*n;k++)
    {
      c[k]=b[k];
    }
  for (k=0; k<n; k++)
    {
	t=scalar_star(c[k+n*k]);
/*	printf("t= %d",t);*/
	for (i=0;i<n;i++)
	  {
	    for (j=0;j<n;j++)
	      {
		c[i+n*j]=secure_max_add(c[i+n*j], c[i+n*k],c[k+n*j],t);
	      }
	  }
      }
   for (k=0;k<n;k++)
     {
       c[k+n*k]=max(c[k+n*k], 0);
     }
}


void matrix_plus(double *b, int n, double *c)
{ int k,i,j;
  double t;
  for (k=0;k<n*n;k++)
    {
      c[k]=b[k];
    }
  for (k=0; k<n; k++)
    {
	t=scalar_star(c[k+n*k]);
/*	printf("t= %d",t);*/
	for (i=0;i<n;i++)
	  {
	    for (j=0;j<n;j++)
	      {
		c[i+n*j]=secure_max_add(c[i+n*j], c[i+n*k],c[k+n*j],t);
	      }
	  }
      }
}

void product_line(int i, double *A, int n, int p, double *B, int q, double *C)
{ int I,j,k,K;
  double m;
    {K=0;
    for (j=0; j<q;j++)
      {
	I=i;
	m=A[I]+B[K];
	for (k=1;k<p; k++)
	  { I=I+n;
	    if (A[I]+B[K+k]>m)
	      m=A[I]+B[K+k];
	  }
	C[j]=m;
	K=K+p;
      }	
    }
}

/* returns the number of solutions */
/* solve is numerically unstable it should not be used */
int solve(double *A, int n, int p, double *B, double *C, int r, double **U,double precision)
{ int q,i,j;
  double *a,*b, *aa, *bb,*AA,*BB,*UU;
  double **V,**UUU;
printf("ENTERING solve\n");
if (n<=0)
  return(0);
a=(double *)calloc(p, sizeof(double));
b=(double *)calloc(p, sizeof(double));
aa=(double *)calloc(r, sizeof(double));
bb=(double *)calloc(r, sizeof(double));
for (i=0;i<p;i++)
  {
    a[i]=A[n*i];
    b[i]=B[n*i];
  }
printf("C=\n");
print_matrix(C,p,r);
printf("a= \t");
print_matrix(a,1,r);
printf("\nb= \t");
print_matrix(b,1,r);
STproduct(a,1,p, C, r, aa);
STproduct(b,1,p, C, r, bb);
printf("aa= \t");
print_matrix(aa,1,r);printf("\n");
printf("bb= \t");print_matrix(bb,1,r);
V=(double **)calloc(1, sizeof(double*));
q=rowbasis2(aa,r,bb,V,precision);
if (q<=0)
  {
    free(bb);free(aa);free(b);free(a);
    return(0);
  }
printf("generator set of the first hyperplane=\n");
print_matrix(*V,r,q);
if (n==1)
  {
    UU=(double *)calloc(p*q, sizeof(double));
    STproduct(C,p,r,*V,q,UU);
    weakbasis(UU,p,q,U,&q,precision);
    return(q);
  }
UU=(double *)calloc(p*q, sizeof(double));
STproduct(C,p,r,*V,q,UU);
UUU=(double **)calloc(1, sizeof(double*));
weakbasis(UU,p,q,UUU,&q,precision);
printf("reduced generator set of the first hyperplane=\n");
print_matrix(*UUU,p,q);
AA=(double *)calloc((n-1)*p, sizeof(double));
BB=(double *)calloc((n-1)*p, sizeof(double));
for (i=1;i<n;i++)
  {
    for (j=0;j<p;j++)
      {
	AA[(i-1)+(n-1)*j]=A[i+n*j];
	BB[(i-1)+(n-1)*j]=B[i+n*j];
      }
  }
printf("calling solve with\n");
print_matrix(AA,n-1,p);
print_matrix(BB,n-1,p);
q=solve(AA,n-1,p,BB,*UUU,q,U,precision);
free(BB);
free(AA);
free(UU);
free(*UUU);
return(q);
}

int solve2(double *A, int n, int p, double *B, double **SOLUTIONS, double precision)
{ int q,qnew,i;
  double *a,*b,*UU,*U;
  double **V,**Z;
  printf("ENTERING solve2\n");
if ((n<=0) || (p<=0))
  return(0);
  U=(double *)calloc(p*p,sizeof(double));
  for (i=0;i<p*p;i++)
    U[i]=EPSILON;
  for (i=0;i<p;i++)
    U[i*p+i]=0;
q=p;
for (i=0; i<n; i++)
  {
    a=(double *)calloc(q, sizeof(double));
    b=(double *)calloc(q, sizeof(double));
    product_line(i,A,n,p,U,q,a);
    product_line(i,B,n,p,U,q,b);
    Z=(double **)calloc(1, sizeof(double*));
    printf("ENTERING rowbasis\n");
    qnew=rowbasis2(a,q,b,Z,precision);
    printf("DONE\n");
    if (qnew==0)
      {
	printf("Exit of solve 2: empty set of solutions\n");
        return(0);
      }
    UU=(double *)calloc(p*qnew,sizeof(double));
    STproduct(U,p,q,*Z,qnew,UU);
    V=(double **)calloc(1, sizeof(double*));
    printf("step %d before number of generators =%d\n",i,qnew);
    weakbasis2(UU,p,qnew,&U,&q,precision);/* weakbasis2 false*/
    printf("step %d number of generators =%d\n",i,q);
    free(V);
    free(UU);
    free(Z);
    free(b);
    free(a);
  }
printf("Exit of solve 2: %d solutions found\n",q);
/*print_matrix(U,p,q);*/
*SOLUTIONS=U;
return(q);
}

double norm(double *U, int n, int p)
{
double m;
int i;
m=EPSILON;
for (i=0; i<n*p; i++)
m=max(m,U[i]);
return(abs(m));
}

int solve3(double *A, int n, int p, double *B, double **SOLUTIONS, double precision)
{ int q,qnew,i;
  double *a,*b,*U;
  double **Z;
  printf("ENTERING solve3\n");
if ((n<=0) || (p<=0))
  return(0);
  U=(double *)calloc(p*p,sizeof(double));
  for (i=0;i<p*p;i++)
    U[i]=EPSILON;
  for (i=0;i<p;i++)
    U[i*p+i]=0;
q=p;
for (i=0; i<n; i++)
  {
    a=(double *)calloc(q, sizeof(double));
    b=(double *)calloc(q, sizeof(double));
    product_line(i,A,n,p,U,q,a);
    product_line(i,B,n,p,U,q,b);
    Z=(double **)calloc(1, sizeof(double*));
    /*    printf("ENTERING rowbasis\n");*/
    qnew=rowbasis3(a,q,b,p,U,Z,precision);
    printf("DONE\n");
    if (qnew==0)
      {
	printf("Exit of solve 3: empty set of solutions\n");
        return(0);
      }
    /*    printf("step %d before number of generators =%d\n",i,qnew);*/
    weakbasis2(*Z,p,qnew,&U,&q,precision);/*weakbasis2 false*/
    /*    printf("step %d number of generators =%d\n",i,q);*/
    free(*Z);
    free(Z);
    free(b);
    free(a);
  }
printf("Exit of solve 3: %d solutions found\n",q);
/*print_matrix(U,p,q);*/
*SOLUTIONS=U;
return(q);
}


