/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Inria 
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
/*------------------------------------------------------
 * Read and Write inside the Scilab stack 
 *------------------------------------------------------*/

#include <string.h>
#include "../stack-c.h"

extern int C2F(dmcopy)  __PARAMS((double *a, integer *na, double *b, integer *nb, integer *m, integer *n));
extern int C2F(stackg)  __PARAMS((integer *id));
extern int C2F(stackp)  __PARAMS((integer *id, integer *macmod));

/* Table of constant values */

static integer cx0 = 0;
static integer cx1 = 1;

/*------------------------------------------------------
 * read a matrix 
 *------------------------------------------------------*/

int C2F(readmat)(namex, m, n, scimat, name_len)
     char *namex;
     integer *m, *n;
     double *scimat;
     unsigned long name_len;
{
    int j;
    j = C2F(creadmat)(namex, m, n, scimat, name_len);
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

int C2F(creadmat)(namex, m, n, scimat, name_len)
     char *namex;
     integer *m, *n;
     double *scimat;
     unsigned long name_len;
{
    integer l;
    integer id[nsiz];

    C2F(str2name)(namex, id, name_len);
    /* read   : from scilab stack -> fortran variable */
    Fin = -1;
    C2F(stackg)(id);
    if (Err > 0) return FALSE_ ; 
    if (Fin == 0) {
      Scierror(4,"Undefined variable %s\r\n",get_fname(namex,name_len));
      return FALSE_;
    }
    if ( *Infstk(Fin ) == 2)  Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    /* get matrix data pointer */
    if (! C2F(getrmat)("creadmat", &Fin, &Fin, m, n, &l, 8L)) 	return FALSE_;

    C2F(dmcopy)(stk(l ), m, scimat, m, m, n);

    return TRUE_;
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

int C2F(creadcmat)(namex, m, n, scimat, name_len)
     char *namex;
     integer *m, *n;
     double *scimat;
     unsigned long name_len;
{
    integer l, ix1;
    integer id[nsiz];

    C2F(str2name)(namex, id, name_len);
    /* read   : from scilab stack -> fortran variable */
    Fin = -1;
    C2F(stackg)(id);
    if (Err > 0) return FALSE_ ; 
    if (Fin == 0) {
      Scierror(4,"Undefined variable %s\r\n",get_fname(namex,name_len));
      return FALSE_;
    }
    if ( *Infstk(Fin ) == 2)  Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    /* get matrix data pointer */
    if (! C2F(getcmat)("creadcmat", &Fin, &Fin, m, n, &l, 8L)) 	return FALSE_;
    ix1 = *m * *n;
    C2F(dmcopy)(stk(l ), m, scimat, m, m, n);
    C2F(dmcopy)(stk(l+ix1 ), m, scimat+ix1, m, m, n);

    return TRUE_;
}

/*----------------------------------------------------------------
 * cwritemat writes vector/matrix in scilab's internal stack
 * logic=cwritemat('matrixname'//char(0),m,n,mat)
 * name: character string; name of the scilab variable ( null terMinated) 
 * m: number of rows 
 *  n: number of columns 
 *  mat: matrix entries stored columnwise in Scilab object 
 *----------------------------------------------------------------*/

int C2F(cwritemat)(namex, m, n, mat, name_len)
     char *namex;
     integer *m, *n;
     double *mat;
     unsigned long name_len;
{
  integer   ix1 = *m * *n;
  integer Rhs_k = Rhs , Top_k = Top ;
  integer l4, id[nsiz], lc, lr;
  
  C2F(str2name)(namex, id, name_len);
  /* jpc april 2002 */ 
  /* ++Top; */
  Top = Top + Nbvars + 1; 
  if (! C2F(cremat)("cwritemat", &Top, &cx0, m, n, &lr, &lc, 9L)) return  FALSE_;
  C2F(dcopy)(&ix1, mat, &cx1, stk(lr ), &cx1);
  Rhs = 0;
  l4 = C2F(iop).lct[3];
  C2F(iop).lct[3] = -1;
  C2F(stackp)(id, &cx0);
  C2F(iop).lct[3] = l4;
  Top = Top_k;
  Rhs = Rhs_k;
  if (Err > 0)  return FALSE_;
  return TRUE_;
} 

int C2F(putvar)(number, namex, name_len)
     /* Put variable number into Scilab internal stack with name "namex" */
     char *namex;
     int  *number;
     unsigned long name_len;
{
  integer Rhs_k = Rhs , Top_k = Top ;
  integer l4, id[nsiz],/* lc, lr,*/ cx0=1;
  
  C2F(str2name)(namex, id, name_len);
  Top = *number + Top -Rhs;
  Rhs = 0;
  l4 = C2F(iop).lct[3];
  C2F(iop).lct[3] = -1;
  C2F(stackp)(id, &cx0);
  C2F(iop).lct[3] = l4;
  Top = Top_k;
  Rhs = Rhs_k;
  if (Err > 0)  return FALSE_;
  return TRUE_;
} 

/*------------------------------------------------------
 *     see creadchain 
 *------------------------------------------------------*/

int C2F(readchain)(namex, itslen, chai, name_len, chai_len)
     char *namex;
     integer *itslen;
     char *chai;
     unsigned long name_len, chai_len;
{
    int j;
    j = C2F(creadchain)(namex, itslen, chai, name_len, chai_len);
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

int C2F(creadchain)(namex, itslen, chai, name_len, chai_len)
     char *namex;
     integer *itslen;
     char *chai;
     unsigned long name_len;
     unsigned long chai_len;
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
    if (Err > 0) return FALSE_ ;
    if (Fin == 0) {
      Scierror(4,"Undefined variable %s\r\n",get_fname(namex,name_len));
      return FALSE_ ;
    }
    if (*Infstk(Fin ) == 2) {
	Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    }
    if (! C2F(getsmat)("creadchain", &Fin, &Fin, &m1, &n1, &cx1, &cx1, &lr1, &nlr1, 10L)) {
	return FALSE_;
    }
    if (m1 * n1 != 1) {
      Scierror(999,"creadchain: argument must be a string\r\n");
      return FALSE_ ;
    }

    ix1 = *itslen - 1;
    *itslen = Min(ix1,nlr1);
    C2F(cvstr)(itslen, istk(lr1 ), chai, &cx1, chai_len);
    chai[*itslen] = '\0';
    return TRUE_ ;
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


int C2F(creadchains)(namex, ir, ic, itslen, chai, name_len, chai_len)
     char *namex;
     integer *ir, *ic, *itslen;
     char *chai;
     unsigned long name_len;
     unsigned long chai_len;
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
    if (Err > 0) return FALSE_ ;

    if (Fin == 0) {
      Scierror(4,"Undefined variable %s\r\n",get_fname(namex,name_len));
      return FALSE_ ;
    }

    if (*Infstk(Fin ) == 2) {
	Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    }
    if (*ir == -1 && *ic == -1) {
	if (! C2F(getsmat)("creadchain", &Fin, &Fin, ir, ic, &cx1, &cx1, &lr1, &nlr1, 10L)) 
	  return FALSE_;
	else 
	  return TRUE_ ; 
    } else {
	if (! C2F(getsmat)("creadchain", &Fin, &Fin, &m1, &n1, ir, ic, &lr1, &nlr1, 10L)) {
	  return FALSE_;
	}
    }
    ix1 = *itslen - 1;
    *itslen = Min(ix1,nlr1);
    C2F(cvstr)(itslen, istk(lr1 ), chai, &cx1, chai_len);
    chai[*itslen]='\0';
    return TRUE_;
}

/*----------------------------------------------------------------
 *     cwritemat writes vector/matrix in scilab's internal stack 
 *     logic=cwritemat('matrixname'//char(0),m,n,mat) 
 *  name: character string; name of the scilab variable ( null terMinated) 
 *  m: number of rows 
 *  n: number of columns 
 *  mat: matrix entries stored columnwise in Scilab object 
 *----------------------------------------------------------------*/

int C2F(cwritechain)(namex, m, chai, name_len, chai_len)
     char *namex;
     integer *m;
     char *chai;
     unsigned long name_len,  chai_len;
{
    integer Rhs_k, Top_k;
    integer l4;
    integer id[nsiz], lr;
    C2F(str2name)(namex, id, name_len);
    Top_k = Top;
    /* jpc april 2002 */ 
    /* ++Top; */
    Top = Top + Nbvars + 1; 
    if (! C2F(cresmat2)("cwritechain", &Top, m, &lr, 11L)) {
	return FALSE_;
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
    if (Err > 0)  return FALSE_;
    return TRUE_ ;
}

/*----------------------------------------------------------------
 *     see cmatptr 
 *----------------------------------------------------------------*/

int C2F(matptr)(namex, m, n, lp, name_len)
     char *namex;
     integer *m, *n, *lp;
     unsigned long name_len;
{
    int ix;
    ix = C2F(cmatptr)(namex, m, n, lp, name_len);
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

int C2F(cmatptr)(namex, m, n, lp, name_len)
     char *namex;
     integer *m, *n, *lp;
     unsigned long name_len;
{
    integer id[nsiz];
    C2F(str2name)(namex, id, name_len);
    /* get the position in fin */
    Fin = -1;
    C2F(stackg)(id);
    if (Fin == 0) {
      Scierror(4,"Undefined variable %s\r\n",get_fname(namex,name_len));
      *m = -1;
      *n = -1;
      return FALSE_;
    }
    /* get data */
    if (*Infstk(Fin ) == 2) {
	Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    }
    if (! C2F(getrmat)("creadmat", &Fin, &Fin, m, n, lp, 8L)) {
	return FALSE_;
    }
    return TRUE_ ;
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

int C2F(cmatcptr)(namex, m, n, lp, name_len)
     char *namex;
     integer *m, *n, *lp;
     unsigned long name_len;
{
    integer id[nsiz];
    C2F(str2name)(namex, id, name_len);
    /* get the position in fin */
    Fin = -1;
    C2F(stackg)(id);
    if (Fin == 0) {
      Scierror(4,"Undefined variable %s\r\n",get_fname(namex,name_len));
      *m = -1;
      *n = -1;
      return FALSE_;
    }
    /* get data */
    if (*Infstk(Fin ) == 2) {
	Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    }
    if (! C2F(getcmat)("creadmat", &Fin, &Fin, m, n, lp, 8L)) {
	return FALSE_;
    }
    return TRUE_ ;
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

int C2F(cmatsptr)(namex, m, n, ix, j, lp, nlr, name_len)
     char *namex;
     integer *m, *n, *lp, *ix, *j, *nlr;
     unsigned long name_len;
{
    integer id[nsiz];
    C2F(str2name)(namex, id, name_len);
    /* get the position in fin */
    Fin = -1;
    C2F(stackg)(id);
    if (Fin == 0) {
      Scierror(4,"Undefined variable %s\r\n",get_fname(namex,name_len));
      *m = -1;
      *n = -1;
      return FALSE_;
    }
    /* get data */
    if (*Infstk(Fin ) == 2) {
	Fin = *istk(iadr(*Lstk(Fin )) + 1 +1);
    }
    if (! C2F(getsmat)("creadmat", &Fin, &Fin, m, n, ix, j, lp, nlr, 8L)) {
	return FALSE_;
    }
    return TRUE_ ;
}

void *Name2ptr(namex)
     char *namex;
     /*  Returns a pointer to the Scilab variable with name namex
         Usage:   int *header;
                  header = (int *) Name2ptr("pipo");
                  header[0], header[1], etc contains header info 
                  about Scilab variable "pipo"   */
{
  int l1; int *loci;
  integer id[nsiz];
  C2F(str2name)(namex, id, strlen(namex));
  /* get the position in fin */
  Fin = -1;
  C2F(stackg)(id);
  if (Fin == 0) {
    Scierror(4,"Undefined variable %s\r\n",get_fname(namex,strlen(namex)));
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

int Name2where(namex)
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
     char *namex;
     /*     unsigned long name_len; */
{
  int loci;
  integer id[nsiz];
  C2F(str2name)(namex, id, strlen(namex));
  /* get the position in fin */
  Fin = -1;
  C2F(stackg)(id);
  if (Fin == 0) {
    Scierror(4,"Undefined variable %s\r\n",get_fname(namex,strlen(namex)));
    return 0;
  }
  loci = *Lstk(Fin);
  return loci;
}

/*----------------------------------------------------------------
 *     string conversion to Scilab ID 
 *     Warning : the character name is null terMinated 
 *             and len(name) is not used 
 *             since it can be wrong (ex when name is transmited 
 *             by fort (intfort : function )
 *----------------------------------------------------------------*/

int C2F(str2name)(namex, id, name_len)
     char *namex;
     integer *id;
     unsigned long name_len;
{
    integer ix;
    integer lon;
    lon = 0;
    for (ix = 0 ; ix < (integer) name_len ; ix++ ) {
      if ( namex[ix] == '\0') break;
      ++lon;
    }
    C2F(cvname)(id, namex, &cx0, lon);
    return 0;
} 

/*----------------------------------------------------------------
 *     objptr returns the adress of "name" 
 *     in scilab's internal stack 
 *----------------------------------------------------------------*/

int C2F(objptr)(namex, lp, fin, name_len)
     char *namex;
     integer *lp; integer *fin;
     unsigned long name_len;
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
      return FALSE_;
    }
    *fin = Fin;
    *lp = *Lstk(Fin );
    if (*Infstk(Fin ) == 2) {
	*lp = *Lstk(*istk(iadr(*lp) + 1 +1) );
    }
    return  TRUE_;
}




