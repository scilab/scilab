/*------------------------------------------------------------------------
 *    Copyright (C) 1998-2000 Enpc/Inria 
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
/*------------------------------------------
 * Scilab stack 
 *------------------------------------------*/
#include <string.h>
#include <stdio.h>

#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif  

#include "../stack-c.h"
#include "../system/msgstore.h"

/* Table of constant values */

static integer cx0 = 0;
static integer cx1 = 1;
static integer cx4 = 4;
static int c_true = TRUE_;
static int c_false = FALSE_;

static int C2F(getwsmati) __PARAMS((char * fname, integer *topk, integer * spos,integer * lw,integer * m, integer *n,integer * ilr,integer * ilrd ,int * inlistx,integer* nel,unsigned long fname_len));

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

int C2F(getlistmat)(fname, topk, spos, lnum, it, m, n, lr, lc, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *it, *m, *n, *lr, *lc;
     unsigned long fname_len;
{
  integer nv, ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE_)
    return FALSE_;

  if (*lnum > nv) {
    Scierror(999,"%s: argument %d should be a list of size at least %d \r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_;
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

int C2F(getmat)(fname, topk, lw, it, m, n, lr, lc, fname_len)
     char *fname;
     integer *topk, *lw, *it, *m, *n, *lr, *lc;
     unsigned long fname_len;
{
  return C2F(getmati)(fname, topk, lw,lstk(*lw), it, m, n, lr, lc, &c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------
 * getrmat like getmat but we check for a real matrix 
 *------------------------------------------------------------------ */

int C2F(getrmat)(fname, topk, lw, m, n, lr, fname_len)
     char *fname;
     integer *topk, *lw, *m, *n, *lr;
     unsigned long fname_len;
{
  integer lc, it;

  if ( C2F(getmat)(fname, topk, lw, &it, m, n, lr, &lc, fname_len) == FALSE_ )
    return FALSE_;

  if (it != 0) {
    Scierror(202,"%s: Argument %d: wrong type argument expecting a real matrix\r\n",
	     get_fname(fname,fname_len), Rhs + (*lw - *topk));
    return FALSE_;
  }
  return TRUE_;
}

/*------------------------------------------------------------------ 
 * matsize :
 *    like getmat but here m,n are given on entry 
 *    and we check that matrix is of size (m,n) 
 *------------------------------------------------------------------ */

int C2F(matsize)(fname, topk, lw, m, n, fname_len)
     char *fname;
     integer *topk, *lw, *m, *n;
     unsigned long fname_len;
{
  integer m1, n1, lc, it, lr;
  
  if (  C2F(getmat)(fname, topk, lw, &it, &m1, &n1, &lr, &lc, fname_len)  == FALSE_)
    return FALSE_;
  if (*m != m1 || *n != n1) {
    Scierror(205,"%s: Argument %d: wrong matrix size (%d,%d) expected \r\n",
	     get_fname(fname,fname_len), Rhs + (*lw - *topk), *m,*n);
    return FALSE_;
  }
  return  TRUE_;
}

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

int C2F(getmati)(fname, topk, spos, lw, it, m, n, lr, lc, inlistx, nel, fname_len)
     char *fname;
     integer *topk, *spos, *lw, *it, *m, *n, *lr, *lc;
     int *inlistx;
     integer *nel;
     unsigned long fname_len;
{
  integer il;
  il = iadr(*lw);
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  if (*istk(il ) != 1) {
    if (*inlistx) 
      Scierror(999,"%s: argument %d >(%d) should be a real or complex matrix\r\n",
	       get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
    else 
      Scierror(201,"%s: argument %d should be a real or complex matrix\r\n",get_fname(fname,fname_len),
	       Rhs + (*spos - *topk));
    return  FALSE_;
  }
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  *it = *istk(il + 3);
  *lr = sadr(il+4);
  if (*it == 1)  *lc = *lr + *m * *n;
  return TRUE_;
} 


/*---------------------------------------------------------- 
 *  listcremat(top,numero,lw,....) 
 *      le numero ieme element de la liste en top doit etre un matrice 
 *      stockee a partir de lstk(lw) 
 *      doit mettre a jour les pointeurs de la liste 
 *      ainsi que stk(top+1) 
 *      si l'element a creer est le dernier 
 *      lw est aussi mis a jour 
 *---------------------------------------------------------- */

int C2F(listcremat)(fname, lw, numi, stlw, it, m, n, lrs, lcs, fname_len)
     char *fname;
     integer *lw, *numi, *stlw, *it, *m, *n, *lrs, *lcs;
     unsigned long fname_len;
{
  integer ix1,il ;
    
  if (C2F(cremati)(fname, stlw, it, m, n, lrs, lcs, &c_true, fname_len)==FALSE_)
    return FALSE_ ;

  *stlw = *lrs + *m * *n * (*it + 1);
  il = iadr(*lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *lstk(*lw +1) = *stlw;
  return TRUE_;
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

int C2F(cremat)(fname, lw, it, m, n, lr, lc, fname_len)
     char *fname;
     integer *lw, *it, *m, *n, *lr, *lc;
     unsigned long fname_len;
{

  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  if ( C2F(cremati)(fname, lstk(*lw ), it, m, n, lr, lc, &c_true, fname_len) == FALSE_)
    return FALSE_ ;
  *lstk(*lw +1) = *lr + *m * *n * (*it + 1);
  return TRUE_;
} 

/*-------------------------------------------------
 * Similar to cremat but we only check for space 
 * no data is stored 
 *-------------------------------------------------*/

int C2F(fakecremat)(lw, it, m, n, lr, lc)
     integer *lw, *it, *m, *n, *lr, *lc;
{
  if (*lw + 1 >= Bot) return FALSE_;
  if (C2F(cremati)("cremat", lstk(*lw ), it, m, n, lr, lc, &c_false, 6L) == FALSE_) 
    return FALSE_;
  *lstk(*lw +1) = *lr + *m * *n * (*it + 1);
  return TRUE_;
} 


/*--------------------------------------------------------- 
 * internal function used by cremat and listcremat 
 *---------------------------------------------------------- */

int C2F(cremati)(fname, stlw, it, m, n, lr, lc, flagx, fname_len)
     char *fname;
     integer *stlw, *it, *m, *n, *lr, *lc;
     int *flagx;
     unsigned long fname_len;
{
  integer ix1;
  integer il;

  il = iadr(*stlw);
  ix1 = il + 4;
  Err = sadr(ix1) + *m * *n * (*it + 1) - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",get_fname(fname,fname_len));
    return FALSE_;
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
  return TRUE_;
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

int C2F(getlistimat)(fname, topk, spos, lnum,it, m, n, lr, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *m, *n, *lr,*it;
     unsigned long fname_len;
{
  integer nv, ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE_)
    return FALSE_;

  if (*lnum > nv) {
    Scierror(999,"%s: argument %d should be a list of size at least %d \r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_;
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

int C2F(getimat)(fname, topk, lw,it, m, n, lr,  fname_len)
     char *fname;
     integer *topk, *lw, *m, *n, *lr,*it;
     unsigned long fname_len;
{
  return C2F(getimati)(fname, topk, lw,lstk(*lw),it,m, n, lr,&c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

int C2F(getimati)(fname, topk, spos, lw, it,m, n, lr, inlistx, nel, fname_len)
     char *fname;
     integer *topk, *spos, *lw,  *m, *n, *lr,*it;
     int *inlistx;
     integer *nel;
     unsigned long fname_len;
{
  integer il;
  il = iadr(*lw);
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  if (*istk(il ) != 8 ) {
    if (*inlistx) 
      Scierror(999,"%s: argument %d >(%d) should be an int matrix\r\n",
	       get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
    else 
      Scierror(201,"%s: argument %d should be a real or complex matrix\r\n",get_fname(fname,fname_len),
	       Rhs + (*spos - *topk));
    return  FALSE_;
  }
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  *it = *istk(il + 3);
  *lr = il+4;
  return TRUE_;
} 


/*---------------------------------------------------------- 
 *  listcreimat(top,numero,lw,....) 
 *      le numero ieme element de la liste en top doit etre un matrice 
 *      stockee a partir de lstk(lw) 
 *      doit mettre a jour les pointeurs de la liste 
 *      ainsi que stk(top+1) 
 *      si l'element a creer est le dernier 
 *      lw est aussi mis a jour 
 *---------------------------------------------------------- */

int C2F(listcreimat)(fname, lw, numi, stlw,it, m, n, lrs, fname_len)
     char *fname;
     integer *lw, *numi, *stlw, *m, *n, *lrs,*it;
     unsigned long fname_len;
{
  integer ix1,il ;
    
  if (C2F(creimati)(fname, stlw,it, m, n, lrs, &c_true, fname_len)==FALSE_)
    return FALSE_ ;
  *stlw = sadr(*lrs + memused(*it,*m * *n));
  il = iadr(*lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *lstk(*lw +1) = *stlw;
  return TRUE_;
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

int C2F(creimat)(fname, lw, it, m, n, lr, fname_len)
     char *fname;
     integer *lw, *it, *m, *n, *lr;
     unsigned long fname_len;
{

  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  if ( C2F(creimati)(fname, lstk(*lw ), it, m, n, lr,&c_true, fname_len) == FALSE_)
    return FALSE_ ;
  *lstk(*lw +1) = sadr(*lr + memused(*it,*m * *n));
  return TRUE_;
} 

/*--------------------------------------------------------- 
 * internal function used by cremat and listcremat 
 *---------------------------------------------------------- */

int C2F(creimati)(fname, stlw, it, m, n, lr, flagx, fname_len)
     char *fname;
     integer *stlw, *it, *m, *n, *lr;
     int *flagx;
     unsigned long fname_len;
{
  integer ix1;
  integer il;

  il = iadr(*stlw);
  ix1 = il + 4;
  Err = sadr(ix1) + memused(*it,(*m)*(*n)) - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",get_fname(fname,fname_len));
    return FALSE_;
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
  return TRUE_;
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

int C2F(getlistbmat)(fname, topk, spos, lnum, m, n, lr, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *m, *n, *lr;
     unsigned long fname_len;
{
  integer nv;
  integer ili;

  if (C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len)== FALSE_)
    return FALSE_ ;

  if (*lnum > nv) {
    Scierror(999,"%s: argument %d should be a list of size at least %d \r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_ ;
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

int C2F(getbmat)(fname, topk, lw, m, n, lr, fname_len)
     char *fname;
     integer *topk, *lw, *m, *n, *lr;
     unsigned long fname_len;
{
  return C2F(getbmati)(fname, topk, lw, lstk(*lw ), m, n, lr, &c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------ 
 * matbsize :
 *    like getbmat but here m,n are given on entry 
 *    and we check that matrix is of size (m,n) 
 *------------------------------------------------------------------ */

int C2F(matbsize)(fname, topk, lw, m, n, fname_len)
     char *fname;
     integer *topk, *lw, *m, *n;
     unsigned long fname_len;
{
  integer m1, n1, lr;
  if ( C2F(getbmat)(fname, topk, lw, &m1, &n1, &lr, fname_len) == FALSE_)
    return FALSE_;
  if (*m != m1 || *n != n1) {
    Scierror(205,"%s: Argument %d, wrong matrix size (%d,%d) expected\r\n",
	     get_fname(fname,fname_len),Rhs + (*lw - *topk),*m,*n);
    return FALSE_;
  }
  return TRUE_;
} 

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

int C2F(getbmati)(fname, topk, spos, lw, m, n, lr, inlistx, nel, fname_len)
     char *fname;
     integer *topk, *spos, *lw, *m, *n, *lr;
     int *inlistx;
     integer *nel;
     unsigned long fname_len;
{
  integer il;

  il = iadr(*lw);
  if (*istk(il ) < 0)    il = iadr(*istk(il +1));

  if (*istk(il ) != 4) {
    if (*inlistx) 
      Scierror(999,"%s: argument %d >(%d) should be a boolean matrix\r\n",
	       get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
    else 
      Scierror(208,"%s: argument %d should be a boolean matrix\r\n",get_fname(fname,fname_len),
	       Rhs + (*spos - *topk));
    return FALSE_;
  };
  *m = *istk(il +1);
  *n = *istk(il +2);
  *lr = il + 3;
  return  TRUE_;
} 

/*------------------------------------------------== 
 *      listcrebmat(top,numero,lw,....) 
 *      le numero ieme element de la liste en top doit etre un bmatrice 
 *      stockee a partir de lstk(lw) 
 *      doit mettre a jour les pointeurs de la liste 
 *      ainsi que stk(top+1) 
 *      si l'element a creer est le dernier 
 *      lw est aussi mis a jour 
 *---------------------------------------------------------- */

int C2F(listcrebmat)(fname, lw, numi, stlw, m, n, lrs, fname_len)
     char *fname;
     integer *lw, *numi, *stlw, *m, *n, *lrs;
     unsigned long fname_len;
{
  integer ix1;
  integer il;

  if ( C2F(crebmati)(fname, stlw, m, n, lrs, &c_true, fname_len)== FALSE_) 
    return FALSE_;

  ix1 = *lrs + *m * *n + 2;
  *stlw = sadr(ix1);
  il = iadr(*lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *lstk(*lw +1) = *stlw;
  return TRUE_;
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

int C2F(crebmat)(fname, lw, m, n, lr, fname_len)
     char *fname;
     integer *lw, *m, *n, *lr;
     unsigned long fname_len;
{
  integer ix1;
  
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_ ;
  }

  if ( C2F(crebmati)(fname, lstk(*lw ), m, n, lr, &c_true, fname_len)== FALSE_)
    return FALSE_ ;

  ix1 = *lr + *m * *n + 2;
  *lstk(*lw +1) = sadr(ix1);
  return TRUE_;
} 

/*-------------------------------------------------
 * Similar to crebmat but we only check for space 
 * no data is stored 
 *-------------------------------------------------*/

int C2F(fakecrebmat)( lw, m, n, lr) 
     integer *lw, *m, *n, *lr;
{
  if (*lw + 1 >= Bot) {
    Scierror(18,"fakecrebmat: too many names\r\n");
    return FALSE_;
  }
  if ( C2F(crebmati)("crebmat", lstk(*lw ), m, n, lr, &c_false, 7L)== FALSE_)
    return FALSE_ ;
  *lstk(*lw +1) = sadr( *lr + *m * *n + 2);
  return TRUE_;
} 

/*--------------------------------------------------------- 
 * internal function used by crebmat and listcrebmat 
 *---------------------------------------------------------- */

int C2F(crebmati)(fname, stlw, m, n, lr, flagx, fname_len)
     char *fname;
     integer *stlw, *m, *n, *lr;
     int *flagx;
     unsigned long fname_len;
{
  integer il;
  il = iadr(*stlw);
  Err = il + 3 + *m * *n - iadr(*lstk(Bot ));
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  if (*flagx) {
    *istk(il ) = 4;
    /*     si m*n=0 les deux dimensions sont mises a zero. */
    *istk(il + 1) = Min(*m , *m * *n);
    *istk(il + 2) = Min(*n,*m * *n);
  }
  *lr = il + 3;
  return TRUE_;
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

int C2F(getlistsparse)(fname, topk, spos, lnum, it, m, n, nel, mnel, icol, lr, lc, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *it, *m, *n, *nel, *mnel, *icol, *lr, *lc;
     unsigned long fname_len;
{
  integer  nv;
  integer ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE_) 
    return FALSE_ ;
  
  if (*lnum > nv) {
    Scierror(999,"%s: argument %d should be a list of size at least %d \r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_;
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

int C2F(getsparse)(fname, topk, lw, it, m, n, nel, mnel, icol, lr, lc, fname_len)
     char *fname;
     integer *topk, *lw, *it, *m, *n, *nel, *mnel, *icol, *lr, *lc;
     unsigned long fname_len;
{
  return C2F(getsparsei)(fname, topk, lw, lstk(*lw ), it, m, n, nel, mnel, icol, lr, lc, &c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------- 
 * getrsparse : lie getsparse but we check for a real matrix  
 *------------------------------------------------------------------- */

int C2F(getrsparse)(fname, topk, lw, m, n, nel, mnel, icol, lr, fname_len)
     char *fname;
     integer *topk, *lw, *m, *n, *nel, *mnel, *icol, *lr;
     unsigned long fname_len;
{
  integer lc, it;  
  if ( C2F(getsparse)(fname, topk, lw, &it, m, n, nel, mnel, icol, lr, &lc, fname_len) == FALSE_ ) 
    return FALSE_;

  if (it != 0) {
    Scierror(202,"%s: Argument %d: wrong type argument expecting a real matrix\r\n",
	     get_fname(fname,fname_len), Rhs + (*lw - *topk));
    return FALSE_;
  }
  return TRUE_;
}

/*--------------------------------------- 
 * internal function for getmat and listgetmat 
 *--------------------------------------- */

int C2F(getsparsei)(fname, topk, spos, lw, it, m, n, nel, mnel, icol, lr, lc, inlistx, nellist, fname_len)
     char *fname;
     integer *topk, *spos, *lw, *it, *m, *n, *nel, *mnel, *icol, *lr, *lc;
     int *inlistx;
     integer *nellist;
     unsigned long fname_len;
{
  integer il;

  il = iadr(*lw);
  if (*istk(il ) < 0)   il = iadr(*istk(il +1));

  if (*istk(il ) != 5) {
    if (*inlistx) 
      Scierror(999,"%s: argument %d >(%d) should be a sparse matrix\r\n",
	       get_fname(fname,fname_len), Rhs + (*spos - *topk), *nellist);
    else 
      Scierror(999,"%s: argument %d should be a sparse matrix\r\n",
	       get_fname(fname,fname_len), Rhs + (*spos - *topk));
    return FALSE_;
  }
  *m   = *istk(il + 1);
  *n   = *istk(il + 2);
  *it  = *istk(il + 3);
  *nel = *istk(il + 4);
  *mnel = il + 5;
  *icol = il + 5 + *m;
  *lr = sadr(il + 5 + *m + *nel);
  if (*it == 1)  *lc = *lr + *nel;
  return TRUE_;
} 

/*---------------------------------------------------------- 
 *      le numero ieme element de la liste en top doit etre une matrice 
 *      sparse stockee a partir de lstk(lw) 
 *      doit mettre a jour les pointeurs de la liste 
 *      ainsi que stk(top+1) 
 *      si l'element a creer est le dernier 
 *      lw est aussi mis a jour 
 * 
 *---------------------------------------------------------- */


int C2F(listcresparse)(fname, lw, numi, stlw, it, m, n, nel, mnel, icol, lrs, lcs, fname_len)
     char *fname;
     integer *lw, *numi, *stlw, *it, *m, *n, *nel, *mnel, *icol, *lrs, *lcs;
     unsigned long fname_len;
{
  integer ix1,il;

  if (C2F(cresparsei)(fname, stlw, it, m, n, nel, mnel, icol, lrs, lcs, fname_len)== FALSE_) 
    return FALSE_ ;

  *stlw = *lrs + *nel * (*it + 1);
  il = iadr(*lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1)) {
    *lstk(*lw +1) = *stlw;
  }
  return TRUE_;
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

int C2F(cresparse)(fname, lw, it, m, n, nel, mnel, icol, lr, lc, fname_len)
     char *fname;
     integer *lw, *it, *m, *n, *nel, *mnel, *icol, *lr, *lc;
     unsigned long fname_len;
{
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_ ;
  }
  
  if ( C2F(cresparsei)(fname, lstk(*lw ), it, m, n, nel, mnel, icol, lr, lc, fname_len)
       == FALSE_) 
    return FALSE_ ;
  *lstk(*lw +1) = *lr + *nel * (*it + 1);
  return TRUE_;
} 


/*--------------------------------------------------------- 
 * internal function used by cremat and listcremat 
 *---------------------------------------------------------- */

int C2F(cresparsei)(fname, stlw, it, m, n, nel, mnel, icol, lr, lc, fname_len)
     char *fname;
     integer *stlw, *it, *m, *n, *nel, *mnel, *icol, *lr, *lc;
     unsigned long fname_len;
{
  integer il,ix1;

  il = iadr(*stlw);
  ix1 = il + 5 + *m + *nel;
  Err = sadr(ix1) + *nel * (*it + 1) - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",get_fname(fname,fname_len));
    return FALSE_;
  };
  *istk(il ) = 5;
  /*   si m*n=0 les deux dimensions sont mises a zero. */
  *istk(il + 1) = Min(*m , *m * *n);
  *istk(il + 2) = Min(*n,*m * *n);
  *istk(il + 3) = *it;
  *istk(il + 4) = *nel;
  *mnel = il + 5;
  *icol = il + 5 + *m;
  ix1 = il + 5 + *m + *nel;
  *lr = sadr(ix1);
  *lc = *lr + *nel;
  return TRUE_;
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

int C2F(getlistvect)(fname, topk, spos, lnum, it, m, n, lr, lc, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *it, *m, *n, *lr, *lc;
     unsigned long fname_len;
{
  if (C2F(getlistmat)(fname, topk, spos, lnum, it, m, n, lr, lc, fname_len)== FALSE_) 
    return FALSE_;

  if (*m != 1 && *n != 1) {
    Scierror(999,"%s: argument %d >(%d) should be a vector \r\n",
	     get_fname(fname,fname_len),Rhs + (*spos - *topk), *lnum);
    return  FALSE_;
  }
  return TRUE_;
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

int C2F(getvect)(fname, topk, lw, it, m, n, lr, lc, fname_len)
     char *fname;
     integer *topk, *lw, *it, *m, *n, *lr, *lc;
     unsigned long fname_len;
{
  if ( C2F(getmat)(fname, topk, lw, it, m, n, lr, lc, fname_len) == FALSE_) 
    return FALSE_;

  if (*m != 1 && *n != 1) {
    Scierror(214,"%s: Argument %d: wrong type argument expecting a vector\r\n",
	     get_fname(fname,fname_len), Rhs + (*lw - *topk));
    return FALSE_;
  };
  return  TRUE_;
}


/*------------------------------------------------------------------ 
 * getrvect : like getvect but we expect a real vector 
 *------------------------------------------------------------------ */

int C2F(getrvect)(fname, topk, lw, m, n, lr, fname_len)
     char *fname;
     integer *topk, *lw, *m, *n, *lr;
     unsigned long fname_len;
{
  if ( C2F(getrmat)(fname, topk, lw, m, n, lr, fname_len)  == FALSE_)
    return FALSE_;

  if (*m != 1 && *n != 1) {
    Scierror(203,"%s: Argument %d: wrong type argument expecting a real vector\r\n",
	     get_fname(fname,fname_len), Rhs + (*lw - *topk));
    return FALSE_;
  }
  return TRUE_ ;
}

/*------------------------------------------------------------------ 
 * vectsize :
 *    like getvect but here n is given on entry 
 *    and we check that vector is of size (n) 
 *------------------------------------------------------------------ */

int C2F(vectsize)(fname, topk, lw, n, fname_len)
     char *fname;
     integer *topk, *lw, *n;
     unsigned long fname_len;
{
  integer m1, n1, lc, lr, it1;

  if ( C2F(getvect)(fname, topk, lw, &it1, &m1, &n1, &lr, &lc, fname_len) == FALSE_) 
    return FALSE_;

  if (*n != m1 * n1) {
    Scierror(206,"%s: Argument %d wrong vector size (%d) expected\r\n",
	     get_fname(fname,fname_len), Rhs + (*lw - *topk), *n);
    return FALSE_;
  }
  return TRUE_;
} 

/**********************************************************************
 * SCALAR   
 **********************************************************************/

/*------------------------------------------------------------------ 
 *     getlistscalar : recupere un scalaire 
 *------------------------------------------------------------------ */

int C2F(getlistscalar)(fname, topk, spos, lnum, lr, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *lr;
     unsigned long fname_len;
{
  integer m, n;
  integer lc, it, nv;
  integer ili;
  
  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE_)
    return FALSE_;

  if (*lnum > nv) {
    Scierror(999,"%s: argument %d should be a list of size at least %d \r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_;
  }
  
  if ( C2F(getmati)(fname, topk, spos, &ili, &it, &m, &n, lr, &lc, &c_true, lnum, fname_len)
       == FALSE_)
    return FALSE_;

  if (m * n != 1) {
    Scierror(999,"%s: argument %d > (%d) should be a scalar\r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_;
  }
  return TRUE_;
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

int C2F(getscalar)(fname, topk, lw, lr, fname_len)
     char *fname;
     integer *topk, *lw, *lr;
     unsigned long fname_len;
{
  integer m, n;

  if ( C2F(getrmat)(fname, topk, lw, &m, &n, lr, fname_len) == FALSE_) 
    return  FALSE_;

  if (m * n != 1) {
    Scierror(204,"%s: Argument %d: wrong wrong type argument expecting a scalar\r\n",
	     get_fname(fname,fname_len),Rhs + (*lw - *topk));
    return FALSE_ ; 
  };
  return TRUE_;
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

int C2F(getlistsmat)(fname, topk, spos, lnum, m,n , ix, j, lr, nlr, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *m, *n,*ix,*j, *lr, *nlr ;
     unsigned long fname_len;
{
  integer nv, ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE_)
    return FALSE_;

  if (*lnum > nv) {
    Scierror(999,"%s: argument %d should be a list of size at least %d \r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_;
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

int C2F(getsmat)(fname, topk, lw, m, n, ix, j, lr, nlr, fname_len)
     char *fname;
     integer *topk, *lw, *m, *n, *ix, *j, *lr, *nlr;
     unsigned long fname_len;
{
  return C2F(getsmati)(fname, topk, lw, lstk(*lw), m, n, ix,j , lr ,nlr,  &c_false, &cx0, fname_len);
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

int C2F(getsimat)(fname, topk, lw, m, n, ix, j, lr, nlr, fname_len)
     char *fname;
     integer *topk, *lw, *m, *n, *ix, *j, *lr, *nlr;
     unsigned long fname_len;
{
  return C2F(getsimati)(fname, topk, lw, lstk(*lw), m, n, ix,j , lr ,nlr,  &c_false, &cx0, fname_len);
}

/*--------------------------------------------------------------------------
 * getlistwsmat : 
 *    similar to getlistsmat but returned values are different 
 *       ilr  : 
 *       ilrd : 
 *    ilr and ilrd : internal coded versions of the strings 
 *    which can be converted to C with ScilabMStr2CM (see stack2.c)
 *------------------------------------------------------------------ */

int C2F(getlistwsmat)(fname, topk, spos, lnum, m, n, ilr, ilrd, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *m, *n, *ilr, *ilrd;
     unsigned long fname_len;
{
  integer nv, ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE_)
    return FALSE_;

  if (*lnum > nv) {
    Scierror(999,"%s: argument %d should be a list of size at least %d \r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_;
  }
  return C2F(getwsmati)(fname, topk, spos, &ili, m, n, ilr, ilrd, &c_true, lnum, fname_len);
} 

/*--------------------------------------------------------------------------
 * getwsmat : checks for a mxn string matrix 
 *    similar to getsmat but returned values are different 
 *    ilr and ilrd : internal coded versions of the strings 
 *    which can be converted to C with ScilabMStr2CM (see stack2.c)
 *--------------------------------------------------------------------------*/

int C2F(getwsmat)(fname, topk, lw, m, n, ilr, ilrd, fname_len)
     char *fname;
     integer *topk, *lw, *m, *n, *ilr, *ilrd;
     unsigned long fname_len;
{
  return C2F(getwsmati)(fname, topk, lw,lstk(*lw), m, n, ilr, ilrd, &c_false, &cx0, fname_len);
}

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

static int C2F(getwsmati)(fname, topk, spos, lw, m, n, ilr, ilrd , inlistx, nel, fname_len)
     char *fname;
     integer *topk, *spos, *lw,  *m, *n, *ilr, *ilrd;
     int *inlistx;
     integer *nel;
     unsigned long fname_len;
{
    integer il;
    il = iadr(*lw);
    if (*istk(il ) < 0) il = iadr(*istk(il +1));
    if (*istk(il ) != 10) {
      if (*inlistx)
	Scierror(999,"%s: argument %d >(%d) should be a matrix of strings\r\n",
		 get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
      else 
	Scierror(207,"%s: Argument %d : wrong type argument, expecting a matrix of strings\r\n",
		 get_fname(fname,fname_len), Rhs + (*spos - *topk));
      return FALSE_;
    }
    *m = *istk(il + 1);
    *n = *istk(il + 2);
    *ilrd = il + 4;
    *ilr =  il + 5 + *m * *n;
    return TRUE_;
} 

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

int C2F(getsmati)(fname, topk, spos, lw, m, n, ix,j , lr ,nlr, inlistx, nel, fname_len)
     char *fname;
     integer *topk, *spos, *lw,  *m, *n, *ix,*j,*lr,*nlr;
     int *inlistx, *nel;
     unsigned long fname_len;
{
  integer il = iadr(*lw);
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  if (*istk(il ) != 10 ) {
    if (*inlistx) 
      Scierror(999,"%s: argument %d >(%d) should be a  string  matrix\r\n",
	       get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
    else 
      Scierror(201,"%s: argument %d should be a string matrix\r\n",get_fname(fname,fname_len),
	       Rhs + (*spos - *topk));
    return  FALSE_;
  }
  C2F(getsimati)(fname, topk, spos, lw, m, n, ix,j , lr ,nlr, inlistx, nel, fname_len);
  return TRUE_;
} 

int C2F(getsimati)(fname, topk, spos, lw, m, n, ix,j , lr ,nlr, inlistx, nel, fname_len)
     char *fname;
     integer *topk, *spos, *lw, *m, *n,*ix,*j,*lr,*nlr;
     int *inlistx;
     integer *nel;
     unsigned long fname_len;
{
  integer k, il =  iadr(*lw);
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
 *     matrice stockee a partir de lstk(lw) 
 *     doit mettre a jour les pointeurs de la liste 
 *     ainsi que lstk(top+1) si l'element a creer est le dernier 
 *     lw est aussi mis a jour 
 *     job==1: nchar est la taille de chaque chaine de la  matrice 
 *     job==2: nchar est le vecteur des tailles des chaines de la 
 *             matrice 
 *     job==3: nchar est le vecteur des pointeurs sur les chaines 
 *             de la matrice 
 *---------------------------------------------------------- */

int C2F(listcresmat)(fname, lw, numi, stlw, m, n, nchar, job, ilrs, fname_len)
     char *fname;
     integer *lw, *numi, *stlw, *m, *n, *nchar, *job, *ilrs;
     unsigned long fname_len;
{
  integer ix1;
  integer il, sz;

  if ( C2F(cresmati)(fname, stlw, m, n, nchar, job, ilrs, &sz, fname_len) == FALSE_ )
    return FALSE_;
  ix1 = *ilrs + sz;
  *stlw = sadr(ix1);
  il = iadr(*lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *lstk(*lw +1) = *stlw;
  return TRUE_;
} 

/*---------------------------------------------------------- 
 * cresmat :
 *   checks that a string matrix [m,n] of strings 
 *   (each string is of length nchar)
 *   can be stored at position  lw on the stack 
 * Note that each string can be filled with getsimat 
 *---------------------------------------------------------- */

int C2F(cresmat)(fname, lw, m, n, nchar, fname_len)
     char *fname;
     integer *lw, *m, *n, *nchar;
     unsigned long fname_len;
{
  int job = 1;
  integer ix1, ilast, sz,lr ;
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return  FALSE_;
  }
  if ( C2F(cresmati)(fname,lstk(*lw), m, n, nchar, &job, &lr, &sz, fname_len) == FALSE_ )
    return FALSE_ ;
  ilast = lr - 1;
  ix1 = ilast + *istk(ilast );
  *lstk(*lw +1) = sadr(ix1);
  return TRUE_;
}

/*------------------------------------------------------------------ 
 *  cresmat1 :
 *   checks that a string matrix [m,1] of string of length nchar[i]
 *   can be stored at position  lw on the stack 
 *   nchar : array of length m giving each string length 
 *  Note that each string can be filled with getsimat 
 *------------------------------------------------------------------ */

int C2F(cresmat1)(fname, lw, m, nchar, fname_len)
     char *fname;
     integer *lw, *m, *nchar;
     unsigned long fname_len;
{
  int job = 2, n=1;
  integer ix1, ilast, sz,lr ;
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return  FALSE_;
  }
  if ( C2F(cresmati)(fname,lstk(*lw), m, &n, nchar, &job, &lr, &sz, fname_len) == FALSE_ )
    return FALSE_ ;
  ilast = lr - 1;
  ix1 = ilast + *istk(ilast );
  *lstk(*lw +1) = sadr(ix1);
  return TRUE_;
}

/*------------------------------------------------------------------ 
 *  cresmat2 :
 *   checks that a string of length nchar can be stored at position  lw 
 *  Out : 
 *     lr : istk(lr+i) give access to the internal array 
 *          allocated for string code 
 *------------------------------------------------------------------ */

int C2F(cresmat2)(fname, lw, nchar, lr, fname_len)
     char *fname;
     integer *lw, *nchar, *lr;
     unsigned long fname_len;
{
  int job = 1, n=1,m=1;
  integer ix1, ilast, sz ;
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return  FALSE_;
  }
  if ( C2F(cresmati)(fname,lstk(*lw), &m, &n, nchar, &job, lr, &sz, fname_len) == FALSE_ )
    return FALSE_ ;

  ilast = *lr - 1;
  ix1 = ilast + *istk(ilast );
  *lstk(*lw +1) = sadr(ix1);
  *lr = ilast + *istk(ilast - 1);
  return TRUE_;
} 

/*------------------------------------------------------------------ 
 * cresmat3 :
 *   Try to create a string matrix S of size mxn 
 *     - nchar: array of size mxn giving the length of string S(i,j)
 *     - buffer : a character array wich contains the concatenation 
 *             of all the strings 
 *     - lw  : stack position for string creation 
 *------------------------------------------------------------------ */

int C2F(cresmat3)(fname, lw, m, n, nchar, buffer, fname_len, buffer_len)
     char *fname;
     integer *lw, *m, *n, *nchar;
     char *buffer;
     unsigned long fname_len;
     unsigned long buffer_len;
{
  int job = 2;
  integer ix1, ilast, sz,lr,lr1 ;
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return  FALSE_;
  }
  if ( C2F(cresmati)(fname,lstk(*lw), m, n, nchar, &job, &lr, &sz, fname_len) == FALSE_ )
    return FALSE_ ;
  ilast = lr - 1;
  ix1 = ilast + *istk(ilast );
  *lstk(*lw +1) = sadr(ix1);

  lr1 = ilast + *istk(ilast - (*m)*(*n) );
  C2F(cvstr)(&sz, istk(lr1), buffer, &cx0, buffer_len);
  return TRUE_;
} 

/*------------------------------------------------------------------ 
 *     checks that an [m,1] string matrix can be stored in the 
 *     stack. 
 *     All chains have the same length nchar 
 *     istk(lr) --- beginning of chains 
 *------------------------------------------------------------------ */

int C2F(cresmat4)(fname, lw, m, nchar, lr, fname_len)
     char *fname;
     integer *lw, *m, *nchar, *lr;
     unsigned long fname_len;
{
  integer ix1,ix, ilast, il, nnchar, kij, ilp;
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  nnchar = 0;
  ix1 = *m;
  for (ix = 1; ix <= ix1; ++ix) nnchar += *nchar;
  il = iadr(*lstk(*lw ));
  ix1 = il + 4 + (nnchar + 1) * *m;
  Err = sadr(ix1) - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",get_fname(fname,fname_len));
    return FALSE_;
  } 
  *istk(il ) = 10;
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
  *lstk(*lw +1) = sadr(ix1);
  *lr = ilast + 1;
  return TRUE_;
}

/*--------------------------------------------------------- 
 * internal function used by cresmat cresmat1 and listcresmat 
 * job : 
 *   case 1: all string are of same length (nchar) in the matrix 
 *   case 2: nchar is a vector which gives string lengthes 
 *   case 3: ? 
 *---------------------------------------------------------- */

int C2F(cresmati)(fname, stlw, m, n, nchar, job, lr, sz, fname_len)
     char *fname;
     integer *stlw, *m, *n, *nchar, *job, *lr, *sz;
     unsigned long fname_len;
{
  integer ix1, ix, il, kij, ilp, mn= (*m)*(*n);
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
  Err = sadr(ix1) - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",
	     get_fname(fname,fname_len));
    return FALSE_;
  };
  
  *istk(il ) = 10;
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
	int ix1 = mn + 1;
	C2F(icopy)(&ix1, nchar, &cx1, istk(ilp ), &cx1);
      }
    }
  *lr = ilp + mn + 1;
  return TRUE_;
}

/*------------------------------------------------------------------ 
 * Try to create a string matrix S of size mxn 
 *     - m is the number of rows of Matrix S 
 *     - n is the number of colums of Matrix S 
 *     - Str : a null terminated array of strings char **Str assumed 
 *             to contain at least m*n strings 
 *     - lw  : where to create the matrix on the stack 
 *------------------------------------------------------------------ */

int cre_smat_from_str_i(fname, lw, m, n, Str, fname_len ,rep)
     char *fname;
     integer *lw, *m, *n;
     char *Str[];
     unsigned long fname_len;
     integer *rep;
{
  integer ix1, ix, ilast, il, nnchar, lr1, kij, ilp;
  integer *pos;

  nnchar = 0;
  for (ix = 0 ; ix < (*m)*(*n) ; ++ix) nnchar += strlen(Str[ix]);
  
  il = iadr(*lw);
  ix1 = il + 4 + (nnchar + 1) + (*m * *n + 1);
  Err = sadr(ix1) - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",
	     get_fname(fname,fname_len));
    return  FALSE_;
  } ;
  *istk(il ) = 10;
  *istk(il + 1) = *m;
  *istk(il + 2) = *n;
  *istk(il + 3) = 0;
  ilp = il + 4;
  *istk(ilp ) = 1;
  ix = 0;
  ix1 = ilp + *m * *n;
  for (kij = ilp + 1; kij <= ix1; ++kij) {
    *istk(kij ) = *istk(kij - 1) + strlen(Str[ix]);
    ++ix;
  }
  ilast = ilp + *m * *n;
  lr1 = ilast + *istk(ilp );
  pos = istk(lr1);
  for ( ix = 0 ; ix < (*m)*(*n) ; ix++) 
    {
      int l = strlen(Str[ix]);
      C2F(cvstr)(&l, pos, Str[ix], &cx0, l);
      pos += l;
    }
  ix1 = ilast + *istk(ilast );
  *rep = sadr(ix1);
  return TRUE_;
} 


int cre_smat_from_str(fname, lw, m, n, Str, fname_len )
     char *fname;
     integer *lw, *m, *n;
     char *Str[];
     unsigned long fname_len;
{
  int rep;
  
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }

  if ( cre_smat_from_str_i(fname, lstk(*lw ), m, n, Str, fname_len,&rep)== FALSE_ )
    return FALSE_;
  *lstk(*lw+1) = rep;
  return TRUE_;
} 


int cre_listsmat_from_str(fname, lw, numi, stlw,  m, n, Str, fname_len )
     char *fname;
     integer *lw, *m, *n,*numi,*stlw;
     char *Str[];
     unsigned long fname_len;
{
  int rep,ix1,il;
  if ( cre_smat_from_str_i(fname, stlw, m, n, Str, fname_len,&rep)== FALSE_ )
    return FALSE_;
  *stlw = rep;
  il = iadr(*lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *lstk(*lw +1) = *stlw;
  return TRUE_;
} 


/*------------------------------------------------------------------ 
 * Try to create a string matrix S of size mxn 
 *     - m is the number of rows of Matrix S 
 *     - n is the number of colums of Matrix S 
 *     - Str : a null terminated array of strings char **Str assumed 
 *             to contain at least m*n strings 
 *     - lw  : where to create the matrix on the stack 
 *------------------------------------------------------------------ */

int cre_sparse_from_ptr_i(fname, lw, m, n, S, fname_len ,rep)
     char *fname;
     integer *lw, *m, *n;
     SciSparse *S;
     unsigned long fname_len;
     integer *rep;
{
  integer ix1,  il, lr, lc;
  integer cx1=1;
  il = iadr(*lw);

  ix1 = il + 5 + *m + S->nel;
  Err = sadr(ix1) + *m * *n * (S->it + 1) - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",
	     get_fname(fname,fname_len));
    return  FALSE_;
  } ;
  *istk(il ) = 5;
  /*   si m*n=0 les deux dimensions sont mises a zero. */
  *istk(il + 1) = Min(*m , *m * *n);
  *istk(il + 2) = Min(*n,*m * *n);
  *istk(il + 3) = S->it;
  *istk(il + 4) = S->nel;
  C2F(icopy)(&S->m, S->mnel, &cx1, istk(il+5 ), &cx1);
  C2F(icopy)(&S->nel, S->icol, &cx1, istk(il+5+*m ), &cx1);
  ix1 = il + 5 + *m + S->nel;
  lr = sadr(ix1);
  lc = lr + S->nel;
  C2F(dcopy)(&S->nel, S->R, &cx1, stk(lr), &cx1);
  if ( S->it == 1) 
    C2F(dcopy)(&S->nel, S->I, &cx1, stk(lc), &cx1);
  *rep = lr + S->nel*(S->it+1);
  return TRUE_;
} 


int cre_sparse_from_ptr(fname, lw, m, n, Str, fname_len )
     char *fname;
     integer *lw, *m, *n;
     SciSparse *Str;
     unsigned long fname_len;
{
  int rep;
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }

  if ( cre_sparse_from_ptr_i(fname, lstk(*lw ), m, n, Str, fname_len,&rep)== FALSE_ )
    return FALSE_;
  *lstk(*lw+1) = rep;
  return TRUE_;
} 


int cre_listsparse_from_ptr(fname, lw, numi, stlw,  m, n, Str, fname_len )
     char *fname;
     integer *lw, *m, *n,*numi,*stlw;
     SciSparse *Str;
     unsigned long fname_len;
{
  int rep,ix1,il;
  if ( cre_sparse_from_ptr_i(fname, stlw, m, n, Str, fname_len,&rep)== FALSE_ )
    return FALSE_;
  *stlw = rep;
  il = iadr(*lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *lstk(*lw +1) = *stlw;
  return TRUE_;
} 



/*------------------------------------------------------------------ 
 * XXXXXXXX : j'en suis la 
 * listcrestring 
 *------------------------------------------------------------------ */

int C2F(listcrestring)(fname, lw, numi, stlw, nch, ilrs, fname_len)
     char *fname;
     integer *lw, *numi, *stlw, *nch, *ilrs;
     unsigned long fname_len;
{
  integer ix1, il ;

  if ( C2F(crestringi)(fname, stlw, nch, ilrs, fname_len) == FALSE_ )
    return FALSE_;

  ix1 = *ilrs - 1 + *istk(*ilrs - 2 +1);
  *stlw = sadr(ix1);
  il = iadr(*lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1)) {
    *lstk(*lw +1) = *stlw;
  }
  return TRUE_;
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

int C2F(crestring)(fname, spos, nchar, ilrs, fname_len)
     char *fname;
     integer *spos, *nchar, *ilrs;
     unsigned long fname_len;
{
  integer ix1;
  if ( C2F(crestringi)(fname, lstk(*spos ), nchar, ilrs, fname_len) == FALSE_) 
    return FALSE_;
  ix1 = *ilrs + *nchar;
  *lstk(*spos +1) = sadr(ix1);
  return TRUE_;
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

int C2F(crestringi)(fname, stlw, nchar, ilrs, fname_len)
     char *fname;
     integer *stlw, *nchar, *ilrs;
     unsigned long fname_len;
{

  integer ix1, ilast, il;

  il = iadr(*stlw);
  ix1 = il + 4 + (*nchar + 1);
  Err = sadr(ix1) - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",
	     get_fname(fname,fname_len));
    return FALSE_;
  } ;
  *istk(il ) = 10;
  *istk(il +1) = 1;
  *istk(il + 1 +1) = 1;
  *istk(il + 2 +1) = 0;
  *istk(il + 3 +1) = 1;
  *istk(il + 4 +1) = *istk(il + 3 +1) + *nchar;
  ilast = il + 5;
  *ilrs = ilast + *istk(ilast - 2 +1);
  return TRUE_;
}


/*---------------------------------------------------------------------
 *  checks if we can store a string of size nchar at position lw 
 *---------------------------------------------------------------------*/

int C2F(fakecresmat2)(lw, nchar, lr)
     integer *lw, *nchar, *lr;
{
  static integer cx17 = 17;
  int retval;
  static integer ilast;
  static integer il;
  il = iadr((*lstk(*lw)));
  Err = sadr(il + 4 + (*nchar + 1)) - *lstk(Bot);
  if (Err > 0) {
    C2F(error)(&cx17);
    retval = FALSE_;
  } else {
    ilast = il + 5;
    *lstk(*lw+1) = sadr(ilast + *istk(ilast));
    *lr = ilast + *istk(ilast - 1);
    retval = TRUE_;
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

int C2F(smatj)(fname, lw, j, fname_len)
     char *fname;
     integer *lw, *j;
     unsigned long fname_len;
{
  integer ix1, ix2;
  integer incj;
  integer ix, m, n;
  integer lj, nj, lr, il1, il2, nlj;
  integer il1j, il2p;

  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  ix1 = *lw - 1;
  ix2 = *lw - 1;
  
  if (! C2F(getsmat)(fname, &ix1, &ix2, &m, &n, &cx1, &cx1, &lr, &nlj, fname_len)) 
    return FALSE_;
  if (*j > n) return FALSE_;
  
  il1 = iadr(*lstk(*lw - 2 +1));
  il2 = iadr(*lstk(*lw ));
  /*     nombre de caracteres de la jieme colonne */
  incj = (*j - 1) * m;
  nj = *istk(il1 + 4 + incj + m ) - *istk(il1 + 4 + incj );
  /*     test de place */
  ix1 = il2 + 4 + m + nj + 1;
  Err = sadr(ix1) - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  *istk(il2 ) = 10;
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
  *lstk(*lw +1) = sadr(ix1);
  return TRUE_;
} 


/*------------------------------------------------------------------ 
 *     copie la matrice de chaine de caracteres stockee en flw 
 *     en tlw, les verifications de dimensions 
 *     ne sont pas faites 
 *     lstk(tlw+1) est modifie si necessaire 
 *------------------------------------------------------------------ */

int C2F(copysmat)(fname, flw, tlw, fname_len)
     char *fname;
     integer *flw, *tlw;
     unsigned long fname_len;
{
  integer ix1;
  integer dflw, fflw;
  integer dtlw;
  dflw = iadr(*lstk(*flw ));
  fflw = iadr(*lstk(*flw +1));
  dtlw = iadr(*lstk(*tlw ));
  ix1 = fflw - dflw;
  C2F(icopy)(&ix1, istk(dflw ), &cx1, istk(dtlw ), &cx1);
  *lstk(*tlw +1) = *lstk(*tlw ) + *lstk(*flw +1) - *lstk(*flw );
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

int C2F(setsimat)(fname, lw, ix, j, nlr, fname_len)
     char *fname;
     integer *lw, *ix, *j, *nlr;
     unsigned long fname_len;
{
  integer k, m, il;
  il = iadr(*lstk(*lw ));
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

int crelist_G(slw, ilen, lw, type)
     integer *slw, *ilen, *lw, type;
{
  integer ix1;
  integer il;
  il = iadr(*lstk(*slw ));
  *istk(il ) = type;
  *istk(il + 1) = *ilen;
  *istk(il + 2) = 1;
  ix1 = il + *ilen + 3;
  *lw = sadr(ix1);
  if (*ilen == 0) *lstk(*lw +1) = *lw;
  return 0;
} 


int C2F(crelist)(slw, ilen, lw)
     integer *slw, *ilen, *lw;
{
  return crelist_G(slw,ilen,lw,15);
} 

int C2F(cretlist)(slw, ilen, lw)
     integer *slw, *ilen, *lw;
{
  return crelist_G(slw,ilen,lw,16);
} 

int C2F(cremlist)(slw, ilen, lw)
     integer *slw, *ilen, *lw;
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

int C2F(lmatj)(fname, lw, j, fname_len)
     char *fname;
     integer *lw, *j;
     unsigned long fname_len;
{
  integer ix1, ix2;
  integer n;
  integer il, ilj, slj;
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  ix1 = *lw - 1;
  ix2 = *lw - 1;
  if (! C2F(getilist)(fname, &ix1, &ix2, &n, j, &ilj, fname_len)) 
    return FALSE_;
  if (*j > n)       return FALSE_;
  /*     a ameliorer */
  il = iadr(*lstk(*lw - 2 +1));
  ix1 = il + 3 + n;
  slj = sadr(ix1) + *istk(il + 2 + (*j - 1) ) - 1;
  n = *istk(il + 2 + *j ) - *istk(il + 2 + (*j - 1) );
  Err = *lstk(*lw ) + n - *lstk(Bot );
  if (Err > 0) return FALSE_;
  C2F(dcopy)(&n, stk(slj ), &cx1, stk(*lstk(*lw ) ), &cx1);
  *lstk(*lw +1) = *lstk(*lw ) + n;
  return TRUE_;
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
 *     de faire un lk=lstk(top);lstk(top)=ili; getmat(...,top,...);stk(top)=lk 
 *------------------------------------------------*/

int C2F(getilist)(fname, topk, lw, n, ix, ili, fname_len)
     char *fname;
     integer *topk, *lw, *n, *ix, *ili;
     unsigned long fname_len;
{
  integer ix1;
  integer itype, il;

  il = iadr(*lstk(*lw ));
  if (*istk(il ) < 0) {
    il = iadr(*istk(il +1));
  }

  itype = *istk(il );
  if (itype < 15 || itype > 17) {
    Scierror(210,"%s: Argument %d: wrong type argument, expecting a list\r\n",
	     get_fname(fname,fname_len) , Rhs + (*lw - *topk));
    return FALSE_;
  }
  *n = *istk(il +1);
  if (*ix <= *n) {
    ix1 = il + 3 + *n;
    *ili = sadr(ix1) + *istk(il + 2 + (*ix - 1) ) - 1;
  } else {
    *ili = 0;
  }
  return TRUE_;
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

int C2F(getpoly)(fname, topk, lw, it, m, n, namex, namel, ilp, lr, lc, fname_len, name_len)
     char *fname;
     integer *topk, *lw, *it, *m, *n;
     char *namex;
     integer *namel, *ilp, *lr, *lc;
     unsigned long fname_len;
     unsigned long name_len;
{
  integer ix1;

  integer il;
  il = iadr(*lstk(*lw ));
  if (*istk(il ) != 2) {
    Scierror(212,"%s: Argument %d: wrong type argument, expecting a polynomial matrix\r\n",
	     get_fname(fname,fname_len), Rhs + (*lw - *topk));
    return FALSE_;
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
  return  TRUE_;

} 


/*------------------------------------------------------------------ 
*     recupere un polynome 
*     md est son degre et son premier element est en 
*     stk(lr),stk(lc) 
*     Finir les tests 
*------------------------------------------------------------------ */

int C2F(getonepoly)(fname, topk, lw, it, md, namex, namel, lr, lc, fname_len, name_len)
     char *fname;
     integer *topk, *lw, *it, *md;
     char *namex;
     integer *namel, *lr, *lc;
     unsigned long fname_len;
     unsigned long name_len;
{
  integer m, n;
  integer ilp;

  if (C2F(getpoly)(fname, topk, lw, it, &m, &n, namex, namel, &ilp, lr, lc, fname_len, 4L)
      == FALSE_)
    return FALSE_;

  if (m * n != 1) {
    Scierror(998,"%s: argument should be a polygon\r\n",
	     get_fname(fname,fname_len));
    return FALSE_;
  }
  *md = *istk(ilp +1) - *istk(ilp ) - 1;
  *lr += *istk(ilp );
  *lc += *istk(ilp );
  return TRUE_;
} 

/*------------------------------------------------------------------ 
 * pmatj : 
 *   checks that there's a polynomial matrix  at position  lw-1 
 *   checks that the j-th column  can be extracted at lw position 
 *   perform the extraction 
 *       lw : position 
 *       j  : column  to be extracted 
 *------------------------------------------------------------------ */

int C2F(pmatj)(fname, lw, j, fname_len)
     char *fname;
     integer *lw, *j;
     unsigned long fname_len;
{
  integer ix1, ix2;
  char namex[4];
  integer incj;
  integer ix, l, m, n, namel;
  integer l2, m2, n2, lc, il, lj, it, lr, il2, ilp;

  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  ix1 = *lw - 1;
  ix2 = *lw - 1;
  if (! C2F(getpoly)(fname, &ix1, &ix2, &it, &m, &n, namex, &namel, &ilp, &lr, &lc, fname_len, 4L)) {
    return FALSE_;
  }
  if (*j > n)     return FALSE_;

  /*     a ameliorer */
  il = iadr(*lstk(*lw - 2 +1));
  incj = (*j - 1) * m;
  il2 = iadr(*lstk(*lw ));
  ix1 = il2 + 4;
  l2 = sadr(ix1);
  m2 = Max(m,1);
  ix1 = il + 9 + m * n;
  l = sadr(ix1);
  n = *istk(il + 8 + m * n );
  ix1 = il2 + 9 + m2;
  l2 = sadr(ix1);
  n2 = *istk(il + 8 + incj + m ) - *istk(il + 8 + incj );
  Err = l2 + n2 * (it + 1) - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",get_fname(fname,fname_len));
    return FALSE_;
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
  *lstk(Top +1) = l2 + n2 * (it + 1);
  il2 += -8;
  *istk(il2 ) = 2;
  *istk(il2 +1) = m2;
  *istk(il2 + 1 +1) = 1;
  *istk(il2 + 2 +1) = it;
  return TRUE_;
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

int C2F(crewmat)(fname, lw, m, lr, fname_len)
     char *fname;
     integer *lw, *m, *lr;
     unsigned long fname_len;
{
  integer il,ix1; 
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  il = iadr(*lstk(*lw ));
  *m = *lstk(Bot ) - sadr(il+4);
  *istk(il ) = 1;
  *istk(il + 1) = 1;
  *istk(il + 2) = *m;
  *istk(il + 3) = 0;
  ix1 = il + 4;
  *lr = sadr(il+4);
  *lstk(*lw +1) = sadr(il+4) + *m;
  return TRUE_;
}

/*------------------------------------------------------------------ 
 * crewimat : uses the rest of the stack as a working area (int)
 *    In : 
 *       lw : position (entier) 
 *    Out: 
 *       m  : size that can be used 
 *       lr : istk(lr+i) is the working area 
 *------------------------------------------------------------------ */

int C2F(crewimat)(fname, lw, m, n, lr, fname_len)
     char *fname;
     integer *lw, *m, *n, *lr;
     unsigned long fname_len;
{
  integer ix1,il;
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  il = iadr(*lstk(*lw ));
  Err = il + 3 + *m * *n - iadr(*lstk(Bot ));
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",
	     get_fname(fname,fname_len));
    return FALSE_;
  }
  *istk(il ) = 4;
  *istk(il + 1) = *m;
  *istk(il + 2) = *n;
  *lr = il + 3;
  ix1 = il + 3 + *m * *n + 2;
  *lstk(*lw +1) = sadr(ix1);
  return TRUE_;
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

int C2F(getwimat)(fname, topk, lw, m, n, lr, fname_len)
     char *fname;
     integer *topk, *lw, *m, *n, *lr;
     unsigned long fname_len;
{
  integer il;
  il = iadr(*lstk(*lw ));
  if (*istk(il ) < 0) {
    il = iadr(*istk(il +1));
  }
  if (*istk(il ) != 4) {
    Scierror(213,"%s: Argument %d: wrong type argument, expecting a working\r\n\tinteger matrix\r\n",
	     get_fname(fname,fname_len),Rhs + (*lw - *topk));
    return FALSE_;
  };
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  *lr = il + 3;
  return TRUE_;
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

int C2F(crestringv)(fname, spos, ilorig, lw, fname_len)
     char *fname;
     integer *spos, *ilorig, *lw;
     unsigned long fname_len;
{
  integer ierr;
  if (C2F(crepointer)(fname, spos, lw, fname_len) == FALSE_) 
    return FALSE_;

  C2F(stringc)(istk(*ilorig ), stk(*lw ), &ierr);

  if (ierr != 0) {
    Scierror(999,"Not enough memory\r\n");
    return FALSE_;
  }
  return TRUE_;
}

/*---------------------------------------------------------- 
 *  listcrepointer(top,numero,lw,....) 
 *---------------------------------------------------------- */

int C2F(listcrepointer)(fname, lw, numi, stlw, lrs, fname_len)
     char *fname;
     integer *lw, *numi, *stlw, *lrs;
     unsigned long fname_len;
{
  integer ix1,il ;
  if (C2F(crepointeri)(fname, stlw,  lrs, &c_true, fname_len)==FALSE_)
    return FALSE_ ;
  *stlw = *lrs + 2;
  il = iadr(*lstk(*lw ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1))  *lstk(*lw +1) = *stlw;
  return TRUE_;
} 

/*---------------------------------------------------------- 
 *  crepointer :
 *---------------------------------------------------------- */

int C2F(crepointer)(fname, lw, lr, fname_len)
     char *fname;
     integer *lw,  *lr;
     unsigned long fname_len;
{

  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  if ( C2F(crepointeri)(fname, lstk(*lw ), lr, &c_true, fname_len) == FALSE_)
    return FALSE_ ;
  *lstk(*lw +1) = *lr + 2;
  return TRUE_;
} 

/*--------------------------------------------------------- 
 * internal function used by crepointer and listcrepointer 
 *---------------------------------------------------------- */

int C2F(crepointeri)(fname, stlw, lr,  flagx, fname_len)
     char *fname;
     integer *stlw, *lr;
     int *flagx;
     unsigned long fname_len;
{
  integer ix1;
  integer il;
  il = iadr(*stlw);
  ix1 = il + 4;
  Err = sadr(ix1) + 2 - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",get_fname(fname,fname_len));
    return FALSE_;
  };
  if (*flagx) {
    *istk(il ) = 128;
    /* if m*n=0 then both dimensions are to be set to zero */
    *istk(il + 1) = 1;
    *istk(il + 2) = 1;
    *istk(il + 3) = 0;
  }
  ix1 = il + 4;
  *lr = sadr(ix1);
  return TRUE_;
} 

/*------------------------------------------------------------------- 
 *     creates a Scilab stringpointer on the stack at position spos 
 *     of size mxn the stringpointer is filled with the datas stored 
 *     in stk(lorig) ( for example created with cstringv ) 
 *     and the data stored at stk(lorig) is freed 
 *------------------------------------------------------------------- */

int C2F(lcrestringmatfromc)(fname, spos, numi, stlw, lorig, m, n, fname_len)
     char *fname;
     integer *spos, *numi, *stlw, *lorig, *m, *n;
     unsigned long fname_len;
{
  integer ix1;
  integer ierr;
  integer il, ilw;
  ilw = iadr(*stlw);
  ix1 = *lstk(Bot ) - *stlw;
  C2F(cstringf)(stk(*lorig ), istk(ilw ), m, n, &ix1, &ierr);
  if (ierr > 0) {
    Scierror(999,"Not enough memory\r\n");
    return FALSE_;
  }
  ix1 = ilw + 5 + *m * *n + *istk(ilw + 4 + *m * *n ) - 1;
  *stlw = sadr(ix1);
  il = iadr(*lstk(*spos ));
  ix1 = il + *istk(il +1) + 3;
  *istk(il + 2 + *numi ) = *stlw - sadr(ix1) + 1;
  if (*numi == *istk(il +1)) {
    *lstk(*spos +1) = *stlw;
  }
  return TRUE_;
}


/*------------------------------------------------------------------- 
 *     creates a Scilab stringmat on the stack at position spos 
 *     of size mxn the stringmat is filled with the datas stored 
 *     in stk(lorig) ( for example created with cstringv ) 
 *     and the data stored at stk(lorig) is freed 
 *------------------------------------------------------------------- */

int C2F(crestringmatfromc)(fname, spos, lorig, m, n, fname_len)
     char *fname;
     integer *spos, *lorig, *m, *n;
     unsigned long fname_len;
{
  integer ix1;
  integer ierr;
  integer ilw;
  ilw = iadr(*lstk(*spos ));
  ix1 = *lstk(Bot ) - *lstk(*spos );
  C2F(cstringf)(stk(*lorig ), istk(ilw ), m, n, &ix1, &ierr);
  if (ierr > 0) {
    Scierror(999,"Not enough memory\r\n");
    return FALSE_;
  }
  ix1 = ilw + 5 + *m * *n + *istk(ilw + 4 + *m * *n ) - 1;
  *lstk(*spos +1) = sadr(ix1);
  return  TRUE_;
}

/*------------------------------------------------------------------ 
 *     getlistvectrow : recupere un vecteur ligne dans une liste 
 *------------------------------------------------------------------ */

int C2F(getlistvectrow)(fname, topk, spos, lnum, it, m, n, lr, lc, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *it, *m, *n, *lr, *lc;
     unsigned long fname_len;
{
  integer nv;
  integer ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE_) 
    return FALSE_;

  if (*lnum > nv) {
    Scierror(999,"%s: argument %d should be a list of size at least %d \r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_;
  }

  if (C2F(getmati)(fname, topk, spos, &ili, it, m, n, lr, lc, &c_true, lnum, fname_len)== 
      FALSE_) 
    return FALSE_;
  if (*m != 1) {
    Scierror(999,"%s: argument %d >(%d) should be a row vector \r\n",
	     get_fname(fname,fname_len),Rhs + (*spos - *topk), *lnum);
    return FALSE_;
  }
  return TRUE_;
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

int C2F(getvectrow)(fname, topk, spos, it, m, n, lr, lc, fname_len)
     char *fname;
     integer *topk, *spos, *it, *m, *n, *lr, *lc;
     unsigned long fname_len;
{
  if (C2F(getmati)(fname, topk, spos, lstk(*spos ), it, m, n, lr, lc, &c_false, &cx0, fname_len) == FALSE_) 
    return FALSE_;

  if (*m != 1) {
    Scierror(999,"%s: argument %d should be a row vector \r\n",
	     get_fname(fname,fname_len),Rhs + (*spos - *topk));
    return FALSE_;
  }
  return TRUE_ ;
} 

/*------------------------------------------------------------------ 
 *
 *------------------------------------------------------------------ */

int C2F(getlistvectcol)(fname, topk, spos, lnum, it, m, n, lr, lc, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *it, *m, *n, *lr, *lc;
     unsigned long fname_len;
{
  integer nv;
  integer ili;
  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE_) 
    return FALSE_;

  if (*lnum > nv) {
    Scierror(999,"%s: argument %d should be a list of size at least %d \r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_;
  }
  if ( C2F(getmati)(fname, topk, spos, &ili, it, m, n, lr, lc, &c_true, lnum, fname_len)
       == FALSE_)
    return FALSE_;

  if (*n != 1) {
    Scierror(999,"%s: argument %d >(%d) should be a column vector \r\n",
	     get_fname(fname,fname_len),Rhs + (*spos - *topk), *lnum);
    return FALSE_;
  }
  return TRUE_;
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

int C2F(getvectcol)(fname, topk, spos, it, m, n, lr, lc, fname_len)
     char *fname;
     integer *topk, *spos, *it, *m, *n, *lr, *lc;
     unsigned long fname_len;
{

  if ( C2F(getmati)(fname, topk, spos, lstk(*spos ), it, m, n, lr, lc, &c_false, &cx0, fname_len)
       == FALSE_ ) 
    return FALSE_;

  if (*n != 1) {
    Scierror(999,"%s: argument %d should be a column vector \r\n",
	     get_fname(fname,fname_len),Rhs + (*spos - *topk));
    return FALSE_;
  }
  return TRUE_;
}


int C2F(getlistsimat)(fname, topk, spos, lnum, m, n, ix, j, lr, nlr, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *m, *n, *ix, *j, *lr, *nlr;
     unsigned long fname_len;
{
  integer nv;
  integer ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE_) 
    return FALSE_;

  if (*lnum > nv) {
    Scierror(999,"%s: argument %d should be a list of size at least %d \r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_;
  }
  return  C2F(getsmati)(fname, topk, spos, &ili, m, n, ix, j, lr, nlr, &c_true, lnum, fname_len);
} 

/*------------------------------------------------------------------- 
 *     recuperation d'un pointer 
 *------------------------------------------------------------------- */

int C2F(getpointer)(fname, topk, lw, lr, fname_len)
     char *fname;
     integer *topk, *lw, *lr;
     unsigned long fname_len;
{
  return C2F(getpointeri)(fname, topk, lw,lstk(*lw), lr, &c_false, &cx0, fname_len);
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

int C2F(getlistpointer)(fname, topk, spos, lnum, lw, fname_len)
     char *fname;
     integer *topk, *spos, *lnum, *lw;
     unsigned long fname_len;
{
  integer nv, ili;

  if ( C2F(getilist)(fname, topk, spos, &nv, lnum, &ili, fname_len) == FALSE_)
    return FALSE_;

  if (*lnum > nv) {
    Scierror(999,"%s: argument %d should be a list of size at least %d \r\n",
	     get_fname(fname,fname_len), Rhs+(*spos - *topk), *lnum);
    return FALSE_;
  }
  return C2F(getpointeri)(fname, topk, spos, &ili, lw, &c_true, lnum, fname_len);
} 

/*------------------------------------------------------------------- 
 * For internal use 
 *------------------------------------------------------------------- */

int C2F(getpointeri)(fname, topk, spos, lw, lr, inlistx, nel, fname_len)
     char *fname;
     integer *topk, *spos, *lw, *lr;
     int *inlistx;
     integer *nel;
     unsigned long fname_len;
{
  integer il;
  il = iadr(*lw);
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  if (*istk(il ) != 128) {
    sciprint("----%d\r\n",*istk(il));
    if (*inlistx) 
      Scierror(999,"%s: argument %d >(%d) should be a boxed pointer\r\n",
	       get_fname(fname,fname_len), Rhs + (*spos - *topk), *nel);
    else 
      Scierror(201,"%s: argument %d should be a boxed pointer\r\n",get_fname(fname,fname_len),
	       Rhs + (*spos - *topk));
    return  FALSE_;
  }
  *lr = sadr(il+4);
  return TRUE_;
} 

/*-----------------------------------------------------------
 *     creates a matlab-like sparse matrix 
 *-----------------------------------------------------------*/

int C2F(mspcreate)(lw, m, n, nzMax, it)
     integer *lw, *m, *n, *nzMax, *it;
{
  integer ix1;
  integer jc, il, ir;

  if (*lw + 1 >= Bot) {
    Scierror(18,"too many names\r\n");
    return FALSE_;
  }

  il = iadr(*lstk(*lw ));
  ix1 = il + 4 + (*n + 1) + *nzMax;
  Err = sadr(ix1) + (*it + 1) * *nzMax - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"stack size exceeded (Use stacksize function to increase it)\r\n");
    return FALSE_;
  };
  *istk(il ) = 7;
  /*        si m*n=0 les deux dimensions sont mises a zero. */
  *istk(il +1) = Min(*m , *m * *n);
  *istk(il + 1 +1) = Min(*n,*m * *n);
  *istk(il + 2 +1) = *it;
  *istk(il + 3 +1) = *nzMax;
  *istk(il + 4 +1) = 0;
  *istk(il + 5 + *n ) = *nzMax;
  jc = il + 5;
  ir = jc + *n + 1;
  ix1 = il + 4 + (*n + 1) + *nzMax;
  *lstk(*lw +1) = sadr(ix1) + (*it + 1) * *nzMax + 1;
  return TRUE_;
}


/**********************************************************************
 * Scilab Error at C level 
 **********************************************************************/

extern int C2F(error)  __PARAMS((integer *n));
extern int C2F(errmgr)  __PARAMS((integer *n, integer *errtyp));
extern int C2F(errmsg)  __PARAMS((integer *n, integer *errtyp));
extern int C2F(errmds)  __PARAMS((integer *num, integer *imess, integer *imode));
extern int C2F(depfun)  __PARAMS((integer *lunit, int *trace, int *first));
extern int C2F(depexec)  __PARAMS((integer *lunit, int *trace, int *first, int *pflag));
extern int C2F(errloc)  __PARAMS((integer *n));

static int Scierror_internal __PARAMS((integer *n,char *buffer));

/* 
 * as sciprint but with an added first argument 
 * which is ignored (used in do_printf) 
 */

#ifdef __STDC__ 
int  Scierror(int iv,char *fmt,...) 
#else 
/*VARARGS0*/
int Scierror(va_alist) va_dcl
#endif 
{
  int retval;
  integer lstr;
  va_list ap;
  char s_buf[bsiz];
#ifdef __STDC__
  va_start(ap,fmt);
#else
  int iv;
  char *fmt;
  va_start(ap);
  iv = va_arg(ap,int);
  fmt = va_arg(ap, char *);
#endif
  retval= vsprintf(s_buf, fmt, ap );
  lstr=strlen(s_buf);
  va_end(ap);
  Scierror_internal(&iv,s_buf);
  return retval;
}

static int Scierror_internal(n,buffer)
     integer *n;
     char *buffer;
{
  integer imode;
  int trace;
  integer imess;
  integer errtyp, num, lct1, len;

  C2F(errmds)(&num, &imess, &imode);
  trace = ! ((num < 0 || num == *n) && imess != 0);
  /*     de-activate output control */
  lct1 = C2F(iop).lct[0];
  C2F(iop).lct[0] = 0;
  errtyp = 0;
  if (C2F(errgst).err1 == 0 && C2F(errgst).err2 == 0) 
    {
      /*     . locate the error in the current statement */
      if (trace) 
	C2F(errloc)(n);
      /*     . output error message */
      else 
	C2F(iop).lct[0] = -1;
      /* XXX  C2F(errmsg)(n, &errtyp); */
      errtyp=0;
      C2F(freemsgtable)();
      C2F(errstore)(n);
      len=strlen(buffer);
      C2F(msgstore)(buffer,&len);
      sciprint(buffer);

      C2F(iop).lct[0] = 0;
    }
  /*     handle the error */
  C2F(errmgr)(n, &errtyp);
  /*     re-activate output control */
  C2F(iop).lct[0] = lct1;
  return 0;
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

char *get_fname(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  int i;
  strncpy(Fname,fname,Min(fname_len,nlgh));
  Fname[fname_len] = '\0';
  for ( i= 0 ; i < fname_len ; i++) 
    if (Fname[i] == ' ') { Fname[i]= '\0'; break;}
  return Fname;
}

/*------------------------------------------------------------------ 
 * realmat : 
 *     Top is supposed to be a matrix 
 *     and the matrix is chnaged to its real part 
 *------------------------------------------------------------------ */

int C2F(realmat)()
{
  integer ix1;
  integer m, n, il;

  il = iadr(*lstk(Top ));
  if (*istk(il + 3 ) == 0) return 0;
  m = *istk(il + 1);
  n = *istk(il + 2);
  *istk(il + 3) = 0;
  ix1 = il + 4;
  *lstk(Top +1) = sadr(ix1) + m * n;
  return 0;
}




/*------------------------------------------------------------------ 
*     copie l'objet qui est a la position lw de la pile 
*     a la position lwd de la pile 
*     copie faite avec dcopy 
*     pas de verification 
*      implicit undefined (a-z) 
*------------------------------------------------------------------ */

int C2F(copyobj)(fname, lw, lwd, fname_len)
     char *fname;
     integer *lw, *lwd;
     unsigned long fname_len;
{
  integer ix1;
  ix1 = *lstk(*lw +1) - *lstk(*lw );
  C2F(dcopy)(&ix1, stk(*lstk(*lw ) ), &cx1, stk(*lstk(*lwd ) ), &cx1);
  *lstk(*lwd +1) = *lstk(*lwd ) + *lstk(*lw +1) - *lstk(*lw );
  return 0;
}


/*------------------------------------------------
 *     copie l'objet qui est a la position lw de la pile
 *     a la position lwd de la pile 
 *     copie faite avec dcopy 
 *     et verification 
 *------------------------------------------------*/

int C2F(vcopyobj)(fname, lw, lwd, fname_len)
     char *fname;
     integer *lw, *lwd;
     unsigned long fname_len;
{
  integer l;
  integer l1, lv;
  l = *lstk(*lw );
  lv = *lstk(*lw +1) - *lstk(*lw );
  l1 = *lstk(*lwd );
  if (*lwd + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  Err = *lstk(*lwd ) + lv - *lstk(Bot );
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",
	     get_fname(fname,fname_len));
    return FALSE_;
  }
  C2F(dcopy)(&lv, stk(l ), &cx1, stk(l1 ), &cx1);
  *lstk(*lwd +1) = *lstk(*lwd ) + lv;
  return TRUE_;
} 



/*------------------------------------------------== 
*     suppose qu'il y a une matrice en lw de taille it1,m1,n1,mn1, 
*     et une autre en lw+1 de taille it2,m2,n2,mn2 
*     et echange les matrices et change les valeurs de it1,m1,n1,... 
*     apres echange la taille de la matrice en lw est stocke ds(it1,m1,n1) 
*     et celle en lw+1 est stocke ds (it2,m2,n2) 
*     effet de bord il faut que lw+2 soit une place libre 
*------------------------------------------------== */


int C2F(swapmat)(fname, topk, lw, it1, m1, n1, mn1, it2, m2, n2, mn2, fname_len)
     char *fname;
     integer *topk, *lw, *it1, *m1, *n1, *mn1, *it2, *m2, *n2, *mn2;
     unsigned long fname_len;
{
  integer ix1, ix2;
  integer lc, lr;
  ix1 = *lw + 1;

  if ( C2F(cremat)(fname, &ix1, it1, m1, n1, &lr, &lc, fname_len)== FALSE_)
    return FALSE_ ;

  ix1 = *lw + 2;
  C2F(copyobj)(fname, lw, &ix1, fname_len);
  ix1 = *lw + 1;
  C2F(copyobj)(fname, &ix1, lw, fname_len);
  ix1 = *lw + 2;
  ix2 = *lw + 1;
  C2F(copyobj)(fname, &ix1, &ix2, fname_len);
  if ( C2F(getmat)(fname, topk, lw, it1, m1, n1, &lr, &lc, fname_len) == FALSE_ )
    return FALSE_;

  ix1 = *lw + 1;

  if (C2F(getmat)(fname, topk, &ix1, it2, m2, n2, &lr, &lc, fname_len) == FALSE_ )
    return FALSE_;

  *mn1 = *m1 * *n1;
  *mn2 = *m2 * *n2;

  return TRUE_;
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

int C2F(insmat)(topk, lw, it, m, n, lr, lc, lr1, lc1)
     integer *topk, *lw, *it, *m, *n, *lr, *lc, *lr1, *lc1;
{

  integer ix1;
  integer c_n1 = -1;
  integer m1, n1;
  integer lc0, it1, lr0;
  
  if (C2F(getmat)("insmat", topk, lw, &it1, &m1, &n1, &lr0, &lc0, 6L) == FALSE_) 
    return FALSE_;

  if (C2F(cremat)("insmat", lw, it, m, n, lr, lc, 6L)  == FALSE_) 
    return FALSE_;

  ix1 = *lw + 1;

  if (C2F(cremat)("insmat", &ix1, &it1, &m1, &n1, lr1, lc1, 6L) == FALSE_) 
    return FALSE_;

  ix1 = m1 * n1 * (it1 + 1);
  C2F(dcopy)(&ix1, stk(lr0 ), &c_n1, stk(*lr1 ), &c_n1);
  return TRUE_;
} 




/*------------------------------------------------
 *     imprime le contenu de la pile en lw en mode entier ou 
 * 	  double precision suivant typ 
 *------------------------------------------------*/

int C2F(stackinfo)(lw, typ)
     integer *lw, *typ;
{
  integer ix, l, m, n;
  integer il, nn;

  if (*lw == 0) {
    return 0;
  }
  il = iadr(*lstk(*lw ));
  if (*istk(il ) < 0) {
    il = iadr(*istk(il +1));
  }
  m = *istk(il +1);
  n = *istk(il + 1 +1);

  sciprint("-----------------stack-info-----------------\r\n");
  sciprint("lw=%d -[istk]-> il lw+1 -[istk]-> %d \r\n",
	   *lw,iadr(*lstk(*lw+1)));
  sciprint("istk(%d:..) ->[%d %d %d %d ....]\r\n",
	   il, istk(il),istk(il+1),istk(il+2),istk(il+3) );
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
  sciprint("\r\n-----------------stack-info-----------------\r\n");
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

int C2F(allmat)(fname, topk, lw, m, n, fname_len)
     char *fname;
     integer *topk, *lw, *m, *n;
     unsigned long fname_len;
{
  integer itype, il;
  il = iadr(*lstk(*lw ));
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  itype = *istk(il );
  if (itype != 1 && itype != 2 && itype != 10) {
    Scierror(209,"%s: Argument %d wrong type argument, expecting a matrix\r\n",
	     get_fname(fname,fname_len) ,  Rhs + (*lw - *topk));
    return FALSE_;
  }
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  return TRUE_;
} 

/*------------------------------------------------
 * Assume that object at position lw is a matrix 
 * and set its size to (m,n) 
 *------------------------------------------------*/

int C2F(allmatset)(fname, lw, m, n, fname_len)
     char *fname;
     integer *lw, *m, *n;
     unsigned long fname_len;
{
  integer il;
  il = iadr(*lstk(*lw ));
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

int C2F(objvide)(fname, lw, fname_len)
     char *fname;
     integer *lw;
     unsigned long fname_len;
{
  if (*lw == 0 || Rhs < 0) {
    ++(*lw);
  }
  *istk(iadr(*lstk(*lw )) ) = 0;
  *lstk(*lw +1) = *lstk(*lw ) + 2;
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

int C2F(getexternal)(fname, topk, lw, namex, typex, setfun, fname_len, name_len)
     char *fname;
     integer *topk, *lw;
     char *namex;
     int *typex;
     void (*setfun) __PARAMS((char *,int *));
     unsigned long fname_len;
     unsigned long name_len;
{
  int ret_value;
  integer irep;
  integer m, n;
  integer il, lr;
  integer nlr;
  int i;
  il = C2F(gettype)(lw);
  switch ( il) {
  case 11 : case 13 : case 15 :
    ret_value = TRUE_;
    *typex = FALSE_;
    break;
  case 10 :
    ret_value = C2F(getsmat)(fname, topk, lw, &m, &n, &cx1, &cx1, &lr, &nlr, fname_len);
    *typex = TRUE_;
    for (i=0; i < name_len ; i++ ) namex[i] = ' ';
    if (ret_value == TRUE_) 
      {
	C2F(cvstr)(&nlr, istk(lr ), namex, &cx1, name_len);
	namex[nlr] = '\0';
	(*setfun)(namex, &irep); /* , name_len); */
	if (irep == 1) 
	  {
	    Scierror(50,"%s: entry point %s not found in predefined tables or link table\r\n",get_fname(fname,fname_len),namex);
	    ret_value = FALSE_;
	  }
      }
    break;
  default: 
    Scierror(211,"%s: Argument %d: wrong type argument, expecting a function\r\n\tor string (external function)\r\n",
	     get_fname(fname,fname_len), Rhs + (*lw - *topk));
    ret_value = FALSE_;
    break;
  }
  return ret_value;
} 

/*------------------------------------------------
 *------------------------------------------------ */

int C2F(checkval)(fname, ival1, ival2, fname_len)
     char *fname;
     integer *ival1, *ival2;
     unsigned long fname_len;
{
  if (*ival1 != *ival2) {
    Scierror(999,"%s: incompatible sizes \r\n",get_fname(fname,fname_len));
    return  FALSE_;
  } ;
  return  TRUE_;
}

/*------------------------------------------------------------- 
 *      recupere si elle existe la variable name dans le stack et 
 *      met sa valeur a la position top et top est incremente 
 *      ansi que rhs 
 *      si la variable cherchee n'existe pas on renvoit false 
 *------------------------------------------------------------- */

int C2F(optvarget)(fname, topk, iel, namex, fname_len, name_len)
     char *fname;
     integer *topk, *iel;
     char *namex;
     unsigned long fname_len;
     unsigned long name_len;
{
  integer id[nsiz];
  C2F(cvname)(id, namex, &cx0, name_len);
  Fin = 0;
  /*     recupere la variable et incremente top */
  C2F(stackg)(id);
  if (Fin == 0) {
    Scierror(999,"%s: optional argument %d not given and default value %s not found\r\n",
	     get_fname(fname,fname_len),*iel,namex);
    return FALSE_;
  }
  ++Rhs;
  return TRUE_;
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

int C2F(bufstore)(fname, lbuf, lbufi, lbuff, lr, nlr, fname_len)
     char *fname;
     integer *lbuf, *lbufi, *lbuff, *lr, *nlr;
     unsigned long fname_len;
{
  *lbufi = *lbuf;
  *lbuff = *lbufi + *nlr - 1;
  *lbuf = *lbuff + 2;
  if (*lbuff > bsiz) {
    Scierror(999,"%f: No more space to store string arguments\r\n",
	     get_fname(fname,fname_len) );
    return FALSE_;
  }
  /* lbufi is a Fortran indice ==> offset -1 at C level */
  C2F(cvstr)(nlr, istk(*lr ), C2F(cha1).buf + (*lbufi - 1), &cx1, *lbuff - (*lbufi - 1));
  C2F(cha1).buf[*lbuff] = '\0';
  return TRUE_;
}


/*------------------------------------------------------------- 
 * 
 *------------------------------------------------------------- */

int C2F(credata)(fname, lw, m, fname_len)
     char *fname;
     integer *lw, m;
     unsigned long fname_len;
{
  integer lr;
  lr = *lstk(*lw );
  if (*lw + 1 >= Bot) {
    Scierror(18,"%s: too many names\r\n",get_fname(fname,fname_len));
    return FALSE_;
  }
  Err = lr + m  - *lstk(Bot);
  if (Err > 0) {
    Scierror(17,"%s: stack size exceeded (Use stacksize function to increase it)\r\n",get_fname(fname,fname_len));
    return FALSE_;
  };
  *lstk(*lw +1) = lr + 1 + m/sizeof(double);
  return TRUE_;
} 


