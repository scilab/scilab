
/* README : 
 * The routines in this file use a pointer to a Matrix 
 * at the Fortran Level this pointer is stored as an integer 
 * so we cast  pointer to long int 
 * f_(fmat) 
 *  long *fmat 
 *  *fmat = (long)spCreate(*n,0,&error);
 * 
 * At fortran level the integer must be an integer *4 
 * in order to store a C long 
 * since we  are in scilab 
 * the pointer transmitted to f_ is an istk(il1) it can in fact contain 
 * something as long as a double 
 * Copyright ENPC (Chancelier)
 */


/*
 *  IMPORTS
 *
 *  >>> Import descriptions:
 *  spConfig.h
 *     Macros that customize the sparse matrix routines.
 *  spmatrix.h
 *     Macros and declarations to be imported by the user.
 *  spDefs.h
 *     Matrix type and macro definitions for the sparse matrix routines.
 */

#define spINSIDE_SPARSE
#include "spConfig.h"
#include "spmatrix.h"
#include "spDefs.h"


#include "../machine.h"

/*
 *
 * lufact1  >>> Creation and LU factorisation of a sparse matrix 
 * Entry << 
 *   val,rc 
 *       arrays of size k and kx2 a(rc(i),rc(*k+i))=val(i)
 *       for i=0,(*k-1)
 *   *n : size of the created square matrix 
 *   *k : number of given values 
 *   *eps : The machine precision number 
 *  Return >> 
 *   fmat : pointer to a long int which is a cast of an adress
 *   *nrank : The numerical rank 
 */


static void 
spFixThresold(eMatrix,eps,releps)
char *eMatrix;
double eps,releps;
{
  MatrixPtr  Matrix = (MatrixPtr)eMatrix;
  Matrix->AbsThreshold = eps;
  Matrix->RelThreshold = releps;
}


static void 
spGetNumRank(eMatrix,n)
char *eMatrix;
int *n;
{
  MatrixPtr  Matrix = (MatrixPtr)eMatrix;
  *n = Matrix->NumRank;
}

extern void cerro();

void 
C2F(lufact1)(val,lln,col,n,nel,fmat,eps,releps,nrank,ierr)
double *val,*eps,*releps;
long *fmat;
int *n,*nel,*nrank,*lln,*col,*ierr;
{
  int error,i,i0,i1,k,j;
  spREAL *pelement;
  *ierr = 0;
  *fmat = (long)spCreate(*n,0,&error);
    if (error != spOKAY) {
        *ierr = 1;
        return;
    }


  i0=0;
  i1=i0;
  i=1;
  for (k = 0 ;k < *nel; k++) {
      i0=i0+1;
      while (i0-i1 > lln[i-1]) {
	  i1=i0;
          i=i+1;
          i0=i0+1;
      }
      j=col[k];
      pelement = spGetElement((char*) *fmat,i,j);
      if (pelement == 0) {
          *ierr=2;
          return;
      }
      spADD_REAL_ELEMENT(pelement,(spREAL)(val[k]));

  }
  /* Fix the AbsThresold with scilex %eps */
  spFixThresold((char*) *fmat,*eps,*releps);
  /* spPrint((char *) *fmat,1,1,1); */
  error = spFactor((char*) *fmat);
  spGetNumRank((char *) *fmat,nrank);
  switch (error) {
  case spZERO_DIAG:
    cerro("zero_diag: A zero was encountered on the diagonal the matrix ");
    break;
  case spNO_MEMORY:
    *ierr=3;
    break;
  case spSINGULAR:
    *ierr=-1; /*Singular matrix" */
    break;
  case spSMALL_PIVOT:
    *ierr=-2; /* matrix is singular at precision level */
    break;
  }
}

/*
 * lusolve1  >>> Solves fmat*x=b 
 *   *fmat : a pointer to the sparse matrix factored by lufact 
 *   b,v 
 *      two arrays of size n the matrix size 
 */
extern void Cout(char *str);

static int 
spSolveCheck(eMatrix)
char *eMatrix;
{
  MatrixPtr Matrix = (MatrixPtr) eMatrix;
  if (Matrix->Error == spSINGULAR OR Matrix->Error == spSMALL_PIVOT )
    {      
	Cout("A matrix is singular or ill conditioned ");
	Cout("result will be fine only if b is in Im(A)");
    }
  return(1);
}

void C2F(lusolve1)(fmat,b,x)
double *b, *x;
long *fmat;
{
/*  if (spSolveCheck((char *) *fmat) != 0)*/
    spSolve((char*) *fmat,(spREAL*)b,(spREAL*)x);
}

/*
 * ludel1  >>> delete sparse matrix 
 *   *fmat : a pointer to the sparse matrix factored by lufact 
 */

void C2F(ludel1)(fmat)
long *fmat;
{
  spDestroy((char*) *fmat);
}

/*
 * lusize  >>> returns in n the size of the sparse matrix 
 *   *fmat : a pointer to the sparse matrix factored by lufact 
 */

static void 
spSize(eMatrix,n)
char *eMatrix;
int *n;
{
  MatrixPtr  Matrix = (MatrixPtr)eMatrix;
  *n=Matrix->Size;
}

void C2F(lusize)(fmat,n)
long *fmat;
int *n;
{
  spSize((char *) *fmat,n);
}

/*
 * luget1   >>> extract the LU coded matrix into a full array 
 *   sigg,sigd :
 *     two arrays of size n which code permutations 
 *   lu :  
 *     an array coded matrix of size nxn where lu will be stored 
 */


/* filling right permutation */
GetSigD(Matrix,indsigd,sigd)
MatrixPtr Matrix;
double sigd[];
int indsigd[];
{
  int I,J,mc=0,last=0;
  int Size=Matrix->Size;
for (I = 1; I <= Size; I++)
  {
    indsigd[I-1]=1;
    indsigd[Size+I-1]=  Matrix->IntToExtColMap[I];
    sigd[I-1]=1.0;
  }
/* counting missing colums */
for (I = 1; I <= Size; I++) 
  if (Matrix->ExtToIntColMap[I]== -1) mc++;
/* filling missing colums */
if (mc != 0)
  {
    for (I = Size -(mc)+1  ; I <= Size; I++)
      {
	for ( J=last+1; J <=Size; J++)
	  {
	    if (Matrix->ExtToIntColMap[J]==-1) 
	      { 
		last=J;break;
	      }
	  }
	indsigd[I-1]=1;
	indsigd[Size+I-1]= last;
      }
  }
}


/* filling left permutation */

GetSigG(Matrix,indsigg,sigg)
MatrixPtr Matrix;
double sigg[];
int indsigg[];
{
  int Size=Matrix->Size;
  int I,J,mc=0,last=0;
  /* counting missing Rows*/
  for (I = 1; I <= Size; I++) 
    if (Matrix->ExtToIntRowMap[I]== -1) mc++;

  for (I = 1; I <= Size-mc ; I++)
  {
	indsigg[I-1]= 1;
	indsigg[Size+Matrix->IntToExtRowMap[I]-1]=I;
	sigg[I-1]=1;
  }
/* filling missing Rows */
if (mc != 0)
  {
    for (I = Size -(mc)+1  ; I <= Size; I++)
      {
	for ( J=last+1; J <=Size; J++)
	  {
	    if (Matrix->ExtToIntRowMap[J]==-1) 
	      { 

		last=J;break;
	      }
	  }
	indsigg[I-1]= 1;
	indsigg[Size+last-1]=I;
	sigg[I-1]=1;
      }
  }
}


static void 
spLuget(eMatrix,indP,P,indl,l,indu,u,indQ,Q)
char *eMatrix;
int *indP,*indl,*indu,*indQ;
double *P,*Q,*l,*u;
{
int I,J;
int lsize,usize;

MatrixPtr Matrix = (MatrixPtr) eMatrix;
ElementPtr  pElement;
int Size;
Size = Matrix->Size;
GetSigD(Matrix,indQ,Q);
GetSigG(Matrix,indP,P);
for (J = 1; J <= Size ; J++)
  {
    indl[J-1] = 0;
    indu[J-1] = 0;
}
lsize=0;
usize=0;
for (I = 1; I <= Size ; I++)
  {
    indu[I-1]=indu[I-1]+1;
    indu[Size+usize]=I;
    u[usize]=1.0;
    usize=usize+1;

    pElement = Matrix->FirstInRow[I];
    while ( pElement != NULL )
      {
	  J = pElement->Col;
	  if (I >= J) {
	      indl[I-1] = indl[I-1]+1;
	      indl[Size+lsize]=J;
	      l[lsize]=(double) pElement->Real ;
	      lsize=lsize+1;

	  }
	  else {
	      indu[I-1] = indu[I-1]+1;
	      indu[Size+usize]=J;
	      u[usize]=(double) pElement->Real ;
	      usize=usize+1;

	  }
	pElement = pElement->NextInRow;
      };
  };
}





void C2F(luget1)(fmat,indP,P,indl,l,indu,u,indQ,Q)
long *fmat;
double *P,*Q,*l,*u;
int *indP,*indl,*indu,*indQ;
{
  spLuget((char *) *fmat,indP,P,indl,l,indu,u,indQ,Q);
}


/*
 * lusiz1   >>> extract the L and U  number of non zero elements
 * lsize and usize
 */


static void 
spLusiz(eMatrix,lsize,usize)
char *eMatrix;
int *lsize,*usize;
{
int J;
MatrixPtr Matrix = (MatrixPtr) eMatrix;
ElementPtr  pElement;
int Size;
Size = Matrix->Size;
*lsize=0;
*usize=Size;
for (J = 1; J <= Size ; J++)
  {
    pElement = Matrix->FirstInCol[J];
    while ( pElement != NULL )
      {
	if (pElement->Row >= J) 
	    *lsize=*lsize+1;
	else
	    *usize=*usize+1;
	pElement = pElement->NextInCol;
    };
  };
}  
void C2F(lusiz1)(fmat,lsize,usize)
long *fmat;
int *lsize,*usize;
{
  spLusiz((char *) *fmat,lsize,usize);
}
