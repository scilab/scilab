/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2002 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 1998-2007 - INRIA - Serge STEER
 * Copyright (C) 2004-2007 - INRIA - Allan CORNET
 * Copyright (C) 2006-2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 */
/*    Scilab Memory Management library (Stack API) */
#include <string.h>
#include "stack-c.h"
#include "stack1.h"
#include "stack2.h"
#include "sciprint.h"
#include "cvstr.h"
#include "localization.h"
#include "Scierror.h"

/* Table of constant values */

static int cx0 = 0;
static int cx1 = 1;
static int cx4 = 4;
static int c_true = TRUE;
static int c_false = FALSE;

/* Static function declarations ... Function only used here */
static int C2F(getwsmati)(char * fname, int *topk, int * spos,int * lw,int * m, int *n,int * ilr,int * ilrd ,int * inlistx,int* nel,unsigned long fname_len);
static int C2F(gethmati)(char *fname,int *topk,int *spos,int *lw,int *m,int *n,int *lr,int *inlistx,int *nel,unsigned long fname_len);
static int C2F(getsmati) (char *fname, int *topk, int *spos, int *lw, int *m, int *n, int *i__, int *j, int *lr, int *nlr, int *inlist__, int *nel, unsigned long fname_len);
static int C2F(getpointeri) (char *fname, int *topk, int *spos, int *lw, int *lr, int *inlist__, int *nel, unsigned long fname_len);
static int C2F(getimati)(char *fname, int *topk, int *spos, int *lw, int *it, int *m, int *n, int *lr, int *inlistx, int *nel, long unsigned int fname_len);
static int C2F(getbmati) (char *fname, int *topk, int *spos, int *lw, int *m, int *n, int *lr, int *inlist__, int *nel, unsigned long fname_len);
static int C2F(cremati)(char *fname,int *stlw,int *it,int *m,int *n,int *lr,int *lc,int *flagx,unsigned long fname_len);
static int C2F(getmati)(char *fname,int *topk,int *spos,int *lw,int *it,int *m,int *n,int *lr,int *lc,int *inlistx,int *nel,unsigned long fname_len);
static int C2F(crebmati)(char *fname,int *stlw,int *m,int *n,int *lr,int *flagx,unsigned long fname_len);
static int C2F(crehmati)(char *fname,int *stlw,int *m,int *n,int *lr,int *flagx,unsigned long fname_len);
static int C2F(crepointeri)(char *fname,int *stlw,int *lr,int *flagx,unsigned long fname_len);
static int C2F(cresparsei)(char *fname,int *stlw,int *it,int *m,int *n,int *nel,int *mnel,int *icol,int *lr,int *lc,unsigned long fname_len); 
static int C2F(getsparsei)(char *fname,int *topk,int *spos,int *lw,int *it,int *m,int *n,int *nel,int *mnel,int *icol,int *lr,int *lc,int *inlistx,int *nellist,unsigned long fname_len);


int C2F(getrsparse)(char *fname, int *topk, int *lw, int *m, int *n,  int *nel, int *mnel, int *icol, int *lr,unsigned long fname_len);
int C2F(getlistsmat)(char *fname,int *topk,int *spos,int *lnum,int *m,int *n,int *ix,int *j,int *lr,int *nlr,unsigned long fname_len);
int cre_smat_from_str_i(char *fname, int *lw, int *m, int *n, char *Str[],unsigned long fname_len, int *rep);
int cre_sparse_from_ptr_i(char *fname, int *lw, int *m, int *n, SciSparse *S, unsigned long fname_len,  int *rep);
int crelist_G(int *slw,int *ilen,int *lw,int type);

/**********************************************************************
 * MATRICES 
 **********************************************************************/

/*------------------------------------------------------------------ 
 * getlistmat : 
 *    checks that spos object is a list 
 *    checks that lnum-element of the list exists and is a matrix 
 *    extracts matrix information(it,m,n,lr,lc) 
 *     In  : 
 *       fname : name of calling function for error message 
 *       topk  : stack ref for error message 
 *       lw    : stack position 
 *     Out : 
 *       [it,m,n] matrix dimensions 
 *       lr : stk(lr+i-1)= real(a(i)) 
 *       lc : stk(lc+i-1)= imag(a(i)) exists only if it==1 
 *------------------------------------------------------------------ */

int C2F(getlistmat)(char *fname,int *topk,int *spos,int *lnum,int *it,int *m,int *n,int *lr,int *lc,unsigned long fname_len)
{
  int nv, ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE )
    return FALSE;

  if (*lnum > nv) {
    Scierror(999,_("%s: argument %d should be a list of size at least %d.\n"),get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }
  return C2F(getmati)(fname, topk, spos, &ili, it, m, n, lr, lc, &c_true, lnum, fname_len);
} 

/*------------------------------------------------------------------- 
 * getmat :
 *     check that object at position lw is a matrix 
 *     In  : 
 *       fname : name of calling function for error message 
 *       topk  : stack ref for error message 
 *       lw    : stack position ( ``in the top sense'' )
 *     Out : 
 *       [it,m,n] matrix dimensions 
 *       lr : stk(lr+i-1)= real(a(i)) 
 *       lc : stk(lc+i-1)= imag(a(i)) exists only if it==1 
 *------------------------------------------------------------------- */

int C2F(getmat)(char *fname,int *topk,int *lw,int *it,int *m,int *n,int *lr,int *lc,unsigned long fname_len)
{
  return C2F(getmati)(fname, topk, lw,Lstk(*lw), it, m, n, lr, lc, &c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------
 * getrmat like getmat but we check for a real matrix 
 *------------------------------------------------------------------ */

int C2F(getrmat)(char *fname,int *topk,int *lw,int *m,int *n,int *lr,unsigned long fname_len)
{
  int lc, it;

  if ( C2F(getmat)(fname, topk, lw, &it, m, n, lr, &lc, fname_len) == FALSE )
    return FALSE;

  if (it != 0) {
    Scierror(202,_("%s: Wrong type for argument %d: Real matrix expected.\n"),get_fname(fname,fname_len), Rhs + (*lw - *topk));
    return FALSE;
  }
  return TRUE;
}
/* ------------------------------------------------------------------
 * getcmat like getmat but we check for a complex matrix
 *------------------------------------------------------------------ */

int C2F(getcmat)(char *fname,int *topk,int *lw,int *m,int *n,int *lr,unsigned long fname_len)
{
  int lc, it;

  if ( C2F(getmat)(fname, topk, lw, &it, m, n, lr, &lc, fname_len) == FALSE )
    return FALSE;

  if (it != 1) {
    Scierror(202,_("%s: Wrong type for argument %d: Real matrix expected.\n"),get_fname(fname,fname_len), Rhs + (*lw - *topk));
    return FALSE;
  }
  return TRUE;
}

/*------------------------------------------------------------------ 
 * matsize :
 *    like getmat but here m,n are given on entry 
 *    and we check that matrix is of size (m,n) 
 *------------------------------------------------------------------ */

int C2F(matsize)(char *fname,int *topk,int *lw,int *m,int *n,unsigned long fname_len)
{
  int m1, n1, lc, it, lr;
  
  if (  C2F(getmat)(fname, topk, lw, &it, &m1, &n1, &lr, &lc, fname_len)  == FALSE)
    return FALSE;
  if (*m != m1 || *n != n1) {
    Scierror(205,_("%s: Wrong size for argument %d: (%d,%d) expected.\n"),get_fname(fname,fname_len), Rhs + (*lw - *topk), *m,*n);
    return FALSE;
  }
  return  TRUE;
}

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

static int C2F(getmati)(char *fname,int *topk,int *spos,int *lw,int *it,int *m,int *n,int *lr,int *lc,int *inlistx,int *nel,unsigned long fname_len)
{
  int il;
  il = iadr(*lw);
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  if (*istk(il ) != 1) {
	  if (*inlistx) {
		  Scierror(999,_("%s: Wrong type for argument %d (List element: %d): Real or complex matrix expected.\n"),get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
	  } else {
		  Scierror(201,_("%s: Wrong type for argument %d: Real or complex matrix expected.\n"),get_fname(fname,fname_len),
	       Rhs + (*spos - *topk));
	  }
    return  FALSE;
  }
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  *it = *istk(il + 3);
  *lr = sadr(il+4);
  if (*it == 1)  *lc = *lr + *m * *n;
  return TRUE;
} 


/*---------------------------------------------------------- 
 *  listcremat(top,numero,lw,....) 
 *      le numero ieme element de la liste en top doit etre un matrice 
 *      stockee a partir de Lstk(lw) 
 *      doit mettre a jour les pointeurs de la liste 
 *      ainsi que stk(top+1) 
 *      si l'element a creer est le dernier 
 *      lw est aussi mis a jour 
 *---------------------------------------------------------- */

int C2F(listcremat)(char *fname,int *lw,int *numi,int *stlw,int *it,int *m,int *n,int *lrs,int *lcs,unsigned long fname_len)
{
  int ix1,il ;
    
  if (C2F(cremati)(fname, stlw, it, m, n, lrs, lcs, &c_true, fname_len)==FALSE)
    return FALSE ;

  *stlw = *lrs + *m * *n * (*it + 1);
  il = iadr(*Lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *Lstk(*lw +1) = *stlw;
  return TRUE;
} 

/*---------------------------------------------------------- 
 *  cremat :
 *   checks that a matrix [it,m,n] can be stored at position  lw 
 *   <<pointers>> to real and imaginary part are returned on success
 *   In : 
 *     lw : position (entier) 
 *     it : type 0 ou 1 
 *     m, n dimensions 
 *   Out : 
 *     lr : stk(lr+i-1)= real(a(i)) 
 *     lc : stk(lc+i-1)= imag(a(i)) exists only if it==1 
 *   Side effect : if matrix creation is possible 
 *     [it,m,n] are stored in Scilab stack 
 *     and lr and lc are returned 
 *     but stk(lr+..) and stk(lc+..) are unchanged 
 *---------------------------------------------------------- */

int C2F(cremat)(char *fname,int *lw,int *it,int *m,int *n,int *lr,int *lc,unsigned long fname_len)
{

  if (*lw + 1 >= Bot) {
	  Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  if ( C2F(cremati)(fname, Lstk(*lw ), it, m, n, lr, lc, &c_true, fname_len) == FALSE)
    return FALSE ;
  *Lstk(*lw +1) = *lr + *m * *n * (*it + 1);
  return TRUE;
} 

/*-------------------------------------------------
 * Similar to cremat but we only check for space 
 * no data is stored 
 *-------------------------------------------------*/

int C2F(fakecremat)(int *lw,int *it,int *m,int *n,int *lr,int *lc)
{
  if (*lw + 1 >= Bot) return FALSE;
  if (C2F(cremati)("cremat", Lstk(*lw ), it, m, n, lr, lc, &c_false, 6L) == FALSE) 
    return FALSE;
  *Lstk(*lw +1) = *lr + *m * *n * (*it + 1);
  return TRUE;
} 


/*--------------------------------------------------------- 
 * internal function used by cremat and listcremat 
 *---------------------------------------------------------- */
static int C2F(cremati)(char *fname,int *stlw,int *it,int *m,int *n,int *lr,int *lc,int *flagx,unsigned long fname_len)
{
  int ix1;
  int il;
  double size = ((double) *m) * ((double) *n) * ((double) (*it + 1));
  il = iadr(*stlw);
  ix1 = il + 4;
  Err = sadr(ix1) - *Lstk(Bot );
  if ( (double) Err > -size ) {
	  Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  };
  if (*flagx) {
    *istk(il ) = 1;
    /* if m*n=0 then both dimensions are to be set to zero */
    *istk(il + 1) = Min(*m , *m * *n);
    *istk(il + 2) = Min(*n ,*m * *n);
    *istk(il + 3) = *it;
  }
  ix1 = il + 4;
  *lr = sadr(ix1);
  *lc = *lr + *m * *n;
  return TRUE;
} 

/*--------------------------------------------------------- 
*     same as cremat, but without test ( we are below bot)
*     and adding a call to putid 
*     cree une variable de type matrice 
*     de nom id 
*     en lw : sans verification de place 
*---------------------------------------------------------- */
int C2F(crematvar)(int *id, int *lw, int *it, int *m, int *n, double *rtab, double *itab)
{

	/* Local variables */
	int i__1;
	static int lc, il, lr;
	static int c__1 = 1;

	/* Parameter adjustments */
	--itab;
	--rtab;
	--id;

	/* Function Body */
	C2F(putid)(&C2F(vstk).idstk[*lw * 6 - 6], &id[1]);
	il = C2F(vstk).lstk[*lw - 1] + C2F(vstk).lstk[*lw - 1] - 1;
	*istk(il) = 1;
	*istk(il+1) = *m;
	*istk(il+2) = *n;
	*istk(il+3) = *it;
	i__1 = il + 4;
	lr = i__1 / 2 + 1;
	lc = lr + *m * *n;
	if (*lw < C2F(vstk).isiz) 
	{
		i__1 = il + 4;
		C2F(vstk).lstk[*lw] = i__1 / 2 + 1 + *m * *n * (*it + 1);
	}
	i__1 = *m * *n;
	C2F(unsfdcopy)(&i__1, &rtab[1], &c__1, stk(lr), &c__1);
	if (*it == 1) 
	{
		i__1 = *m * *n;
		C2F(unsfdcopy)(&i__1, &itab[1], &c__1, stk(lc), &c__1);
	}
	return 0;
} 


/*--------------------------------------------------------- 
*     crebmat without check and call to putid 
*---------------------------------------------------------- */
int C2F(crebmatvar)(int *id, int *lw, int *m, int *n, int *val)
{
	/* Local variables */
	static int il, lr;
	int i__1;
	static int c__1 = 1;

	/* Parameter adjustments */
	--val;
	--id;

	C2F(putid)(&C2F(vstk).idstk[*lw * 6 - 6], &id[1]);
	il = C2F(vstk).lstk[*lw - 1] + C2F(vstk).lstk[*lw - 1] - 1;
	*istk(il) = 4;
	*istk(il+1) = *m;
	*istk(il+2) = *n;
	lr = il + 3;
	i__1 = il + 3 + *m * *n + 2;
	C2F(vstk).lstk[*lw] = i__1 / 2 + 1;
	i__1 = *m * *n;
	C2F(icopy)(&i__1, &val[1], &c__1, istk(lr), &c__1);
	return 0;
} 
/*--------------------------------------------------------- 
*     cresmatvar without check and call to putid 
*---------------------------------------------------------- */
int C2F(cresmatvar)(int *id, int *lw, char *str, int *lstr, unsigned long str_len)
{
	static int il, mn, lr1, ix1, ilp;
	static int ilast;
	static int c__0 = 0;

	/* Parameter adjustments */
	--id;

	C2F(putid)(&C2F(vstk).idstk[*lw * 6 - 6], &id[1]);
	il = C2F(vstk).lstk[*lw - 1] + C2F(vstk).lstk[*lw - 1] - 1;
	mn = 1;
	ix1 = il + 4 + (*lstr + 1) + (mn + 1);
	*istk(il) = 10;
	*istk(il+1) = 1;
	*istk(il+2) = 1;
	*istk(il+3) = 0;
	ilp = il + 4;
	*istk(ilp) = 1;
	*istk(ilp+1) = *istk(ilp) + *lstr;
	ilast = ilp + mn;
	lr1 = ilast + *istk(ilp);
	C2F(cvstr)(lstr, istk(lr1), str, &c__0, str_len);
	ix1 = ilast + *istk(ilast);
	C2F(vstk).lstk[*lw] = ix1 / 2 + 1;
	return 0;
}

/**********************************************************************
 * INT MATRICES 
 **********************************************************************/

/* compute requested memory in number of ints */

#define memused(it,mn) ((((mn)*( it % 10))/sizeof(int))+1)


/*------------------------------------------------------------------ 
 * getilistmat : 
 *    checks that spos object is a list 
 *    checks that lnum-element of the list exists and is an int matrix 
 *    extracts matrix information(it,m,n,lr) 
 *     In  : 
 *       fname : name of calling function for error message 
 *       topk  : stack ref for error message 
 *       lw    : stack position 
 *     Out : 
 *       [it,m,n] matrix dimensions 
 *       it : 1,2,4,11,12,14 
 *       lr : istk(lr+i-1)   : matrix data must be properly cast 
 *                             according to it value 
 *------------------------------------------------------------------ */

int C2F(getlistimat)(char *fname,int *topk,int *spos,int *lnum,int *it,int *m,int *n,int *lr,unsigned long fname_len)
{
  int nv, ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE)
    return FALSE;

  if (*lnum > nv) {
    Scierror(999,_("%s: Wrong type for argument %d: List of size at least %d expected.\n"),get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }
  return C2F(getimati)(fname, topk, spos, &ili,it, m, n, lr,  &c_true, lnum, fname_len);
} 

/*------------------------------------------------------------------- 
 * getimat :
 *     check that object at position lw is an int matrix 
 *     In  : 
 *       fname : name of calling function for error message 
 *       topk  : stack ref for error message 
 *       lw    : stack position ( ``in the top sense'' )
 *     Out : 
 *       [it,m,n] matrix dimensions 
 *       lr : istk(lr+i-1)= a(i)
 *------------------------------------------------------------------- */

int C2F(getimat)(char *fname,int *topk,int *lw,int *it,int *m,int *n,int *lr,unsigned long fname_len)
{
  return C2F(getimati)(fname, topk, lw,Lstk(*lw),it,m, n, lr,&c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

static int C2F(getimati)(char *fname,int *topk,int *spos,int *lw,int *it,int *m,int *n,int *lr,int *inlistx,int *nel,unsigned long fname_len)
{
  int il;
  il = iadr(*lw);
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  if (*istk(il ) != 8 ) {
    if (*inlistx) 
      Scierror(999,_("%s: Wrong type for argument %d (List element: %d): Int matrix expected.\n"),get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
    else 
      Scierror(201,_("%s: Wrong type for argument %d: Real or complex matrix expected.\n"),get_fname(fname,fname_len),Rhs + (*spos - *topk));
    return  FALSE;
  }
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  *it = *istk(il + 3);
  *lr = il+4;
  return TRUE;
} 


/*---------------------------------------------------------- 
 *  listcreimat(top,numero,lw,....) 
 *      le numero ieme element de la liste en top doit etre un matrice 
 *      stockee a partir de Lstk(lw) 
 *      doit mettre a jour les pointeurs de la liste 
 *      ainsi que stk(top+1) 
 *      si l'element a creer est le dernier 
 *      lw est aussi mis a jour 
 *---------------------------------------------------------- */

int C2F(listcreimat)(char *fname,int *lw,int *numi,int *stlw,int *it,int *m,int *n,int *lrs,unsigned long fname_len)
{
  int ix1,il ;
    
  if (C2F(creimati)(fname, stlw,it, m, n, lrs, &c_true, fname_len)==FALSE)
    return FALSE ;
  *stlw = sadr(*lrs + memused(*it,*m * *n));
  il = iadr(*Lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *Lstk(*lw +1) = *stlw;
  return TRUE;
} 

/*---------------------------------------------------------- 
 *  creimat :
 *   checks that an int matrix [it,m,n] can be stored at position  lw 
 *   <<pointers>> to real and imaginary part are returned on success
 *   In : 
 *     lw : position (entier) 
 *     it : type 1,2,4,11,12,14 
 *     m, n dimensions 
 *   Out : 
 *     lr : istk(lr+i-1)=> a(i)   
 *   Side effect : if matrix creation is possible 
 *     [it,m,n] are stored in Scilab stack 
 *     and lr is returned 
 *     but stk(lr+..) are unchanged 
 *---------------------------------------------------------- */

int C2F(creimat)(char *fname,int *lw,int *it,int *m,int *n,int *lr,unsigned long fname_len)
{

  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  if ( C2F(creimati)(fname, Lstk(*lw ), it, m, n, lr,&c_true, fname_len) == FALSE)
    return FALSE ;
  *Lstk(*lw +1) = sadr(*lr + memused(*it,*m * *n));
  return TRUE;
} 

/*--------------------------------------------------------- 
 * internal function used by cremat and listcremat 
 *---------------------------------------------------------- */

int C2F(creimati)(char *fname,int *stlw,int *it,int *m,int *n,int *lr,int *flagx,unsigned long fname_len)
{
  int ix1;
  int il;
  double size =  memused(*it,((double)*m)*((double) *n));
  il = iadr(*stlw);
  ix1 = il + 4;
  Err = sadr(ix1) - *Lstk(Bot );
  if (Err > -size ) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  };
  if (*flagx) {
    *istk(il ) = 8;
    /* if m*n=0 then both dimensions are to be set to zero */
    *istk(il + 1) = Min(*m , *m * *n);
    *istk(il + 2) = Min(*n ,*m * *n);
    *istk(il + 3) = *it;
  }
  ix1 = il + 4;
  *lr = ix1;
  return TRUE;
} 


/**********************************************************************
 * BOOLEAN MATRICES 
 **********************************************************************/

/*------------------------------------------------------------------ 
 * getlistbmat : 
 *    checks that spos object is a list 
 *    checks that lnum-element of the list exists and is a boolean matrix 
 *    extracts matrix information(m,n,lr) 
 *------------------------------------------------------------------ */

int C2F(getlistbmat)(char *fname,int *topk,int *spos,int *lnum,int *m,int *n,int *lr,unsigned long fname_len)
{
  int nv;
  int ili;

  if (C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len)== FALSE)
    return FALSE ;

  if (*lnum > nv) {
    Scierror(999,_("%s: Wrong size for argument %d: At least %d expected.\n"),get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE ;
  }
  
  return C2F(getbmati)(fname, topk, spos, &ili, m, n, lr, &c_true, lnum, fname_len);
} 

/*------------------------------------------------------------------- 
 * getbmat :
 *     check that object at position lw is a boolean matrix 
 *     In  : 
 *       fname : name of calling function for error message 
 *       lw    : stack position 
 *     Out : 
 *       [m,n] matrix dimensions 
 *       lr : istk(lr+i-1)= a(i)
 *------------------------------------------------------------------- */

int C2F(getbmat)(char *fname,int *topk,int *lw,int *m,int *n,int *lr,unsigned long fname_len)
{
  return C2F(getbmati)(fname, topk, lw, Lstk(*lw ), m, n, lr, &c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------ 
 * matbsize :
 *    like getbmat but here m,n are given on entry 
 *    and we check that matrix is of size (m,n) 
 *------------------------------------------------------------------ */

int C2F(matbsize)(char *fname,int *topk,int *lw,int *m,int *n,unsigned long fname_len)
{
  int m1, n1, lr;
  if ( C2F(getbmat)(fname, topk, lw, &m1, &n1, &lr, fname_len) == FALSE)
    return FALSE;
  if (*m != m1 || *n != n1) {
    Scierror(205,_("%s: Wrong size for argument %d: (%d,%d) expected.\n"),get_fname(fname,fname_len),Rhs + (*lw - *topk),*m,*n);
    return FALSE;
  }
  return TRUE;
} 

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

static int C2F(getbmati)(char *fname,int *topk,int *spos,int *lw,int *m,int *n,int *lr,int *inlistx,int *nel,unsigned long fname_len)
{
  int il;

  il = iadr(*lw);
  if (*istk(il ) < 0)    il = iadr(*istk(il +1));

  if (*istk(il ) != 4) {
    if (*inlistx) 
      Scierror(999,_("%s: Wrong type for argument %d (List element: %d): Boolean matrix expected.\n"),get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
    else 
      Scierror(208,_("%s: Wrong type for argument %d: Boolean matrix expected.\n"),get_fname(fname,fname_len),
	       Rhs + (*spos - *topk));
    return FALSE;
  };
  *m = *istk(il +1);
  *n = *istk(il +2);
  *lr = il + 3;
  return  TRUE;
} 

/*------------------------------------------------== 
 *      listcrebmat(top,numero,lw,....) 
 *      le numero ieme element de la liste en top doit etre un bmatrice 
 *      stockee a partir de Lstk(lw) 
 *      doit mettre a jour les pointeurs de la liste 
 *      ainsi que stk(top+1) 
 *      si l'element a creer est le dernier 
 *      lw est aussi mis a jour 
 *---------------------------------------------------------- */

int C2F(listcrebmat)(char *fname,int *lw,int *numi,int *stlw,int *m,int *n,int *lrs,unsigned long fname_len)
{
  int ix1;
  int il;

  if ( C2F(crebmati)(fname, stlw, m, n, lrs, &c_true, fname_len)== FALSE) 
    return FALSE;

  ix1 = *lrs + *m * *n + 2;
  *stlw = sadr(ix1);
  il = iadr(*Lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *Lstk(*lw +1) = *stlw;
  return TRUE;
}

/*---------------------------------------------------------- 
 *  crebmat :
 *   checks that a boolean matrix [m,n] can be stored at position  lw 
 *   <<pointers>> to data is returned on success
 *   In : 
 *     lw : position (entier) 
 *     m, n dimensions 
 *   Out : 
 *     lr : istk(lr+i-1)= a(i) 
 *   Side effect : if matrix creation is possible 
 *     [m,n] are stored in Scilab stack 
 *     and lr is  returned 
 *     but istk(lr+..) is unchanged 
 *---------------------------------------------------------- */

int C2F(crebmat)(char *fname,int *lw,int *m,int *n,int *lr,unsigned long fname_len)
{
  int ix1;
  
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE ;
  }

  if ( C2F(crebmati)(fname, Lstk(*lw ), m, n, lr, &c_true, fname_len)== FALSE)
    return FALSE ;

  ix1 = *lr + *m * *n + 2;
  *Lstk(*lw +1) = sadr(ix1);
  return TRUE;
} 

/*-------------------------------------------------
 * Similar to crebmat but we only check for space 
 * no data is stored 
 *-------------------------------------------------*/

int C2F(fakecrebmat)(int *lw,int *m,int *n,int *lr) 
{
  if (*lw + 1 >= Bot) {
	Scierror(18,_("%s: Too many names.\n"),"fakecrebmat");
    return FALSE;
  }
  if ( C2F(crebmati)("crebmat", Lstk(*lw ), m, n, lr, &c_false, 7L)== FALSE)
    return FALSE ;
  *Lstk(*lw +1) = sadr( *lr + *m * *n + 2);
  return TRUE;
} 

/*--------------------------------------------------------- 
 * internal function used by crebmat and listcrebmat 
 *---------------------------------------------------------- */

static int C2F(crebmati)(char *fname,int *stlw,int *m,int *n,int *lr,int *flagx,unsigned long fname_len)
{
  double size = ((double) *m) * ((double) *n) ;
  int il;
  il = iadr(*stlw);
  Err = il + 3  - iadr(*Lstk(Bot ));
  if (Err > -size ) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  if (*flagx) {
    *istk(il ) = 4;
    /*     si m*n=0 les deux dimensions sont mises a zero. */
    *istk(il + 1) = Min(*m , *m * *n);
    *istk(il + 2) = Min(*n,*m * *n);
  }
  *lr = il + 3;
  return TRUE;
}

/**********************************************************************
 * SPARSE MATRICES 
 *       [it,m,n,nel,mnel,icol,lr,lc] 
 *       nel : number of non nul elements 
 *       istk(mnel+i-1), i=1,m : number of non nul elements of row i 
 *       non nul elements are stored in row order as follows:
 *       istk(icol+j-1) ,j=1,nel, column of the j-th non null element 
 *       stk(lr + j-1)  ,j=1,nel, real value of the j-th non null element 
 *       stk(lc + j-1)  ,j=1,nel, imag. value of the j-th non null element 
 *       lc is to be used only if matrix is complex (it==1)
 **********************************************************************/

/*------------------------------------------------------------------ 
 * getlistsparse : 
 *    checks that spos object is a list 
 *    checks that lnum-element of the list exists and is a sparse matrix 
 *    extracts matrix information(it,m,n,nel,mnel,icol,lr,lc) 
 *------------------------------------------------------------------ */

int C2F(getlistsparse)(char *fname,int *topk,int *spos,int *lnum,int *it,int *m,int *n,int *nel,int *mnel,int *icol,int *lr,int *lc,unsigned long fname_len)
{
  int  nv;
  int ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE) 
    return FALSE ;
  
  if (*lnum > nv) {
    Scierror(999,_("%s: Wrong size for argument %d: At least %d expected.\n"),
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }

  return C2F(getsparsei)(fname, topk, spos, &ili, it, m, n, nel, mnel, icol, lr, lc, &c_true, lnum, fname_len);

} 

/*------------------------------------------------------------------- 
 * getsparse :
 *     check that object at position lw is a sparse matrix 
 *     In  : 
 *       fname : name of calling function for error message 
 *       lw    : stack position 
 *     Out : 
 *       [it,m,n,nel,mnel,icol,lr,lc] matrix dimensions 
 *------------------------------------------------------------------- */

int C2F(getsparse)(char *fname,int *topk,int *lw,int *it,int *m,int *n,int *nel,int *mnel,int *icol,int *lr,int *lc,unsigned long fname_len)
{
  return C2F(getsparsei)(fname, topk, lw, Lstk(*lw ), it, m, n, nel, mnel, icol, lr, lc, &c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------- 
 * getrsparse : lie getsparse but we check for a real matrix  
 *------------------------------------------------------------------- */

int C2F(getrsparse)(char *fname, int *topk, int *lw, int *m, int *n,  int *nel, int *mnel, int *icol, int *lr,unsigned long fname_len)
{
  int lc, it;  
  if ( C2F(getsparse)(fname, topk, lw, &it, m, n, nel, mnel, icol, lr, &lc, fname_len) == FALSE ) 
    return FALSE;

  if (it != 0) {
    Scierror(202,_("%s: Wrong type for argument %d: Real matrix expected.\n"),get_fname(fname,fname_len), Rhs + (*lw - *topk));
    return FALSE;
  }
  return TRUE;
}

/*--------------------------------------- 
 * internal function for getmat and listgetmat 
 *--------------------------------------- */

static int C2F(getsparsei)(char *fname,int *topk,int *spos,int *lw,int *it,int *m,int *n,int *nel,int *mnel,int *icol,int *lr,int *lc,int *inlistx,int *nellist,unsigned long fname_len)
{
  int il;

  il = iadr(*lw);
  if (*istk(il ) < 0)   il = iadr(*istk(il +1));

  if (*istk(il ) != sci_sparse) {
    if (*inlistx) 
      Scierror(999,_("%s: Wrong type for argument %d (List element: %d): Sparse matrix expected.\n"),get_fname(fname,fname_len), Rhs + (*spos - *topk), *nellist);
    else 
      Scierror(999,_("%s: Wrong type for argument %d: Sparse matrix expected.\n"),get_fname(fname,fname_len), Rhs + (*spos - *topk));
    return FALSE;
  }
  *m   = *istk(il + 1);
  *n   = *istk(il + 2);
  *it  = *istk(il + 3);
  *nel = *istk(il + 4);
  *mnel = il + 5;
  *icol = il + 5 + *m;
  *lr = sadr(il + 5 + *m + *nel);
  if (*it == 1)  *lc = *lr + *nel;
  return TRUE;
} 

/*---------------------------------------------------------- 
 *      le numero ieme element de la liste en top doit etre une matrice 
 *      sparse stockee a partir de Lstk(lw) 
 *      doit mettre a jour les pointeurs de la liste 
 *      ainsi que stk(top+1) 
 *      si l'element a creer est le dernier 
 *      lw est aussi mis a jour 
 * 
 *---------------------------------------------------------- */


int C2F(listcresparse)(char *fname,int *lw,int *numi,int *stlw,int *it,int *m,int *n,int *nel,int *mnel,int *icol,int *lrs,int *lcs,unsigned long fname_len)
{
  int ix1,il;

  if (C2F(cresparsei)(fname, stlw, it, m, n, nel, mnel, icol, lrs, lcs, fname_len)== FALSE) 
    return FALSE ;

  *stlw = *lrs + *nel * (*it + 1);
  il = iadr(*Lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1)) {
    *Lstk(*lw +1) = *stlw;
  }
  return TRUE;
} 

/*---------------------------------------------------------- 
 *  cresparse :
 *   checks that a sparse matrix [it,m,n,nel,mnel,icol] can be stored at position  lw 
 *   <<pointers>> to real and imaginary part are returned on success
 *   In : 
 *     lw : position (entier) 
 *     it : type 0 ou 1 
 *     m, n,nel  dimensions 
 *   Out : 
 *     mnel,icol,lr,lc 
 *   Side effect : if matrix creation is possible 
 *     [it,m,n,nel] are stored in Scilab stack 
 *     and mnel,icol,lr and lc are returned 
 *     but data is unchanged 
 *---------------------------------------------------------- */

int C2F(cresparse)(char *fname,int *lw,int *it,int *m,int *n,int *nel,int *mnel,int *icol,int *lr,int *lc,unsigned long fname_len)
{
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE ;
  }
  
  if ( C2F(cresparsei)(fname, Lstk(*lw ), it, m, n, nel, mnel, icol, lr, lc, fname_len)
       == FALSE) 
    return FALSE ;
  *Lstk(*lw +1) = *lr + *nel * (*it + 1);
  return TRUE;
} 


/*--------------------------------------------------------- 
 * internal function used by cremat and listcremat 
 *---------------------------------------------------------- */

static int C2F(cresparsei)(char *fname,int *stlw,int *it,int *m,int *n,int *nel,int *mnel,int *icol,int *lr,int *lc,unsigned long fname_len)
{
  int il,ix1;

  il = iadr(*stlw);
  ix1 = il + 5 + *m + *nel;
  Err = sadr(ix1) + *nel * (*it + 1) - *Lstk(Bot );
  if (Err > 0) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  };
  *istk(il ) = sci_sparse;
  /*   if m*n=0 the 2 dims are set to zero */
  if ( *m == 0  ||  *n == 0 )  /* use this new test in place of the product m * n (bruno) */
    {
      *istk(il + 1) = 0; *istk(il + 2) = 0;
    }
  else
    {
      *istk(il + 1) = *m; *istk(il + 2) = *n;
    }
  *istk(il + 3) = *it;
  *istk(il + 4) = *nel;
  *mnel = il + 5;
  *icol = il + 5 + *m;
  ix1 = il + 5 + *m + *nel;
  *lr = sadr(ix1);
  *lc = *lr + *nel;
  return TRUE;
} 

/**********************************************************************
 * VECTORS  
 **********************************************************************/

/*------------------------------------------------------------------ 
 * getlistvect : 
 *    checks that spos object is a list 
 *    checks that lnum-element of the list exists and is a vector 
 *    extracts vector information(it,m,n,lr,lc) 
 *------------------------------------------------------------------ */

int C2F(getlistvect)(char *fname,int *topk,int *spos,int *lnum,int *it,int *m,int *n,int *lr,int *lc,unsigned long fname_len)
{
  if (C2F(getlistmat)(fname, topk, spos, lnum, it, m, n, lr, lc, fname_len)== FALSE) 
    return FALSE;

  if (*m != 1 && *n != 1) {
    Scierror(999,_("%s: Wrong type for argument %d (List element: %d): Vector expected.\n"),get_fname(fname,fname_len),Rhs + (*spos - *topk), *lnum);
    return  FALSE;
  }
  return TRUE;
} 

/*------------------------------------------------------------------- 
 * getvect :
 *     check that object at position lw is a vector 
 *     In  : 
 *       fname : name of calling function for error message 
 *       lw    : stack position 
 *     Out : 
 *       [it,m,n] matrix dimensions 
 *       lr : stk(lr+i-1)= real(a(i)) 
 *       lc : stk(lc+i-1)= imag(a(i)) exists only if it==1 
 *------------------------------------------------------------------- */

int C2F(getvect)(char *fname,int *topk,int *lw,int *it,int *m,int *n,int *lr,int *lc,unsigned long fname_len)
{
  if ( C2F(getmat)(fname, topk, lw, it, m, n, lr, lc, fname_len) == FALSE) 
    return FALSE;

  if (*m != 1 && *n != 1) {
    Scierror(214,_("%s: Wrong type for argument %d: Vector expected.\n"),get_fname(fname,fname_len), Rhs + (*lw - *topk));
    return FALSE;
  };
  return  TRUE;
}


/*------------------------------------------------------------------ 
 * getrvect : like getvect but we expect a real vector 
 *------------------------------------------------------------------ */

int C2F(getrvect)(char *fname,int *topk,int *lw,int *m,int *n,int *lr,unsigned long fname_len)
{
  if ( C2F(getrmat)(fname, topk, lw, m, n, lr, fname_len)  == FALSE)
    return FALSE;

  if (*m != 1 && *n != 1) {
    Scierror(203,_("%s: Wrong type for argument %d: Real vector expected.\n"),get_fname(fname,fname_len), Rhs + (*lw - *topk));
    return FALSE;
  }
  return TRUE ;
}

/*------------------------------------------------------------------ 
 * vectsize :
 *    like getvect but here n is given on entry 
 *    and we check that vector is of size (n) 
 *------------------------------------------------------------------ */

int C2F(vectsize)(char *fname,int *topk,int *lw,int *n,unsigned long fname_len)
{
  int m1, n1, lc, lr, it1;

  if ( C2F(getvect)(fname, topk, lw, &it1, &m1, &n1, &lr, &lc, fname_len) == FALSE) 
    return FALSE;

  if (*n != m1 * n1) {
    Scierror(206,_("%s: Wrong size for argument %d: %d expected.\n"),get_fname(fname,fname_len), Rhs + (*lw - *topk), *n);
    return FALSE;
  }
  return TRUE;
} 

/**********************************************************************
 * SCALAR   
 **********************************************************************/

/*------------------------------------------------------------------ 
 *     getlistscalar : recupere un scalaire 
 *------------------------------------------------------------------ */

int C2F(getlistscalar)(char *fname,int *topk,int *spos,int *lnum,int *lr,unsigned long fname_len)
{
  int m, n;
  int lc, it, nv;
  int ili;
  
  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE)
    return FALSE;

  if (*lnum > nv) {
    Scierror(999,_("%s: Wrong size for argument %d: At least %d expected.\n"),
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }
  
  if ( C2F(getmati)(fname, topk, spos, &ili, &it, &m, &n, lr, &lc, &c_true, lnum, fname_len)
       == FALSE)
    return FALSE;

  if (m * n != 1) {
    Scierror(999,_("%s: Wrong type for argument %d (List element: %d): Scalar expected.\n"),get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }
  return TRUE;
}

/*------------------------------------------------------------------ 
 * getscalar :
 *     check that object at position lw is a scalar 
 *     In  : 
 *       fname : name of calling function for error message 
 *       lw    : stack position 
 *     Out : 
 *       lr : stk(lr)= scalar_value 
 *------------------------------------------------------------------ */

int C2F(getscalar)(char *fname,int *topk,int *lw,int *lr,unsigned long fname_len)
{
  int m, n;

  if ( C2F(getrmat)(fname, topk, lw, &m, &n, lr, fname_len) == FALSE) 
    return  FALSE;

  if (m * n != 1) {
    Scierror(204,_("%s: Wrong type for argument %d: Scalar, '%s' or '%s' expected.\n"),get_fname(fname,fname_len),Rhs + (*lw - *topk), "min","max");
    return FALSE ; 
  };
  return TRUE;
} 

/**********************************************************************
 * STRING and string Matrices 
 **********************************************************************/

/*------------------------------------------------------------------ 
 * getlistsmat : 
 *    checks that spos object is a list 
 *    checks that lnum-element of the list exists and is a string matrix 
 *    extracts string matrix information in m,n and (ix,j)-th string in lr nlr
 *     In  : 
 *       fname : name of calling function for error message 
 *       topk  : stack ref for error message 
 *       spos  : stack position 
 *       lnum  : element position in the list
 *       ix,j  : indices of the requested element 
 *     Out : 
 *       [m,n] : lnum smatrix dimensions 
 *       lr  : istk(lr+i-1) gives string(ix,j) interbal codes 
 *       nlr : length of (ix,j)-th string 
 *------------------------------------------------------------------ */

int C2F(getlistsmat)(char *fname,int *topk,int *spos,int *lnum,int *m,int *n,int *ix,int *j,int *lr,int *nlr,unsigned long fname_len)
{
  int nv, ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE)
    return FALSE;

  if (*lnum > nv) {
    Scierror(999,_("%s: Wrong size for argument %d: At least %d expected.\n"),
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }
  return C2F(getsmati)(fname, topk, spos, &ili,  m, n, ix,j, lr, nlr, &c_true, lnum, fname_len);
} 

/*------------------------------------------------------------------- 
 * getsmat :
 *     check that object at position lw is a string matrix 
 *     In  : 
 *       fname : name of calling function for error message 
 *       lw    : stack position 
 *       (ix,j): indices of the string element requested 
 *     Out : 
 *       [m,n] matrix dimensions 
 *       lr  : istk(lr+i-1) gives string(ix,j) internal codes 
 *       nlr : length of (ix,j)-th string 
 * Note : getsmat can be used to get a(1,1) and check that a is a string matrix 
 *        then other elements can be accessed through getsimat 
 *------------------------------------------------------------------- */

int C2F(getsmat)(char *fname,int *topk,int *lw,int *m,int *n,int *ix,int *j,int *lr,int *nlr,unsigned long fname_len)
{
  return C2F(getsmati)(fname, topk, lw, Lstk(*lw), m, n, ix,j , lr ,nlr,  &c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------ 
 * getsimat :
 *     In  : 
 *       fname : name of calling function for error message 
 *       lw    : stack position 
 *       (ix,j): indices of the string element requested 
 *     Out : 
 *       [m,n] matrix dimensions 
 *       lr  : istk(lr+i-1) gives string(ix,j) internal codes 
 *       nlr : length of (ix,j)-th string 
 * Note : like getsmat but do not check that object is a string matrix 
 *------------------------------------------------------------------- */

int C2F(getsimat)(char *fname,int *topk,int *lw,int *m,int *n,int *ix,int *j,int *lr,int *nlr,unsigned long fname_len)
{
  return C2F(getsimati)(fname, topk, lw, Lstk(*lw), m, n, ix,j , lr ,nlr,  &c_false, &cx0, fname_len);
}

/*--------------------------------------------------------------------------
 * getlistwsmat : 
 *    similar to getlistsmat but returned values are different 
 *       ilr  : 
 *       ilrd : 
 *    ilr and ilrd : internal coded versions of the strings 
 *    which can be converted to C with ScilabMStr2CM (see stack2.c)
 *------------------------------------------------------------------ */

int C2F(getlistwsmat)(char *fname,int *topk,int *spos,int *lnum,int *m,int *n,int *ilr,int *ilrd,unsigned long fname_len)
{
  int nv, ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE)
    return FALSE;

  if (*lnum > nv) {
    Scierror(999,_("%s: Wrong size for argument %d: At least %d expected.\n"),get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }
  return C2F(getwsmati)(fname, topk, spos, &ili, m, n, ilr, ilrd, &c_true, lnum, fname_len);
} 

/*--------------------------------------------------------------------------
 * getwsmat : checks for a mxn string matrix 
 *    similar to getsmat but returned values are different 
 *    ilr and ilrd : internal coded versions of the strings 
 *    which can be converted to C with ScilabMStr2CM (see stack2.c)
 *--------------------------------------------------------------------------*/

int C2F(getwsmat)(char *fname,int *topk,int *lw,int *m,int *n,int *ilr,int *ilrd,unsigned long fname_len)
{
  return C2F(getwsmati)(fname, topk, lw,Lstk(*lw), m, n, ilr, ilrd, &c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

static int C2F(getwsmati)(char *fname,int *topk,int *spos,int *lw,int *m,int *n,int *ilr,int *ilrd ,int *inlistx,int *nel,unsigned long fname_len)
{
    int il;
    il = iadr(*lw);
    if (*istk(il ) < 0) il = iadr(*istk(il +1));
    if (*istk(il ) != sci_strings) {
		if (*inlistx){
			Scierror(999,_("%s: Wrong type for argument %d (List element: %d): Matrix of strings expected.\n"),get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
		} else {
			Scierror(207,_("%s: Wrong type for argument %d: Matrix of strings expected.\n"),get_fname(fname,fname_len), Rhs + (*spos - *topk));
		}
		return FALSE;
    }
    *m = *istk(il + 1);
    *n = *istk(il + 2);
    *ilrd = il + 4;
    *ilr =  il + 5 + *m * *n;
    return TRUE;
} 

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

static int C2F(getsmati)(char *fname,int *topk,int *spos,int *lw,int *m,int *n,int *ix,int *j,int *lr,int *nlr,int *inlistx,int *nel,unsigned long fname_len)
{
  int il = iadr(*lw);
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  if (*istk(il ) != sci_strings ) {
	  if (*inlistx) {
		  Scierror(999,_("%s: Wrong type for argument %d (List element: %d): String matrix expected.\n"),get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
	  } else {
		  Scierror(201,_("%s: Wrong type for argument %d: String matrix expected.\n"),get_fname(fname,fname_len), Rhs + (*spos - *topk));
	  }
    return  FALSE;
  }
  C2F(getsimati)(fname, topk, spos, lw, m, n, ix,j , lr ,nlr, inlistx, nel, fname_len);
  return TRUE;
} 

int C2F(getsimati)(char *fname,int *topk,int *spos,int *lw,int *m,int *n,int *ix,int *j ,int *lr ,int *nlr,int *inlistx,int *nel,unsigned long fname_len)
{
  int k, il =  iadr(*lw);
  if (*istk(il ) < 0) il = iadr(*istk(il +1)); 
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  k = *ix - 1 + (*j - 1) * *m;
  *lr = il + 4 + *m * *n + *istk(il + 4 + k );
  *nlr = *istk(il + 4 + k +1) - *istk(il + 4 + k );
  return 0;
}

/*---------------------------------------------------------- 
 *     listcresmat(top,numero,lw,....) 
 *     le  ieme element de la liste en top doit etre une 
 *     matrice stockee a partir de Lstk(lw) 
 *     doit mettre a jour les pointeurs de la liste 
 *     ainsi que Lstk(top+1) si l'element a creer est le dernier 
 *     lw est aussi mis a jour 
 *     job==1: nchar est la taille de chaque chaine de la  matrice 
 *     job==2: nchar est le vecteur des tailles des chaines de la 
 *             matrice 
 *     job==3: nchar est le vecteur des pointeurs sur les chaines 
 *             de la matrice 
 *---------------------------------------------------------- */

int C2F(listcresmat)(char *fname,int *lw,int *numi,int *stlw,int *m,int *n,int *nchar,int *job,int *ilrs,unsigned long fname_len)
{
  int ix1;
  int il, sz;

  if ( C2F(cresmati)(fname, stlw, m, n, nchar, job, ilrs, &sz, fname_len) == FALSE )
    return FALSE;
  ix1 = *ilrs + sz;
  *stlw = sadr(ix1);
  il = iadr(*Lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *Lstk(*lw +1) = *stlw;
  return TRUE;
} 

/*---------------------------------------------------------- 
 * cresmat :
 *   checks that a string matrix [m,n] of strings 
 *   (each string is of length nchar)
 *   can be stored at position  lw on the stack 
 * Note that each string can be filled with getsimat 
 *---------------------------------------------------------- */

int C2F(cresmat)(char *fname,int *lw,int *m,int *n,int *nchar,unsigned long fname_len)
{
  int job = 1;
  int ix1, ilast, sz,lr ;
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return  FALSE;
  }
  if ( C2F(cresmati)(fname,Lstk(*lw), m, n, nchar, &job, &lr, &sz, fname_len) == FALSE )
    return FALSE ;
  ilast = lr - 1;
  ix1 = ilast + *istk(ilast );
  *Lstk(*lw +1) = sadr(ix1);
  /* empty strings */
  if ( *nchar == 0)   *Lstk(*lw +1) += 1;
  return TRUE;
}

/*------------------------------------------------------------------ 
 *  cresmat1 :
 *   checks that a string matrix [m,1] of string of length nchar[i]
 *   can be stored at position  lw on the stack 
 *   nchar : array of length m giving each string length 
 *  Note that each string can be filled with getsimat 
 *------------------------------------------------------------------ */

int C2F(cresmat1)(char *fname,int *lw,int *m,int *nchar,unsigned long fname_len)
{
  int job = 2, n=1;
  int ix1, ilast, sz,lr ;
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return  FALSE;
  }
  if ( C2F(cresmati)(fname,Lstk(*lw), m, &n, nchar, &job, &lr, &sz, fname_len) == FALSE )
    return FALSE ;
  ilast = lr - 1;
  ix1 = ilast + *istk(ilast );
  *Lstk(*lw +1) = sadr(ix1);
  return TRUE;
}

/*------------------------------------------------------------------ 
 *  cresmat2 :
 *   checks that a string of length nchar can be stored at position  lw 
 *  Out : 
 *     lr : istk(lr+i) give access to the internal array 
 *          allocated for string code 
 *------------------------------------------------------------------ */

int C2F(cresmat2)(char *fname,int *lw,int *nchar,int *lr,unsigned long fname_len)
{
  int job = 1, n=1,m=1;
  int ix1, ilast, sz ;
  if (*lw + 1 >= Bot) {
   Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return  FALSE;
  }
  if ( C2F(cresmati)(fname,Lstk(*lw), &m, &n, nchar, &job, lr, &sz, fname_len) == FALSE )
    return FALSE ;

  ilast = *lr - 1;
  ix1 = ilast + *istk(ilast );
  *Lstk(*lw +1) = sadr(ix1);
  /* empty strings */
  if ( *nchar == 0)   *Lstk(*lw +1) += 1;
  *lr = ilast + *istk(ilast - 1);
  return TRUE;
} 

/*------------------------------------------------------------------ 
 * cresmat3 :
 *   Try to create a string matrix S of size mxn 
 *     - nchar: array of size mxn giving the length of string S(i,j)
 *     - buffer : a character array wich contains the concatenation 
 *             of all the strings 
 *     - lw  : stack position for string creation 
 *------------------------------------------------------------------ */

int C2F(cresmat3)(char *fname,int *lw,int *m,int *n,int *nchar,char *buffer,unsigned long fname_len,unsigned long buffer_len)
{
  int job = 2;
  int ix1, ilast, sz,lr,lr1 ;
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return  FALSE;
  }
  if ( C2F(cresmati)(fname,Lstk(*lw), m, n, nchar, &job, &lr, &sz, fname_len) == FALSE )
    return FALSE ;
  ilast = lr - 1;
  ix1 = ilast + *istk(ilast );
  *Lstk(*lw +1) = sadr(ix1);

  lr1 = ilast + *istk(ilast - (*m)*(*n) );
  C2F(cvstr)(&sz, istk(lr1), buffer, &cx0, buffer_len);
  return TRUE;
} 

/*------------------------------------------------------------------ 
 *     checks that an [m,1] string matrix can be stored in the 
 *     stack. 
 *     All chains have the same length nchar 
 *     istk(lr) --- beginning of chains 
 *------------------------------------------------------------------ */

int C2F(cresmat4)(char *fname,int *lw,int *m,int *nchar,int *lr,unsigned long fname_len)
{
  int ix1,ix, ilast, il, nnchar, kij, ilp;
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  nnchar = 0;
  ix1 = *m;
  for (ix = 1; ix <= ix1; ++ix) nnchar += *nchar;
  il = iadr(*Lstk(*lw ));
  ix1 = il + 4 + (nnchar + 1) * *m;
  Err = sadr(ix1) - *Lstk(Bot );
  if (Err > 0) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  } 
  *istk(il ) = sci_strings;
  *istk(il + 1) = *m;
  *istk(il + 2) = 1;
  *istk(il + 3) = 0;
  ilp = il + 4;
  *istk(ilp ) = 1;
  ix1 = ilp + *m;
  for (kij = ilp + 1; kij <= ix1; ++kij) {
    *istk(kij ) = *istk(kij - 1) + *nchar;
  }
  ilast = ilp + *m;
  ix1 = ilast + *istk(ilast );
  *Lstk(*lw +1) = sadr(ix1);
  *lr = ilast + 1;
  return TRUE;
}

/*--------------------------------------------------------- 
 * internal function used by cresmat cresmat1 and listcresmat 
 * job : 
 *   case 1: all string are of same length (nchar) in the matrix 
 *   case 2: nchar is a vector which gives string lengthes 
 *   case 3: ? 
 *---------------------------------------------------------- */

int C2F(cresmati)(char *fname,int *stlw,int *m,int *n,int *nchar,int *job,int *lr,int *sz,unsigned long fname_len)
{
  int ix1, ix, il, kij, ilp, mn= (*m)*(*n);
  il = iadr(*stlw);
 
 /* compute the size of chains */ 
  *sz = 0;
  switch ( *job ) 
    {
    case 1 : *sz = mn * nchar[0];   break;
    case 2 : for (ix = 0 ; ix < mn ; ++ix) *sz += nchar[ix];  break;
    case 3 : *sz = nchar[mn] - 1;  break;
    }
  /* check the stack for space */
  ix1 = il + 4 + mn + 1 + *sz;
  Err = sadr(ix1) - *Lstk(Bot );
  if (Err > 0) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  };
  
  *istk(il ) = sci_strings;
  *istk(il + 1) = *m;
  *istk(il + 2) = *n;
  *istk(il + 3) = 0;
  ilp = il + 4;
  *istk(ilp ) = 1;
  switch ( *job ) 
    {
    case 1 :
      ix1 = mn  + ilp;
      for (kij = ilp + 1; kij <= ix1; ++kij) {
	*istk(kij) = *istk(kij - 1) + nchar[0];
      }
      break;
    case 2 :
      ix = 0;
      ix1 = mn + ilp;
      for (kij = ilp + 1; kij <= ix1; ++kij) {
	*istk(kij ) = *istk(kij - 2 +1) + nchar[ix]; 
	++ix;
      }
      break;
    case 3 :
      {
	ix1 = mn + 1;
	C2F(icopy)(&ix1, nchar, &cx1, istk(ilp ), &cx1);
      }
    }
  *lr = ilp + mn + 1;
  return TRUE;
}

/*------------------------------------------------------------------ 
 * Try to create a string matrix S of size mxn 
 *     - m is the number of rows of Matrix S 
 *     - n is the number of colums of Matrix S 
 *     - Str : a null terminated array of strings char **Str assumed 
 *             to contain at least m*n strings 
 *     - lw  : where to create the matrix on the stack 
 *------------------------------------------------------------------ */

int cre_smat_from_str_i(char *fname, int *lw, int *m, int *n, char *Str[],unsigned long fname_len, int *rep)
{
  int ix1, ix, ilast, il, nnchar, lr1, kij, ilp;
  int *pos;

  nnchar = 0;
  if (Str) for (ix = 0 ; ix < (*m)*(*n) ; ++ix) nnchar += (int) strlen(Str[ix]);
  else nnchar = 0;
  
  il = iadr(*lw);
  ix1 = il + 4 + (nnchar + 1) + (*m * *n + 1);
  Err = sadr(ix1) - *Lstk(Bot );
  if (Err > 0) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return  FALSE;
  } ;
  *istk(il ) = sci_strings;
  *istk(il + 1) = *m;
  *istk(il + 2) = *n;
  *istk(il + 3) = 0;
  ilp = il + 4;
  *istk(ilp ) = 1;
  ix = 0;
  ix1 = ilp + *m * *n;
  for (kij = ilp + 1; kij <= ix1; ++kij) 
  {
	  if (Str)
	  {
		*istk(kij ) = *istk(kij - 1) + (int)strlen(Str[ix]);
		++ix;
	  }
	  else
	  {
		*istk(kij ) = *istk(kij - 1) + 0;
		++ix;
	  }
  }
  ilast = ilp + *m * *n;
  lr1 = ilast + *istk(ilp );
  pos = istk(lr1);
  if (Str)
  {
	  for ( ix = 0 ; ix < (*m)*(*n) ; ix++) 
	  {
		  int l = (int)strlen(Str[ix]);
		  C2F(cvstr)(&l, pos, Str[ix], &cx0, l);
		  pos += l;
	  }
  }
  ix1 = ilast + *istk(ilast );
  *rep = sadr(ix1);
  return TRUE;
} 


int cre_smat_from_str(char *fname,int *lw,int *m,int *n,char *Str[],unsigned long fname_len)
{
  int rep;
  
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }

  if ( cre_smat_from_str_i(fname, Lstk(*lw ), m, n, Str, fname_len,&rep)== FALSE )
    return FALSE;
  *Lstk(*lw+1) = rep;
  return TRUE;
} 


int cre_listsmat_from_str(char *fname,int *lw,int *numi,int *stlw,int *m,int *n,char *Str[],unsigned long fname_len )
{
  int rep,ix1,il;
  if ( cre_smat_from_str_i(fname, stlw, m, n, Str, fname_len,&rep)== FALSE )
    return FALSE;
  *stlw = rep;
  il = iadr(*Lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *Lstk(*lw +1) = *stlw;
  return TRUE;
} 


/*------------------------------------------------------------------ 
 * Try to create a sparse matrix S of size mxn 
 *     - m is the number of rows of Matrix S 
 *     - n is the number of colums of Matrix S 
 *     - Str : a null terminated array of strings char **Str assumed 
 *             to contain at least m*n strings 
 *     - lw  : where to create the matrix on the stack 
 *------------------------------------------------------------------ */

int cre_sparse_from_ptr_i(char *fname, int *lw, int *m, int *n, SciSparse *S, unsigned long fname_len,  int *rep)
{
  double size = (double) ( (S->nel)*(S->it + 1) );

  int ix1,  il, lr, lc;
  int cx1l=1;
  il = iadr(*lw);

  ix1 = il + 5 + *m + S->nel;
  Err = sadr(ix1)  - *Lstk(Bot );
  if (Err > -size ) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return  FALSE;
  } ;
  *istk(il ) = sci_sparse;
  /* note: code sligtly modified (remark of C. Deroulers in the newsgroup) */
  if ( (*m == 0)  |  (*n == 0) ) {
      *istk(il + 1) = 0; 
      *istk(il + 2) = 0;
  } else {
    *istk(il + 1) = *m; 
    *istk(il + 2) = *n;
  }
  /* end of the modified code */
  *istk(il + 3) = S->it;
  *istk(il + 4) = S->nel;
  C2F(icopy)(&S->m, S->mnel, &cx1l, istk(il+5 ), &cx1l);
  C2F(icopy)(&S->nel, S->icol, &cx1l, istk(il+5+*m ), &cx1l);
  ix1 = il + 5 + *m + S->nel;
  lr = sadr(ix1);
  lc = lr + S->nel;
  C2F(dcopy)(&S->nel, S->R, &cx1l, stk(lr), &cx1l);
  if ( S->it == 1) 
    C2F(dcopy)(&S->nel, S->I, &cx1l, stk(lc), &cx1l);
  *rep = lr + S->nel*(S->it+1);
  return TRUE;
} 


int cre_sparse_from_ptr(char *fname,int *lw,int *m,int *n,SciSparse *Str,unsigned long fname_len )
{
  int rep;
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }

  if ( cre_sparse_from_ptr_i(fname, Lstk(*lw ), m, n, Str, fname_len,&rep)== FALSE )
    return FALSE;
  *Lstk(*lw+1) = rep;
  return TRUE;
} 


int cre_listsparse_from_ptr(char *fname,int *lw,int *numi,int *stlw,int *m,int *n,SciSparse *Str,unsigned long fname_len )
{
  int rep,ix1,il;
  if ( cre_sparse_from_ptr_i(fname, stlw, m, n, Str, fname_len,&rep)== FALSE )
    return FALSE;
  *stlw = rep;
  il = iadr(*Lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *Lstk(*lw +1) = *stlw;
  return TRUE;
} 



/*------------------------------------------------------------------ 
 * TODO : add comments
 * listcrestring 
 *------------------------------------------------------------------ */

int C2F(listcrestring)(char *fname,int *lw,int *numi,int *stlw,int *nch,int *ilrs,unsigned long fname_len)
{
  int ix1, il ;

  if ( C2F(crestringi)(fname, stlw, nch, ilrs, fname_len) == FALSE )
    return FALSE;

  ix1 = *ilrs - 1 + *istk(*ilrs - 2 +1);
  *stlw = sadr(ix1);
  il = iadr(*Lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1)) {
    *Lstk(*lw +1) = *stlw;
  }
  return TRUE;
}

/*------------------------------------------------------------------ 
*     verifie que l'on peut stocker une matrice [1,1] 
*     de chaine de caracteres a la position spos du stack 
*     en renvoyant .true. ou .false.  suivant la reponse. 
*     nchar est le nombre de caracteres que l'on veut stocker 
*     Entree : 
*       spos : position (entier) 
*     Sortie : 
*       ilrs 
*------------------------------------------------------------------ */

int C2F(crestring)(char *fname,int *spos,int *nchar,int *ilrs,unsigned long fname_len)
{
  int ix1;
  if ( C2F(crestringi)(fname, Lstk(*spos ), nchar, ilrs, fname_len) == FALSE) 
    return FALSE;
  ix1 = *ilrs + *nchar;
  *Lstk(*spos +1) = sadr(ix1);
  /* empty strings */
  if ( *nchar == 0)   *Lstk(*spos +1) += 1;
  return TRUE;
}

/*------------------------------------------------------------------ 
 *     verifie que l'on peut stocker une matrice [1,1] 
 *     de chaine de caracteres  a la position stlw en renvoyant .true. ou .false. 
 *     suivant la reponse. 
 *     nchar est le nombre de caracteres que l'on veut stcoker 
 *     Entree : 
 *       stlw : position (entier) 
 *     Sortie : 
 *       nchar : nombre de caracteres stockable 
 *       lr : pointe sur  a(1,1)=istk(lr) 
 *------------------------------------------------------------------ */

int C2F(crestringi)(char *fname,int *stlw,int *nchar,int *ilrs,unsigned long fname_len)
{

  int ix1, ilast, il;

  il = iadr(*stlw);
  ix1 = il + 4 + (*nchar + 1);
  Err = sadr(ix1) - *Lstk(Bot );
  if (Err > 0) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  } ;
  *istk(il ) = sci_strings;
  *istk(il +1) = 1;
  *istk(il + 1 +1) = 1;
  *istk(il + 2 +1) = 0;
  *istk(il + 3 +1) = 1;
  *istk(il + 4 +1) = *istk(il + 3 +1) + *nchar;
  ilast = il + 5;
  *ilrs = ilast + *istk(ilast - 2 +1);
  return TRUE;
}


/*---------------------------------------------------------------------
 *  checks if we can store a string of size nchar at position lw 
 *---------------------------------------------------------------------*/

int C2F(fakecresmat2)(int *lw,int *nchar,int *lr)
{
  static int cx17 = 17;
  int retval;
  static int ilast;
  static int il;
  il = iadr((*Lstk(*lw)));
  Err = sadr(il + 4 + (*nchar + 1)) - *Lstk(Bot);
  if (Err > 0) 
  {
    Error(cx17);
    retval = FALSE;
  }
  else
  {
    ilast = il + 5;
    *Lstk(*lw+1) = sadr(ilast + *istk(ilast));
    *lr = ilast + *istk(ilast - 1);
    retval = TRUE;
  }
  return retval;
}

/*------------------------------------------------------------------ 
 *     verifie qu'il y a une matrice de chaine de caracteres en lw-1 
 *     et verifie que l'on peut stocker l'extraction de la jieme colonne 
 *     en lw : si oui l'extraction est faite 
 *     Entree : 
 *       lw : position (entier) 
 *       j  : colonne a extraire 
 *------------------------------------------------------------------ */

int C2F(smatj)(char *fname,int *lw,int *j,unsigned long fname_len)
{
  int ix1, ix2;
  int incj;
  int ix, m, n;
  int lj, nj, lr, il1, il2, nlj;
  int il1j, il2p;

  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  ix1 = *lw - 1;
  ix2 = *lw - 1;
  
  if (! C2F(getsmat)(fname, &ix1, &ix2, &m, &n, &cx1, &cx1, &lr, &nlj, fname_len)) 
    return FALSE;
  if (*j > n) return FALSE;
  
  il1 = iadr(*Lstk(*lw - 2 +1));
  il2 = iadr(*Lstk(*lw ));
  /*     nombre de caracteres de la jieme colonne */
  incj = (*j - 1) * m;
  nj = *istk(il1 + 4 + incj + m ) - *istk(il1 + 4 + incj );
  /*     test de place */
  ix1 = il2 + 4 + m + nj + 1;
  Err = sadr(ix1) - *Lstk(Bot );
  if (Err > 0) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  *istk(il2 ) = sci_strings;
  *istk(il2 +1) = m;
  *istk(il2 + 1 +1) = 1;
  *istk(il2 + 2 +1) = 0;
  il2p = il2 + 4;
  il1j = il1 + 4 + incj;
  *istk(il2p ) = 1;
  ix1 = m;
  for (ix = 1; ix <= ix1; ++ix) {
    *istk(il2p + ix ) = *istk(il2p - 1 + ix ) + *istk(il1j + ix ) - *istk(il1j + ix - 2 +1);
  }
  lj = *istk(il1 + 4 + incj ) + il1 + 4 + m * n;
  C2F(icopy)(&nj, istk(lj ), &cx1, istk(il2 + 4 + m +1), &cx1);
  ix1 = il2 + 4 + m + nj + 1;
  *Lstk(*lw +1) = sadr(ix1);
  return TRUE;
} 


/*------------------------------------------------------------------ 
 *     copie la matrice de chaine de caracteres stockee en flw 
 *     en tlw, les verifications de dimensions 
 *     ne sont pas faites 
 *     Lstk(tlw+1) est modifie si necessaire 
 *------------------------------------------------------------------ */

int C2F(copysmat)(char *fname,int *flw,int *tlw,unsigned long fname_len)
{
  int ix1;
  int dflw, fflw;
  int dtlw;
  dflw = iadr(*Lstk(*flw ));
  fflw = iadr(*Lstk(*flw +1));
  dtlw = iadr(*Lstk(*tlw ));
  ix1 = fflw - dflw;
  C2F(icopy)(&ix1, istk(dflw ), &cx1, istk(dtlw ), &cx1);
  *Lstk(*tlw +1) = *Lstk(*tlw ) + *Lstk(*flw +1) - *Lstk(*flw );
  return 0;
}

/*------------------------------------------------------------------ 
 *     lw designe une matrice de chaine de caracteres 
 *     on veut changer la taille de la chaine (i,j) 
 *     et lui donner la valeur nlr 
 *     cette routine si (i,j) != (m,n) fixe 
 *     le pointeur de l'argument i+j*m +1 
 *     sans changer les valeurs de la matrice 
 *     si (i,j)=(m,n) fixe juste la longeur de la chaine 
 *     Entree : 
 *       fname : nom de la routine appellante pour le message 
 *       d'erreur 
 *       lw : position dans la pile 
 *       i,j : indice considere 
 *       m,n : taille de la matrice 
 *       lr  : 
 *------------------------------------------------------------------ */

int C2F(setsimat)(char *fname,int *lw,int *ix,int *j,int *nlr,unsigned long fname_len)
{
  int k, m, il;
  il = iadr(*Lstk(*lw ));
  m = *istk(il +1);
  k = *ix - 1 + (*j - 1) * m;
  *istk(il + 4 + k +1) = *istk(il + 4 + k ) + *nlr;
  return 0;
}

/**********************************************************************
 * LISTS
 **********************************************************************/

/*------------------------------------------------------------------- 
 * crelist :creation of a list with ilen elements at slw position 
 * cretlist:creation of a tlist with ilen elements at slw position 
 * cremlist:creation of an mlist with ilen elements at slw position 
 *    In : slw and ilen 
 *    Out : lw 
 *     first element can be stored at postion stk(lw) 
 * Note : elements are to be added to close the list creation 
 *------------------------------------------------------------------- */

int crelist_G(int *slw,int *ilen,int *lw,int type)
{
  int ix1;
  int il;
  il = iadr(*Lstk(*slw ));
  *istk(il ) = type;
  *istk(il + 1) = *ilen;
  *istk(il + 2) = 1;
  ix1 = il + *ilen + 3;
  *lw = sadr(ix1);
  if (*ilen == 0) *Lstk(*slw +1) = *lw;
  return 0;
} 


int C2F(crelist)(int *slw,int *ilen,int *lw)
{
  return crelist_G(slw,ilen,lw,15);
} 

int C2F(cretlist)(int *slw,int *ilen,int *lw)
{
  return crelist_G(slw,ilen,lw,16);
} 

int C2F(cremlist)(int *slw,int *ilen,int *lw)
{
  return crelist_G(slw,ilen,lw,17);
} 


/*------------------------------------------------------------------ 
 * lmatj : 
 *   checks that there's a list at position  lw-1 
 *   checks that the j-th element can be extracted at lw position 
 *   perform the extraction 
 *       lw : position 
 *       j  : element to be extracted 
 *------------------------------------------------------------------ */

int C2F(lmatj)(char *fname,int *lw,int *j,unsigned long fname_len)
{
  int ix1, ix2;
  int n;
  int il, ilj, slj;
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  ix1 = *lw - 1;
  ix2 = *lw - 1;
  if (! C2F(getilist)(fname, &ix1, &ix2, &n, j, &ilj, fname_len)) 
    return FALSE;
  if (*j > n)       return FALSE;
  /*     a ameliorer */
  il = iadr(*Lstk(*lw - 2 +1));
  ix1 = il + 3 + n;
  slj = sadr(ix1) + *istk(il + 2 + (*j - 1) ) - 1;
  n = *istk(il + 2 + *j ) - *istk(il + 2 + (*j - 1) );
  Err = *Lstk(*lw ) + n - *Lstk(Bot );
  if (Err > 0) return FALSE;
  C2F(scidcopy)(&n, stk(slj ), &cx1, stk(*Lstk(*lw ) ), &cx1);
  *Lstk(*lw +1) = *Lstk(*lw ) + n;
  return TRUE;
} 

/*------------------------------------------------
 *     renvoit .true. si l'argument en lw est une liste 
 *     Entree : 
 *      fname : nom de la routine appellante pour le message 
 *          d'erreur 
 *      lw : position ds la pile 
 *      i  : element demande 
 *     Sortie : 
 *      n  : nombre d'elements ds la liste 
 *      ili : le ieme element commence en istk(iadr(ili)) 
 *     ==> pour recuperer un argument il suffit 
 *     de faire un lk=Lstk(top);Lstk(top)=ili; getmat(...,top,...);stk(top)=lk 
 *------------------------------------------------*/

int C2F(getilist)(char *fname,int *topk,int *lw,int *n,int *ix,int *ili,unsigned long fname_len)
{
  int ix1;
  int itype, il;

  il = iadr(*Lstk(*lw ));
  if (*istk(il ) < 0) {
    il = iadr(*istk(il +1));
  }

  itype = *istk(il );
  if (itype < sci_list || itype > sci_mlist) {
    Scierror(210,_("%s: Wrong type for argument %d: List expected.\n"),get_fname(fname,fname_len) , Rhs + (*lw - *topk));
    return FALSE;
  }
  *n = *istk(il +1);
  if (*ix <= *n) {
    ix1 = il + 3 + *n;
    *ili = sadr(ix1) + *istk(il + 2 + (*ix - 1) ) - 1;
  } else {
    *ili = 0;
  }
  return TRUE;
} 

/**********************************************************************
 * POLYNOMS 
 **********************************************************************/

/*------------------------------------------------
*     renvoit .true. si l'argument en lw est une matrice de polynome 
*             sinon appelle error et renvoit .false. 
*     Entree : 
*       fname : nom de la routine appellante pour le message 
*       d'erreur 
*       lw    : position ds la pile 
*     Sortie 
*       [it,m,n] caracteristiques de la matrice 
*       name : nom de la variable muette ( character*4) 
*       namel : taille de name <=4 ( uncounting trailling blanks) 
*       soit lij=istk(ilp+(i-1)+(j-1)*m) 
*       alors le degre zero de l'elements (i,j) est en 
*       stk(lr+lij) (partie reelle ) et stk(lc+lij) (imag) 
*       le degre de l'elt (i,j)= l(i+1)j - lij -1 
*      implicit undefined (a-z) 
*------------------------------------------------*/

int C2F(getpoly)(char *fname,int *topk,int *lw,int *it,int *m,int *n,char *namex,int *namel,int *ilp,int *lr,int *lc,unsigned long fname_len,unsigned long name_len)
{
  int ix1;

  int il;
  il = iadr(*Lstk(*lw ));
  if (*istk(il ) != 2) {
    Scierror(212,_("%s: Wrong type for argument %d: Polynomial matrix expected.\n"),get_fname(fname,fname_len), Rhs + (*lw - *topk));
    return FALSE;
  } ;
  *m = *istk(il +1);
  *n = *istk(il +2);
  *it = *istk(il + 3);
  *namel = 4;
  C2F(cvstr)(namel, istk(il + 4), namex, &cx1, 4L);
 L11:
  if (*namel > 0) {
    if ( namex[*namel - 1] == ' ') {
      --(*namel);
      goto L11;
    }
  }
  *ilp = il + 8;
  ix1 = *ilp + *m * *n + 1;
  *lr = sadr(ix1) - 1;
  *lc = *lr + *istk(*ilp + *m * *n ) - 1;
  return  TRUE;

} 


/*------------------------------------------------------------------ 
*     recupere un polynome 
*     md est son degre et son premier element est en 
*     stk(lr),stk(lc) 
*     Finir les tests 
*------------------------------------------------------------------ */

int C2F(getonepoly)(char *fname,int *topk,int *lw,int *it,int *md,char *namex,int *namel,int *lr,int *lc, unsigned long fname_len, unsigned long name_len)
{
  int m, n;
  int ilp;

  if (C2F(getpoly)(fname, topk, lw, it, &m, &n, namex, namel, &ilp, lr, lc, fname_len, 4L)
      == FALSE)
    return FALSE;

  if (m * n != 1) {
    Scierror(998,_("%s: Wrong type for argument %d : Polygon expected.\n"),get_fname(fname,fname_len),Rhs + (*lw - *topk));
    return FALSE;
  }
  *md = *istk(ilp +1) - *istk(ilp ) - 1;
  *lr += *istk(ilp );
  *lc += *istk(ilp );
  return TRUE;
} 

/*------------------------------------------------------------------ 
 * pmatj : 
 *   checks that there's a polynomial matrix  at position  lw-1 
 *   checks that the j-th column  can be extracted at lw position 
 *   perform the extraction 
 *       lw : position 
 *       j  : column  to be extracted 
 *------------------------------------------------------------------ */

int C2F(pmatj)(char *fname,int *lw,int *j,unsigned long fname_len)
{
  int ix1, ix2;
  char namex[4];
  int incj;
  int ix, l, m, n, namel;
  int l2, m2, n2, lc, il, lj, it, lr, il2, ilp;

  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  ix1 = *lw - 1;
  ix2 = *lw - 1;
  if (! C2F(getpoly)(fname, &ix1, &ix2, &it, &m, &n, namex, &namel, &ilp, &lr, &lc, fname_len, 4L)) {
    return FALSE;
  }
  if (*j > n)     return FALSE;

  /*     a ameliorer */
  il = iadr(*Lstk(*lw - 2 +1));
  incj = (*j - 1) * m;
  il2 = iadr(*Lstk(*lw ));
  ix1 = il2 + 4;
  l2 = sadr(ix1);
  m2 = Max(m,1);
  ix1 = il + 9 + m * n;
  l = sadr(ix1);
  n = *istk(il + 8 + m * n );
  ix1 = il2 + 9 + m2;
  l2 = sadr(ix1);
  n2 = *istk(il + 8 + incj + m ) - *istk(il + 8 + incj );
  Err = l2 + n2 * (it + 1) - *Lstk(Bot );
  if (Err > 0) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  C2F(icopy)(&cx4, istk(il + 3 +1), &cx1, istk(il2 + 3 +1), &cx1);
  il2 += 8;
  il = il + 8 + incj;
  lj = l - 1 + *istk(il );
  *istk(il2 ) = 1;
  ix1 = m2;
  for (ix = 1; ix <= ix1; ++ix) {
    *istk(il2 + ix ) = *istk(il2 - 1 + ix ) + *istk(il + ix ) - *istk(il - 1 + ix );
  }
  C2F(dcopy)(&n2, stk(lj ), &cx1, stk(l2 ), &cx1);
  if (it == 1) {
    C2F(dcopy)(&n2, stk(lj + n ), &cx1, stk(l2 + n2 ), &cx1);
  }
  *Lstk(Top +1) = l2 + n2 * (it + 1);
  il2 += -8;
  *istk(il2 ) = 2;
  *istk(il2 +1) = m2;
  *istk(il2 + 1 +1) = 1;
  *istk(il2 + 2 +1) = it;
  return TRUE;
}

/**********************************************************************
 * WORKING ARRAYS 
 **********************************************************************/

/*------------------------------------------------------------------ 
 * crewmat : uses the rest of the stack as a working area (double)
 *    In : 
 *       lw : position (entier) 
 *    Out: 
 *       m  : size that can be used 
 *       lr : stk(lr+i) is the working area 
 *------------------------------------------------------------------ */

int C2F(crewmat)(char *fname,int *lw,int *m,int *lr,unsigned long fname_len)
{
  int il,ix1; 
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  il = iadr(*Lstk(*lw ));
  *m = *Lstk(Bot ) - sadr(il+4);
  *istk(il ) = 1;
  *istk(il + 1) = 1;
  *istk(il + 2) = *m;
  *istk(il + 3) = 0;
  ix1 = il + 4;
  *lr = sadr(il+4);
  *Lstk(*lw +1) = sadr(il+4) + *m;
  return TRUE;
}

/*------------------------------------------------------------------ 
 * crewimat : uses the rest of the stack as a working area (int)
 *    In : 
 *       lw : position (entier) 
 *    Out: 
 *       m  : size that can be used 
 *       lr : istk(lr+i) is the working area 
 *------------------------------------------------------------------ */

int C2F(crewimat)(char *fname,int *lw,int *m,int *n,int *lr,unsigned long fname_len)
{
  double size = ((double) *m) * ((double) *n ); 
  int ix1,il;
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  il = iadr(*Lstk(*lw ));
  Err = il + 3  - iadr(*Lstk(Bot ));
  if (Err > -size ) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  *istk(il ) = 4;
  *istk(il + 1) = *m;
  *istk(il + 2) = *n;
  *lr = il + 3;
  ix1 = il + 3 + *m * *n + 2;
  *Lstk(*lw +1) = sadr(ix1);
  return TRUE;
}

/*------------------------------------------------
 * getwimat : used to get information about 
 *     a working area set by crewimat 
 *     In : 
 *       fname, topk, lw 
 *     Out : 
 *       m, n :  dimensions 
 *       lr : working area is istk(lr+i) i=0,m*n-1
 *------------------------------------------------ */

int C2F(getwimat)(char *fname,int *topk,int *lw,int *m,int *n,int *lr,unsigned long fname_len)
{
  int il;
  il = iadr(*Lstk(*lw ));
  if (*istk(il ) < 0) {
    il = iadr(*istk(il +1));
  }
  if (*istk(il ) != sci_boolean) {
    Scierror(213,_("%s: Wrong type for argument %d: Working int matrix expected.\n"),get_fname(fname,fname_len),Rhs + (*lw - *topk));
    return FALSE;
  };
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  *lr = il + 3;
  return TRUE;
} 

/*------------------------------------------------------------------ 
 *     creation of an object of type pointer at spos position on the stack 
 *     the pointer points to an object of type char matrix created by 
 *     the c routine stringc and is filled with a scilab stringmat 
 *     which was stored at istk(ilorig) 
 *     stk(lw) is used to transmit the pointer 
 *     F: transforme une stringmat scilab en un objet de type 
 *     pointeur qui pointe vers une traduction en C de la stringMat 
 *------------------------------------------------------------------- */

int C2F(crestringv)(char *fname,int *spos,int *ilorig,int *lw,unsigned long fname_len)
{
  int ierr;
  if (C2F(crepointer)(fname, spos, lw, fname_len) == FALSE) 
    return FALSE;

  C2F(stringc)(istk(*ilorig ), (char ***)stk(*lw ), &ierr);

  if (ierr != 0) {
    Scierror(999,_("%s: No more memory.\n"), fname);
    return FALSE;
  }
  return TRUE;
}

/*---------------------------------------------------------- 
 *  listcrepointer(top,numero,lw,....) 
 *---------------------------------------------------------- */

int C2F(listcrepointer)(char *fname,int *lw,int *numi,int *stlw,int *lrs,unsigned long fname_len)
{
  int ix1,il ;
  if (C2F(crepointeri)(fname, stlw,  lrs, &c_true, fname_len)==FALSE)
    return FALSE ;
  *stlw = *lrs + 2;
  il = iadr(*Lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *Lstk(*lw +1) = *stlw;
  return TRUE;
} 

/*---------------------------------------------------------- 
 *  crepointer :
 *---------------------------------------------------------- */

int C2F(crepointer)(char *fname,int *lw,int *lr,unsigned long fname_len)
{

  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  if ( C2F(crepointeri)(fname, Lstk(*lw ), lr, &c_true, fname_len) == FALSE)
    return FALSE ;
  *Lstk(*lw +1) = *lr + 2;
  return TRUE;
} 

/*--------------------------------------------------------- 
 * internal function used by crepointer and listcrepointer 
 *---------------------------------------------------------- */
static int C2F(crepointeri)(char *fname,int *stlw,int *lr,int *flagx,unsigned long fname_len)
{
  int ix1;
  int il;
  il = iadr(*stlw);
  ix1 = il + 4;
  Err = sadr(ix1) + 2 - *Lstk(Bot );
  if (Err > 0) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  };
  if (*flagx) {
    *istk(il ) = sci_lufact_pointer;
    /* if m*n=0 then both dimensions are to be set to zero */
    *istk(il + 1) = 1;
    *istk(il + 2) = 1;
    *istk(il + 3) = 0;
  }
  ix1 = il + 4;
  *lr = sadr(ix1);
  return TRUE;
} 

/*------------------------------------------------------------------- 
 *     creates a Scilab stringpointer on the stack at position spos 
 *     of size mxn the stringpointer is filled with the datas stored 
 *     in stk(lorig) ( for example created with cstringv ) 
 *     and the data stored at stk(lorig) is freed 
 *------------------------------------------------------------------- */

int C2F(lcrestringmatfromc)(char *fname,int *spos,int *numi,int *stlw,int *lorig,int *m,int *n,unsigned long fname_len)
{
  int ix1;
  int ierr;
  int il, ilw;
  ilw = iadr(*stlw);
  ix1 = *Lstk(Bot ) - *stlw;
  C2F(cstringf)((char ***)stk(*lorig ), istk(ilw ), m, n, &ix1, &ierr);
  if (ierr > 0) {
    Scierror(999,_("%s: No more memory.\n"), fname);
    return FALSE;
  }
  ix1 = ilw + 5 + *m * *n + *istk(ilw + 4 + *m * *n ) - 1;
  *stlw = sadr(ix1);
  il = iadr(*Lstk(*spos ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1)) {
    *Lstk(*spos +1) = *stlw;
  }
  return TRUE;
}


/*------------------------------------------------------------------- 
 *     creates a Scilab stringmat on the stack at position spos 
 *     of size mxn the stringmat is filled with the datas stored 
 *     in stk(lorig) ( for example created with cstringv ) 
 *     and the data stored at stk(lorig) is freed 
 *------------------------------------------------------------------- */

int C2F(crestringmatfromc)(char *fname,int *spos,int *lorig,int *m,int *n,unsigned long fname_len)
{
  int ix1;
  int ierr;
  int ilw;
  ilw = iadr(*Lstk(*spos ));
  ix1 = *Lstk(Bot ) - *Lstk(*spos );
  C2F(cstringf)((char ***)stk(*lorig ), istk(ilw ), m, n, &ix1, &ierr);
  if (ierr > 0) {
    Scierror(999,_("%s: No more memory.\n"), fname);
    return FALSE;
  }
  ix1 = ilw + 5 + *m * *n + *istk(ilw + 4 + *m * *n ) - 1;
  *Lstk(*spos +1) = sadr(ix1);
  return  TRUE;
}

/*------------------------------------------------------------------ 
 *     getlistvectrow : recupere un vecteur ligne dans une liste 
 *------------------------------------------------------------------ */

int C2F(getlistvectrow)(char *fname,int *topk,int *spos,int *lnum,int *it,int *m,int *n,int *lr,int *lc,unsigned long fname_len)
{
  int nv;
  int ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE) 
    return FALSE;

  if (*lnum > nv) {
    Scierror(999,_("%s: Wrong size for argument %d: At least %d expected.\n"),get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }

  if (C2F(getmati)(fname, topk, spos, &ili, it, m, n, lr, lc, &c_true, lnum, fname_len)== 
      FALSE) 
    return FALSE;
  if (*m != 1) {
    Scierror(999,_("%s: Wrong type for argument %d (List element: %d): Row vector expected.\n"),get_fname(fname,fname_len),Rhs + (*spos - *topk), *lnum);
    return FALSE;
  }
  return TRUE;
} 


/*------------------------------------------------------------------ 
 *     Fonction normalement identique a getmat mais rajoutee 
 *     pour ne pas avoir a changer le stack.f de interf 
 *     renvoit .true. si l'argument en spos est une matrice 
 *             sinon appelle error et renvoit .false. 
 *     Entree : 
 *       fname : nom de la routine appellante pour le message 
 *       d'erreur 
 *       spos    : position ds la pile 
 *     Sortie 
 *       [it,m,n] caracteristiques de la matrice 
 *       lr : pointe sur la partie reelle ( si la matrice est a 
 *              a(1,1)=stk(lr) 
 *            si l'on veut acceder a des entiers 
 * 			   a(1,1)=istk(adr(lr,0)) 
 *       lc : pointe sur la partie imaginaire si elle existe sinon sur zero 
 *------------------------------------------------------------------ */

int C2F(getvectrow)(char *fname,int *topk,int *spos,int *it,int *m,int *n,int *lr,int *lc,unsigned long fname_len)
{
  if (C2F(getmati)(fname, topk, spos, Lstk(*spos ), it, m, n, lr, lc, &c_false, &cx0, fname_len) == FALSE) 
    return FALSE;

  if (*m != 1) {
    Scierror(999,_("%s: Wrong type for argument %d: Row vector expected.\n"),get_fname(fname,fname_len),Rhs + (*spos - *topk));
    return FALSE;
  }
  return TRUE ;
} 

/*------------------------------------------------------------------ 
 *
 *------------------------------------------------------------------ */

int C2F(getlistvectcol)(char *fname,int *topk,int *spos,int *lnum,int *it,int *m,int *n,int *lr,int *lc,unsigned long fname_len)
{
  int nv;
  int ili;
  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE) 
    return FALSE;

  if (*lnum > nv) {
    Scierror(999,_("%s: Wrong size for argument %d: At least %d expected.\n"),get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }
  if ( C2F(getmati)(fname, topk, spos, &ili, it, m, n, lr, lc, &c_true, lnum, fname_len)
       == FALSE)
    return FALSE;

  if (*n != 1) {
    Scierror(999,_("%s: argument %d >(%d) should be a column vector.\n"), get_fname(fname,fname_len),Rhs + (*spos - *topk), *lnum);
    return FALSE;
  }
  return TRUE;
}

/*------------------------------------------------------------------ 
*     Fonction normalement identique a getmat mais rajoutee 
*     pour ne pas avoir a changer le stack.f de interf 
*     renvoit .true. si l'argument en spos est une matrice 
*             sinon appelle error et renvoit .false. 
*     Entree : 
*       fname : nom de la routine appellante pour le message 
*       d'erreur 
*       spos    : position ds la pile 
*     Sortie 
*       [it,m,n] caracteristiques de la matrice 
*       lr : pointe sur la partie reelle ( si la matrice est a 
*              a(1,1)=stk(lr) 
*            si l'on veut acceder a des entiers 
* 			   a(1,1)=istk(adr(lr,0)) 
*       lc : pointe sur la partie imaginaire si elle existe sinon sur zero 
*------------------------------------------------------------------ */

int C2F(getvectcol)(char *fname,int *topk,int *spos,int *it,int *m,int *n,int *lr,int *lc,unsigned long fname_len)
{

  if ( C2F(getmati)(fname, topk, spos, Lstk(*spos ), it, m, n, lr, lc, &c_false, &cx0, fname_len)
       == FALSE ) 
    return FALSE;

  if (*n != 1) {
    Scierror(999,_("%s: Wrong type for argument %d: Column vector expected.\n"),get_fname(fname,fname_len),Rhs + (*spos - *topk));
    return FALSE;
  }
  return TRUE;
}


int C2F(getlistsimat)(char *fname,int *topk,int *spos,int *lnum,int *m,int *n,int *ix,int *j,int *lr,int *nlr,unsigned long fname_len)
{
  int nv;
  int ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE) 
    return FALSE;

  if (*lnum > nv) {
    Scierror(999,_("%s: Wrong size for argument %d: At least %d expected.\n"),get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }
  return  C2F(getsmati)(fname, topk, spos, &ili, m, n, ix, j, lr, nlr, &c_true, lnum, fname_len);
} 

/*------------------------------------------------------------------- 
 *     recuperation d'un pointer 
 *------------------------------------------------------------------- */

int C2F(getpointer)(char *fname,int *topk,int *lw,int *lr,unsigned long fname_len)
{
  return C2F(getpointeri)(fname, topk, lw,Lstk(*lw), lr, &c_false, &cx0, fname_len);
} 

/*------------------------------------------------------------------ 
 * getlistpointer : 
 *    checks that spos object is a list 
 *    checks that lnum-element of the list exists and is a pointer 
 *    extracts pointer value 
 *     In  : 
 *       fname : name of calling function for error message 
 *       topk  : stack ref for error message 
 *       spos    : stack position 
 *     Out : 
 *       lw : stk(lw) a <<pointer>> casted to a double 
 *------------------------------------------------------------------ */

int C2F(getlistpointer)(char *fname,int *topk,int *spos,int *lnum,int *lw,unsigned long fname_len)
{
  int nv, ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE)
    return FALSE;

  if (*lnum > nv) {
    Scierror(999,_("%s: Wrong size for argument %d: At least %d expected.\n"),get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }
  return C2F(getpointeri)(fname, topk, spos, &ili, lw, &c_true, lnum, fname_len);
} 

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

static int C2F(getpointeri)(char *fname,int *topk,int *spos,int *lw,int *lr,int *inlistx,int *nel,unsigned long fname_len)
{
  int il;
  il = iadr(*lw);
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  if (*istk(il ) != sci_lufact_pointer) {
    sciprint("----%d\n",*istk(il));
    if (*inlistx) 
      Scierror(197,_("%s: Wrong type for argument %d (List element: %d): Boxed pointer expected.\n"),get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
    else 
      Scierror(198,_("%s: Wrong type for argument %d: Boxed pointer expected.\n"),get_fname(fname,fname_len),
	       Rhs + (*spos - *topk));
    return  FALSE;
  }
  *lr = sadr(il+4);
  return TRUE;
} 

/*-----------------------------------------------------------
 *     creates a matlab-like sparse matrix 
 *-----------------------------------------------------------*/

int C2F(mspcreate)(int *lw,int *m,int *n,int *nzMax,int *it)
{
  int ix1;
  int jc, il, ir; int NZMAX;
  int k,pr;
  double size;
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),"");
    return FALSE;
  }

  il = iadr(*Lstk(*lw ));
  NZMAX=*nzMax;
  if (NZMAX==0) NZMAX=1;
  ix1 = il + 4 + (*n + 1) + NZMAX;
  size = (*it + 1) * NZMAX ;
  Err = sadr(ix1)  - *Lstk(Bot );
  if (Err > -size ) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),"");
    return FALSE;
  };
  *istk(il ) = sci_matlab_sparse;
  /*        si m*n=0 les deux dimensions sont mises a zero. 
  *istk(il +1) = Min(*m , *m * *n);
  *istk(il + 1 +1) = Min(*n, *m * *n);     */
  *istk(il +1) = *m;
  *istk(il + 2) = *n;
  *istk(il + 3) = *it;
  *istk(il + 4) = NZMAX;
  jc = il + 5;

  for (k=0; k<*n+1; ++k) *istk(jc+k)=0;  /* Jc =0 */
  ir = jc + *n + 1;
  for (k=0; k<NZMAX; ++k) *istk(ir+k)=0;   /* Ir = 0 */
  pr = sadr(ir + NZMAX );

  for (k=0; k<NZMAX; ++k) *stk(pr+k)=0;    /* Pr =0  */
  ix1 = il + 4 + (*n + 1) + NZMAX;
  *Lstk(*lw +1) = sadr(ix1) + (*it + 1) * NZMAX + 1;

  C2F(intersci).ntypes[*lw-Top+Rhs-1] = '$';
  C2F(intersci).iwhere[*lw-Top+Rhs-1] = *Lstk(*lw);
  /* C2F(intersci).lad[*lw-Top+Rhs-1] = should point to numeric data */
  return TRUE;
}

/**********************************************************************
 * Utilities 
 **********************************************************************/

/*------------------------------------------
 * get_fname used for function names which can be non   
 * null-terminated strings when coming from 
 * a Fortran call
 *------------------------------------------*/

static char Fname[nlgh+1];

char *get_fname(char *fname,unsigned long fname_len)
{
  int i=0;
  int minlength = Min(fname_len,nlgh);
  strncpy(Fname,fname,minlength);
  Fname[minlength] = '\0';
  for ( i= 0 ; i < (int) minlength ; i++) 
    if (Fname[i] == ' ') { Fname[i]= '\0'; break;}
  return Fname;
}

/*------------------------------------------------------------------ 
 * realmat : 
 *     Top is supposed to be a matrix 
 *     and the matrix is chnaged to its real part 
 *------------------------------------------------------------------ */

int C2F(realmat)(void)
{
  int ix1;
  int m, n, il;

  il = iadr(*Lstk(Top ));
  if (*istk(il + 3 ) == 0) return 0;
  m = *istk(il + 1);
  n = *istk(il + 2);
  *istk(il + 3) = 0;
  ix1 = il + 4;
  *Lstk(Top +1) = sadr(ix1) + m * n;
  return 0;
}




/*------------------------------------------------------------------ 
*     copie l'objet qui est a la position lw de la pile 
*     a la position lwd de la pile 
*     copie faite avec dcopy 
*     pas de verification 
*      implicit undefined (a-z) 
*------------------------------------------------------------------ */

int C2F(copyobj)(char *fname,int *lw,int *lwd,unsigned long fname_len)
{
  int ix1,l,ld;
  l=*Lstk(*lw );
  ld=*Lstk(*lwd );

  ix1 = *Lstk(*lw +1) - l;
  /* check for overlaping region */
  if (l+ix1>ld||ld+ix1>l) 
    C2F(unsfdcopy)(&ix1, stk(l), &cx1, stk(ld), &cx1);
  else
    C2F(scidcopy)(&ix1, stk(l), &cx1, stk(ld), &cx1);
  *Lstk(*lwd +1) = ld + ix1;
  return 0;
}



/*------------------------------------------------
 *     copie l'objet qui est a la position lw de la pile
 *     a la position lwd de la pile 
 *     copie faite avec dcopy 
 *     et verification 
 *------------------------------------------------*/

int C2F(vcopyobj)(char *fname,int *lw,int *lwd,unsigned long fname_len)
{
  int l;
  int l1, lv;
  l = *Lstk(*lw );
  lv = *Lstk(*lw +1) - *Lstk(*lw );
  l1 = *Lstk(*lwd );
  if (*lwd + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  Err = *Lstk(*lwd ) + lv - *Lstk(Bot );
  if (Err > 0) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  /* check for overlaping region */
  if (l+lv>l1||l1+lv>l) 
    C2F(unsfdcopy)(&lv, stk(l), &cx1, stk(l1), &cx1);
  else
    C2F(scidcopy)(&lv, stk(l), &cx1, stk(l1), &cx1);

  *Lstk(*lwd +1) = *Lstk(*lwd ) + lv;
  return TRUE;
} 



/*------------------------------------------------== 
*     suppose qu'il y a une matrice en lw de taille it1,m1,n1,mn1, 
*     et une autre en lw+1 de taille it2,m2,n2,mn2 
*     et echange les matrices et change les valeurs de it1,m1,n1,... 
*     apres echange la taille de la matrice en lw est stocke ds(it1,m1,n1) 
*     et celle en lw+1 est stocke ds (it2,m2,n2) 
*     effet de bord il faut que lw+2 soit une place libre 
*------------------------------------------------== */


int C2F(swapmat)(char *fname,int *topk,int *lw,int *it1,int *m1,int *n1,int *mn1,int *it2,int *m2,int *n2,int *mn2,unsigned long fname_len)
{
  int ix1, ix2;
  int lc, lr;
  ix1 = *lw + 1;

  if ( C2F(cremat)(fname, &ix1, it1, m1, n1, &lr, &lc, fname_len)== FALSE)
    return FALSE ;

  ix1 = *lw + 2;
  C2F(copyobj)(fname, lw, &ix1, fname_len);
  ix1 = *lw + 1;
  C2F(copyobj)(fname, &ix1, lw, fname_len);
  ix1 = *lw + 2;
  ix2 = *lw + 1;
  C2F(copyobj)(fname, &ix1, &ix2, fname_len);
  if ( C2F(getmat)(fname, topk, lw, it1, m1, n1, &lr, &lc, fname_len) == FALSE )
    return FALSE;

  ix1 = *lw + 1;

  if (C2F(getmat)(fname, topk, &ix1, it2, m2, n2, &lr, &lc, fname_len) == FALSE )
    return FALSE;

  *mn1 = *m1 * *n1;
  *mn2 = *m2 * *n2;

  return TRUE;
} 


/*------------------------------------------------== 
*     verifie qu'en lw il y a une matrice de taille (it1,m1,n1) 
*     deplace cette matrice en lw+1, en reservant en lw 
*     la place pour stocker une matrice (it,m,n) 
*     insmat  verifie  qu'on a la place de faire tout ca 
*     appelle error en cas de probleme 
*     Remarque : noter par exemple que si it=it1,m1=m,n1=n 
*        alors apres le contenu de la matrice en lw est une copie de 
*        celle en lw+1 
*     Remarque : lw doit etre top car sinon on perd ce qu'il y avait avant 
*        en lw+1,....,lw+n 
*     Entree : 
*        lw : position 
*        it ,m,n : taille de la matrice a inserer 
*     Sortie : 
*       lr : pointe sur la partie reelle de la matrice 
*            en lw (   a(1,1)=stk(lr)) 
*       lc : pointe sur la partie imaginaire si besoin est 
*       lr1,lc1 : meme signification mais pour la matrice en lw+1 
*            ( matrice qui a ete copiee de lw a lw+1 
*------------------------------------------------== */

int C2F(insmat)(int *topk,int *lw,int *it,int *m,int *n,int *lr,int *lc,int *lr1,int *lc1)
{

  int ix1;
  int c_n1 = -1;
  int m1, n1;
  int lc0, it1, lr0;
  
  if (C2F(getmat)("insmat", topk, lw, &it1, &m1, &n1, &lr0, &lc0, 6L) == FALSE) 
    return FALSE;

  if (C2F(cremat)("insmat", lw, it, m, n, lr, lc, 6L)  == FALSE) 
    return FALSE;

  ix1 = *lw + 1;

  if (C2F(cremat)("insmat", &ix1, &it1, &m1, &n1, lr1, lc1, 6L) == FALSE) 
    return FALSE;

  ix1 = m1 * n1 * (it1 + 1);
  C2F(dcopy)(&ix1, stk(lr0 ), &c_n1, stk(*lr1 ), &c_n1);
  return TRUE;
} 




/*------------------------------------------------
 *     imprime le contenu de la pile en lw en mode entier ou 
 * 	  double precision suivant typ 
 *------------------------------------------------*/

int C2F(stackinfo)(int *lw,int *typ)
{
  int ix, l, m, n;
  int il, nn;

  if (*lw == 0) {
    return 0;
  }
  il = iadr(*Lstk(*lw ));
  if (*istk(il ) < 0) {
    il = iadr(*istk(il +1));
  }
  m = *istk(il +1);
  n = *istk(il + 1 +1);

  sciprint("-----------------stack-info-----------------\n");
  sciprint("lw=%d -[istk]-> il lw+1 -[istk]-> %d\n",*lw,iadr(*Lstk(*lw+1)));
  sciprint("istk(%d:..) ->[%d %d %d %d ....]\n",il, istk(il),istk(il+1),istk(il+2),istk(il+3) );
  if (*typ == 1) {
    l = sadr(il+4);
    nn = Min(m*n,3);
    for (ix = 0; ix <= nn-1 ; ++ix) {
      sciprint("%5.2f  ",stk(l + ix ));
    }
  } else {
    l = il + 4;
    nn = Min(m*n,3);
    for (ix = 0; ix <= nn-1; ++ix) {
      sciprint("%5d  ",istk(l + ix ));
    }
  }
  sciprint("\n");
  sciprint("-----------------stack-info-----------------\n");
  return 0;
}



/*------------------------------------------------
 * allmat :
 *  checks if object at position lw is a matrix 
 *  (scalar,string,polynom) 
 *  In : 
 *     fname,topk,lw 
 *  Out : 
 *     m,n
 *------------------------------------------------*/

int C2F(allmat)(char *fname,int *topk,int *lw,int *m,int *n,unsigned long fname_len)
{
  int itype, il;
  il = iadr(*Lstk(*lw ));
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  itype = *istk(il );
  if (itype != sci_matrix && itype != sci_poly && itype != sci_strings) {
    Scierror(209,_("%s: Wrong type for argument %d: Matrix expected.\n"),get_fname(fname,fname_len) ,  Rhs + (*lw - *topk));
    return FALSE;
  }
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  return TRUE;
} 

/*------------------------------------------------
 * Assume that object at position lw is a matrix 
 * and set its size to (m,n) 
 *------------------------------------------------*/

int C2F(allmatset)(char *fname,int *lw,int *m,int *n,unsigned long fname_len)
{
  int il;
  il = iadr(*Lstk(*lw ));
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  *istk(il + 1) = *m;
  *istk(il + 2) = *n;
  return 0;
} 

/*------------------------------------------------
 *     cree un objet vide en lw et met a jour lw+1 
 *     en fait lw doit etre top 
 *     verifie les cas particuliers lw=0 ou lw=1 
 *     ainsi que le cas particulier ou une fonction 
 *     n'a pas d'arguments (ou il faut faire top=top+1) 
 *------------------------------------------------ */

int C2F(objvide)(char *fname,int *lw,unsigned long fname_len)
{
  if (*lw == 0 || Rhs < 0) {
    ++(*lw);
  }
  *istk(iadr(*Lstk(*lw )) ) = 0;
  *Lstk(*lw +1) = *Lstk(*lw ) + 2;
  return 0;
}

/*------------------------------------------------
 *     renvoit .true. si l'argument en lw est un ``external'' 
 *             sinon appelle error et renvoit .false. 
 *     si l'argument est un external de type string 
 *         on met a jour la table des fonctions externes 
 *         corespondante en appellant fun 
 *     Entree : 
 *       fname : nom de la routine appellante pour le message 
 *       d'erreur 
 *       topk : numero d'argument d'appel pour le message d'ereur 
 *       lw    : position ds la pile 
 *     Sortie 
 *       type vaut true ou false 
 *       si l'external est de type chaine de caracteres 
 *       la chaine est mise ds name 
 *       et type est mise a true 
 *------------------------------------------------ */

int C2F(getexternal)(char *fname,int *topk,int *lw,char *namex,int *typex,void (*setfun)(char *,int *),unsigned long fname_len,unsigned long name_len)
{
  int ret_value;
  int irep;
  int m, n;
  int il, lr;
  int nlr;
  int i;
  il = C2F(gettype)(lw);
  switch ( il) {
  case sci_u_function : case sci_c_function : case sci_list :
    ret_value = TRUE;
    *typex = FALSE;
    break;
  case sci_strings :
    ret_value = C2F(getsmat)(fname, topk, lw, &m, &n, &cx1, &cx1, &lr, &nlr, fname_len);
    *typex = TRUE;
    for (i=0; i < (int)name_len ; i++ ) namex[i] = ' ';
    if (ret_value == TRUE) 
      {
	C2F(cvstr)(&nlr, istk(lr ), namex, &cx1, name_len);
	namex[nlr] = '\0';
	(*setfun)(namex, &irep); /* , name_len); */
	if (irep == 1) 
	  {
		  Scierror(50,_("%s: entry point %s not found in predefined tables or link table.\n"),get_fname(fname,fname_len),namex);
	    ret_value = FALSE;
	  }
      }
    break;
  default: 
    Scierror(211,_("%s: Wrong type for argument %d: Function or string (external function) expected.\n"),get_fname(fname,fname_len), Rhs + (*lw - *topk));
    ret_value = FALSE;
    break;
  }
  return ret_value;
} 

/*------------------------------------------------
 *------------------------------------------------ */

int C2F(checkval)(char *fname,int *ival1,int *ival2,unsigned long fname_len)
{
  if (*ival1 != *ival2) 
  {
    Scierror(999,_("%s: Incompatible sizes.\n"),get_fname(fname,fname_len));
    return  FALSE;
  } ;
  return  TRUE;
}

/*------------------------------------------------------------- 
 *      recupere si elle existe la variable name dans le stack et 
 *      met sa valeur a la position top et top est incremente 
 *      ansi que rhs 
 *      si la variable cherchee n'existe pas on renvoit false 
 *------------------------------------------------------------- */

int C2F(optvarget)(char *fname,int *topk,int *iel,char *namex,unsigned long fname_len,unsigned long name_len)
{
  int id[nsiz];
  C2F(cvname)(id, namex, &cx0, name_len);
  Fin = 0;
  /*     recupere la variable et incremente top */
  C2F(stackg)(id);
  if (Fin == 0) {
    Scierror(999,_("%s: Optional argument %d not given and default value %s not found.\n"),get_fname(fname,fname_len),*iel,namex);
    return FALSE;
  }
  ++Rhs;
  return TRUE;
}


/*------------------------------------------------------------- 
 * this routine adds nlr characters (coded in istk(*lr)) 
 * + a null character in Scilab character buffer C2F(cha1).buf
 * the characters are stored at position lbuf 
 * lbuf,lbufi,lbuff are the updated : 
 *     buf(lbufi:lbuff) will give the characters in buf at Fortran level 
 * and lbuf will give the next available position (i.e lbuff+2 since 
 * '\0' is added at position lbuff+1 
 * 
 * Note that at Fortran level buf(lbufi:lbuff) can be used as a C string argument 
 * since it is null terminated 
 *
 *------------------------------------------------------------- */

int C2F(bufstore)(char *fname,int *lbuf,int *lbufi,int *lbuff,int *lr,int *nlr,unsigned long fname_len)
{
  *lbufi = *lbuf;
  *lbuff = *lbufi + *nlr - 1;
  *lbuf = *lbuff + 2;
  if (*lbuff > bsiz) {
    Scierror(999,_("%f: No more space to store string arguments.\n"),get_fname(fname,fname_len) );
    return FALSE;
  }
  /* lbufi is a Fortran indice ==> offset -1 at C level */
  C2F(cvstr)(nlr, istk(*lr ), C2F(cha1).buf + (*lbufi - 1), &cx1, *lbuff - (*lbufi - 1));
  C2F(cha1).buf[*lbuff] = '\0';
  return TRUE;
}


/*------------------------------------------------------------- 
 * 
 *------------------------------------------------------------- */
int C2F(credata)(char *fname,int *lw,int m,unsigned long fname_len)
{
  int lr;
  lr = *Lstk(*lw );
  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  
  Err = lr   - *Lstk(Bot);
  if (Err > -m ) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  };
  /*  *Lstk(*lw +1) = lr + 1 + m/sizeof(double);  */
  /*    type 0  */
  *istk(iadr(lr)) = 0;
  *Lstk(*lw +1) = lr + (m+sizeof(double)-1)/sizeof(double);
  return TRUE;
} 
/* ==============================================================
MATRIX OF HANDLE
================================================================= */
/*--------------------------------------------------------- 
 * internal function used by crehmat and listcrehmat 
 *---------------------------------------------------------- */

static int C2F(crehmati)(char *fname,int *stlw,int *m,int *n,int *lr,int *flagx,unsigned long fname_len)
{
  int ix1;
  int il;
  double size = ((double) *m) * ((double) *n);
  il = iadr(*stlw);
  ix1 = il + 4;
  Err = sadr(ix1) - *Lstk(Bot );
  if ( (double) Err > -size ) {
    Scierror(17,_("%s: stack size exceeded (Use stacksize function to increase it).\n"),get_fname(fname,fname_len));
    return FALSE;
  };
  if (*flagx) {
    *istk(il ) = sci_handles;
    /* if m*n=0 then both dimensions are to be set to zero */
    *istk(il + 1) = Min(*m , *m * *n);
    *istk(il + 2) = Min(*n ,*m * *n);
    *istk(il + 3) = 0;
  }
  ix1 = il + 4;
  *lr = sadr(ix1);
  return TRUE;
} 

/*---------------------------------------------------------- 
 *  listcrehmat(top,numero,lw,....) 
 *      le numero ieme element de la liste en top doit etre un matrice 
 *      stockee a partir de Lstk(lw) 
 *      doit mettre a jour les pointeurs de la liste 
 *      ainsi que stk(top+1) 
 *      si l'element a creer est le dernier 
 *      lw est aussi mis a jour 
 *---------------------------------------------------------- */

int C2F(listcrehmat)(char *fname,int *lw,int *numi,int *stlw,int *m,int *n,int *lrs,unsigned long fname_len)
{
  int ix1,il ;
    
  if (C2F(crehmati)(fname, stlw, m, n, lrs, &c_true, fname_len)==FALSE)
    return FALSE ;

  *stlw = *lrs + *m * *n;
  il = iadr(*Lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *Lstk(*lw +1) = *stlw;
  return TRUE;
} 

/*---------------------------------------------------------- 
 *  crehmat :
 *   checks that a matrix of handle of size [m,n] can be stored at position  lw 
 *   <<pointers>> to data is returned on success
 *   In : 
 *     lw : position (entier) 
 *     m, n dimensions 
 *   Out : 
 *     lr : stk(lr+i-1)= h(i)
 *   Side effect : if matrix creation is possible 
 *     [m,n] are stored in Scilab stack 
 *     and lr is returned but stk(lr+..)  are unchanged 
 *---------------------------------------------------------- */

int C2F(crehmat)(char *fname,int *lw,int *m,int *n,int *lr,unsigned long fname_len)
{

  if (*lw + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),get_fname(fname,fname_len));
    return FALSE;
  }
  if ( C2F(crehmati)(fname, Lstk(*lw ), m, n, lr, &c_true, fname_len) == FALSE)
    return FALSE ;
  *Lstk(*lw +1) = *lr + *m * *n;
  return TRUE;
} 
/*------------------------------------------------------------------ 
 * getlisthmat : 
 *    checks that spos object is a list 
 *    checks that lnum-element of the list exists and is a matrix 
 *    extracts matrix information(m,n,lr) 
 *     In  : 
 *       fname : name of calling function for error message 
 *       topk  : stack ref for error message 
 *       lw    : stack position 
 *     Out : 
 *       [m,n] matrix dimensions 
 *       lr : stk(lr+i-1)= h(i)) 
 *------------------------------------------------------------------ */

int C2F(getlisthmat)(char *fname,int *topk,int *spos,int *lnum,int *m,int *n,int *lr,unsigned long fname_len)
{
  int nv, ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE)
    return FALSE;

  if (*lnum > nv) {
    Scierror(999,_("%s: Wrong size for argument %d: At least %d expected.\n"),get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE;
  }
  return C2F(gethmati)(fname, topk, spos, &ili, m, n, lr, &c_true, lnum, fname_len);
} 

/*------------------------------------------------------------------- 
 * gethmat :
 *     check that object at position lw is a matrix 
 *     In  : 
 *       fname : name of calling function for error message 
 *       topk  : stack ref for error message 
 *       lw    : stack position ( ``in the top sense'' )
 *     Out : 
 *       [m,n] matrix dimensions 
 *       lr : stk(lr+i-1)= h(i)
 *------------------------------------------------------------------- */

int C2F(gethmat)(char *fname,int *topk,int *lw,int *m,int *n,int *lr,unsigned long fname_len)
{
  return C2F(gethmati)(fname, topk, lw,Lstk(*lw), m, n, lr, &c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

static int C2F(gethmati)(char *fname,int *topk,int *spos,int *lw,int *m,int *n,int *lr,int *inlistx,int *nel,unsigned long fname_len)
{
  int il;
  il = iadr(*lw);
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  if (*istk(il ) != sci_handles) {
    if (*inlistx) 
      Scierror(999,_("%s: Wrong type for argument %d (List element: %d): Matrix of handle expected.\n"),get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
    else 
      Scierror(200,_("%s: Wrong type for argument %d: Matrix of handle expected.\n"),get_fname(fname,fname_len),
	       Rhs + (*spos - *topk));
    return  FALSE;
  }
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  *lr = sadr(il+4);
  return TRUE;
} 

