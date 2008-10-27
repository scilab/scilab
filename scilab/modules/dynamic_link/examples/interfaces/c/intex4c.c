/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file is released into the public domain
 */

#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

#include "machine.h"
#include "stack-c.h" 


/**************************************************
 * examples of an hand written interface 
 * Shows how to create Scilab variable from a C array 
 * (of int, float or double type). This is mainly used 
 * when the arrays are allocated in the C-code and we 
 * want to return the allocated arrays as Scilab objects
 **************************************************/

/*------------------------------------------------------
 * Creating a scilab variable from a pointer
 * intex4c_1 : a pointer to a double array 
 * intex4c_2 : a pointer to an int  array 
 * intex4c_3 : a pointer to a  char array (i.e a string) 
 * intex4c_4 : 3,4,5 in one interface 
 * The interfaced functions are at the end of this file 
 *-------------------------------------------------------*/

int C2F(dblearray)(double **a, int *m, int *n,int *err);
int C2F(intarray) (int **a, int *m, int *n,int *err);
int C2F(crestr) (char **a, int *m,int *err);

/*------------------------------------------------------
 * intex4c_1 : a pointer to a double array 
 *-------------------------------------------------------*/

int intex4c(char* fname)
{ 
  static double l2[] ={ 1.0,2.0,3.0}, *ptr2= l2;
  double *l1;
  int m1,n1,ierr=0,m2=3,n2=1;
  CheckRhs(0,0);
  CheckLhs(1,2);
  C2F(dblearray)( &l1,&m1,&n1,&ierr);
  if ( ierr > 0 )  { Scierror(999,"%s: Internal error\n",fname);  return 0;  }
  /* first example : l1 was allocated inside dblearray */ 
  CreateVarFromPtr( 1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
  
  free(l1); /* freeing space allocated inside dblearray */ 
  /* second example : l2 is defined here */
  CreateVarFromPtr( 2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &ptr2);

  LhsVar(1) = 1;
  LhsVar(2) = 2;
  return(0);
}


/*------------------------------------------------------
 * intex4c_2 : a pointer to an int  array 
 *-------------------------------------------------------*/

int intex4c_2(char* fname)
{ 
  int *l1;
  int m1,n1,ierr=0;

  CheckRhs(0,0);
  CheckLhs(1,1);
  C2F(intarray)( &l1, &m1, &n1, &ierr);
  if ( ierr > 0 )  { Scierror(999,"%s: Internal error\n",fname);  return 0 ;  }
  CreateVarFromPtr( 1,MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
  free(l1);
  /* we can access and change data through VarPtr
   * but it does not work for variables of type 's' (sparse) and 'S' 
   * (string matrices)
   */
  *istk(VarPtr(1)) = 12;
  LhsVar(1) = 1;
  return(0);
}

/*------------------------------------------------------
 * intex4c_3 : a pointer to a  char array (i.e a string) 
 *-------------------------------------------------------*/

int intex4c_3(char* fname)
{ 
  char *l1;
  int m1,n1=1,ierr=0;

  CheckRhs(0,0);
  CheckLhs(1,1);
  C2F(crestr)( &l1,&m1,&ierr);
  if ( ierr > 0 )  { Scierror(999,"%s: Internal error\n",fname);  return 0;  }
  CreateVarFromPtr( 1,STRING_DATATYPE, &m1, &n1, &l1);
  free(l1);
  LhsVar(1) = 1;
  return(0);
}


/*------------------------------------------------------
 * intex4c_4 :
 *-------------------------------------------------------*/

int intex4c_4(char* fname)
{ 
  char *l1;
  int *l2;
  double *l3;

  int m1,n1=1,m2,n2,m3,n3,ierr=0;

  CheckRhs(0,0);
  CheckLhs(1,4);
  C2F(crestr)( &l1, &m1, &ierr);
  C2F(intarray)( &l2, &m2, &n2, &ierr);
  C2F(dblearray)( &l3, &m3, &n3, &ierr);
  if ( ierr > 0 )  { Scierror(999,"%s: Internal error\n",fname);  return 0;  }
  /* Creating variable #1 of type character and dimension m1 x n1 (m1=1) */
  CreateVarFromPtr( 1,STRING_DATATYPE, &m1, &n1, &l1);
  CreateVarFromPtr( 2,MATRIX_OF_INTEGER_DATATYPE, &m2, &n2, &l2);
  CreateVarFromPtr( 3,MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);
  /* Free  memory */
  free(l1);  free(l2);   free(l3);
  /* we change the first value of each variable */
  *cstk(VarPtr(1)) = 'X';
  *istk(VarPtr(2)) = 12;
  *stk(VarPtr(3)) = 89.0;
  /* Returning 3 variables (#1, #2, #3) to Scilab  */
  LhsVar(1) = 1;
  LhsVar(2) = 2;
  LhsVar(3) = 3;
  return(0);
}

/*--------------------------------------------------------------
 *  Example of array created by C code
 *  converted to Scilab output variable
 *---------------------------------------------------------------*/

/*     double array    */

int C2F(dblearray)(double **a,int *m,int *n,int*ierr)

{
  int i ;
  *n=5 ;
  *m=3 ;
  *a= ( double *) malloc( (unsigned) (*m)*(*n) *sizeof(double));
  if ( *a != (double *) 0)   
    {
      *ierr=0;
      for ( i= 0 ; i < (*m)*(*n) ; i++) (*a)[i] = i+1;
    }
  else 
    *ierr=1;
  return(0);
}


/*     integer array    */

int C2F(intarray)(int **a,int *m,int *n,int *ierr) 
{
  int i ;
  *n=5 ;
  *m=3 ;
  *a= ( int *) malloc( (unsigned) (*m)*(*n) *sizeof(int));
  if ( *a != (int *) 0)   
    {
      *ierr=0;
      for ( i= 0 ; i < (*m)*(*n) ; i++) (*a)[i] = i+1;
    }
  else 
      *ierr=1;
  return(0);
}

/*************************************************************
 * Example of character string created here.
 * Its length is sent back as an output.
 * It is converted into Scilab variable in the interface program.
 * The allocated array is freed in the interface program.
 *************************************************************/

#define MYSTR "Scilab is ..."

int C2F(crestr)(char **a,int *m,int *ierr)
{
  *m= strlen(MYSTR);
  *a= (char *) malloc((unsigned) (*m+1)*sizeof(char));
  if ( *a != (char *) 0) 
    {
      *ierr=0;
      sprintf((*a),MYSTR);
    }
  else
    {
      *ierr=1;
      printf("malloc : No more space\n");
    }
  return(0);
}

