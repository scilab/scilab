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
 */

/*------------------------------------------------------
 * Read and Write inside the Scilab stack
 *------------------------------------------------------*/

#include <string.h>
#include "stack3.h"
#include "stack-c.h"
#include "cvstr.h"
#include "localization.h"
#include "Scierror.h"
extern int C2F(dmcopy)(double *a, integer *na, double *b, integer *nb, integer *m, integer *n);
extern int C2F(stackp)(integer *id, integer *macmod);

/*------------------------------------------------------*/
void *Name2ptr(char *namex);
int Name2where(char *namex);

/* Table of constant values */

static integer cx0 = 0;
static integer cx1 = 1;

/*------------------------------------------------------
 * read a matrix
 *------------------------------------------------------*/

int C2F(readmat)(char *namex,integer *m, integer *n, double *scimat, unsigned long name_len)
{
  C2F(creadmat)(namex, m, n, scimat, name_len);
  return 0;
}

/*----------------------------------------------------------------
 * readmat reads vector/matrix in scilab's internal stack
 * calling sequence
 *     logic=creadmat('matrixname',m,n,scimat)
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
 *                      scimat(6)=Amat(3,2)
 *----------------------------------------------------------------*/

int C2F(creadmat)(char *namex, integer *m, integer *n, double *scimat, unsigned long name_len)
{
    integer l;
    integer id[nsiz];

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

int C2F(creadcmat)(char *namex, integer *m, integer *n, double *scimat, unsigned long name_len)
{
    integer l, ix1;
    integer id[nsiz];

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

/*----------------------------------------------------------------
 * cwritemat writes vector/matrix in scilab's internal stack
 * logic=cwritemat('matrixname'//char(0),m,n,mat)
 * name: character string; name of the scilab variable ( null terMinated)
 * m: number of rows
 * n: number of columns
 * mat: matrix entries stored columnwise in Scilab object
----------------------------------------------------------------*/

int C2F(cwritemat)(char *namex, integer *m, integer *n,  double *mat, unsigned long name_len)
{
  integer   ix1 = *m * *n;
  integer Rhs_k = Rhs , Top_k = Top ;
  integer l4, id[nsiz], lc, lr;

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


/*--------------------------------------------------------------------------*/
/**
* cwritecmat writes vector/matrix in scilab's internal stack
* name: character string; name of the scilab variable ( null terMinated)
* m: number of rows
* n: number of columns
* mat: matrix entries stored columnwise in Scilab object
* for complex number
*/
/*--------------------------------------------------------------------------*/
int C2F(cwritecmat)(char *namex,integer *m, integer*n,double *mat,unsigned long name_len)
{
	integer   ix1 = *m * *n *2; /* real part + imaginary part */
	integer Rhs_k = Rhs , Top_k = Top ;
	integer l4, id[nsiz], lc, lr;
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
  integer Rhs_k = Rhs , Top_k = Top ;
  integer l4, id[nsiz],/* lc, lr,*/ cx0_2=1;

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
 *------------------------------------------------------*/

int C2F(readchain)(char *namex,  integer *itslen, char *chai,  unsigned long name_len, unsigned long chai_len)
{
    C2F(creadchain)(namex, itslen, chai, name_len, chai_len);
    return 0;
}

/*------------------------------------------------------
 *     this routine reads a string in scilab's  memory
 *     and store it into chai
 * !calling sequence
 *     integer       itslen
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

int C2F(creadchain)(char *namex,  integer *itslen,  char *chai,  unsigned long name_len,  unsigned long chai_len)
{
    integer ix1;
    integer m1, n1;
    integer id[nsiz];
    integer lr1;
    integer nlr1;

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
 *     integer       itslen
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


int C2F(creadchains)(char *namex, integer *ir, integer *ic, integer *itslen, char *chai, unsigned long name_len,  unsigned long chai_len)
{
    integer ix1;
    integer m1, n1;
    integer id[nsiz];
    integer lr1;
    integer nlr1;

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

/*----------------------------------------------------------------
 *     cwritemat writes vector/matrix in scilab's internal stack
 *     logic=cwritemat('matrixname'//char(0),m,n,mat)
 *  name: character string; name of the scilab variable ( null terMinated)
 *  m: number of rows
 *  n: number of columns
 *  mat: matrix entries stored columnwise in Scilab object
 *----------------------------------------------------------------*/

int C2F(cwritechain)(char *namex, integer *m, char *chai, unsigned long name_len, unsigned long chai_len)
{
    integer Rhs_k, Top_k;
    integer l4;
    integer id[nsiz], lr;
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

int C2F(matptr)(char *namex, integer *m, integer *n, integer *lp, unsigned long name_len)
{
  C2F(cmatptr)(namex, m, n, lp, name_len);
  return 0;
}

/*----------------------------------------------------------------
 * !purpose
 *     matptr returns the adress of real matrix "name"
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

int C2F(cmatptr)(char *namex, integer *m,integer *n,integer *lp, unsigned long name_len)
{
    integer id[nsiz];
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
 *     cmatcptr returns the adress of complex matrix "name"
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

int C2F(cmatcptr)(char *namex, integer *m, integer *n, integer *lp, unsigned long name_len)
{
    integer id[nsiz];
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
 *     matptr returns the adress of real matrix "name"
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

int C2F(cmatsptr)(char *namex, integer *m, integer *n,integer *ix,integer *j,integer *lp,integer *nlr, unsigned long name_len)
{
    integer id[nsiz];
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
  integer id[nsiz];
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
  integer id[nsiz];
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
int C2F(str2name)(char *namex, integer *id, unsigned long name_len)
{
	integer ix = 0;
	integer lon = 0;

	for (ix = 0 ; ix < (integer) name_len ; ix++ ) 
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
 *     objptr returns the adress of "name"
 *     in scilab's internal stack
 *----------------------------------------------------------------*/

int C2F(objptr)(char *namex, integer *lp, integer *fin, unsigned long name_len)
{
    integer id[nsiz];
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
int C2F(creadbmat)(char *namex, integer *m, integer *n, int *scimat, unsigned long name_len)
{
	integer l = 0;
	integer id[nsiz];
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
int C2F(cwritebmat)(char *namex, integer *m, integer *n,  int *mat, unsigned long name_len)
{
	integer   ix1 = *m * *n;
	integer Rhs_k = Rhs , Top_k = Top ;
	integer l4, id[nsiz], lr;

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
int C2F(cmatbptr)(char *namex, integer *m,integer *n,integer *lp, unsigned long name_len)
{
	integer id[nsiz];
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
	integer m1, n1;
	integer id[nsiz];
	integer lr1;
	integer nlr1;
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
	for(iIndex = 0 ; iIndex < _iEnd ; iIndex++)
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

	Warning		: if _piPow == NULL, this fonction return _piRows and _piCols with good values to allow array of Pow and array of Values.
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
	int iAddrOffset		= 0;
	int iIndex			= 0;

	if(iValType < 0)
	{
		iAddrBase		= iadr(*istk(iAddrBase + 1));
		iValType		= *istk(iAddrBase);
	}
	*_piRows			= *istk(iAddrBase + 1);
	*_piCols			= *istk(iAddrBase + 2);
	*_piVarName			= istk(iAddrBase + 4);
	iAddrOffset			= iAddrBase + 8; //4 Stack header + 4 variable name
	if(iValType != sci_poly)
	{
		//Error bad type of data
		return;
	}
	

	if(_piPow == NULL)
		return;

	/*Get all offest*/
	for(iIndex = 0 ; iIndex < *_piRows * *_piCols; iIndex++)
	{
		_piPow[iIndex] = *istk(iAddrOffset + iIndex + 1) - *istk(iAddrOffset + iIndex );
	}

	*_piReal	= sadr(iAddrOffset + 1 + *_piRows * *_piCols);
	if(_piImg != NULL && iIsComplex(_iVarNum))
		*_piImg		= sadr(iAddrOffset + 1 + *_piRows * *_piCols) + iArraySum(_piPow, 0, *_piRows * *_piCols);

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
	int iAddrColByRow	= 0;
	int iAddElemByRow	= 0;
	int iAddrRealData	= 0;
	int iAddrImgData	= 0;
	int iIndex			= 0;
	if(iValType < 0)
	{
		iAddrBase		= iadr(*istk(iAddrBase + 1));
		iValType		= *istk(iAddrBase);
	}

	*_piRows			= *istk(iAddrBase + 1);
	*_piCols			= *istk(iAddrBase + 2);
	*_piTotalElem		= *istk(iAddrBase + 4);

	//if ptr are NULL, juste return the matrix size
	if(_piElemByRow == NULL || _piColByRow == NULL)
		return;

	iAddElemByRow		= iAddrBase + 5;
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
	if(_piImg != NULL && iIsComplex(_iVarNum))
		*_piImg	 = sadr(iAddrRealData) + *_piTotalElem;

	C2F(intersci).ntypes[_iVarNum - 1] = '$' ;
	C2F(intersci).iwhere[_iVarNum - 1] = *Lstk(_iVarNum);
	C2F(intersci).lad[_iVarNum - 1] = *_piReal;
}

/*CreateCVarFromPtr(2, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);*/
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

	*istk(iAddrBase)	= 2;
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

	*istk(iAddrBase)	= 5;
	*istk(iAddrBase + 1)= _iRows;
	*istk(iAddrBase + 2)= _iCols;
	*istk(iAddrBase + 3)= 0; // Non complex values
	*istk(iAddrBase + 4)= _iTotalElem;

	iAddElemByRow		= iAddrBase + 5;
	iAddrColByRow		= iAddElemByRow + _iRows;
	iAddrRealData		= iAddrColByRow + _iTotalElem;
	iAddrImgData		= iAddrColByRow + 2 * _iTotalElem;

	for(iIndex = 0 ; iIndex < _iTotalElem ; iIndex++)
	{
		int iTemp		= 0;
		*istk(iAddElemByRow + iIndex) = _piElemByRow[iIndex];
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
