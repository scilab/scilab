/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef STACK2_H
#define  STACK2_H


#if defined(__SCILAB_TOOLBOX__) && !defined(__INTERNAL_API_SCILAB__) && !defined(__USE_DEPRECATED_STACK_FUNCTIONS__) && !defined(__MEX_INCLUDE__)
    #error Using stack2.h is deprecated. Please use api_scilab instead (try 'help api_scilab'). Note the stack2.h API will be removed after Scilab 6.0. You can define __USE_DEPRECATED_STACK_FUNCTIONS__ to bypass this error.
#endif

#include "machine.h"

/**
 * Convert an Scilab array of double to a fortran format
 *
 * @param ptr the array of double
 * @param size size of the array
 * @param lda
 */
void SciToF77(double *ptr, int size, int lda);

/**
 * Convert an Fortran array of double to a Scilab format
 *
 * @param ptr the array of double
 * @param size size of the array
 * @param lda
 */
void F77ToSci(double *ptr, int size, int lda);
void double2z(double *ptr,  double *ptr77z, int size, int lda);
void z2double(double *ptrz, double *ptrsci, int size, int lda);

/* from int */

extern  int C2F(tpconv)(int *,int *,int *,void *, int *,void *, int *);

/* from system */

extern int C2F(mklist)(int *);
extern int C2F(mklistt)(int *,int *);
extern int C2F(funs)(int *);
extern int C2F(putid)(int *,int *);
extern int C2F(allowptr)(int *);
extern int C2F(ref2val)(void);
extern int C2F(allops)(void);
extern int C2F(putfunnam)(char *fname,int *lw, unsigned long);

/* from somewhere (maybe stack1.h) */

extern void C2F(setfeval)(char *,int *);
extern int C2F(fakecresmat2)(int *lw, int *nchar, int *lr);
/* from stack2.h */
int C2F(isoptlw)(int *topk,int  *lw, char *namex, unsigned long name_len);
int C2F(numopt)(void);

int C2F(convert2sci)(int *);

/* 
 * returns the type of object at position lw in the stack
 * @param lw position
 * @return the type of object at position lw in the stack
 */
int C2F(gettype)(int *lw);


/**
 * int C2F(putlhsvar)()
 *     This function put on the Scilab stack the lhs
 *     variables which are at the position lhsvar(i)
 *     on the calling stack
 *     Warning : this function supposes that the last
 *     variable on the stack is at position top-rhs+nbvars
 * @return
 */
int C2F(putlhsvar)(void);

int C2F(isref)(int *lw);
int C2F(vartype)(int *number);
int C2F(elementtype)(int *lnumber,int *number);

/**
 * Create a variable into the Scilab stack
 * You will probably need to do after a istk(xxxx) = yourvalue;
 * TODO : add a @see to the type of data
 * @param lw Position in the stack
 * @param type__ Type of data ('c','d','r','i','l','b')
 * @param m number of line of the matrix
 * @param n number of columns of the matrix
 * @param[out] lr the address of the data in Scilab Stack
 * @param type_len length of the char *type__
 */
int C2F(createvar)(int *lw, char *type__, int *m, int *n, int *lr, unsigned long type_len);

int C2F(creatework) (int *number,int *m,int  *lr);

int C2F(setworksize) (int *number,int *size);


/**
 * Create a complex variable into the Scilab stack
 * You will probably need to do after a istk(xxxx) = yourvalue;
 * TODO : add a @see to the type of data
 * @example
 * Add the content of comm_size into the stack of Scilab
 * @code
 	int n1=1,m1=1,l1=0;
	int comm_size=42;
	CreateVar(1, "i", &m1, &n1 ,&l1);
	*istk(l1)=comm_size;
 * @endcode
 * @param lw Position in the stack
 * @param type__ Type of data ('c','d','r','i','l','b')
 * @param it if it is complex or not (not sure)
 * @param m number of line of the matrix
 * @param n number of columns of the matrix
 * @param[out] lr the address of the real data in Scilab Stack
 * @param[out] lc the address of the complex data in Scilab Stack
 * @param type_len length of the char *type__
 * @return the result of the operation
 */
int C2F(createcvar)(int *lw, char *type__, int *it, int *m, int *n, int *lr, int *lc, unsigned long type_len);


int C2F(createlist)(int *lw, int *nel);
int C2F(createvarfrom)(int *lw, char *type__, int *m, int *n, int *lr, int *lar, unsigned long type_len);
int C2F(createcvarfrom)(int *lw, char *type__, int *it, int *m, int *n, int *lr, int *lc, int *lar, int *lac, unsigned long type_len);
int C2F(createlistvarfrom)(int *lnumber, int *number, char *type__, int *m, int *n, int *lr, int *lar, unsigned long type_len);

int C2F(getmatdims) (int *number, int *m, int *n);


/**
 * getrhsvar :
 *     get the argument number <<number>>
 *     the argument must be of type type ('c','d','r','i','f','l','b')
 *     return values m,n,lr
 *     c : string  (m-> number of characters and n->1)
 *     d,r,i : matrix of double,float or integer
 *     f : external (function)
 *     b : boolean matrix
 *     l : a list  (m-> number of elements and n->1)
 *         for each element of the list an other function
 *         must be used to <<get>> them
 *     side effects : arguments in the common intersci are modified
 *     see examples in addinter-examples
 * @param number the number of the argument in the function
 * @param type__ the type of the variable
 * @param m number of line of the matrix
 * @param n number of columns of the matrix
 * @param lr the address of the data in Scilab Stack
 * @param type_len length of the char *type__
 * @return the result of the operation
 */
int C2F(getrhsvar)(int *number, char *type__, int *m, int *n, int *lr, unsigned long type_len);


/**
 * getrhsvar :
 *     get the argument number <<number>>  which is a complex matrices
 *     the argument must be of type type ('d','r','i')
 *     return values m,n,lr
 *     d,r,i : matrix of double,float or integer
 *     side effects : arguments in the common intersci are modified
 *     see examples in addinter-examples
 * @param number the number of the argument in the function
 * @param type__ the type of the variable
 * @param m number of line of the matrix
 * @param n number of columns of the matrix
 * @param lr the address of the real data in Scilab Stack
 * @param lc the address of the complex data in Scilab Stack
 * @param type_len length of the char *type__
 * @return the result of the operation
 */
int C2F(getrhscvar)(int *number, char *type__, int *it, int *m, int *n, int *lr, int *lc, unsigned long type_len);


int C2F(getlistrhsvar)(int *lnumber, int *number, char *type__, int *m, int *n, int *lr,  unsigned long type_len);
int C2F(getlistrhscvar)(int *lnumber, int *number, char *type__, int *it, int *m, int *n, int *lr, int *lc, unsigned long type_len);



/**
 * Create a variable into the Scilab stack from a pointer
 * @param lw Position in the stack
 * @param type__ Type of data (i for integer, c for char, b for boolean ...)
 * @param m number of line of the matrix
 * @param n number of columns of the matrix
 * @param iptr the data we want to add into the stack
 * @param type_len length of the char *type__
 */
int C2F(createvarfromptr)(int *number, char *type__, int *m, int *n,void *iptr, unsigned long type_len);


/**
 * Create a complex variable into the Scilab stack from a pointer
 * @param lw Position in the stack
 * @param type__ Type of data (i for integer, c for char, b for boolean ...)
 * @param m number of line of the matrix
 * @param n number of columns of the matrix
 * @param iptr the data we want to add into the stack
 * @param iptc the complex data we want to add into the stack
 * @param type_len length of the char *type__
 */
int C2F(createcvarfromptr)(int *number, char *type__, int *it, int *m, int *n, double *iptr, double *iptc, unsigned long type_len);

int C2F(mklistfromvars)(int *pos, int *n);
int C2F(mktlistfromvars)(int *pos, int *n);
int C2F(mkmlistfromvars)(int *pos, int *n);
int C2F(in2str)(int *n, int *line, char *str, unsigned long str_len);
int C2F(callscifun)(char *string, unsigned long string_len);
int C2F(scifunction)(int *number, int *ptr, int *mlhs, int *mrhs);
int C2F(scistring)(int *ifirst, char *thestring, int *mlhs, int *mrhs, unsigned long thestring_len);
int C2F(scibuiltin)(int *number, int *ifun, int *ifin, int *mlhs, int *mrhs);
int C2F(sciops)(int *number, int *op, int *mlhs, int *mrhs);
int C2F(getrhssys)(int *lw, int *n, int *m, int *p, int *ptra, int *ptrb, int *ptrc, int *ptrd, int *ptrx0, double *h__);
int C2F(errorinfo)(char *fname, int *info, unsigned long fname_len);
int C2F(maxvol)(int *lw, char *type__, unsigned long type_len);

int  C2F(createlistvarfromptr) (int *lnumber, int *number, char *typex, int *m, int *n, void *iptr, long unsigned int type_len);

int C2F(createlistcvarfromptr) (int * lnumber,int * number, char * typex,int *it,int * m,int * n, void *iptr, void * iptc, unsigned long ttype_len);
int C2F(createlistcvarfrom)(int *lnumber, int *number, char *typex, int *it, int *m,int *n,int *lr,int *lc,int *lar,int *lac, unsigned long type_len);

int C2F(createdata)(int *lw, int n);

char * ArgPosition(int i);
char * ArgsPosition(int i,int j);

int IsRef(int lw);
int Ref2val(int from , int to ) ;

void *GetVarPtr(int n);
void *GetData(int lw);
void *GetRawData(int lw);
int GetDataSize(int lw);
void *GetDataFromName( char *name );
int C2F(createreffromname)(int number, char *name );
int C2F(createreference)( int number, int pointed );
int C2F(changetoref)( int number, int pointed );

int check_square(int pos,int m,int n);
int check_vector(int pos,int m,int n);
int check_row(int pos,int m,int n);
int check_col(int pos,int m,int n);
int check_scalar(int pos,int m,int n);
int check_dims(int pos,int m,int n,int m1,int n1);
int check_length(int pos,int m,int n);
int check_same_dims  (int pos1,int pos2,int m1,int n1,int m2,int n2);
int check_dim_prop (int pos1,int pos2,int n);
int check_one_dim  (int pos1,int dim,int val,int valref);

int check_list_square(int lpos,int pos,int m,int n);
int check_list_vector(int lpos,int pos,int m,int n);
int check_list_row(int lpos,int pos,int m,int n);
int check_list_col(int lpos,int pos,int m,int n);
int check_list_scalar(int lpos,int pos,int m,int n);
int check_list_one_dim(int lpos,int pos,int dim,int val,int valref);

int C2F(overload)(int *lw,char *fname,unsigned long l);

#endif
