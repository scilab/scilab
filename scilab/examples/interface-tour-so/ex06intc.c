#include "stack-c.h"

/**************************************************
 * examples of an hand written interface 
 * Shows how to pass  Scilab sparse matrices 
 *       how to create and return new sparse matrices
 *       how to pass  Scilab intxx matrices 
 *       how to create and return new intxx matrices
 * 
 * The way sparse matrix are coded at C level is 
 * described in stack-c.h in the structure 
 * note that GetRhsVar(..,"s",...) returns a 
 * C-structure which shares data with Scilab 
 * stack through pointers, thus the size of data 
 * cannot be changed. 
 **************************************************/

int intex6c_1(fname)
  char* fname;
{ 
  int m1,n1,i,m,n;
  SciSparse S;
  CheckRhs(1,1);
  CheckLhs(1,3);

  /* we expect a sparse matrix as first argument 
   * the sparse matrix data is stored in S object 
   */ 

  GetRhsVar(1,"s",&m1,&n1,&S);

  /* we change data i.e A -> 2*real(A)+3*imag(A); 
   * Note that this transformation does not change 
   * the number and position of non-null elements of A
   * and thus does not change the size of data 
   * reserved in the Scilab stack. 
   * In order to change the internal size 
   * of a Sparse matrix one has to create a new 
   * object (see next example) 
   */

  for ( i=0 ; i < S.nel ; i++ ) 
      S.R[i] *= 2.0; 
  if ( S.it == 1) 
    for ( i=0 ; i < S.nel ; i++ ) 
      S.I[i] *= 3.0; 
  m=1,n=S.nel;
  CreateVarFromPtr(2,"i",&m,&n,&S.icol);
  m=1,n=m1;
  CreateVarFromPtr(3,"i",&m,&n,&S.mnel);
  LhsVar(1) =1;
  LhsVar(2) =2;
  LhsVar(3) =3;
  return(0);
}


/* 
 * Here we create a new sparse matrix in Scilab stack 
 * CreateVarFromPtr
 */


int intex6c_2(fname)
  char* fname;
{ 
  static int mnel[5] ={0,2,3,1,1};
  static int icol[7] ={1,2,3,1,2,3,5};
  static double R[7] ={1.0,2.0,3.0,4.0,5.0,6.0,7.0};
  static SciSparse S ={ 5,5,0,7, mnel,icol,R,NULL};
  CheckRhs(0,0);
  CheckLhs(1,1);
  CreateVarFromPtr(1,"s",&S.m,&S.n,&S);
  LhsVar(1) =1;
  return(0);
}


/**************************************************** 
 * int matrix: 
 * here we use a SciIntMat* as third argument of GetRhsVar 
 * M.it gives the type and M.D points to the data of 
 * Scilab objects.
 ******************************************************/

int intex6c_3(fname)
  char* fname;
{ 
  int m1,n1,l1,inc=1,zero=0,mn,l3,l4;
  SciIntMat M;
  CheckRhs(1,1);
  CheckLhs(1,3);

  /* 
   * we expect an int matrix as first argument 
   * data is accessed through M.D which has to be properly casted 
   * for example if M.it == I_UCHAR (see stack-c.h for the I_XXX ) 
   * then data is accessed through ((uchar *) M.D)[i] 
   * or using macros  IC_UCHAR(M.D)[i] (see stack-c.hfor the IC_XXX)
   */ 

  GetRhsVar(1,"I",&m1,&n1,&M);
  mn = m1*n1;  

  /* we create a double matrix of the same size */
  
  CreateVar(2,"d",&m1,&n1,&l1);

  /* we use a utility function fron the int package 
   * to convert from int* to double 
   */ 

  C2F(tpconv)(&M.it,&zero,&mn, M.D, &inc, stk(l1), &inc);

  /* we copy the int matrix 
   * l3 on entry must give the type (M.it) 
   * and on return istk(l3) will points to the new data area 
   * if l4 != -1 then istk(l4) must give a pointer to data which 
   * is to be used for creation. 
   * both l3 and l4 are changed 
   */
  CreateVarFrom(3,"I",&m1,&n1,(l3=M.it,&l3),(l4=M.l,&l4));

  /* we copy the int matrix using an other method */

  CreateVarFromPtr(4,"I",&m1,&n1,&M); 

  /* we return the double matrix and the two copies  */ 

  LhsVar(1) =2 ;
  LhsVar(2) =3 ;
  LhsVar(3) =4 ;
  return(0);
}

/* creation of an int matrix */

int intex6c_4(fname)
  char* fname;
{ 
  int m1,n1,l1,l2,typ,inc=1,zero=0,mn;

  CheckRhs(1,1);
  CheckLhs(1,1);

  /* 
   * we expect a double matrix 
   */ 

  GetRhsVar(1,"d",&m1,&n1,&l1);
  
  /* we create an int matrix of uint16 type  
   * l1 is used on entry to give the type 
   * and as a return value. For example 
   * if l1 = I_UINT16 on entry, data can be accessed 
   * through IC_UINT16(istk(l2))[i]
   * 
   */
  l2 = typ= I_UINT16;
  CreateVar(2,"I",&m1,&n1,&l2);
  mn = m1*n1;

  /* we use a utility function fron the int package 
   * to convert from double to uint8
   * since tpconv will perfrom the cast we can transmit 
   * istk(l2) but we could also write IC_UINT8(istk(l2))
   */ 

  C2F(tpconv)(&zero,&typ,&mn,stk(l1), &inc,istk(l2), &inc);

  /* we return the new uint16 matrix  matrix */ 
  LhsVar(1) =2 ;
  return(0);
}









