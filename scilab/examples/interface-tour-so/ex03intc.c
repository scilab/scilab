#include <string.h> 
#include <stdio.h>
#include "stack-c.h"

/**************************************************
 * examples of an hand written interface 
 * Shows how to pass 
 *       - Scilab boolean matrices as arguments 
 *       how to create and return new boolean matrices
 **************************************************/

/*-------------------------------------------------
 * GetRhsVar(..,'b',...) and CreateVar(..,'b',...
 *-------------------------------------------------*/

static void not __PARAMS((int * iv1,int *iv2,int  size));

int intex3c_1(fname) 
     char *fname;
{
  static int l1,m1, n1,l2;
  static int minlhs=1, minrhs=1, maxlhs=1, maxrhs=1;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* a boolean matrix argument */ 
  
  GetRhsVar(1, "b", &m1, &n1, &l1);

  /* create a new boolean matrix same size as first argument 
   *
   * note that this is only for illustration since we could directly work 
   * with istk(l1) which can be modified and returned. 
   */

  CreateVar(2, "b",&m1,&n1,&l2);

  /* fills new matrix  */
  
  not(istk(l2),istk(l1),m1*n1);

  LhsVar(1) = 2; /* returns new matrix */
  return 0;
}


static void not( iv1,iv2, size)
     int *iv1,*iv2;
     int size;
{
  int i;
  for ( i=0 ; i < size ; i++) iv1[i] = ! iv2[i];
}

/*---------------------------------------------
 * GetRhsVar(...,'b',...) 
 *---------------------------------------------*/

int intex3c_2(fname) 
     char *fname;
{
  static int l1,m1, n1;
  static int minlhs=1, minrhs=1, maxlhs=1, maxrhs=1;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* a boolean matrix argument */ 
  
  GetRhsVar(1, "b", &m1, &n1, &l1);

  /* we modify the first argument (in fact a copy of the transmited 
   * argument since arguments are transmited by value 
   */ 

  not(istk(l1),istk(l1),m1*n1);

  LhsVar(1) = 1; /* return the modified version of argument */

  return 0;
}

/*---------------------------------------------
 * CreateVarFrom(...,'b',...) 
 *---------------------------------------------*/

int intex3c_3(fname) 
     char *fname;
{
  static int l1,m1, n1,l2;
  static int minlhs=1, minrhs=1, maxlhs=1, maxrhs=1;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* a boolean matrix argument */ 
  
  GetRhsVar(1, "b", &m1, &n1, &l1);

  /* we make a copy of first argument 
   * the new array can be accessed through 
   * istk(l2). Again as pointed out in the first 
   * example this is just for illustrative purpose 
   * since GetRhsVar(1,..) gave us a pointer to a copy 
   * of transmited argument. 
   */ 

  CreateVarFrom(2,"b",&m1,&n1,&l2, &l1);

  LhsVar(1) = 2; /* return the copy */

  return 0;
}




