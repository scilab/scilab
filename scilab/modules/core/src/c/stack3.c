/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - INRIA
 * Copyright (C) 1998-2000 - ENPC
 * Copyright (C) 2003 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 */

/*------------------------------------------------------
 * Read and Write inside the Scilab stack
 *------------------------------------------------------*/

#include <string.h>
#include "stack-c.h"

#include "call_scilab.h"
#include "cvstr.h"
#include "localization.h"
#include "Scierror.h"
#include "code2str.h"
#include "elementary_functions.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"

extern int C2F(stackp)(int *id, int *macmod);
extern int C2F(dcopy)();

/*------------------------------------------------------*/
void *Name2ptr(char *namex);
int Name2where(char *namex);

/* Table of constant values */

static int cx0 = 0;
static int cx1 = 1;

/*------------------------------------------------------
 * read a matrix
 *------------------------------------------------------*/

int C2F(readmat)(char *namex,int *m, int *n, double *scimat, unsigned long name_len)
{
  C2F(creadmat)(namex, m, n, scimat, name_len);
  return 0;
}

/*----------------------------------------------------------------
 * creadmat reads vector/matrix in scilab's internal stack
 * calling sequence
 *     logic=creadmat('matrixname',m,n,scimat)
 *  matrixname: character string; name of the scilab variable.
 *  m: number of rows (output of creadmat)
 *  n: number of columns (output of creadmat)
 *  scimat: matrix entries stored columnwise (output of creadmat)
 *    Example of use:
 *    Amat is a real 2 x 3 scilab matrix
 *    your subroutine should be as follows:
 *    subroutine mysubr(...)
 *    ...
 *    call creadmat('Amat',m,n,scimat)
 *    => m=3 , n=2, and scimat(1)=Amat(1,1)
 *                      scimat(2)=Amat(2,1)
 *                      scimat(3)=Amat(3,1)
 *                      scimat(4)=Amat(1,2) ...
 *                      scimat(5)=Amat(3,2)
 *                      scimat(6)=Amat(3,2)
 *----------------------------------------------------------------*/

int C2F(creadmat)(char *namex, int *m, int *n, double *scimat, unsigned long name_len)
{
    int l;
    int id[nsiz];

    C2F(str2name)(namex, id, name_len);
    /* read   : from scilab stack -> fortran variable */
    Fin = -1;
    C2F(stackg)(id);
    if (Err > 0) return FALSE ;
    if (Fin == 0) {
		Scierror(4,_("Undefined variable %s.\n"),get_fname(namex,name_len));
      return FALSE;
    }
    if ( *Infstk(Fin ) == 2)  Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    /* get matrix data pointer */
    if (! C2F(getrmat)("creadmat", &Fin, &Fin, m, n, &l, 8L)) 	return FALSE;

    C2F(dmcopy)(stk(l ), m, scimat, m, m, n);

    return TRUE;
}
/*----------------------------------------------------------------
 * creadcmat reads vector/matrix in scilab's internal stack
 * calling sequence
 *     logic=creadcmat('matrixname',m,n,scimat)
 *  matrixname: character string; name of the scilab variable.
 *  m: number of rows (output of readmat)
 *  n: number of columns (output of readmat)
 *  scimat: matrix entries stored columnwise (output of readmat)
 *    Example of use:
 *    Amat is a real 2 x 3 scilab matrix
 *    your subroutine should be as follows:
 *    subroutine mysubr(...)
 *    ...
 *    call readmat('Amat',m,n,scimat)
 *    => m=3 , n=2, and scimat(1)=Amat(1,1)
 *                      scimat(2)=Amat(2,1)
 *                      scimat(3)=Amat(3,1)
 *                      scimat(4)=Amat(1,2) ...
 *                      scimat(5)=Amat(3,2)
 *
 * Note d'Albert Y
 * 20/12/2003
 *    Cette routine est une simple copie de creadmat et légèrement
 *    modifiée pour les matrices complexes
 *
 *----------------------------------------------------------------*/

int C2F(creadcmat)(char *namex, int *m, int *n, double *scimat, unsigned long name_len)
{
    int l, ix1;
    int id[nsiz];

    C2F(str2name)(namex, id, name_len);
    /* read   : from scilab stack -> fortran variable */
    Fin = -1;
    C2F(stackg)(id);
    if (Err > 0) return FALSE ;
    if (Fin == 0) {
      Scierror(4,_("Undefined variable %s.\n"),get_fname(namex,name_len));
      return FALSE;
    }
    if ( *Infstk(Fin ) == 2)  Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    /* get matrix data pointer */
    if (! C2F(getcmat)("creadcmat", &Fin, &Fin, m, n, &l, 8L)) 	return FALSE;
    ix1 = *m * *n;
    C2F(dmcopy)(stk(l ), m, scimat, m, m, n);
    C2F(dmcopy)(stk(l+ix1 ), m, scimat+ix1, m, m, n);

    return TRUE;
}

/**
 * cwritemat writes vector/matrix in scilab's internal stack
 * logic=cwritemat('matrixname'//char(0),m,n,mat)
 * @param name character string; name of the scilab variable ( null terMinated)
 * @param m number of rows
 * @param n number of columns
 * @param mat matrix entries stored columnwise in Scilab object
 */

int C2F(cwritemat)(char *namex, int *m, int *n,  double *mat, unsigned long name_len)
{
  int   ix1 = *m * *n;
  int Rhs_k = Rhs , Top_k = Top ;
  int l4, id[nsiz], lc, lr;

  C2F(str2name)(namex, id, name_len);

  Top = Top + Nbvars + 1;
  if (! C2F(cremat)("cwritemat", &Top, &cx0, m, n, &lr, &lc, 9L)) return  FALSE;
  C2F(dcopy)(&ix1, mat, &cx1, stk(lr ), &cx1);
  Rhs = 0;
  l4 = C2F(iop).lct[3];
  C2F(iop).lct[3] = -1;
  C2F(stackp)(id, &cx0);
  C2F(iop).lct[3] = l4;
  Top = Top_k;
  Rhs = Rhs_k;
  if (Err > 0)  return FALSE;
  return TRUE;
}


/**
 * cwritecmat writes vector/matrix in scilab's internal stack
 * @param name: character string; name of the scilab variable ( null terMinated)
 * @param m: number of rows
 * @param n: number of columns
 * @param mat: matrix entries stored columnwise in Scilab object for complex number
*/
int C2F(cwritecmat)(char *namex,int *m, int*n,double *mat,unsigned long name_len)
{
	int   ix1 = *m * *n *2; /* real part + imaginary part */
	int Rhs_k = Rhs , Top_k = Top ;
	int l4, id[nsiz], lc, lr;
	int IT=1; /* Type Complex */

	C2F(str2name)(namex, id, name_len);

	Top = Top + Nbvars + 1;
	if (! C2F(cremat)("cwritecmat", &Top, &IT, m, n, &lr, &lc, 10L)) return  FALSE;
	C2F(dcopy)(&ix1, mat, &cx1, stk(lr ), &cx1);
	Rhs = 0;
	l4 = C2F(iop).lct[3];
	C2F(iop).lct[3] = -1;
	C2F(stackp)(id, &cx0);
	C2F(iop).lct[3] = l4;
	Top = Top_k;
	Rhs = Rhs_k;
	if (Err > 0)  return FALSE;
	return TRUE;
}
/*--------------------------------------------------------------------------*/
 /* Put variable number into Scilab internal stack with name "namex" */
int C2F(putvar)(int  *number,char *namex,  unsigned long name_len)
{
  int Rhs_k = Rhs , Top_k = Top ;
  int l4, id[nsiz],/* lc, lr,*/ cx0_2=1;

  C2F(str2name)(namex, id, name_len);
  Top = *number + Top -Rhs;
  Rhs = 0;
  l4 = C2F(iop).lct[3];
  C2F(iop).lct[3] = -1;
  C2F(stackp)(id, &cx0_2);
  C2F(iop).lct[3] = l4;
  Top = Top_k;
  Rhs = Rhs_k;
  if (Err > 0)  return FALSE;
  return TRUE;
}

/*------------------------------------------------------
 *     see creadchain
 * Obsolete function. please use creadchain
 *------------------------------------------------------*/

int C2F(readchain)(char *namex,  int *itslen, char *chai,  unsigned long name_len, unsigned long chai_len)
{
    C2F(creadchain)(namex, itslen, chai, name_len, chai_len);
    return 0;
}

/*------------------------------------------------------
 *     this routine reads a string in scilab's  memory
 *     and store it into chai
 * !calling sequence
 *     int       itslen
 *     character*(*) chai,name
 *     name    : character string = name of scilab variable (input)
 *     chai    : chain to be read (output)
 *               null terMinated
 *     itslen  : (input) Maximum number of character that can ne stored
 *               in chain
 *               (output) number of copied characters into chai
 *     if Scilab variable x='qwert' exists
 *     character ch*(10)
 *     l=10
 *     logic= creadchain('x',l,ch) returns l=5 and ch='qwert'
 *------------------------------------------------------*/

int C2F(creadchain)(char *namex,  int *itslen,  char *chai,  unsigned long name_len,  unsigned long chai_len)
{
    int ix1;
    int m1, n1;
    int id[nsiz];
    int lr1;
    int nlr1;

    Err = 0;
    C2F(str2name)(namex, id, name_len);
    Fin = -1;
    C2F(stackg)(id);
    if (Err > 0) return FALSE ;
    if (Fin == 0) {
      Scierror(4,_("Undefined variable %s.\n"),get_fname(namex,name_len));
      return FALSE ;
    }
    if (*Infstk(Fin ) == 2) {
	Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    }
    if (! C2F(getsmat)("creadchain", &Fin, &Fin, &m1, &n1, &cx1, &cx1, &lr1, &nlr1, 10L)) {
	return FALSE;
    }
    if (m1 * n1 != 1) {
      Scierror(999,_("%s: argument must be a string.\n"),"creadchain");
      return FALSE ;
    }

    ix1 = *itslen - 1;
    *itslen = Min(ix1,nlr1);
    C2F(cvstr)(itslen, istk(lr1 ), chai, &cx1, chai_len);
    chai[*itslen] = '\0';
    return TRUE ;
}

/*----------------------------------------------------------------------
 *     this routine reads name(ir,ic) in scilab's  memory
 *     and store it into chai
 *     if ir=ic=-1 on entry then the routines returns in ir,ic
 *     the size of the matrix
 * !calling sequence
 *     int       itslen
 *     character*(*) chai,name
 *     name    : character string = name of scilab variable (input)
 *     chai    : chain to be read (output)
 *               null terMinated
 *     itslen  : (input) Maximum number of character that can be stored
 *               in chain
 *               (output) number of copied characters into chai
 *     if Scilab variable x='qwert' exists
 *     character ch*(10)
 *     l=10
 *     logic= creadchain('x',l,ch) returns l=5 and ch='qwert'
 *----------------------------------------------------------------------*/


int C2F(creadchains)(char *namex, int *ir, int *ic, int *itslen, char *chai, unsigned long name_len,  unsigned long chai_len)
{
    int ix1;
    int m1, n1;
    int id[nsiz];
    int lr1;
    int nlr1;

    Err = 0;
    C2F(str2name)(namex, id, name_len);
    Fin = -1;
    C2F(stackg)(id);
    if (Err > 0) return FALSE ;

    if (Fin == 0) {
      Scierror(4,_("Undefined variable %s.\n"),get_fname(namex,name_len));
      return FALSE ;
    }

    if (*Infstk(Fin ) == 2) {
	Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    }
    if (*ir == -1 && *ic == -1) {
	if (! C2F(getsmat)("creadchain", &Fin, &Fin, ir, ic, &cx1, &cx1, &lr1, &nlr1, 10L))
	  return FALSE;
	else
	  return TRUE ;
    } else {
	if (! C2F(getsmat)("creadchain", &Fin, &Fin, &m1, &n1, ir, ic, &lr1, &nlr1, 10L)) {
	  return FALSE;
	}
    }
    ix1 = *itslen - 1;
    *itslen = Min(ix1,nlr1);
    C2F(cvstr)(itslen, istk(lr1 ), chai, &cx1, chai_len);
    chai[*itslen]='\0';
    return TRUE;
}

/**
 *  cwritemat writes vector/matrix in scilab's internal stack
 *  logic=cwritemat('matrixname'//char(0),m,n,mat)
 * @param name character string; name of the scilab variable ( null terMinated)
 * @param m number of rows
 * @param n number of columns
 * @param mat matrix entries stored columnwise in Scilab object
 *----------------------------------------------------------------*/

int C2F(cwritechain)(char *namex, int *m, char *chai, unsigned long name_len, unsigned long chai_len)
{
    int Rhs_k, Top_k;
    int l4;
    int id[nsiz], lr;
    C2F(str2name)(namex, id, name_len);
    Top_k = Top;

    Top = Top + Nbvars + 1;
    if (! C2F(cresmat2)("cwritechain", &Top, m, &lr, 11L)) {
	return FALSE;
    }
    C2F(cvstr)(m, istk(lr ), chai, &cx0, chai_len);
    Rhs_k = Rhs;
    Rhs = 0;
    l4 = C2F(iop).lct[3];
    C2F(iop).lct[3] = -1;
    C2F(stackp)(id, &cx0);
    C2F(iop).lct[3] = l4;
    Top = Top_k ;
    Rhs = Rhs_k ;
    if (Err > 0)  return FALSE;
    return TRUE ;
}

/*----------------------------------------------------------------
 *     see cmatptr
 *----------------------------------------------------------------*/

int C2F(matptr)(char *namex, int *m, int *n, int *lp, unsigned long name_len)
{
  C2F(cmatptr)(namex, m, n, lp, name_len);
  return 0;
}

/*----------------------------------------------------------------
 * !purpose
 *     matptr returns the address of real matrix "name"
 *     in scilab's internal stack
 *     m=number of rows
 *     n=number of columns
 *     stk(lp),stk(lp+1),...,stk(lp+m*n-1)= entries (columnwise)
 *     If matrix "name" not in Scilab stack, returns m=n=-1.
 *    Example of use:
 *    Amat is a real 2 x 3 scilab matrix
 *    your subroutine should be as follows:
 *    subroutine mysubr(...)
 *    ...
 *    logic= cmatptr('Amat',m,n,lp)
 *    => m=3 , n=2, and stk(lp)=Amat(1,1)
 *                      stk(lp+1)=Amat(2,1)
 *                      stk(lp+2)=Amat(3,1)
 *                      stk(lp+3)=Amat(1,2) ...
 *                      stk(lp+5)=Amat(3,2)
 *   see example in fydot.f file
 *   see also  readmat.f, matz.f
 *----------------------------------------------------------------*/

int C2F(cmatptr)(char *namex, int *m,int *n,int *lp, unsigned long name_len)
{
    int id[nsiz];
    C2F(str2name)(namex, id, name_len);
    /* get the position in fin */
    Fin = -1;
    C2F(stackg)(id);
    if (Fin == 0) {
      Scierror(4,_("Undefined variable %s.\n"),get_fname(namex,name_len));
      *m = -1;
      *n = -1;
      return FALSE;
    }
    /* get data */
    if (*Infstk(Fin ) == 2) {
	Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    }
    if (! C2F(getrmat)("creadmat", &Fin, &Fin, m, n, lp, 8L)) {
	return FALSE;
    }
    return TRUE ;
}

/*----------------------------------------------------------------
 * !purpose
 *     cmatcptr returns the address of complex matrix "name"
 *     in scilab's internal stack
 *     m=number of rows
 *     n=number of columns
 *     stk(lp),stk(lp+1),...,stk(lp+m*n-1)= entries (columnwise)
 *     If matrix "name" not in Scilab stack, returns m=n=-1.
 *    Example of use:
 *    Amat is a real 2 x 3 scilab matrix
 *    your subroutine should be as follows:
 *    subroutine mysubr(...)
 *    ...
 *    logic= cmatptr('Amat',m,n,lp)
 *    => m=3 , n=2, and stk(lp)=Amat(1,1)
 *                      stk(lp+1)=Amat(2,1)
 *                      stk(lp+2)=Amat(3,1)
 *                      stk(lp+3)=Amat(1,2) ...
 *                      stk(lp+5)=Amat(3,2)
 *   see example in fydot.f file
 *   see also  readmat.f, matz.f
 *
 * Note d'Albert Y
 * 20/12/2003
 *    Cette routine est une simple copie de creadmat et légèrement
 *    modifiée pour les matrices complexes
 *
 *----------------------------------------------------------------*/

int C2F(cmatcptr)(char *namex, int *m, int *n, int *lp, unsigned long name_len)
{
    int id[nsiz];
    C2F(str2name)(namex, id, name_len);
    /* get the position in fin */
    Fin = -1;
    C2F(stackg)(id);
    if (Fin == 0) {
      Scierror(4,_("Undefined variable %s.\n"),get_fname(namex,name_len));
      *m = -1;
      *n = -1;
      return FALSE;
    }
    /* get data */
    if (*Infstk(Fin ) == 2) {
	Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    }
    if (! C2F(getcmat)("creadmat", &Fin, &Fin, m, n, lp, 8L)) {
	return FALSE;
    }
    return TRUE ;
}

/*----------------------------------------------------------------
 * !purpose
 *     matptr returns the address of real matrix "name"
 *     in scilab's internal stack
 *     m=number of rows
 *     n=number of columns
 *     stk(lp),stk(lp+1),...,stk(lp+m*n-1)= entries (columnwise)
 *     If matrix "name" not in Scilab stack, returns m=n=-1.
 *    Example of use:
 *    Amat is a real 2 x 3 scilab matrix
 *    your subroutine should be as follows:
 *    subroutine mysubr(...)
 *    ...
 *    logic= cmatptr('Amat',m,n,lp)
 *    => m=3 , n=2, and stk(lp)=Amat(1,1)
 *                      stk(lp+1)=Amat(2,1)
 *                      stk(lp+2)=Amat(3,1)
 *                      stk(lp+3)=Amat(1,2) ...
 *                      stk(lp+5)=Amat(3,2)
 *   see example in fydot.f file
 *   see also  readmat.f, matz.f
 *----------------------------------------------------------------*/

int C2F(cmatsptr)(char *namex, int *m, int *n,int *ix,int *j,int *lp,int *nlr, unsigned long name_len)
{
    int id[nsiz];
    C2F(str2name)(namex, id, name_len);
    /* get the position in fin */
    Fin = -1;
    C2F(stackg)(id);
    if (Fin == 0) {
      Scierror(4,_("Undefined variable %s.\n"),get_fname(namex,name_len));
      *m = -1;
      *n = -1;
      return FALSE;
    }
    /* get data */
    if (*Infstk(Fin ) == 2) {
	Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    }
    if (! C2F(getsmat)("creadmat", &Fin, &Fin, m, n, ix, j, lp, nlr, 8L)) {
	return FALSE;
    }
    return TRUE ;
}

/*  Returns a pointer to the Scilab variable with name namex
	Usage:   int *header;
	header = (int *) Name2ptr("pipo");
	header[0], header[1], etc contains header info
	about Scilab variable "pipo"
*/
void *Name2ptr(char *namex)
{
  int l1; int *loci;
  int id[nsiz];
  C2F(str2name)(namex, id, (unsigned long)strlen(namex));
  /* get the position in fin */
  Fin = -1;
  C2F(stackg)(id);
  if (Fin == 0) {
    Scierror(4,_("Undefined variable %s.\n"),get_fname(namex,(long int)strlen(namex)));
    return 0;
  }
  /* get data */
  if (*Infstk(Fin ) == 2) {
    Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
  }
  loci = (int *) stk(*Lstk(Fin));
  if (loci[0] < 0)
    {
      l1 = loci[1];
      loci = (int *) stk(l1);
    }
  return loci;
}


/* returns the position in the internal stack of the variable with name
   namex
   Usage:
   int l=Name2where("pipo");
   stk(l) points to Scilab variable named "pipo"
   e.g. if pipo is a standard real matrix
   stk(l)[2]=first entry of the matrix pipo(1,1)
   stk(l)[3]=pipo(2,1)  etc
   (The header of pipo is given by istk(iadr(h))[0],
   istk(iadr(h))[1], etc )
*/
int Name2where(char *namex)
{
  int id[nsiz];
  C2F(str2name)(namex, id, (unsigned long)strlen(namex));
  /* get the position in fin */
  Fin = -1;
  C2F(stackg)(id);
  if (Fin == 0)
  {
	Scierror(4,_("Undefined variable %s.\n"),get_fname(namex,(long int)strlen(namex)));
    return 0;
  }
  return *Lstk(Fin);
}

/*----------------------------------------------------------------
 *     string conversion to Scilab ID
 *     Warning : the character name is null terMinated
 *             and len(name) is not used
 *             since it can be wrong (ex when name is transmited
 *             by fort (intfort : function )
 *----------------------------------------------------------------*/
int C2F(str2name)(char *namex, int *id, unsigned long name_len)
{
	int ix = 0;
	int lon = 0;

	for (ix = 0 ; ix < (int)  name_len ; ix++ )
	{
		if ( namex[ix] == '\0') break;
		lon++;
	}

	lon = (int)strlen(namex);
	/* remove blanks in namex */
	for (ix = 0; ix < lon; ix++)
	{
		if ( namex[ix] == ' ')
		{
			namex[ix] = '\0';
			lon = (int)strlen(namex);
			break;
		}
	}
	C2F(cvname)(id, namex, &cx0, lon);
	return 0;
}
/*----------------------------------------------------------------
 *     objptr returns the address of "name"
 *     in scilab's internal stack
 *----------------------------------------------------------------*/

int C2F(objptr)(char *namex, int *lp, int *fin, unsigned long name_len)
{
    int id[nsiz];
    *lp = 0;
    /*     ---- get the id */
    C2F(str2name)(namex, id, name_len);
    /*     ---- get the position in fin */
    Fin = -1;
    C2F(stackg)(id);
    if (Fin == 0) {
      C2F(putid)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], id);
      /*         we juste return false and lp is set to zero */
      /*         call error(4) */
      return FALSE;
    }
    *fin = Fin;
    *lp = *Lstk(Fin );
    if (*Infstk(Fin ) == 2) {
	*lp = *Lstk(*istk(iadr(*lp) + 1 +1) );
    }
    return  TRUE;
}
/*--------------------------------------------------------------------------*/
/* read and write a boolean matrix in scilab stack */
/*--------------------------------------------------------------------------*/
int C2F(creadbmat)(char *namex, int *m, int *n, int *scimat, unsigned long name_len)
{
	int l = 0;
	int id[nsiz];
	int c_x = 1;
	int N = 0;

	C2F(str2name)(namex, id, name_len);
	/* read   : from scilab stack -> fortran variable */
	Fin = -1;
	C2F(stackg)(id);
	if (Err > 0) return FALSE ;
	if (Fin == 0) {
		Scierror(4,_("Undefined variable %s.\n"),get_fname(namex,name_len));
		return FALSE;
	}
	if ( *Infstk(Fin ) == 2)  Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);

	/* get matrix data pointer */
	if (! C2F(getbmat)("creadbmat", &Fin, &Fin, m, n, &l , 9L))	return FALSE;

	N = *n * *m;
	C2F(icopy)(&N,istk(l),&c_x,scimat,&c_x);

	return TRUE;
}
/*--------------------------------------------------------------------------*/
int C2F(cwritebmat)(char *namex, int *m, int *n,  int *mat, unsigned long name_len)
{
	int   ix1 = *m * *n;
	int Rhs_k = Rhs , Top_k = Top ;
	int l4, id[nsiz], lr;

	C2F(str2name)(namex, id, name_len);
	Top = Top + Nbvars + 1;
	if (! C2F(crebmat)("cwritebmat", &Top, m, n, &lr, 10L)) return  FALSE;

	C2F(icopy)(&ix1, mat, &cx1, istk(lr ), &cx1);
	Rhs = 0;
	l4 = C2F(iop).lct[3];
	C2F(iop).lct[3] = -1;
	C2F(stackp)(id, &cx0);
	C2F(iop).lct[3] = l4;
	Top = Top_k;
	Rhs = Rhs_k;
	if (Err > 0)  return FALSE;
	return TRUE;

}
/*--------------------------------------------------------------------------*/
int C2F(cmatbptr)(char *namex, int *m,int *n,int *lp, unsigned long name_len)
{
	int id[nsiz];
	C2F(str2name)(namex, id, name_len);
	/* get the position in fin */
	Fin = -1;
	C2F(stackg)(id);
	if (Fin == 0)
	{
		Scierror(4,_("Undefined variable %s.\n"),get_fname(namex,name_len));
		*m = -1;
		*n = -1;
		return FALSE;
	}
	/* get data */
	if (*Infstk(Fin ) == 2)
	{
		Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
	}

	if (! C2F(getbmat)("creadbmat", &Fin, &Fin, m, n, lp , 9L))	return FALSE;

	return TRUE ;
}

/*--------------------------------------------------------------------------*/
/**
	returns length of a "chain variable" in scilab
	example :
	in scilab --> str = "abcdefghijklmnopqrstuvwxyz";
	in C getlengthchain("str") returns 26
	error returns -1
*/
int getlengthchain(char *namex)
{
	int m1, n1;
	int id[nsiz];
	int lr1;
	int nlr1;
	unsigned long name_len= (unsigned long)strlen(namex);

	Err = 0;
	C2F(str2name)(namex, id, name_len);
	Fin = -1;
	C2F(stackg)(id);
	if (Err > 0)  return -1;
	if (Fin == 0) return -1;


	if (*Infstk(Fin ) == 2)
	{
		Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
	}

	if (! C2F(getsmat)("getlengthchain", &Fin, &Fin, &m1, &n1, &cx1, &cx1, &lr1, &nlr1, 14L)) return -1;

	if (m1 * n1 != 1)  return -1;
	return nlr1;
}

/*Compute sum of array's elements*/
int iArraySum(int *_piArray, int _iStart, int _iEnd)
{
	/*Recursive function*/
	/*
	if(_iStart == _iEnd)
		return _piArray[_iEnd];
	return _piArray[_iEnd] + iArraySum(_piArray, _iStart, _iEnd-1);
	*/

	/*Iterative function*/
	int iIndex = 0;
	int iVal = 0;
	for(iIndex = _iStart >= 0 ? _iStart : 0; iIndex < _iEnd ; iIndex++)
	{
		iVal += _piArray[iIndex];
	}
	return iVal;
}

/*--------------------------------------------------------------------------*/
/**
	returns 1 if the variable is complex else 0
*/
int iIsComplex(int _iVar)
{
	/*Check if variable are by value or by reference*/
	int it = 0;
	int il = iadr(*Lstk(Top - Rhs + _iVar));
	int iValType = *istk(il);

	if( iValType < 0)
	{//by reference
        il = iadr(*istk(il+1));
	}
	it = *istk(il + 3);

	//if it > 1 the value is an interger so is not complex
	if(it > 1)
		it = 0;
	return it;
}

/*--------------------------------------------------------------------------*/
/**
	GetRhsPolyVar

	returns array of coeff of polynom matrix
	example :

	In			:
	_iVarNum	: Variable number in the stack

	Out			:
	_piRows		: Matrix lines
	_piCols		: Matrix columns
	_piPow		: Array of max pow for each polymon in the matrix ( size = iRows * iCols )
	_piReal		: Returns a pointer on array of coeff of polynom matrix ( size = sum of _piPow )

	Warning		: if _piPow == NULL, this fonction return _piRows and _piCols with good values to alloc array of Pow and array of Values.
*/
void GetRhsPolyVar(int _iVarNum, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, int* _piReal)
{
	GetRhsCPolyVar(_iVarNum, _piVarName, _piRows, _piCols, _piPow, _piReal, NULL);
}

/*--------------------------------------------------------------------------*/
/**
	GetRhsCPolyVar

	returns array of coeff of complex polynom matrix
	_piImg		: Returns a pointer on array of img coeff of polynom matrix ( size = sum of _piPow )

*/
void GetRhsCPolyVar(int _iVarNum, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, int* _piReal, int *_piImg)
{
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iVarNum));
	int iValType		= *istk(iAddrBase);

	if(iValType < 0)
	{
		iAddrBase		= iadr(*istk(iAddrBase + 1));
		iValType		= *istk(iAddrBase);
	}

	iGetPolyFromAddress(iAddrBase, _piVarName, _piRows, _piCols, _piPow, _piReal, _piImg);

	C2F(intersci).ntypes[_iVarNum - 1] = '$' ;
	C2F(intersci).iwhere[_iVarNum - 1] = *Lstk(_iVarNum);
	C2F(intersci).lad[_iVarNum - 1] = *_piReal;
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
void GetRhsSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow, int* _piReal)
{
	GetRhsCSparseVar(_iVarNum, _piRows, _piCols, _piTotalElem, _piElemByRow, _piColByRow, _piReal, NULL);
}

void GetRhsCSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow, int* _piReal, int* _piImg)
{
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iVarNum));
	int iValType		= *istk(iAddrBase);
	if(iValType < 0)
	{
		iAddrBase		= iadr(*istk(iAddrBase + 1));
		iValType		= *istk(iAddrBase);
	}

	iGetSparseFromAddress(iAddrBase, _piRows, _piCols, _piTotalElem, _piElemByRow, _piColByRow, _piReal, _piImg);

	C2F(intersci).ntypes[_iVarNum - 1] = '$' ;
	C2F(intersci).iwhere[_iVarNum - 1] = *Lstk(_iVarNum);
	C2F(intersci).lad[_iVarNum - 1] = *_piReal;
}

void GetRhsBooleanSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow)
{
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iVarNum));
	int iValType		= *istk(iAddrBase);

	if(iValType < 0)
	{
		iAddrBase		= iadr(*istk(iAddrBase + 1));
		iValType		= *istk(iAddrBase);
	}

	iGetBooleanSparseFromAddress(iAddrBase, _piRows, _piCols, _piTotalElem, _piElemByRow, _piColByRow);
	C2F(intersci).ntypes[_iVarNum - 1] = '$' ;
	C2F(intersci).iwhere[_iVarNum - 1] = *Lstk(_iVarNum);
}

void CreatePolyVarFromPtr(int _iNewVal, int** _piVarName, int _iRows, int _iCols, int *_piPow, double* _pdblRealData)
{
	CreateCPolyVarFromPtr(_iNewVal, _piVarName, _iRows, _iCols, _piPow, _pdblRealData, NULL);
}

void CreateCPolyVarFromPtr(int _iNewVal, int** _piVarName, int _iRows, int _iCols, int *_piPow, double* _pdblRealData, double* _pdblImgData)
{
	int iIndex			= 0;
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iNewVal));
	int iAddrPtr		= 0;
	int iAddrData		= 0;

	*istk(iAddrBase)	= sci_poly;
	*istk(iAddrBase + 1)= _iRows;
	*istk(iAddrBase + 2)= _iCols;
	*istk(iAddrBase + 3)= 0; // Non complex values
	memcpy(istk(iAddrBase + 4), *_piVarName, 4 * sizeof(int)); // name of variable ( scilab format )

	iAddrPtr = iAddrBase + 8;
	*istk(iAddrPtr) = 1;
	iAddrPtr++;
	for(iIndex = 0 ; iIndex < _iRows * _iCols ; iIndex++)
	{
		*istk(iAddrPtr + iIndex) = _piPow[iIndex] + *istk(iAddrPtr -1 + iIndex);
	}
	iAddrData = iAddrBase + (9 + _iRows * _iCols);


	for(iIndex = 0 ; iIndex < iArraySum(_piPow, 0, _iRows * _iCols) ; iIndex++)
	{
		int iTemp = sadr(iAddrData) + iIndex;
		*stk(iTemp) = _pdblRealData[iIndex];
	}

	if(_pdblImgData != NULL)
	{
		//iAddrData  += iArraySum(_piPow, 0, _iRows * _iCols) + 1;
		*istk(iAddrBase + 3) = 1; // Use complex values
		for(iIndex = 0 ; iIndex < iArraySum(_piPow, 0, _iRows * _iCols) ; iIndex++)
		{
			int iTemp = sadr(iAddrData) + iArraySum(_piPow, 0, _iRows * _iCols) + iIndex;
			*stk(iTemp) = _pdblImgData[iIndex];
		}
	}

	C2F(intersci).ntypes[Top - Rhs + _iNewVal - 1]	= '$';
	C2F(intersci).iwhere[Top - Rhs + _iNewVal - 1]	= *Lstk(_iNewVal);
	C2F(intersci).lad[Top - Rhs + _iNewVal - 1]		= sadr(iAddrData);

	if(_pdblImgData == NULL)
		*Lstk(Top - Rhs + _iNewVal + 1) = sadr(iAddrData) + iIndex;
	else
		*Lstk(Top - Rhs + _iNewVal + 1) = sadr(iAddrData) + iArraySum(_piPow, 0, _iRows * _iCols) + iIndex;
}

void GetRhsStringVar(int _iVarNum, int* _piRows, int* _piCols, int* _piLen, char* _pstData)
{
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iVarNum));
	int iValType		= *istk(iAddrBase);
	int iAddrData		= 0;
	if(iValType < 0)
	{
		iAddrBase		= iadr(*istk(iAddrBase + 1));
		iValType		= *istk(iAddrBase);
	}

	iGetStringFromAddress(iAddrBase, _piRows, _piCols, _piLen, &iAddrData);

	if(iAddrData == 0)
	{
		return;
	}

	if(_pstData == NULL)
	{
		return;
	}
	code2str(&_pstData, (int*) cstk(iAddrData), iArraySum(_piLen, 0, *_piRows * *_piCols));
	
	C2F(intersci).ntypes[_iVarNum - 1] = '$' ;
	C2F(intersci).iwhere[_iVarNum - 1] = *Lstk(_iVarNum);
}

void CreateSparseVarFromPtr(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int* _piElemByRow, int* _piColByRow, double* _pdblRealData)
{
	CreateCSparseVarFromPtr(_iNewVal, _iRows, _iCols, _iTotalElem, _piElemByRow, _piColByRow, _pdblRealData, NULL);
}

void CreateCSparseVarFromPtr(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int* _piElemByRow, int* _piColByRow, double* _pdblRealData, double* _pdblImgData)
{
	int iIndex			= 0;
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iNewVal));
	int iAddElemByRow	= 0;
	int iAddrColByRow	= 0;
	int iAddrRealData	= 0;
	int iAddrImgData	= 0;

	*istk(iAddrBase)	= sci_sparse;
	*istk(iAddrBase + 1)= _iRows;
	*istk(iAddrBase + 2)= _iCols;
	*istk(iAddrBase + 3)= 0; // Non complex values
	*istk(iAddrBase + 4)= _iTotalElem;

	iAddElemByRow		= iAddrBase + 5;
	iAddrColByRow		= iAddElemByRow + _iRows;
	iAddrRealData		= iAddrColByRow + _iTotalElem;
	iAddrImgData		= iAddrColByRow + 2 * _iTotalElem;

	for(iIndex = 0 ; iIndex < _iRows ; iIndex++)
		*istk(iAddElemByRow + iIndex) = _piElemByRow[iIndex];

	for(iIndex = 0 ; iIndex < _iTotalElem ; iIndex++)
	{
		int iTemp		= 0;
		*istk(iAddrColByRow + iIndex) = _piColByRow[iIndex];
		iTemp			= sadr(iAddrRealData) + iIndex;
		*stk(iTemp)		= _pdblRealData[iIndex];
	}

	if(_pdblImgData != NULL)
	{
		*istk(iAddrBase + 3)= 1; // Use complex values
		for(iIndex = 0 ; iIndex < _iTotalElem ; iIndex++)
		{
			int iTemp		= 0;
			iTemp		= sadr(iAddrImgData) + iIndex;
			*stk(iTemp)	= _pdblImgData[iIndex];
		}
	}

	C2F(intersci).ntypes[Top - Rhs + _iNewVal - 1]	= '$';
	C2F(intersci).iwhere[Top - Rhs + _iNewVal - 1]	= *Lstk(_iNewVal);
	C2F(intersci).lad[Top - Rhs + _iNewVal - 1]		= sadr(iAddrRealData);

	if(_pdblImgData == NULL)
		*Lstk(Top - Rhs + _iNewVal + 1) = sadr(iAddrRealData) + iIndex;
	else
		*Lstk(Top - Rhs + _iNewVal + 1) = sadr(iAddrRealData) + _iTotalElem + iIndex;


}

void CreateCBooleanSparseVarFromPtr(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int* _piElemByRow, int* _piColByRow)
{
	int iIndex			= 0;
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iNewVal));
	int iAddElemByRow	= 0;
	int iAddrColByRow	= 0;
	int iAddrRealData	= 0;

	*istk(iAddrBase)	= sci_boolean_sparse;
	*istk(iAddrBase + 1)= _iRows;
	*istk(iAddrBase + 2)= _iCols;
	*istk(iAddrBase + 3)= 0; // Non complex values
	*istk(iAddrBase + 4)= _iTotalElem;

	iAddElemByRow		= iAddrBase + 5;
	iAddrColByRow		= iAddElemByRow + _iRows;
	iAddrRealData		= iAddrColByRow + _iTotalElem;

	for(iIndex = 0 ; iIndex < _iRows ; iIndex++)
		*istk(iAddElemByRow + iIndex) = _piElemByRow[iIndex];

	for(iIndex = 0 ; iIndex < _iTotalElem ; iIndex++)
	{
//		int iTemp		= 0;
		*istk(iAddrColByRow + iIndex) = _piColByRow[iIndex];
//		iTemp			= sadr(iAddrRealData) + iIndex;
//		*stk(iTemp)		= _piRealData[iIndex];
	}

	C2F(intersci).ntypes[Top - Rhs + _iNewVal - 1]	= '$';
	C2F(intersci).iwhere[Top - Rhs + _iNewVal - 1]	= *Lstk(_iNewVal);
	C2F(intersci).lad[Top - Rhs + _iNewVal - 1]		= sadr(iAddrRealData);

	*Lstk(Top - Rhs + _iNewVal + 1) = sadr(iAddrRealData) + iIndex;
}

int GetDimFromVar(int _iVarNum, int _iNum/*Oo*/, int* _piVal)
{
	int iType				= GetType(_iVarNum);
	int iRows				= 0;
	int iCols				= 0;
	int iRealData			= 0;
	double *pdblRealData	= NULL;
	int *piRealData			= NULL;

	if(iType == sci_matrix)
	{
		if(iIsComplex(_iVarNum))
		{
			Error(89);
			return _iNum;
		}
		GetRhsVar(_iVarNum, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData = stk(iRealData);
		*_piVal = (int)Max(pdblRealData[0], 0);
	}
	else if(iType == sci_ints)
	{
		int iComplex	= iIsComplex(_iVarNum);
		int iYType		= 4;
		int iXInc		= 1;
		int iYInc		= 1;

		GetRhsVar(_iVarNum, MATRIX_OF_INTEGER_DATATYPE, &iRows, &iCols, &iRealData);
		if(iRows * iCols != 1)
		{
			Error(89);
			return _iNum;
		}
		iRows = 1;
		piRealData = istk(iRealData);
		C2F(tpconv)(&iComplex, &iYType, &iRows, piRealData, &iXInc, _piVal, &iYInc);
		*_piVal = Max(*_piVal, 0);
	}
	else
	{
		Error(89);
		return _iNum;
	}
	return 0;
}

void CreateBooleanVarFromPtr(int _iNewVal, int _iRows, int _iCols, int* _piBoolData)
{
	int iIndex			= 0;
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iNewVal));
	int iAddrRealData	= 0;

	*istk(iAddrBase)	= sci_boolean;
	*istk(iAddrBase + 1)= _iRows;
	*istk(iAddrBase + 2)= _iCols;
	iAddrRealData		= iAddrBase + 3;

	for(iIndex = 0 ; iIndex < _iRows*_iCols ; iIndex++)
		*istk(iAddrRealData + iIndex) = _piBoolData[iIndex];

	C2F(intersci).ntypes[Top - Rhs + _iNewVal - 1]	= '$';
	C2F(intersci).iwhere[Top - Rhs + _iNewVal - 1]	= *Lstk(_iNewVal);
	C2F(intersci).lad[Top - Rhs + _iNewVal - 1]		= sadr(iAddrRealData);
}

void CheckVarUsed(int _iVarNum)
{
	int iType = GetType(_iVarNum);
	int iVar = Top - Rhs + _iVarNum;

	int iAddress = iadr(*Lstk(iVar));
	if (*istk(iAddress ) < 0)
		iAddress = iadr(*istk(iAddress +1));


	switch(iType)
	{
	case sci_matrix :
		iAddress += 4;
		break;
	case sci_poly :
		iAddress += 9 + (*istk(iAddress + 1) * *istk(iAddress + 2));
		break;
	case sci_boolean :
		iAddress += 3;
		break;
	case sci_sparse :
		iAddress += 5 + *istk(iAddress + 1) + *istk(iAddress + 4);
		break;
	case sci_ints :
		iAddress += 4;
		break;
	case sci_strings :
		iAddress += 4;
	default:
		break;
	}
//	C2F(intersci).iwhere[_iNewVal - 1]	= *Lstk(iNewPos);

	C2F(intersci).ntypes[_iVarNum - 1] = '$' ;
	C2F(intersci).iwhere[_iVarNum - 1] = *Lstk(iVar);
	C2F(intersci).lad[_iVarNum - 1] = sadr(iAddress);

}

void CheckAllVarUsed(int _iStart, int _iEnd)
{
	int iIndex = 0;
	int iEnd = _iEnd;
	int iStart = _iStart;
	if(iEnd == -1)
		iEnd = Rhs;
	if(iStart == -1)
		iStart = 1;

	for(iIndex = iStart ; iIndex <= iEnd ; iIndex++)
		CheckVarUsed(iIndex);
}


void GetVarDimension(int _iVarNum, int* _piRows, int* _piCols)
{
	int iVar = Top - Rhs + _iVarNum;

	int iAddress = iadr(*Lstk(iVar));
	if (*istk(iAddress ) < 0)
		iAddress = iadr(*istk(iAddress +1));

	*_piRows = *istk(iAddress + 1);
	*_piCols = *istk(iAddress + 2);
}

int iGetOrient(int _iVal)
{
	int iMode			= 0;
	int iRows			= 0;
	int iCols			= 0;
	int iRealData		= 0;
	char **szRealData	= 0;

	if(GetType(2) == sci_matrix)
	{
		GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		iMode = (int)stk(iRealData)[0];
	}
	else if(GetType(2) == sci_strings)
	{
		GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &iRows, &iCols, &szRealData);
		iMode = (int)*szRealData[0];
		freeArrayOfString(szRealData, iRows * iCols);
	}
	else
	{
		Error(44);
		return -2;
	}

	if(iRows != 1 || iCols != 1)
	{
		Error(89);
		return -2;
	}

	if(iMode == ROW_LETTER || iMode == BY_ROWS)
		iMode = BY_ROWS;
	else if(iMode == COL_LETTER || iMode == BY_COLS)
		iMode = BY_COLS;
	else if(iMode == STAR_LETTER || iMode == BY_ALL)
		iMode = BY_ALL;
	else if(iMode == MTLB_LETTER || iMode == BY_MTLB)
	{//J'ai pas tout compris dans le fonctionnement pour MtLb
		iMode = BY_ALL;
		if(iRows > 1)
			iMode = BY_ROWS;
		else if(iCols > 1)
			iMode = BY_COLS;
	}
	else
	{
		Error(44);
		return -2;
	}
	return iMode;
}

/*
_iAllocMatrixDoubleOrComplex --
  Reserve space in stack for a matrix of real or complex.
Arguments
  _iPos : index of the Scilab variable
  _iComplex : type data in the matrix. If  is 0, the
     _iComplex = 0 > real data
     _iComplex = 1 > complex data
  _iRows : number of rows in the matrix
  _iCols : number of columns in the matrix
  _pdblRealData : pointer to the block of data for real values
  _pdblImgData : pointer to the block of data for complex values
Note
  This is a private, support routine for iAllocMatrixOfDouble and iAllocComplexMatrixOfDouble.
  It should not be used outside.
*/
int	_iAllocMatrixDoubleOrComplex(int _iPos, int _iComplex, int _iRows, int _iCols, double **_pdblRealData, double **_pdblImgData)
{
	int iNewPos			= Top - Rhs + _iPos;
	int iSize			= _iRows * _iCols * (_iComplex + 1);
	int iAddr			= iadr(*Lstk(iNewPos));
	int iAddrData		= iAddr + 4;

	Err					= sadr(iAddr + 4) - *Lstk(Bot);

	if(Err > -iSize)
		return 17;

	iAllocComplexMatrixOfDoubleToAddress(istk(iAddr), _iComplex, _iRows, _iCols, _pdblRealData, _pdblImgData);

	intersci_.ntypes[_iPos - 1]	= '$';
	intersci_.iwhere[_iPos - 1]	= *Lstk(iNewPos);
	intersci_.lad[_iPos - 1]	= sadr(iAddrData);

	*Lstk(iNewPos + 1) = sadr(iAddrData) + _iRows * _iCols * (_iComplex + 1);
	return 0;
}

/*for internal use, assign values in variable without, VarNum, just scilab address*/
int	iAllocComplexMatrixOfDoubleToAddress(int* _piAddr, int _iComplex, int _iRows, int _iCols, double **_pdblRealData, double **_pdblImgData)
{
//	int iAddrData		= _iAddr + 4;

	_piAddr[0]			= sci_matrix;
	_piAddr[1]			= Min(_iRows, _iRows * _iCols);
	_piAddr[2]			= Min(_iCols, _iRows * _iCols);
	_piAddr[3]			= _iComplex;


	*_pdblRealData		= (double*)(_piAddr + 4);
	if(_iComplex != 0)
		*_pdblImgData	= *_pdblRealData + _iRows * _iCols;

	return 0;
}

/*
iAllocMatrixOfDouble --
  Reserve space in stack for a matrix of double.
Arguments
  _iPos : index of the Scilab variable
  _iRows : number of rows in the matrix
  _iCols : number of columns in the matrix
  _pdblRealData : pointer to the block of data for real values
*/
int iAllocMatrixOfDouble(int _iPos, int _iRows, int _iCols, double **_pdblRealData)
{
	if(_iPos + 1 > Bot) 
		return 10;//Too many names

	return _iAllocMatrixDoubleOrComplex(_iPos, 0, _iRows, _iCols, _pdblRealData, NULL);
}

/*
iAllocComplexMatrixOfDouble --
  Reserve space in stack for a matrix of complex.
Arguments
  _iPos : index of the Scilab variable
  _iRows : number of rows in the matrix
  _iCols : number of columns in the matrix
  _pdblRealData : pointer to the block of data for real values
  _pdblImgData : pointer to the block of data for complex values
*/
int	iAllocComplexMatrixOfDouble(int _iPos, int _iRows, int _iCols, double **_pdblRealData, double **_pdblImgData)
{
	return _iAllocMatrixDoubleOrComplex(_iPos, 1, _iRows,  _iCols, _pdblRealData, _pdblImgData);
}

/*Reserve space in stack for a matrix of polynom*/
int iAllocMatrixOfPoly(int _iNewVal, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData)
{
	return iAllocComplexMatrixOfPoly(_iNewVal, 0, _piVarName, _iRows, _iCols, _piPow, _pdblRealData, NULL);
}

/*Reserve space in stack for a matrix of complex polynom*/
int iAllocComplexMatrixOfPoly(int _iNewVal, int _iComplex, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData, double** _pdblImgData)
{
	int iNewPos			= Top - Rhs + _iNewVal;
	//int iNewPos			= Top + _iNewVal;
	int iAddrBase		= iadr(*Lstk(iNewPos));
	int iAddrData		= 0;

	iAllocComplexMatrixOfPolyToAddress(iAddrBase, _iComplex, _piVarName, _iRows, _iCols, _piPow, _pdblRealData, _pdblImgData);
	iAddrData = iAddrBase + (9 + _iRows * _iCols);

	C2F(intersci).ntypes[_iNewVal - 1]	= '$';
	C2F(intersci).iwhere[_iNewVal - 1]	= *Lstk(iNewPos);
	C2F(intersci).lad[_iNewVal - 1]		= sadr(iAddrData);

	*Lstk(iNewPos + 1) = sadr(iAddrData) + iArraySum(_piPow, 0, _iRows * _iCols) * (_iComplex + 1) * 2;
	return 0;
}


/*for internal use, assign values in variable without, VarNum, just scilab address*/
int iAllocComplexMatrixOfPolyToAddress(int _iAddr, int _iComplex, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData, double** _pdblImgData)
{
	int iAddrBase		= _iAddr;
	int iAddrPtr		= 0;
	int iAddrData		= 0;
	int iIndex			= 0;

	*istk(iAddrBase)	= sci_poly;
	*istk(iAddrBase + 1)= _iRows;
	*istk(iAddrBase + 2)= _iCols;
	*istk(iAddrBase + 3)= _iComplex;
	memcpy(istk(iAddrBase + 4), *_piVarName, 4 * sizeof(int)); // name of variable ( scilab format )

	iAddrPtr = iAddrBase + 8;
	*istk(iAddrPtr) = 1;
	iAddrPtr++;

	for(iIndex = 0 ; iIndex < _iRows * _iCols ; iIndex++)
	{
		*istk(iAddrPtr + iIndex) = _piPow[iIndex] + *istk(iAddrPtr -1 + iIndex);
	}
	iAddrData = iAddrBase + (9 + _iRows * _iCols);


	*_pdblRealData = stk(sadr(iAddrData));
	if(_iComplex != 0)
		*_pdblImgData = stk(sadr(iAddrData) + iArraySum(_piPow, 0, _iRows * _iCols));

	return 0;
}

/*Reserve space in stack for a matrix of string*/
int iAllocMatrixOfString(int _iNewVal, int _iRows, int _iCols, int *_piLen, char** _piStringData)
{
	int iNewPos			= Top - Rhs + _iNewVal;
	//int iNewPos			= Top + _iNewVal;
	int iAddrBase		= iadr(*Lstk(iNewPos));
	int iAddrData		= 0;

	iAllocMatrixOfStringToAddress(iAddrBase, _iRows, _iCols, _piLen, _piStringData);
	iAddrData = iAddrBase + (5 + _iRows * _iCols);

	*_piStringData = cstk(sadr(iAddrData));

	C2F(intersci).ntypes[_iNewVal - 1]	= '$';
	C2F(intersci).iwhere[_iNewVal - 1]	= *Lstk(iNewPos);
	C2F(intersci).lad[_iNewVal - 1]		= sadr(iAddrData);

	*Lstk(iNewPos + 1) = sadr(iAddrData) + iArraySum(_piLen, 0, _iRows * _iCols);
	return 0;
}

int iAllocMatrixOfStringToAddress(int _iAddr, int _iRows, int _iCols, int *_piLen, char **_piStringData)
{
	int iAddrPtr	= 0;
	int iIndex		= 0;
	*istk(_iAddr)	= sci_strings;
	*istk(_iAddr + 1)= _iRows;
	*istk(_iAddr + 2)= _iCols;
	*istk(_iAddr + 3)= 0; // Non complex values

	iAddrPtr = _iAddr + 4;
	*istk(iAddrPtr) = 1;
	iAddrPtr++;
	for(iIndex = 0 ; iIndex < _iRows * _iCols ; iIndex++)
	{
		*istk(iAddrPtr + iIndex) = _piLen[iIndex] + *istk(iAddrPtr -1 + iIndex);
	}
	return 0;
}

/*Reserve space in stack for a sparse matrix*/
int iAllocSparseMatrix(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int** _piElemByRow, int** _piColByRow, double** _pdblRealData)
{
	return iAllocComplexSparseMatrix(_iNewVal, 0, _iRows, _iCols, _iTotalElem, _piElemByRow, _piColByRow, _pdblRealData, NULL);
}

/*Reserve space in stack for a complex sparse matrix*/
int iAllocComplexSparseMatrix(int _iNewVal,int _iComplex, int _iRows, int _iCols, int _iTotalElem, int** _piElemByRow, int** _piColByRow, double** _pdblRealData, double** _pdblImgData)
{
	int iNewPos			= Top - Rhs + _iNewVal;
	//int iNewPos			= Top + _iNewVal;
	int iAddrBase		= iadr(*Lstk(iNewPos));
	int iAddElemByRow	= 0;
	int iAddrColByRow	= 0;
	int iAddrRealData	= 0;
	int iAddrImgData	= 0;

	*istk(iAddrBase)	= sci_sparse;
	*istk(iAddrBase + 1)= _iRows;
	*istk(iAddrBase + 2)= _iCols;
	*istk(iAddrBase + 3)= _iComplex;
	*istk(iAddrBase + 4)= _iTotalElem;

	iAddElemByRow		= iAddrBase + 5;
	iAddrColByRow		= iAddElemByRow + _iRows;
	iAddrRealData		= iAddrColByRow + _iTotalElem;
	iAddrImgData		= iAddrColByRow + 2 * _iTotalElem;


	*_piElemByRow = istk(iAddElemByRow);
	*_piColByRow = istk(iAddrColByRow);
	*_pdblRealData = stk(sadr(iAddrRealData));

	if(_iComplex != 0)
		*_pdblImgData = stk(sadr(iAddrImgData));

	C2F(intersci).ntypes[_iNewVal - 1]	= '$';
	C2F(intersci).iwhere[_iNewVal - 1]	= *Lstk(iNewPos);
	C2F(intersci).lad[_iNewVal - 1]		= sadr(iAddrRealData);

	if(_iComplex == 0)
		*Lstk(iNewPos + 1) = sadr(iAddrRealData) + _iTotalElem;
	else
		*Lstk(iNewPos + 1) = sadr(iAddrRealData) + 2 * _iTotalElem;

	return 0;

}

/*Reserve space in stack for a matrix of boolean value*/
int iAllocMatrixOfBoolean(int _iNewVal, int _iRows, int _iCols, int** _piBoolData)
{
	int iNewPos			= Top - Rhs + _iNewVal;
	//int iNewPos			= Top + _iNewVal;
	int iAddrBase		= iadr(*Lstk(iNewPos));
	int iAddrRealData	= 0;

	*istk(iAddrBase)	= sci_boolean;
	*istk(iAddrBase + 1)= _iRows;
	*istk(iAddrBase + 2)= _iCols;
	iAddrRealData		= iAddrBase + 3;

	*_piBoolData = istk(iAddrRealData);

	C2F(intersci).ntypes[_iNewVal - 1]	= '$';
	C2F(intersci).iwhere[_iNewVal - 1]	= *Lstk(iNewPos);
	C2F(intersci).lad[_iNewVal - 1]		= sadr(iAddrRealData);

	*Lstk(iNewPos + 1) = sadr(iAddrBase + _iRows * _iCols + 2);
	return 0;
}


/*Reserve space in stack for a boolean sparse matrix*/
int iAllocBooleanSparseMatrix(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int** _piElemByRow, int** _piColByRow)
{
	int iNewPos			= Top - Rhs + _iNewVal;
	//int iNewPos			= Top + _iNewVal;
	int iAddrBase		= iadr(*Lstk(iNewPos));
	int iAddElemByRow	= 0;
	int iAddrColByRow	= 0;
	int iAddrRealData	= 0;

	*istk(iAddrBase)	= sci_boolean_sparse;
	*istk(iAddrBase + 1)= _iRows;
	*istk(iAddrBase + 2)= _iCols;
	*istk(iAddrBase + 3)= 0; // Non complex values
	*istk(iAddrBase + 4)= _iTotalElem;

	iAddElemByRow		= iAddrBase + 5;
	iAddrColByRow		= iAddElemByRow + _iRows;
	iAddrRealData		= iAddrColByRow + _iTotalElem;

	*_piElemByRow = istk(iAddElemByRow);
	*_piColByRow = istk(iAddrColByRow);

	C2F(intersci).ntypes[_iNewVal - 1]	= '$';
	C2F(intersci).iwhere[_iNewVal - 1]	= *Lstk(iNewPos);
	C2F(intersci).lad[_iNewVal - 1]		= sadr(iAddrRealData);

	*Lstk(iNewPos + 1) = sadr(iAddrRealData);
	return 0;
}

int iGetListItemType(int _iVar, int* _piParentList, int *_piItemNumber, int *_pElemType)
{
	int *piBase			= NULL;
	int *piOffset		= NULL;
	int *piItem			= NULL;
	int iIndex			= 0;

	if(_piParentList == NULL)
	{
		int iAddrBase	=	iadr(*Lstk(Top - Rhs + _iVar));
		int iValType	= *istk(iAddrBase);

		if(iValType < 0)
		{
			iAddrBase		=	iadr(*istk(iAddrBase + 1));
			iValType		= *istk(iAddrBase);
		}

		piBase				= istk(iAddrBase);
		piOffset			= piBase + 2;
	}
	else
	{
		if(IsKindOfList(_piParentList))
		{
			piBase			= _piParentList;
			piOffset		= _piParentList + 2;
		}
		else
		{
			return 0;
		}
	}

	*_piItemNumber		= piBase[1];

	if(_pElemType == NULL)
		return 0;

	//Warning : variable starts at a even ( pair ) address, so if the size is odd, we add 1 to have a even address.
	piItem		= piOffset + (1 + *_piItemNumber + !(*_piItemNumber % 2));
	for(iIndex = 0 ; iIndex < *_piItemNumber ; iIndex++)
	{
		_pElemType[iIndex] = piItem[(piOffset[iIndex] - 1) * 2]; //-/+ 1
	}
	return 0;
}

int* iGetAddressFromItemPos(int *_piParent, int _iItemPos)
{
	int iItemCount		= _piParent[1];
	int *piOffset		= _piParent + 2;

	int *piAddrItem		= NULL;

	if(_iItemPos > iItemCount)
		return 0;

	piAddrItem			= piOffset + iItemCount + 1 + !(iItemCount % 2);
	//iAddrItem			= iAddrOffset + iItemCount + 1 + !(iItemCount % 2);
/*
	for(iIndex = 0 ; iIndex < _iItemNumber ; iIndex++)
		iAddrItem	+= (*istk(iAddrOffset + _iItemNumber) - *istk(iAddrOffset + _iItemNumber - 1)) * 2;
*/
//	iAddrItem	+= (piOffset[_iItemPos - 1] - 1) * (sizeof(double) / sizeof(int));
	piAddrItem	+= (piOffset[_iItemPos] - 1) * (sizeof(double) / sizeof(int));

	return piAddrItem;
}

int iGetAddressFromItemNumber(int _iVar, int _iItemNumber)
{
	int	iItemCount		= 0;
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iVar));
	int iAddrOffset		= iAddrBase + 2;
	int iAddrItem		= 0;
	int *piOffset		= istk(iAddrOffset);

	iItemCount			= *istk(iAddrBase + 1);
	if(_iItemNumber > iItemCount)
		return 0;

	iAddrItem			= iAddrOffset + iItemCount + 1 + !(iItemCount % 2);
/*
	for(iIndex = 0 ; iIndex < _iItemNumber ; iIndex++)
		iAddrItem	+= (*istk(iAddrOffset + _iItemNumber) - *istk(iAddrOffset + _iItemNumber - 1)) * 2;
*/
	iAddrItem	+= (piOffset[_iItemNumber - 1] - 1) * (sizeof(double) / sizeof(int));

	return iAddrItem;
}

int* iGetListItemPointerFromItemNumber(int _iVar, int* _piParentList, int _iItemNumber)
{
	int *pItemAddr = _piParentList;
	int iItemCount	= 0;
	int *piOffset		= NULL;
	int *piItem			= NULL;

	if(pItemAddr == NULL)
	{//parent is the current list
		int iAddrBase	=	iadr(*Lstk(Top - Rhs + _iVar));
		int iValType	= *istk(iAddrBase);

		if(iValType < 0)
		{
			iAddrBase		=	iadr(*istk(iAddrBase + 1));
			iValType		= *istk(iAddrBase);
		}

		pItemAddr	= istk(iAddrBase);
	}

	if(!IsKindOfList(pItemAddr))
	{
		return 0;
	}

	iItemCount	= pItemAddr[1];
	piOffset		= pItemAddr + 2;

	if(_iItemNumber > iItemCount)
		return 0;

	piItem			= piOffset + iItemCount + 1 + !(iItemCount % 2);
/*
	for(iIndex = 0 ; iIndex < _iItemNumber ; iIndex++)
		iAddrItem	+= (*istk(iAddrOffset + _iItemNumber) - *istk(iAddrOffset + _iItemNumber - 1)) * 2;
*/
	//_pElemType[iIndex] = piItem[(piOffset[iIndex] - 1) * 2]
	//iAddrItem	+= (piOffset[_iItemNumber - 1] - 1) * (sizeof(double) / sizeof(int));
	piItem += (piOffset[_iItemNumber] - 1) * 2;

	return piItem;
}

int iGetListItemDouble(int _iVar, int _iItemNumber, int *_piRows, int *_piCols, double **_pdblReal, double **_pdblImg)
{
	int iAddrItem		= 0;
	int iReal			= 0;
	int iImg			= 0;

	//iAddrItem is on the asked value
	iAddrItem = iGetAddressFromItemNumber(_iVar, _iItemNumber);
	if(iAddrItem != 0)
		iGetDoubleFromAddress(iAddrItem, _piRows, _piCols, &iReal, &iImg);

	*_pdblReal		= stk(iReal);
	if(_pdblImg != NULL && iImg != 0)
		*_pdblImg	= stk(iImg);
	return 0;
}

int iGetListItemPoly(int _iVar, int _iItemNumber, int **_piVarName, int *_piRows, int *_piCols, int *_piPow, double **_pdblReal, double **_pdblImg)
{
	int iReal		= 0;
	int iImg		= 0;
	int iAddrItem = iGetAddressFromItemNumber(_iVar, _iItemNumber);

	if(iAddrItem != 0)
		iGetPolyFromAddress(iAddrItem, _piVarName, _piRows, _piCols, _piPow, &iReal, &iImg);

	if(iReal == 0)
		return 0;

	*_pdblReal		= stk(iReal);
	if(*_pdblImg != NULL && iImg != 0)
		*_pdblImg	= stk(iImg);
	return 0;
}

int iGetListItemSparse(int _iVar, int _iItemNumber, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, double **_pdblReal, double **_pdblImg)
{
	return 0;
}

int iGetListItemString(int _iVar, int _iItemNumber, int *_piRows, int *_piCols, int *_piLen, char* _pszData)
{
	int iAddrData	= 0;
	int iAddrItem	= iGetAddressFromItemNumber(_iVar, _iItemNumber);

	iGetStringFromAddress(iAddrItem, _piRows, _piCols, _piLen, &iAddrData);

	if(iAddrData == 0 || _pszData == NULL)
		return 0;

	code2str(&_pszData, (int*) cstk(iAddrData), iArraySum(_piLen, 0, *_piRows * *_piCols));
	return 0;
}

//Get SubList reference
int* iGetListItemList(int _iVar, int* _piParentList, int _iItemPos)
{
	int *piChild		= NULL;


	if(_piParentList == NULL)
	{//parent is the current list
		int iAddrBase	=	iadr(*Lstk(Top - Rhs + _iVar));
		int iValType	= *istk(iAddrBase);

		if(iValType < 0)
		{
			iAddrBase		=	iadr(*istk(iAddrBase + 1));
			iValType		= *istk(iAddrBase);
		}

		_piParentList	= istk(iAddrBase);
	}

	if(!IsKindOfList(_piParentList))
	{
		return 0;
	}

	if(_iItemPos == 0)
	{
		piChild = _piParentList;
	}
	else
	{
		piChild = iGetAddressFromItemPos(_piParentList, _iItemPos);
	}

	if(!IsKindOfList(piChild))
		return NULL;
	return piChild;
}

//Get SubItem String
int iGetListSubItemString(int _iVar, int* _piParentList, int _iItemNumber, int *_piRows, int *_piCols, int *_piLen, char* _pszData)
{
	int *piString = NULL;
	int* piItemAdd = iGetListItemPointerFromItemNumber(_iVar, _piParentList, _iItemNumber);

	if(piItemAdd == NULL)
	{
		return 1;
	}

	iGetStringFromPointer(piItemAdd, _piRows, _piCols, _piLen, &piString);

	if(_piLen == NULL || _pszData == NULL)
	{
		return 0;
	}

	code2str(&_pszData, piString, iArraySum(_piLen, 0, *_piRows * *_piCols));
	{
		return 0;
	}
}


//Internal fonctions to retrieve varaibles information from Address ( old "il" )
int iGetDoubleFromAddress(int _iAddr, int *_piRows, int *_piCols, int *_piReal, int *_piImg)
{
	int iAddrOffset		= 0;
	*_piRows			= *istk(_iAddr + 1);
	*_piCols			= *istk(_iAddr + 2);
	iAddrOffset			= _iAddr + 4;//Stack header

	*_piReal	= sadr(iAddrOffset);
	if(*istk(_iAddr + 3) == 1)//complex variable and allocated buffer
		*_piImg		= sadr(iAddrOffset + *_piRows * *_piCols);
	return 0;
}

int iGetPolyFromAddress(int _iAddr, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, int* _piReal, int *_piImg)
{
	int iIndex			= 0;
	int iAddrOffset		= 0;
	*_piRows			= *istk(_iAddr + 1);
	*_piCols			= *istk(_iAddr + 2);
	*_piVarName			= istk(_iAddr + 4);
	iAddrOffset			= _iAddr + 8; //4 Stack header + 4 variable name


	if(_piPow == NULL)
		return 0;

	/*Get all offest*/
	for(iIndex = 0 ; iIndex < *_piRows * *_piCols; iIndex++)
		_piPow[iIndex] = *istk(iAddrOffset + iIndex + 1) - *istk(iAddrOffset + iIndex );

	*_piReal	= sadr(iAddrOffset + 1 + *_piRows * *_piCols);
	if(_piImg != NULL && *istk(_iAddr + 3) == 1)//complex variable and allocated buffer
		*_piImg		= sadr(iAddrOffset + 1 + *_piRows * *_piCols) + iArraySum(_piPow, 0, *_piRows * *_piCols);
	return 0;
}

int iGetSparseFromAddress(int _iAddr, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow, int* _piReal, int* _piImg)
{
	int iAddrColByRow	= 0;
	int iAddElemByRow	= 0;
	int iAddrRealData	= 0;
	int iAddrImgData	= 0;
	int iIndex			= 0;

	*_piRows			= *istk(_iAddr + 1);
	*_piCols			= *istk(_iAddr + 2);
	*_piTotalElem		= *istk(_iAddr + 4);

	//if ptr are NULL, juste return the matrix size
	if(_piElemByRow == NULL || _piColByRow == NULL)
		return 0;

	iAddElemByRow		= _iAddr + 5;
	iAddrColByRow		= iAddElemByRow + *_piRows;
	iAddrRealData		= iAddrColByRow + *_piTotalElem;
	iAddrImgData		= iAddrColByRow + 2 * (*_piTotalElem);

	for(iIndex = 0 ; iIndex < *_piRows ; iIndex++)
	{
		_piElemByRow[iIndex] = *istk(iAddElemByRow + iIndex);
	}

	for(iIndex = 0 ; iIndex < *_piTotalElem ; iIndex++)
	{
		_piColByRow[iIndex] = *istk(iAddrColByRow + iIndex);
	}

	*_piReal = sadr(iAddrRealData);
	if(_piImg != NULL && *istk(_iAddr + 3) == 1)
		*_piImg	 = sadr(iAddrRealData) + *_piTotalElem;
	return 0;
}

int iGetBooleanSparseFromAddress(int _iAddr, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow)
{
	int iAddrColByRow	= 0;
	int iAddElemByRow	= 0;
	int iIndex			= 0;
	*_piRows			= *istk(_iAddr + 1);
	*_piCols			= *istk(_iAddr + 2);
	*_piTotalElem		= *istk(_iAddr + 4);

	//if ptr are NULL, juste return the matrix size
	if(_piElemByRow == NULL || _piColByRow == NULL)
		return 0;

	iAddElemByRow		= _iAddr + 5;
	iAddrColByRow		= iAddElemByRow + *_piRows;

	for(iIndex = 0 ; iIndex < *_piRows ; iIndex++)
		_piElemByRow[iIndex] = *istk(iAddElemByRow + iIndex);

	for(iIndex = 0 ; iIndex < *_piTotalElem ; iIndex++)
		_piColByRow[iIndex] = *istk(iAddrColByRow + iIndex);
	return 0;
}

int iGetBooleanFromAddress(int _iAddr, int *_piRows, int *_piCols, int* _piBool)
{
	return 0;
}

int iGetStringFromAddress(int _iAddr, int *_piRows, int *_piCols, int *_piLen, int* _piString)
{
	int iAddrOffset		= 0;
	int iAddrData		= 0;
	int iIndex			= 0;

	*_piRows			= *istk(_iAddr + 1);
	*_piCols			= *istk(_iAddr + 2);


	if(_piLen == NULL)
		return 0;

	iAddrOffset			= _iAddr + 4;
	/*Get all offest*/
	for(iIndex = 0 ; iIndex < *_piRows * *_piCols; iIndex++)
		_piLen[iIndex] = *istk(iAddrOffset + iIndex + 1) - *istk(iAddrOffset + iIndex );

/*
	iAddrData = iAddrBase + (5 + _iRows * _iCols);

	*_pszRealData = cstk(sadr(iAddrData));
*/
	iAddrData = _iAddr + (5 + *_piRows * *_piCols);

	*_piString			= cadr(iAddrData);
	return 0;
}

int iGetStringFromPointer(int* _piAddr, int *_piRows, int *_piCols, int *_piLen, int** _piString)
{
	int iIndex			= 0;
	int *piOffset		= NULL;

	*_piRows				= _piAddr[1];
	*_piCols				= _piAddr[2];


	if(_piLen == NULL)
		return 0;

	piOffset			= _piAddr + 4;

	/*Get all offest*/
	for(iIndex = 0 ; iIndex < *_piRows * *_piCols; iIndex++)
		_piLen[iIndex] = piOffset[iIndex + 1] - piOffset[iIndex];

	*_piString			= _piAddr + (5 + (*_piRows) * (*_piCols));
	return 0;
}

void vGetPointerFromDoubleComplex(doublecomplex *_poComplex, int _iSize, double *_pdblReal, double *_pdblImg)
{

	int iTwo	= 2;
	int iOne	= 1;
	double *pReal = &_poComplex[0].r;
	double *pImg = &_poComplex[0].i;

	if(_pdblReal != NULL && _pdblImg != NULL)
	{
		C2F(dcopy)(&_iSize, pReal, &iTwo, _pdblReal, &iOne);
		C2F(dcopy)(&_iSize, pImg, &iTwo, _pdblImg, &iOne);
	}
	else if(_pdblReal != NULL && _pdblImg == NULL)
	{
		C2F(dcopy)(&_iSize, pReal, &iTwo, _pdblReal, &iOne);
	}
	else if(_pdblReal == NULL && _pdblImg != NULL)
	{
		C2F(dcopy)(&_iSize, pImg, &iTwo, _pdblImg, &iOne);
	}
}
doublecomplex* oGetDoubleComplexFromPointer(double *_pdblReal, double *_pdblImg, int _iSize)
{
	doublecomplex *poComplex = (doublecomplex*)MALLOC(sizeof(doublecomplex) * _iSize);
	int iTwo	= 2;
	int iOne	= 1;
	double *pReal = &poComplex[0].r;
	double *pImg = &poComplex[0].i;

	if(_pdblReal != NULL && _pdblImg != NULL)
	{

		C2F(dcopy)(&_iSize, _pdblReal, &iOne, pReal, &iTwo);
		C2F(dcopy)(&_iSize, _pdblImg, &iOne, pImg, &iTwo);
	}
	else if(_pdblReal != NULL && _pdblImg == NULL)
	{
		double ZERO = 0.;
		C2F(dcopy)(&_iSize, _pdblReal, &iOne, pReal, &iTwo);
		C2F(dset)(&_iSize, &ZERO, pImg, &iTwo);
	}
	else if(_pdblReal == NULL && _pdblImg != NULL)
	{
		double ZERO = 0.;
		C2F(dset)(&_iSize, &ZERO, pReal, &iTwo);
		C2F(dcopy)(&_iSize, _pdblImg, &iOne, pImg, &iTwo);
	}
	else
	{
		FREE(poComplex);
		return NULL;
	}
	return poComplex;
}
void vFreeDoubleComplexFromPointer(doublecomplex *_poComplex)
{
	if(_poComplex != NULL)
		FREE(_poComplex);
}

/*
Create a list in scilab stack
This function only "reserves" VarNum
To add data, call iListAllocxxxx
*/
int* iAllocTList(int _iVar, int _iItemNumber)
{
	return iAllocListCommon(_iVar, _iItemNumber, sci_tlist);
}

int* iAllocList(int _iVar, int _iItemNumber)
{
	return iAllocListCommon(_iVar, _iItemNumber, sci_list);
}

int* iAllocMList(int _iVar, int _iItemNumber)
{
	return iAllocListCommon(_iVar, _iItemNumber, sci_mlist);
}

int* iAllocHyperMatrix(int _iVar, int _iDims)
{
	return iAllocMList(_iVar, _iDims);
}

int* iAllocListCommon(int _iVar, int _iItemNumber, int _iListType)
{
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iVar));
	int *pRoot			= NULL;

	pRoot = iListAllocListCommon(_iVar, istk(iAddrBase), 0 /*Main Root*/ , _iItemNumber, _iListType);


	if(_iItemNumber == 0)
		*Lstk(Top - Rhs + _iVar + 1) = sadr(iAddrBase + _iItemNumber + 3);

	C2F(intersci).ntypes[Top - Rhs + _iVar - 1]	= '$';
	C2F(intersci).iwhere[Top - Rhs + _iVar - 1]	= *Lstk(_iVar);
	// + 3 : 1 pour le type + 1 pour la taille + 1 pour le premier increment tjs a 1
	C2F(intersci).lad[Top - Rhs + _iVar - 1]	= sadr(iAddrBase + _iItemNumber + 3);

	return pRoot;
}

int* iListAllocListCommon(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber, int _iListType)
{
	int iIndex			= 0;
	int *piChild		= NULL;

	int *piItemPos		= 0;
	if(_iItemPos == 0)
	{
		piChild = _piParentList;
	}
	else
	{
		piChild = iGetAddressFromItemPos(_piParentList, _iItemPos);
	}


	piChild[0]	= _iListType;
	piChild[1]	= _iItemNumber;
	piItemPos	= piChild + 2;

	piItemPos[0]		= 1; //offset of the first variable

	//Ensure that offset are null before add variable in the list
	for(iIndex = 1 ; iIndex <= _iItemNumber ; iIndex++)
		piItemPos[iIndex] = 0;

	return piChild;
}

int* iListAllocList(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber)
{
	return iListAllocListCommon(_iVar, _piParentList, _iItemPos, _iItemNumber, sci_list);
}

int* iListAllocTList(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber)
{
	return iListAllocListCommon(_iVar, _piParentList, _iItemPos, _iItemNumber, sci_tlist);
}

int* iListAllocMList(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber)
{
	return iListAllocListCommon(_iVar, _piParentList, _iItemPos, _iItemNumber, sci_mlist);
}

int* iListAllocHyperMatrix(int _iVar, int* _piParentList, int _iItemPos, int _iDims)
{
	return iListAllocMList(_iVar, _piParentList, _iItemPos, _iDims);
}

int iListAllocMatrixOfDouble(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double **_pdblRealData)
{
	return iListAllocComplexMatrixOfDouble(_iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblRealData, NULL);
}

int iListAllocComplexMatrixOfDouble(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double **_pdblRealData, double **_pdblImgData)
{
	//int iAddrBase		= iadr(*Lstk(Top - Rhs + _iVar));
	int iType			= _piParent[0];
	int iItemNumber		= _piParent[1];
	int *piItemPos		= &_piParent[2];
	int iIndex			= 0;
	int* piItemAddr		= NULL;

	if(iType != sci_list && iType != sci_tlist && iType != sci_mlist)
		return 1;

	if(_iItemPos <= 0 || iItemNumber < _iItemPos)
		return 2;

	/*check if previous variables are already stored*/
	for(iIndex = 0 ; iIndex < _iItemPos ; iIndex++)
	{
		if(piItemPos[iIndex] == 0)
		{
			return 3;
		}
	}

	/*complete Start address of the new variable*/
	piItemAddr = iGetAddressFromItemPos(_piParent, _iItemPos);
	iAllocComplexMatrixOfDoubleToAddress(piItemAddr, _iComplex, _iRows, _iCols, _pdblRealData, _pdblImgData);

	//iAllocComplexMatrixOfDoubleToAddress(iStart, _iComplex, _iRows, _iCols, _pdblRealData, _pdblImgData);

	piItemPos[_iItemPos] = piItemPos[_iItemPos - 1] + 2 + _iRows * _iCols * (_iComplex + 1);

	if(_iItemPos == iItemNumber)
	{
		int* piEnd = (int*) (*_pdblRealData + _iRows * _iCols * (_iComplex + 1));
		vCloseNode(_iVar, _piParent, _iItemPos, piEnd);
	}

	return 0;
}

void vCloseNode(int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd)
{
	//Get Root address;
	int *piRoot = istk(iadr(*Lstk(Top - Rhs + _iVar)));
	int *piItemPos = &_piCurrentNode[2];
	int iItemNumber		= _piCurrentNode[1];

	//check if we are in bigger list.
	if(piRoot == _piCurrentNode)
	{//Main List, just close
		//Close current list
		int iScale = (int)(_piEnd - piRoot);
		int iDoubleSclale = iScale / 2;
		*Lstk(Top - Rhs + _iVar + 1) = *Lstk(Top - Rhs + _iVar) + iDoubleSclale;
	}
	else //Not the main list, try to go up
	{
		/* \o/ */
		int iPos	= 0;
		int *piParentParent = piGetParentNode(piRoot, _piCurrentNode, &iPos);
		int *piOffset = &piParentParent[2];
		int Ch1  = piItemPos[_iItemPos] + 1 + iItemNumber/2 + 1;
		piOffset[iPos+1] = piOffset[iPos] - 1 + Ch1;
		if(piRoot == piParentParent && ( iPos + 1) == iItemNumber)
		{
			//Close Parent list
			int iScale = (int)(_piEnd - piRoot);
			int iDoubleSclale = iScale / 2;
			*Lstk(Top - Rhs + _iVar + 1) = *Lstk(Top - Rhs + _iVar) + iDoubleSclale;
		}
	}
}

int* piGetParentNode(int* _piStart, int* _piToFind, int *_piPos)
{
	if(IsKindOfList(_piStart))
	{
		int iItem	= _piStart[1];
		int iIndex	= 0;
		for(iIndex = 0 ; iIndex < iItem ; iIndex++)
		{
			int *piChild = iGetAddressFromItemPos(_piStart, iIndex + 1);
			if(piChild == _piToFind)
			{
				*_piPos	= iIndex;
				return _piStart;
			}
			else
			{
				int *piTemp = piGetParentNode(piChild, _piToFind, _piPos);
				if(piTemp != 0)
				{
					return piTemp;
				}
			}
		}
	}
	return 0;
}

int IsKindOfList(int* _piNode)
{
	if(_piNode[0] == sci_list || _piNode[0] == sci_tlist || _piNode[0] == sci_mlist)
		return 1;
	else
		return 0;
}

int iListAllocMatrixOfPoly(int _iVar, int* _piParent, int _iItemPos, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData)
{
	return iListAllocComplexMatrixOfPoly(_iVar, _piParent, _iItemPos, 0, _piVarName, _iRows, _iCols, _piPow, _pdblRealData, NULL);
}

/*Reserve space in stack for a matrix of complex polynom*/
int iListAllocComplexMatrixOfPoly(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData, double** _pdblImgData)
{
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iVar));
	int iType			= *istk(iAddrBase);
	int iItemNumber		= *istk(iAddrBase + 1);
	int *piItemPos		= istk(iAddrBase + 2); // size == iItemNumber
	int iStart			= 0;
	int iIndex			= 0;
	int iAddrData		= 0;

	if(iType != sci_list && iType != sci_tlist && iType != sci_mlist)
		return 1;

	if(_iItemPos <= 0 || iItemNumber < _iItemPos)
		return 2;

	/*check if previous variables are already stored*/
	for(iIndex = 0 ; iIndex < _iItemPos ; iIndex++)
	{
		if(piItemPos[iIndex] == 0)
		{
			return 3;
		}
	}

	iStart = iGetAddressFromItemNumber(_iVar, _iItemPos);
	iAllocComplexMatrixOfPolyToAddress(iStart, _iComplex, _piVarName, _iRows, _iCols, _piPow, _pdblRealData, _pdblImgData);

	iAddrData = iAddrBase + (9 + _iRows * _iCols);
	piItemPos[_iItemPos] = piItemPos[_iItemPos - 1] + ((9 + _iRows * _iCols) + !((_iRows * _iCols) % 2)) / 2 + iArraySum(_piPow, 0, _iRows * _iCols) * (_iComplex + 1);
	if(_iItemPos == iItemNumber)
	{
		//*Lstk(Top - Rhs + _iVar + 1) = sadr(iAddrData) + iArraySum(_piPow, 0, _iRows * _iCols) * (_iComplex + 1) * 2;
		int* piEnd = (int*) (*_pdblRealData + iArraySum(_piPow, 0, _iRows * _iCols) * (_iComplex + 1));
		vCloseNode(_iVar, _piParent, _iItemPos, piEnd);
	}
	return 0;
}


int iListAllocString(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int *_piLen, char** _pszData)
{
	int iAddrBase		= iadr(*Lstk(Top - Rhs + _iVar));
	int iType			= *istk(iAddrBase);
	int iItemNumber		= *istk(iAddrBase + 1);
	int *piItemPos		= istk(iAddrBase + 2); // size == iItemNumber
	int iStart			= 0;
	int iIndex			= 0;
	int iAddrData		= 0;

	if(iType != sci_list && iType != sci_tlist && iType != sci_mlist)
		return 1;

	if(_iItemPos <= 0 || iItemNumber < _iItemPos)
		return 2;

	/*check if previous variables are already stored*/
	for(iIndex = 0 ; iIndex < _iItemPos ; iIndex++)
	{
		if(piItemPos[iIndex] == 0)
		{
			return 3;
		}
	}

	iStart = iGetAddressFromItemNumber(_iVar, _iItemPos);
	iAllocMatrixOfStringToAddress(iStart, _iRows, _iCols, _piLen, _pszData);

	iAddrData = iAddrBase + (9 + _iRows * _iCols);
	piItemPos[_iItemPos] = piItemPos[_iItemPos - 1] + ((9 + _iRows * _iCols) + !((_iRows * _iCols) % 2)) / 2 + iArraySum(_piLen, 0, _iRows * _iCols);
	if(_iItemPos == iItemNumber)
		*Lstk(Top - Rhs + _iVar + 1) = sadr(iAddrData) + iArraySum(_piLen, 0, _iRows * _iCols) * 2;

	return 0;
}

/*
GetRhsVarMatrixDouble --
  Returns a pointer on the data of a matrix of double.
Arguments
  number :
  _iPos : index of the Scilab variable
  _iRows : number of rows in the matrix
  _iCols : number of columns in the matrix
  _pdblRealData : pointer to the block of data for real values
*/
int GetRhsVarMatrixDouble(int number, int *_iRows, int *_iCols, double **_pdblRealData)
{
	unsigned long type_len;
	int lr;
	char typex;
	type_len = 1L;
	typex = MATRIX_OF_DOUBLE_DATATYPE[0];
	C2F(getrhsvar)(&number, &typex, _iRows, _iCols, &lr, type_len);
	*_pdblRealData = stk(lr);
	return 0;
}
/*
GetRhsVarMatrixComplex --
  Returns a pointer on the data of a matrix of double.
Arguments
  number :
  _iPos : index of the Scilab variable
  _iRows : number of rows in the matrix
  _iCols : number of columns in the matrix
  _pdblRealData : pointer to the block of data for real values
  _pdblImgData : pointer to the block of data for complex values
*/
int GetRhsVarMatrixComplex(int number, int *_iRows, int *_iCols, double **_pdblRealData, double **_pdblImgData)
{
	unsigned long type_len;
	int lr;
	int lc;
	char typex;
	int it;
	it = 1;
	type_len = 1L;
	typex = MATRIX_OF_DOUBLE_DATATYPE[0];
	C2F(getrhscvar)(&number, &typex, &it, _iRows, _iCols, &lr, &lc, type_len);
	*_pdblRealData = stk(lr);
	*_pdblImgData = stk(lc);
	return 0;
}

int *GetLengthStringMatrixByName(char *name_, int *m, int *n)
{
	int *lenghtMatrix = NULL;
	unsigned long name_len= (unsigned long)strlen(name_);
	int x = 0;
	int y = 0;
	int mn = 0;
	int lp = 0;
	int j = 0;
	
	int iposx = 0, iposy = 0;
	int lengthAtiposxiposy = 0;

	if ( ! C2F(cmatsptr)  (name_, m,n, &iposx, &iposy, &lp, &lengthAtiposxiposy, name_len) )
	{
		*m = -1;
		*n = -1;
		return NULL;
	}

	mn = *m * *n;
	lenghtMatrix = (int*)MALLOC(mn * sizeof(int));
	if (!lenghtMatrix)
	{
		*m = -1;
		*n = -1;
		return NULL;
	}

	j = 0;
	for (x = 1; x <= *m;x++)
	{
		for (y = 1; y <= *n;y++) 
		{
			if ( !C2F(cmatsptr)  (name_, m,n, &x, &y, &lp, &lengthAtiposxiposy, name_len) )
			{
				FREE(lenghtMatrix);
				*m = -1;
				*n = -1;
				return NULL;
			}
			/* scilab string not finished by '\0' */
			/* we add to the length */
			lenghtMatrix[j] = lengthAtiposxiposy + 1;
			j++;
		}
	}
	return lenghtMatrix;
}

/*Nouveau début, FAIRE DU TRI DANS TOUT CE MERDIER TONIO !!!!!!!*/

int iGetDoubleFromPointer(int* _piAddr, int *_piRows, int *_piCols, double** _pdblReal)
{
	return 0;
}

int iGetComplexDoubleFromPointer(int* _piAddr, int *_piRows, int *_piCols, double** _pdblReal, double** _pdblImg)
{
	return 0;
}

int iGetPolyFromPointer(int* _piAddr, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, double** _pdblReal)
{
	return 0;
}

int iGetComplexPolyFromPointer(int* _piAddr, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, double** _pdblReal, double** _pdblImg)
{
	return 0;
}

int iGetBooleanFromPointer(int* _piAddr, int *_piRows, int *_piCols, int** _piBool)
{
	return 0;
}

int iGetSparseFromPointer(int* _piAddr, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, double **_pdblReal)
{
	return 0;
}

int iGetComplexSparseFromPointer(int* _piAddr, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, double **_pdblReal, double **_pdblImg)
{
	return 0;
}

int iGetBooleanSparseFromPointer(int* _piAddr, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, int** _piBool)
{
	return 0;
}

int iGetMatlabSparseFromPointer(int* _piAddr, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, double **_pdblReal)
{
	return 0;
}

int iGetComplexMatlabSparseFromPointer(int* _piAddr, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, double **_pdblReal, double **_pdblImg)
{
	return 0;
}

int iGetIntFromPointer(int* _piAddr, int *_piRows, int *_piCols, int *_piPrecision, int** _piInt)
{
	return 0;
}

int iGetHandleFromPointer(int* _piAddr, int *_piRows, int *_piCols, int** _piHandle)
{
	return 0;
}


