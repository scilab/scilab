/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 1998-2008 - INRIA - Allan CORNET
 * Copyright (C) 1998-2008 - INRIA - Serge STEER
 * Copyright (C) 1998-2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 1998-2008 - INRIA - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 */
/*------------------------------------------------------------------------
 *    Scilab Memory Management library (Stack API)
 --------------------------------------------------------------------------*/

/*---------------------------------------------------------------------
 * Interface Library:   ilib
 *---------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>

#include "MALLOC.h"
#include "stack-c.h"
#include "sciprint.h"
#include "stack2.h"
#include "cvstr.h"
#include "parse.h"
#include "men_Sutils.h"
#include "int2db.h"
#include "rea2b.h"
#include "Scierror.h"
#include "localization.h"
#include "callinterf.h"
#include "CallScilab.h"
#include "recursionFunction.h"
#include "doublecomplex.h"

#ifdef _MSC_VER
#define abs(x) ((x) >= 0 ? (x) : -(x)) /* pour abs  C2F(mvfromto) line 2689 */
#endif
/* Table of constant values */
static int cx1 = 1;
static int cx0 = 0;

static char *Get_Iname(void);
static int C2F(mvfromto)(int *itopl,int *);

static int rhs_opt_find(char *name,rhs_opts opts[]);
static void rhs_opt_print_names(rhs_opts opts[]);
extern int C2F(isbyref)(int * fun);


/*------------------------------------------------*/
void strcpy_tws(char *str1,char *str2, int len);
int C2F(copyvarfromsciptr)(int lw, int n,int l);
static int intersci_push(void);
static void intersci_pop(void);


static void ConvertData(unsigned char *type, int size,int l);
/*------------------------------------------------
 * checkrhs: checks right hand side arguments
 *-----------------------------------------------*/

int C2F(checkrhs)(char *fname, int *iMin, int *iMax, unsigned long  fname_len)
{
  /*
   * store the name in recu array, fname can be a non null terminated char array
   * Get_Iname() can be used in other function to get the interface name
   */

  C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], fname, &cx0, fname_len);

  if ( Rhs < *iMin || Rhs > *iMax)
    {
		if (*iMin==*iMax) { /* No optional argument */
			Scierror(77,_("%s: Wrong number of input argument(s): %d expected.\n"),get_fname(fname,fname_len), *iMax);
		}else{
			Scierror(77,_("%s: Wrong number of input argument(s): %d to %d expected.\n"),get_fname(fname,fname_len), *iMin, *iMax);
		}
      return FALSE;
    }
  return TRUE;
}

/*------------------------------------------------
 * checkrhs: checks left hand side arguments
 *-----------------------------------------------*/

int C2F(checklhs)(char *fname, int *iMin, int *iMax, unsigned long  fname_len)
{
  if ( Lhs < *iMin || Lhs > *iMax)
    {
		if (*iMin == *iMax) { /* No optional argument */
			Scierror(78,_("%s: Wrong number of output argument(s): %d expected.\n"),get_fname(fname,fname_len), *iMax);
		}else{
			Scierror(78,_("%s: Wrong number of output argument(s): %d to %d expected.\n"),get_fname(fname,fname_len), *iMin, *iMax);
		}
      return FALSE;
    }
  return TRUE;
}

/*---------------------------------------------------------------------
 * isopt:
 * returns the status of the variable number k
 * if its an optional variable f(x=...)
 * returns .true. and variable name in namex
 * namex must have a size of nlgh + 1
 *---------------------------------------------------------------------*/

int C2F(isopt)(int *k, char *namex,unsigned long name_len)
{
  int i1 =  *k + Top - Rhs;
  if ( C2F(isoptlw)(&Top, &i1, namex, name_len) == FALSE) return FALSE ;
  /* add a '\0' at the end of the string removing trailing blanks */
  for ( i1 = nlgh-1 ; i1 >=0 ; i1--) { if ( namex[i1] != ' ') break ;}
  namex[i1+1]='\0';
  return TRUE;
}

/*---------------------------------------
 * isoptlw :
 * returns the status of the variable at position lw in the stack
 * if its an optional variable f(x=...)
 * returns .true. and variable name in namex
 *--------------------------------------- */

int C2F(isoptlw)(int *topk,int  *lw, char *namex, unsigned long name_len)
{
  if (*Infstk(*lw ) != 1) return FALSE ;
  C2F(cvname)(&C2F(vstk).idstk[(*lw) * nsiz - nsiz], namex, &cx1, name_len);
  return TRUE;
}

/*---------------------------------------
 * firstopt :
 * return the position of the first optionnal argument
 * given as xx=val in the calling sequence.
 * If no such argument it returns Rhs+1.
 *--------------------------------------- */
int C2F(firstopt)()

{
  int k;
  for (k = 1; k <= Rhs ; ++k)
    if (*Infstk(k + Top - Rhs )==1)
      return k;
  return(Rhs+1);
}


/*---------------------------------------
 * findopt :
 * checks if option str has been passed.
 * If yes returns the position of the variable
 * If no  returns 0
 *--------------------------------------- */

int C2F(findopt)(char * str,rhs_opts opts[])
{
  int i;
  i = rhs_opt_find(str,opts);
  if ( i>=0 )
    if (opts[i].position>0)
      return opts[i].position;

  return 0;
}


/*---------------------------------------
 * numopt :
 *  returns the number of optional variables
 *  given as xx=val in the caling sequence
 *  top must have a correct value when using this function
 *--------------------------------------- */

int C2F(numopt)()
{
  int k, ret=0;
  for (k = 1; k <= Rhs ; ++k)
    if ( *Infstk(k + Top - Rhs) == 1 ) ret++;
  return ret;
}

/*---------------------------------------------------------------------
 * vartype:
 *   type of variable number number in the stack
 *---------------------------------------------------------------------*/

int C2F(vartype)(int *number)
{
  int ix1=  *number + Top - Rhs;
  return C2F(gettype)(&ix1);
}

/*------------------------------------------------
 * gettype:
 *    returns the type of object at position lw in the stack
 *------------------------------------------------*/

int C2F(gettype)(int *lw)
{
  int il;
  il = iadr(*Lstk(*lw ));
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  return *istk(il);
}


/*------------------------------------------------
 * overloadtype:
 *    set mechanism to overloaded function fname if object type
 *    does not fit given type
 *------------------------------------------------*/

static int overloadtype(int *lw,char *fname,unsigned char *typ)
{
  int il=0;
  int ityp=0;
  il = iadr(*Lstk(*lw ));
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  switch (*typ) {
  case 'c' : /* string */
  case 'S' : /* string Matrix */
    ityp=sci_strings;
    break;
  case 'd' :  case 'i' :  case 'r' :  case 'z' :   /* numeric */
    ityp=sci_matrix;
    break ;
  case 'b' : /* boolean */
    ityp=sci_boolean;
    break;
  case 'h' : /* handle */
    ityp=sci_handles;
    break;
  case 'l' : /* list */
    ityp=sci_list;
    break;
  case 't' : /* tlist */
    ityp=sci_tlist;
    break;
  case 'm' : /* mlist */
    ityp=sci_mlist;
    break;
  case 'f' : /* external */
    ityp=sci_c_function;
    break;
  case 'p' : /* pointer */
    ityp=sci_lufact_pointer;
    break;
  case 's' : /* sparse */
    ityp= sci_sparse;
    break;
  case 'I' : /* int matrix */
    ityp=sci_ints;
    break;
  case 'x' : /* polynomial matrix */
	  ityp=sci_poly;
    break;

  }
  if (*istk(il ) != ityp) {
    return C2F(overload)(lw,fname,(unsigned long)strlen(fname));
  }
  return 1;
}


/*------------------------------------------------
 * overload
 *    set mechanism to overloaded function fname for object lw
 *------------------------------------------------*/

int C2F(overload)(int *lw,char *fname,unsigned long l)
{
  C2F(putfunnam)(fname,lw,l);
  C2F(com).fun=-1;
  return 0;
}



/*------------------------------------------------
 * ogettype : unused
 *------------------------------------------------*/
int C2F(ogettype)(int *lw)
{
  return  *istk(iadr(*Lstk(*lw )) );
}


/*----------------------------------------------------
 * Optional arguments f(....., arg =val,...)
 *          in interfaces
 * function get_optionals : example is provided in
 *    examples/addinter-examples/intex2c.c
 *----------------------------------------------------*/


int get_optionals(char *fname ,rhs_opts opts[])
{
  int k,i=0;
  char name[nlgh+1];
  int nopt = NumOpt(); /* optional arguments on the stack */

  /* reset first field since opts is declared static in calling function */
  /* this could be avoided with ansi compilers by removing static in the
   * opts declaration */

  while ( opts[i].name != NULL )
    {
      opts[i].position = -1;
      i++;
    }

  /* Walking through last arguments */

  for ( k = Rhs - nopt + 1; k <= Rhs ;k++)
    {
      if ( IsOpt(k,name) == 0  )
	{
	  Scierror(999,_("%s: Optional arguments name=val must be at the end.\n"),fname);
	  return 0;
	}
      else
	{
	  int isopt = rhs_opt_find(name,opts);
	  if ( isopt >= 0 )
	    {
	      rhs_opts *ro = &opts[isopt];
	      ro->position = k;
	      if (ro->type[0] != '?')
		GetRhsVar(ro->position, ro->type,&ro->m,&ro->n,&ro->l);
	    }
	  else
	    {
	      sciprint(_("%s: Unrecognized optional arguments %s.\n"),fname,name);
	      rhs_opt_print_names(opts) ;
	      Error(999);
	      return(0);
	    }
	}
    }
  return 1;
}

/* Is name in opts */

int rhs_opt_find(char *name,rhs_opts opts[])
{
  int rep=-1,i=0;
  while ( opts[i].name != NULL )
	  {
		  int cmp;
		  /* name is terminated by white space and we want to ignore them */
		  if ( (cmp=strcmp(name,opts[i].name)) == 0 )
			  {
				  rep = i ; break;
			  }
		  else if ( cmp < 0 )
			  {
				  break;
			  }
		  else
			  {
				  i++;
			  }
	  }
  return rep;
}

void rhs_opt_print_names(rhs_opts opts[])
      /* array of optinal names (in alphabetical order)
		       * the array is null terminated */
{
  int i=0;
  if ( opts[i].name == NULL )
    {
      sciprint(_("Optional argument list is empty.\n"));
      return;
    }
  sciprint(_("Optional arguments list: \n"));
  while ( opts[i+1].name != NULL )
    {
      sciprint("%s, ",opts[i].name);
      i++;
    }
  sciprint(_("and %s.\n"),opts[i].name);
}

/*---------------------------------------------------------------------
 * isref :
 *   checks if variable number lw is on the stack
 *   or is just a reference to a variable on the stack
 *---------------------------------------------------------------------*/

int IsRef(int number) {
  return C2F(isref)(&number);
}

int C2F(isref)(int *number)
{
  int il,lw;
  lw = *number + Top - Rhs;
  if ( *number > Rhs) {
    Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),"isref","isref");
    return FALSE;
  }
  il = iadr(*Lstk(lw));
  if ( *istk(il) < 0)
    return TRUE ;
  else
    return FALSE ;
}

/*---------------------------------------------------------------------
 *     create a variable number lw in the stack of type
 *     type and size m,n
 *     the argument must be of type type ('c','d','r','i','l','b')
 *     return values m,n,lr
 *     c : string  (m-> number of characters and n->1)
 *     d,r,i : matrix of double,float or integer
 *     b : boolean matrix
 *     l : a list  (m-> number of elements and n->1)
 *         for each element of the list an other function
 *         must be used to <<get>> them
 *     side effects : arguments in the common intersci are modified
 *     see examples in addinter-examples
 *---------------------------------------------------------------------*/

int C2F(createvar)(int *lw,char *typex,int *m,int *n,int  *lr,unsigned long type_len)
{
  int ix1, ix, it=0, lw1, lcs, IT;
  unsigned char Type = *typex;
  char *fname = Get_Iname();
  if (*lw > intersiz) {
    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"createvar");
    return FALSE ;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  if (*lw < 0) {
    Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"createvar");
    return FALSE ;
  }
  switch (Type )
    {
    case 'c' :
      ix1 = *m * *n;
      if (! C2F(cresmat2)(fname, &lw1, &ix1, lr, nlgh)) return FALSE;
      *lr = cadr(*lr);
	  // Fill the string with spaces
      for (ix = 0; ix < (*m)*(*n) ; ++ix) *cstk(*lr+ix)= ' ';
      *cstk(*lr+ (*m)*(*n) )= '\0';
      C2F(intersci).ntypes[*lw - 1] = Type;
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      break;
    case 'd' :
      if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh))    return FALSE;
      C2F(intersci).ntypes[*lw - 1] = Type;
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      break;
    case 'z' :
      IT = 1;
      if (!(*Lstk(lw1) % 2) ) *Lstk(lw1) = *Lstk(lw1)+1;
      if (! C2F(cremat)(fname, &lw1, &IT, m, n, lr, &lcs, nlgh))    return FALSE;
      C2F(intersci).ntypes[*lw - 1] = Type;
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      *lr = sadr(*lr);
      break;
    case 'l' :
      C2F(crelist)(&lw1, m, lr);
      C2F(intersci).ntypes[*lw - 1] = '$';
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      break;
    case 't' :
      C2F(cretlist)(&lw1, m, lr);
      C2F(intersci).ntypes[*lw - 1] = '$';
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      break;
    case 'm' :
      C2F(cremlist)(&lw1, m, lr);
      C2F(intersci).ntypes[*lw - 1] = '$';
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      break;
    case 'r' :
      if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh)) return FALSE;
      *lr = iadr(*lr);
      C2F(intersci).ntypes[*lw - 1] = Type;
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      break ;
    case 'i' :
      if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh)) return FALSE;
      *lr = iadr(*lr) ;
      C2F(intersci).ntypes[*lw - 1] = Type;
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      break ;
    case 'b' :
      if (! C2F(crebmat)(fname, &lw1, m, n, lr, nlgh))  return FALSE;
      C2F(intersci).ntypes[*lw - 1] = Type;
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      break;
    case 'p' :
      if (! C2F(crepointer)(fname, &lw1, lr, nlgh))    return FALSE;
      C2F(intersci).ntypes[*lw - 1] = '$';
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      break;
    case 'I' :
      it = *lr ; /* on entry lr gives the int type */
      if (! C2F(creimat)(fname, &lw1, &it, m, n, lr, nlgh))    return FALSE;
      C2F(intersci).ntypes[*lw - 1] = '$';
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      break;
    case 'h' :
      if (! C2F(crehmat)(fname, &lw1, m, n, lr, nlgh))    return FALSE;
      C2F(intersci).ntypes[*lw - 1] = Type;
      C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
      C2F(intersci).lad[*lw - 1] = *lr;
      break;
	  // TODO : add a default case
    }
  return TRUE;
}

/*---------------------------------------------------------------------
 *     create a variable number lw in the stack of type
 *     type and size m,n
 *     the argument must be of type type ('d','r','i')
 *     return values m,n,lr
 *     d,r,i : matrix of double,float or integer
 *     side effects : arguments in the common intersci are modified
 *     see examples in addinter-examples
 *     Like createvar but for complex matrices
 *---------------------------------------------------------------------*/

int C2F(createcvar)(int *lw, char *typex,int *it,int *m,int *n,int *lr,int *lc,unsigned long type_len)
{
  unsigned char Type = *typex ;
  int lw1;
  char *fname = Get_Iname();
  if (*lw > intersiz) {
    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"createcvar");
    return FALSE;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  if (*lw < 0) {
  Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"createcvar");
    return FALSE;
  }
  switch ( Type )  {
  case 'd' :
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, lc, nlgh))  return FALSE;
    C2F(intersci).ntypes[*lw - 1] = Type;
    C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
    C2F(intersci).lad[*lw - 1] = *lr;
    break;
  case 'r' :
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, lc, nlgh))  return FALSE;
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    C2F(intersci).ntypes[*lw - 1] = Type;
    C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
    C2F(intersci).lad[*lw - 1] = *lr;
    break;
  case 'i' :
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, lc, nlgh))  return FALSE;
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    C2F(intersci).ntypes[*lw - 1] = Type;
    C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
    C2F(intersci).lad[*lw - 1] = *lr;
    break;
  }
  return TRUE;
}

/*---------------------------------------------------------------------
 *     create a variable number lw on the stack of type
 *     list with nel elements
 *---------------------------------------------------------------------*/

int C2F(createlist)(int *lw,int *nel)
{
  char *fname = Get_Iname();
  int lr, lw1;
  if (*lw > intersiz) {
	Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"createlist");
    return FALSE;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  if (*lw < 0) {
	Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"createlist");
    return FALSE;
  }
  C2F(crelist)(&lw1, nel, &lr);
  C2F(intersci).ntypes[*lw - 1] = '$';
  C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
  C2F(intersci).lad[*lw - 1] = lr;
  return TRUE;
}

/*---------------------------------------------------------------------
 *     create a variable number lw on the stack of type
 *     type and size m,n
 *     the argument must be of type type ('c','d','r','i','b')
 *     return values m,n,lr,lar
 *     lar is also an input value
 *     if lar != -1 var is filled with data stored at lar
 *---------------------------------------------------------------------*/

int C2F(createvarfrom)(int *lw,char *typex,int *m,int *n,int *lr,int *lar,unsigned long type_len)
{
  int M=*m,N=*n,MN=M*N;
  unsigned char Type = *typex;
  int inc=1;
  int it=0, lw1, lcs;
  char *fname = Get_Iname();
  if (*lw > intersiz) {
    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"createvarfrom");
    return FALSE;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  if (*lw < 0) {
  	Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"createvarfrom");
    return FALSE;
  }
  switch ( Type ) {
  case 'c' :
    if (! C2F(cresmat2)(fname, &lw1, &MN, lr, nlgh)) return FALSE;
    if (*lar != -1)  C2F(cvstr1)(&MN, istk(*lr), cstk(*lar), &cx0,  MN + 1);
    *lar = *lr;
    *lr = cadr(*lr);
    M=MN; N= 1;
    break;
  case 'd' :
    if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh))  return FALSE;
    if (*lar != -1)  C2F(dcopy)(&MN, stk(*lar), &cx1, stk(*lr), &cx1);
    *lar = *lr;
    break;
  case 'r' :
    if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh)) return FALSE;
    if (*lar != -1)   C2F(rea2db)(&MN, sstk(*lar), &cx1, stk(*lr), & cx1);
    *lar = *lr;
    *lr = iadr(*lr);
    break;
  case 'i' :
    if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh)) return FALSE;
    if (*lar != -1) C2F(int2db)(&MN,istk(*lar), &cx1, stk(*lr), &cx1);
    *lar = *lr;
    *lr = iadr(*lr);
    break;
  case 'b' :
    if (! C2F(crebmat)(fname, &lw1, m, n, lr, nlgh)) return FALSE;
    if (*lar != -1) C2F(icopy)(&MN, istk(*lar), &cx1, istk(*lr), &cx1);
    *lar = *lr;
    break;
  case 'I' :
    it = *lr;
    if (! C2F(creimat)(fname, &lw1, &it, m, n, lr,  nlgh))  return FALSE;
    if (*lar != -1)
      C2F(tpconv)(&it,&it,&MN,istk(*lar), &inc,istk(*lr), &inc);
    *lar = *lr;
    break;
  case 'p' :
    MN=1;
    if (! C2F(crepointer)(fname, &lw1, lr, nlgh))    return FALSE;
    if (*lar != -1)  C2F(dcopy)(&MN, stk(*lar), &cx1, stk(*lr), &cx1);
    *lar = *lr;
    break;
  case 'h' :
    if (! C2F(crehmat)(fname, &lw1, m, n, lr, nlgh))  return FALSE;
    if (*lar != -1)  C2F(dcopy)(&MN, stk(*lar), &cx1, stk(*lr), &cx1);
    *lar = *lr;
    break;
  }
  C2F(intersci).ntypes[*lw - 1] = '$';
  C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
  C2F(intersci).lad[*lw - 1] = *lr;
  return TRUE;
}


/*---------------------------------------------------------------------
 *     create a variable number lw on the stack of type
 *     type and size m,n
 *     the argument must be of type type ('d','r','i')
 *     return values it,m,n,lr,lc,lar,lac
 *     lar is also an input value
 *     if lar != -1 var is filled with data stored at lar
 *     idem for lac
 *     ==> like createvarfrom for complex matrices
*---------------------------------------------------------------------*/

int C2F(createcvarfrom)(int *lw,char *typex,int *it,int *m,int *n,int *lr,int *lc,int *lar,int *lac,unsigned long type_len)
{
  unsigned char Type = *typex;
  int MN;
  int lw1, lcs;
  char *fname =     Get_Iname();
  if (*lw > intersiz) {
    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"createcvarfrom");
    return FALSE;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  MN = (*m)*(*n);
  if (*lw < 0) {
    Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"createcvarfrom");
    return FALSE;
  }
  switch ( Type ) {
  case 'd' :
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, lc, nlgh)) return FALSE;
    if (*lar != -1) C2F(dcopy)(&MN, stk(*lar), &cx1,stk(*lr) , &cx1);
    if (*lac != -1 && *it == 1) C2F(dcopy)(&MN, stk(*lac), &cx1,stk(*lc) , &cx1);
    *lar = *lr;
    *lac = *lc;
    break;
  case 'r' :
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, lc, nlgh)) return FALSE;
    if (*lar != -1) C2F(rea2db)(&MN, sstk(*lar), &cx1, stk(*lr), &cx1);
    if (*lac != -1 && *it==1) C2F(rea2db)(&MN, sstk(*lac), &cx1, stk(*lc), &cx1);
    *lar = *lr;
    *lac = *lc;
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break ;
  case 'i' :
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, &lcs, nlgh)) return FALSE;
    if (*lar != -1) C2F(int2db)(&MN, istk(*lar), &cx1, stk(*lr), & cx1);
    if (*lac != -1 && (*it==1)) C2F(int2db)(&MN, istk(*lac), &cx1, stk(*lc), &cx1);
    *lar = *lr;
    *lac = *lc;
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  }
  C2F(intersci).ntypes[*lw - 1] = '$';
  C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
  C2F(intersci).lad[*lw - 1] = *lr;
  return TRUE;
}

/*---------------------------------------------------------------------
 *     This function must be called after createvar(lnumber,'l',...)
 *     Argument lnumber is a list
 *     we want here to get its argument number number
 *     the argument must be of type type ('c','d','r','i','b')
 *     input values lnumber,number,type,lar
 *     lar : input value ( -1 or the adress of an object which is used
 *           to fill the new variable data slot.
 *     lar must be a variable since it is used as input and output
 *     return values m,n,lr,lar
 *         (lar --> data is coded at stk(lar)
 *          lr  --> data is coded at istk(lr) or stk(lr) or sstk(lr)
 *                  or cstk(lr)
 *     c : string  (m-> number of characters and n->1)
 *     d,r,i : matrix of double,float or integer
 *---------------------------------------------------------------------*/

int C2F(createlistvarfrom)(int *lnumber,int *number,char * typex,int *m,int *n,int *lr,int *lar,unsigned long type_len)
{
  unsigned Type = *typex;
  int lc, ix1, it = 0, mn = (*m)*(*n),inc=1;
  char *fname = Get_Iname();
  if (*lnumber > intersiz) {
    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"createlistvar");
    return FALSE;
  }
  switch ( Type ) {
  case 'c' :
    *n = 1;
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcrestring)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1], m, lr, nlgh)) {
      return FALSE;
    }
    if (*lar != -1) C2F(cvstr1)(m, istk(*lr), cstk(*lar), &cx0,  *m * *n + 1);
    *lar = *lr;
    *lr = cadr( *lr);
    break;
  case 'd' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, lr, &lc, nlgh)) {
      return FALSE;
    }
    if (*lar != -1) C2F(dcopy)(&mn, stk(*lar), &cx1,stk(*lr) , &cx1);
    *lar = *lr;
    break;
  case 'r' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, lr, &lc, nlgh)) {
      return FALSE;
    }
    if (*lar != -1) 	C2F(rea2db)(&mn, sstk(*lar), &cx1, stk(*lr), &cx1);
    *lar = *lr;
    *lr = iadr(*lr);
    break;
  case 'i' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, lr, &lc, nlgh)) {
      return FALSE;
    }
    if (*lar != -1) 	C2F(int2db)(&mn, istk(*lar), &cx1, stk(*lr), &cx1);
    *lar = *lr;
    *lr = iadr(*lr);
    break;
  case 'b' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcrebmat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1]
			   , m, n, lr, nlgh)) {
      return FALSE;
    }
    if (*lar != -1) C2F(icopy)(&mn, istk(*lar), &cx1, istk(*lr), &cx1);
    *lar = *lr;
    break;
  case 'I' :
    it = *lr ; /* it gives the type on entry */
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcreimat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, lr, nlgh)) {
      return FALSE;
    }
    if (*lar != -1)
      C2F(tpconv)(&it,&it,&mn,istk(*lar), &inc,istk(*lr), &inc);
    *lar = *lr;
    break;
  case 'p' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcrepointer)(fname, &ix1, number,
			      &C2F(intersci).lad[*lnumber - 1], lr, nlgh))
      {
	return FALSE;
      }
    if (*lar != -1) *stk(*lr)= *stk(*lar);
    *lar = *lr;
    break;
  case 'h' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcrehmat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			   m, n, lr, nlgh)) {
      return FALSE;
    }
    if (*lar != -1) C2F(dcopy)(&mn, stk(*lar), &cx1,stk(*lr) , &cx1);
    *lar = *lr;
    break;
  default :
    Scierror(999,_("%s: (%s) bad third argument!\n"),fname,"createlistvar");
    return FALSE;
    break;
  }
  return TRUE;
}



/*---------------------------------------------------------------------
 * create a complex list variable from data
 *---------------------------------------------------------------------*/

int C2F(createlistcvarfrom)(int *lnumber, int *number, char *typex, int *it, int *m,int *n,int *lr,int *lc,int *lar,int *lac, unsigned long type_len)
{
  int ix1;
  int mn = (*m)*(*n);
  unsigned char Type = *typex;
  char *fname = Get_Iname();

  if (*lnumber > intersiz) {
    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"createlistcvar");
    return FALSE;
  }

  switch ( Type ) {
  case 'd' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],it, m, n, lr, lc, nlgh))
      return FALSE;
    if (*lar != -1) C2F(dcopy)(&mn,  stk(*lar), &cx1, stk(*lr), &cx1);
    if (*lac != -1 && *it==1) C2F(dcopy)(&mn, stk(*lac), &cx1,stk(*lc) , &cx1);
    *lar = *lr;
    *lac = *lc;
    break;
  case 'r' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  it, m, n, lr, lc, nlgh))
      return FALSE;
    if (*lar != -1) C2F(rea2db)(&mn, sstk(*lar), &cx1, stk(*lr), &cx1);
    if (*lac != -1 && *it==1) C2F(rea2db)(&mn, sstk(*lac), &cx1, stk(*lc), & cx1);
    *lar = *lr;
    *lac = *lc;
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  case 'i' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  it, m, n, lr, lc, nlgh))
      return FALSE;
    if (*lar != -1) C2F(int2db)(&mn,istk(*lar), &cx1, stk(*lr), &cx1);
    if (*lac != -1 && *it==1) C2F(int2db)(&mn, istk(*lac), &cx1, stk(*lc), &cx1);
    *lar = *lr;
    *lac = *lc;
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  default :
    Scierror(999,_("%s: (%s) bad third argument!\n"),fname,"createlistcvar");
    return FALSE;
  }
  return TRUE;
}



/*---------------------------------------------------------------------
 *     This function must be called after createvar(lnumber,'l',...)
 *     Argument lnumber is a list
 *     we want here to get its argument number number
 *     the argument must be of type type ('c','d','r','i','b')
 *     input values lnumber,number,type,lar
 *     lar : input value ( -1 or the adress of an object which is used
 *           to fill the new variable data slot.
 *     lar must be a variable since it is used as input and output
 *     return values m,n,lr,lar
 *         (lar --> data is coded at stk(lar)
 *          lr  --> data is coded at istk(lr) or stk(lr) or sstk(lr)
 *                  or cstk(lr)
 *     c : string  (m-> number of characters and n->1)
 *     d,r,i : matrix of double,float or integer
 *---------------------------------------------------------------------*/

int C2F(createlistvarfromptr)(int *lnumber,int * number,char *typex,int *m,int *n,void *iptr,unsigned long type_len)
{
  unsigned Type = *typex;
  int lc, ix1, it = 0, lr,inc=1;
  char *fname = Get_Iname();
  if (*lnumber > intersiz) {
    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"createlistvarfromptr");
    return FALSE;
  }

  ix1 = *lnumber + Top - Rhs;  /* factorization of this term (Bruno 9 march 2005, bugfix ) */
  switch ( Type ) {
  case 'c' :
    *n = 1;
    if (! C2F(listcrestring)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1], m, &lr, nlgh)) {
      return FALSE;
    }
    C2F(cchar)(m,(char **) iptr, istk(lr));
    break;
  case 'd' :
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, &lr, &lc, nlgh)) {
      return FALSE;
    }
    ix1= (*m)*(*n);
    C2F(cdouble) (&ix1,(double **) iptr, stk(lr));
    break;
  case 'r' :
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, &lr, &lc, nlgh)) {
      return FALSE;
    }
    ix1= (*m)*(*n);
    C2F(cfloat) (&ix1,(float **) iptr, stk(lr));
    break;
  case 'i' :
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, &lr, &lc, nlgh)) {
      return FALSE;
    }
    ix1 = *m * *n;
    C2F(cint)(&ix1,(int **) iptr, stk(lr));
    break;
  case 'b' :
    if (! C2F(listcrebmat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1]
			   , m, n, &lr, nlgh)) {
      return FALSE;
    }
    ix1 = *m * *n;
    C2F(cbool)(&ix1,(int **) iptr, istk(lr));
    break;
  case 'S' :
    if ( !cre_listsmat_from_str(fname,&ix1, number, &C2F(intersci).lad[*lnumber - 1]
				, m, n, (char **) iptr, nlgh)) /* XXX */
      return FALSE;
    break;
  case 's' :
    if ( !cre_listsparse_from_ptr(fname,&ix1,number,
			      &C2F(intersci).lad[*lnumber - 1]
			      , m, n, (SciSparse *) iptr, nlgh))
      return FALSE;
    break;
  case 'I' :
    it = ((SciIntMat *) iptr)->it;
    if (! C2F(listcreimat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, &lr, nlgh)) {
      return FALSE;
    }
    ix1= (*m)*(*n);
    C2F(tpconv)(&it,&it,&ix1,((SciIntMat *) iptr)->D, &inc,istk(lr), &inc);
    break;
  case 'p' :
    if (! C2F(listcrepointer)(fname, &ix1, number,
			      &C2F(intersci).lad[*lnumber - 1],&lr,nlgh))
      {
	return FALSE;
      }
    *stk(lr) = (double) ((unsigned long int) iptr);
    break;
  default :
    Scierror(999,_("%s: (%s) bad third argument!\n"),fname,"createlistcvar");
    return FALSE;
    break;
  }
  return TRUE;
}


/*---------------------------------------------------------------------
 *     This function must be called after createvar(lnumber,'l',...)
 *     Argument lnumber is a list
 *     we want here to get its argument number number
 *     the argument must be of type type ('c','d','r','i','b')
 *     input values lnumber,number,type,lar
 *     lar : input value ( -1 or the adress of an object which is used
 *           to fill the new variable data slot.
 *     lar must be a variable since it is used as input and output
 *     return values m,n,lr,lar
 *         (lar --> data is coded at stk(lar)
 *          lr  --> data is coded at istk(lr) or stk(lr) or sstk(lr)
 *                  or cstk(lr)
 *     c : string  (m-> number of characters and n->1)
 *     d,r,i : matrix of double,float or integer
 *---------------------------------------------------------------------*/

int C2F(createlistcvarfromptr)(int *lnumber,int *number,char *typex,int *it,int *m,int *n,void *iptr,void *iptc,unsigned long type_len)
{
  unsigned Type = *typex;
  int lr,lc, ix1;
  char *fname = Get_Iname();
  if (*lnumber > intersiz) {
    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"createlistvarfromptr");
    return FALSE;
  }
  switch ( Type ) {
  case 'd' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  it, m, n, &lr, &lc, nlgh)) {
      return FALSE;
    }
    ix1= (*m)*(*n);
    C2F(cdouble) (&ix1,(double **) iptr, stk(lr));
    if ( *it == 1) C2F(cdouble) (&ix1,(double **) iptc, stk(lc));
    break;
  case 'r' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  it, m, n, &lr, &lc, nlgh)) {
      return FALSE;
    }
    ix1= (*m)*(*n);
    C2F(cfloat) (&ix1,(float **) iptr, stk(lr));
    if ( *it == 1)     C2F(cfloat) (&ix1,(float **) iptc, stk(lc));
    break;
  case 'i' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  it, m, n, &lr, &lc, nlgh)) {
      return FALSE;
    }
    ix1 = *m * *n;
    C2F(cint)(&ix1,(int **) iptr, stk(lr));
    if ( *it == 1)     C2F(cint)(&ix1,(int **) iptc, stk(lc));
    break;
  default :
    Scierror(999,_("%s: (%s) bad third argument!\n"),fname,"createlistcvarfromptr");
    return FALSE;
    break;
  }
  return TRUE;
}


/*---------------------------------------------------------------------
 * use the rest of the stack as working area
 * the allowed size (in double) is returned in m
 *---------------------------------------------------------------------*/

int C2F(creatework)(int *number,int *m,int *lr)
{
  int n,it=0,lw1,lcs,il;
  char *fname = Get_Iname();
  if (*number > intersiz) {

    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"creatework");
    return FALSE ;
  }
  Nbvars = Max(*number,Nbvars);
  lw1 = *number + Top - Rhs;
  if (lw1 < 0) {
	Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"creatework");
    return FALSE ;
  }
  il = iadr(*Lstk(lw1));
  *m = *Lstk(Bot ) - sadr(il+4);
  n = 1;
  if (! C2F(cremat)(fname, &lw1, &it, m, &n, lr, &lcs, nlgh))    return FALSE;
  return TRUE;
}


/*---------------------------------------------------------------------
 * This can be used with creatework to
 * set the size of object which was intialy sized to the whole
 * remaining space with creatework
 * Moreover informations the objet is recorded
 *---------------------------------------------------------------------*/

int C2F(setworksize)(int *number,int *size)
{
  int lw1;
  char *fname = Get_Iname();
  if (*number > intersiz) {
	Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"creatework");
    return FALSE ;
  }
  Nbvars = Max(*number,Nbvars);
  lw1 = *number + Top - Rhs;
  if (lw1 < 0) {
    Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"setworksize");
    return FALSE ;
  }
  *Lstk(lw1+1) = *Lstk(lw1) + *size ;
  C2F(intersci).ntypes[*number - 1] = '$';
  C2F(intersci).iwhere[*number - 1] = *Lstk(lw1);
  C2F(intersci).lad[*number - 1] = 0; /* not to be used XXXX */
  return TRUE;
}


/*---------------------------------------------------------------------
 * getmatdims :
 *     check if argument number <<number>> is a matrix and
 *     returns its dimensions
 *---------------------------------------------------------------------*/

int C2F(getmatdims)(int *number,int *m,int *n)
{
  char *fname = Get_Iname();
  int il,lw,typ;

  lw = *number + Top - Rhs;
  if ( *number > Rhs) {
    Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"getmatdims");
    return FALSE;
  }

  il = iadr(*Lstk(lw));
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  typ = *istk(il );
  if (typ > sci_strings) {
    Scierror(199,_("%s: Wrong type for argument %d: Matrix expected.\n"), fname,*number);
    return  FALSE;
  }
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  return TRUE;
}

/*---------------------------------------------------------------------
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
 *---------------------------------------------------------------------*/

int C2F(getrhsvar)(int *number,char *typex,int *m,int *n,int *lr,unsigned long type_len)
{
	int ierr = 0,il1 = 0,ild1 = 0,nn = 0;
	int lrr = 0;
	char *fname = Get_Iname();
	char **items = NULL, namex[nlgh+1];
	unsigned char Type = *(unsigned char *) typex;
	int topk = 0,ltype = 0, m1 = 0, n1 = 0, lc = 0,lr1 = 0, it = 0, lw = 0, ile = 0, ils = 0, ix1 = 0,ix2 = 0;
	int mnel = 0,icol = 0;
	SciSparse *Sp = NULL;
	SciIntMat *Im = NULL;
	/* we accept a call to getrhsvar after a createvarfromptr call */
	if ( *number > Rhs && *number > Nbvars ) {
		Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"getrhsvar");
		return FALSE;
	}

	Nbvars = Max(Nbvars,*number);
	lw = *number + Top - Rhs;

	if (*number > intersiz) {
		Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"getrhsvar");
		return FALSE;
	}

	if (overloadtype(&lw,fname,&Type) == 0) return FALSE;

	topk = Top;
	switch ( Type )
	{
	case 'c' :
		*n = 1;
		if (! C2F(getsmat)(fname,&topk,&lw,&m1,&n1,&cx1,&cx1,lr,m, nlgh))
			return FALSE;
		ix2 = *m * *n;
		/* in case where ix2 is 0 in2str adds the \0 char after the end of
		the storage of the variable, so it writes over the next variable
		tp avoid this pb we shift up by one the location where the
		data is written*/
		lrr=*lr;
		if (ix2==0) lrr--;

		C2F(in2str)(&ix2, istk(*lr), cstk(cadr(*lr)), ix2 + 1);
		*lr = cadr(*lr);
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break;

	case 'd' :
		if (! C2F(getmat)(fname, &topk, &lw, &it, m, n, lr, &lc, nlgh)) return FALSE;
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break ;
	case 'z' :
		if (! C2F(getmat)(fname, &topk, &lw, &it, m, n, lr, &lc, nlgh)) return FALSE;
		ix2 = *m * *n;
		if ((it != 1) && (ix2 !=0))
		{
			Scierror(999,_("%s: Wrong type for argument: Complex expected.\n"),fname);
			return FALSE;
		};
		if (!(*lr % 2) ) {  /* bad adress (lr is even) shift up the stack */
			double2z(stk(*lr), stk(*lr)-1, ix2, ix2);
			*istk(iadr(*lr)-4)=133;
			*istk(iadr(*lr)-3)=iadr(*lr + 2*ix2-1);
			*istk( iadr(*lr + 2*ix2-1) )= *m;
			*istk( iadr(*lr + 2*ix2-1) +1 )= *n;
			C2F(intersci).ntypes[*number - 1] = Type ;
			C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
			C2F(intersci).lad[*number - 1] = *lr-1;
			*lr = sadr(*lr-1);
		}
		else {
			SciToF77(stk(*lr), ix2, ix2);
			C2F(intersci).ntypes[*number - 1] = Type ;
			C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
			C2F(intersci).lad[*number - 1] = *lr;
			*lr = sadr(*lr);
		};
		break ;
	case 'r' :
		if (! C2F(getmat)(fname, &topk, &lw, &it, m, n, lr, &lc, nlgh))  return FALSE;
		ix1 = *m * *n;
		C2F(simple)(&ix1, stk(*lr), sstk(iadr(*lr)));
		*lr = iadr(*lr);
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break;
	case 'i' :
		if (! C2F(getmat)(fname, &topk, &lw, &it, m, n, lr, &lc, nlgh)) return FALSE;
		ix1 = *m * *n;
		C2F(entier)(&ix1, stk(*lr), istk(iadr(*lr)));
		*lr = iadr(*lr) ;
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break;
	case 'b' :
		if (! C2F(getbmat)(fname, &topk, &lw, m, n, lr, nlgh))  return FALSE;
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break;
	case 'l' :    
	case 't' :    
	case 'm' :
		*n = 1;
		if (! C2F(getilist)(fname, &topk, &lw, m, n, lr, nlgh))  return FALSE;
		/* No data conversion for list members ichar(type)='$' */
		Type = '$' ;
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break;
	case 'S' :
		/** getwsmat : must be back in stack1.c from xawelm.f */
		if (! C2F(getwsmat)(fname,&topk,&lw,m,n,&il1,&ild1, nlgh)) return FALSE;
		nn= (*m)*(*n);
		ScilabMStr2CM(istk(il1),&nn,istk(ild1),&items,&ierr);
		if ( ierr == 1) return FALSE;
		Type = '$';
		/*
		* Warning : lr must have the proper size when calling getrhsvar
		* char **Str1; .... GetRhsVar(...., &lr)
		*/
		*((char ***) lr) = items ;
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break;
	case 's' :
		/* sparse matrices */
		Sp = (SciSparse *) lr ;
		if (! C2F(getsparse)(fname,&topk,&lw,&it,m,n,&(Sp->nel),&mnel,&icol,&lr1,&lc,nlgh))
			return FALSE;
		Sp->m = *m ; Sp->n = *n ; Sp->it = it;
		Sp->mnel = istk(mnel);
		Sp->icol = istk(icol);
		Sp->R = stk(lr1);
		Sp->I = (it==1) ? stk(lc): NULL;
		Type = '$';
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break;
	case 'I' :
		/* int matrices */
		Im = (SciIntMat *) lr ;
		if (! C2F(getimat)(fname,&topk,&lw,&it,m,n,&lr1,nlgh))
			return FALSE;
		Im->m = *m ; Im->n = *n ; Im->it = it; Im->l = lr1;
		Im->D = istk(lr1);
		Type = '$';
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break;
	case 'f' :
		/* XXXXXX : gros bug ici car getexternal a besoin de savoir
		pour quelle fonction on recupere un external
		or ici on presuppose que c'est setfeval
		de plus on ne sait pas exactement de quel type d'external il s'agit
		*/

		/*      int function getrhsvar(number,type,m,n,lr) */
		*lr = *Lstk(lw);
		ils = iadr(*lr) + 1;
		*m = *istk(ils);
		ile = ils + *m * nsiz + 1;
		*n = *istk(ile);
		if (! C2F(getexternal)(fname, &topk, &lw, namex, &ltype, C2F(setfeval), nlgh,
			nlgh)) {
				return FALSE;
		}
		Type = '$';
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break;
	case 'p' :
		if (! C2F(getpointer)(fname, &topk, &lw,lr, nlgh)) return FALSE;
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break;
	case 'h' :
		if (! C2F(gethmat)(fname, &topk, &lw, m, n, lr, nlgh)) return FALSE;
		C2F(intersci).ntypes[*number - 1] = Type ;
		C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
		C2F(intersci).lad[*number - 1] = *lr;
		break ;
	}
	return TRUE;
}


/*---------------------------------------------------------------------
 * getrhsvcar :
 *     get the argument number <<number>>
 *     the argument must be of type type ('d','r','i')
 *     like getrhsvar but for complex matrices
 *---------------------------------------------------------------------*/

int C2F(getrhscvar)(int *number,char *typex,int *it,int *m,int *n,int *lr,int *lc,unsigned long type_len)
{
  int ix1, lw, topk;
  unsigned char Type = *typex;
  char *fname = Get_Iname();

  Nbvars = Max(Nbvars,*number);
  lw = *number + Top - Rhs;
  if (*number > Rhs) {
    Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"getrhscvar");
    return FALSE;
  }
  if (*number > intersiz) {
  	Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"getrhscvar");
    return FALSE;
  }
  topk = Top;
  switch ( Type ) {
  case 'd' :
    if (! C2F(getmat)(fname, &topk, &lw, it, m, n, lr, lc, nlgh)) return FALSE;
    break;
  case 'r' :
    if (! C2F(getmat)(fname, &topk, &lw, it, m, n, lr, lc, nlgh)) return FALSE;
    ix1 = *m * *n * (*it + 1);
    C2F(simple)(&ix1, stk(*lr), sstk(iadr(*lr)));
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  case 'i' :
    if (! C2F(getmat)(fname, &topk, &lw, it, m, n, lr, lc, nlgh)) return FALSE;
    ix1 = *m * *n * (*it + 1);
    C2F(entier)(&ix1, stk(*lr), istk(iadr(*lr)));
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  }
  C2F(intersci).ntypes[*number - 1] = Type;
  C2F(intersci).iwhere[*number - 1] = *Lstk(lw);
  C2F(intersci).lad[*number - 1] = *lr;
  return TRUE;
}
/*---------------------------------------------------------------------
 * elementtype:
 *   returns the type of the element indexed by *number in the list
 *   whose variable number is *lnumber. If the indexed element does not exist
 *   the function returns 0.
 *---------------------------------------------------------------------*/

int C2F(elementtype)(int *lnumber, int *number)
{
  int il,lw,itype,n,ix,ili;
  char *fname = Get_Iname();

  if (*lnumber > Rhs) {
    Scierror(999,_("%s: bad call to %s!\n"),fname, "elementtype");
    return FALSE;
  }

  lw = *lnumber + Top - Rhs; /*index of the variable numbered *lnumber in the stack */
  il = iadr(*Lstk(lw));
  if (*istk(il) < 0) il = iadr(*istk(il + 1));
  itype = *istk(il ); /* type of the variable numbered *lnumber */
  if (itype <  sci_list || itype > sci_mlist) { /* check if it is really a list */
    Scierror(210,_("%s: Wrong type for argument %d: List expected.\n"),fname,*lnumber);
    return FALSE;
  }
  n = *istk(il + 1);/* number of elements in the list */
  itype = 0; /*default answer if *number is not a valid element index */
  if (*number<=n && *number>0) {
    ix = sadr(il + 3 + n); /* adress of the first list element */
    if (*istk(il + 1+ *number) < *istk(il + *number + 2)) { /* the required element is defined */
      ili = iadr(ix + *istk(il + 1+ *number) - 1); /* adress of the required element */
      itype = *istk(ili);
    }
  }
  return itype;
}

/*---------------------------------------------------------------------
 *     This function must be called after getrhsvar(lnumber,'l',...)
 *     Argument lnumber is a list
 *     we want here to get its argument number number
 *     the argument must be of type type ('c','d','r','i','b')
 *     return values m,n,lr,lar
 *         (lar --> data is coded at stk(lar)
 *          lr  --> data is coded at istk(lr) or stk(lr) or sstk(lr)
 *                  or cstk(lr)
 *     c : string  (m-> number of characters and n->1)
 *     d,r,i : matrix of double,float or integer
 *---------------------------------------------------------------------*/

int C2F(getlistrhsvar)(int *lnumber,int *number,char *typex,int *m,int *n,int *lr,unsigned long type_len)
{
  int lr1;
  char **items;
  int il1,ild1,nn,ierr=0;
  char *fname = Get_Iname();
  int m1, n1, lc, it, lw, topk = Top, ix1,ix2;
  unsigned char Type = *typex;
  int mnel,icol;
  SciSparse *Sp;
  SciIntMat *Im;

  Nbvars = Max(Nbvars,*lnumber);
  lw = *lnumber + Top - Rhs;
  if (*lnumber > Rhs) {
    Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"getlistrhsvar");
    return FALSE;
  }
  if (*lnumber > intersiz) {
    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"getlistrhsvar");
    return FALSE;
  }

  switch ( Type ) {
  case 'c' :
    *n = 1;
    if (! C2F(getlistsimat)(fname, &topk, &lw, number, &m1, &n1, &cx1, &cx1,lr, m, nlgh))
      return FALSE;
    ix2 = *m * *n;
    C2F(in2str)(&ix2, istk(*lr), cstk(cadr(*lr)), ix2 + 1);
    *lr = cadr(*lr);
    break;
  case 'd' :
    if (! C2F(getlistmat)(fname, &topk, &lw, number, &it, m, n, lr, &lc, nlgh))
      return FALSE;
    break;
  case 'r' :
    if (! C2F(getlistmat)(fname, &topk, &lw, number, &it, m, n, lr, &lc, nlgh))
      return FALSE;
    ix1 = *m * *n;
    C2F(simple)(&ix1, stk(*lr), sstk(iadr(*lr)));
    *lr = iadr(*lr);
    break;
  case 'i' :
    if (! C2F(getlistmat)(fname, &topk, &lw, number, &it, m, n, lr, &lc, nlgh))
      return FALSE;
    ix1 = *m * *n;
    C2F(entier)(&ix1, stk(*lr), istk(iadr(*lr)));
    *lr = iadr(*lr);
    break;
  case 'b' :
    if (! C2F(getlistbmat)(fname, &topk, &lw, number, m, n, lr, nlgh))
      return FALSE;
    *lr = *lr;
    break;
  case 'z' :
    if (! C2F(getlistmat)(fname, &topk, &lw,number, &it, m, n, lr, &lc, nlgh)) return FALSE;
    ix2 = *m * *n;
    if ((it != 1) && (ix2 !=0)){
      Scierror(999,_("%s: argument %d > (%d) should be a complex matrix.\n"),fname, Rhs + (lw -topk) , *number);
      return FALSE;
    };
      if (!(*lr % 2) ) {  /* bad adress (lr is even) shift up the stack */
	double2z(stk(*lr), stk(*lr)-1, ix2, ix2);
	ix2=2*ix2;
	*istk(iadr(*lr)-4)=133;
	*istk(iadr(*lr)-3)=iadr(*lr + ix2);
	*istk( iadr(*lr + ix2) )= *m;
	*istk( iadr(*lr + ix2) +1 )= *n;
	*lr = sadr(*lr-1);
      } else
	{
      SciToF77(stk(*lr), ix2, ix2);
      *lr = sadr(*lr);
	}
    break;
  case 'S' :
    /** getwsmat : must be back in stack1.c from xawelm.f */
    if (! C2F(getlistwsmat)(fname,&topk,&lw,number,m,n,&il1,&ild1, nlgh)) return FALSE;
    nn= (*m)*(*n);
    ScilabMStr2CM(istk(il1),&nn,istk(ild1),&items,&ierr);
    if ( ierr == 1) return FALSE;
    Type = '$';
    /*
     * Warning : lr must have the proper size when calling getrhsvar
     * char **Str1; .... GetRhsVar(...., &lr)
     */
    *((char ***) lr) = items ;
    break;
  case 's' :
    /* sparse matrices */
    Sp = (SciSparse *) lr ;
    if (! C2F(getlistsparse)(fname,&topk,&lw,number,&it,m,n,&(Sp->nel),&mnel,&icol,&lr1,&lc,nlgh))
      return FALSE;
    Sp->m = *m ; Sp->n = *n ; Sp->it = it;
    Sp->mnel = istk(mnel);
    Sp->icol = istk(icol);
    Sp->R = stk(lr1);
    Sp->I = stk(lc);
    Type = '$';
    break;
  case 'I' :
    /* int matrices */
    Im = (SciIntMat *) lr ;
    if (! C2F(getlistimat)(fname,&topk,&lw,number,&it,m,n,&lr1,nlgh))
      return FALSE;
    Im->m = *m ; Im->n = *n ; Im->it = it; Im->l = lr1;
    Im->D = istk(lr1);
    Type = '$';
    break;
  case 'p' :
    if (! C2F(getlistpointer)(fname, &topk, &lw, number, lr,  nlgh))
      return FALSE;
    break;
  default :
    Scierror(999,_("%s: bad call to %s (third argument %c).\n"),fname,"getlistrhsvar",Type);
    return FALSE;
  }
  /* can't perform back data conversion with lists */
  C2F(intersci).ntypes[*number - 1] = '$';
  return TRUE ;
}

/*---------------------------------------------------------------------
 * for complex
 *---------------------------------------------------------------------*/

int C2F(getlistrhscvar)(int *lnumber,int *number,char *typex,int *it,int *m,int *n,int *lr,int *lc,unsigned long type_len)
{
  int ix1, topk= Top, lw;
  char *fname = Get_Iname();
  unsigned  char    Type = * typex;

  Nbvars = Max(Nbvars,*lnumber);
  lw = *lnumber + Top - Rhs;
  if (*lnumber > Rhs) {
    Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"getlistrhscvar");
    return FALSE;
  }
  if (*lnumber > intersiz) {
    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"getlistrhscvar");
    return FALSE;
  }
  switch ( Type ) {
  case 'd' :
    if (! C2F(getlistmat)(fname, &topk, &lw, number, it, m, n, lr, lc, nlgh)) return FALSE;
    break;
  case 'r' :
    if (! C2F(getlistmat)(fname, &topk, &lw, number, it, m, n, lr, lc, nlgh))
      return FALSE;
    ix1 = *m * *n * (*it + 1);
    C2F(simple)(&ix1, stk(*lr), sstk(iadr(*lr)));
    *lr  = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  case 'i' :
    if (! C2F(getlistmat)(fname, &topk, &lw, number, it, m, n, lr, lc, nlgh))
      return FALSE;
    ix1 = *m * *n * (*it + 1);
    C2F(entier)(&ix1, stk(*lr), istk(iadr(*lr)));
    *lr = iadr(*lr) ;
    *lc = *lr + *m * *n;
    break;
  default :
    Scierror(999,_("%s: (%s) bad third argument!\n"),fname,"getlistrhscvar");

    return FALSE;
  }
  /* can't perform back data conversion with lists */
  C2F(intersci).ntypes[*number - 1] = '$';
  return TRUE;
}

/*---------------------------------------------------------------------
 *     creates variable number number of type "type" and dims m,n
 *     from pointer ptr
 *
 *---------------------------------------------------------------------*/

int C2F(createvarfromptr)(int *number,char *typex,int *m,int *n,void *iptr,unsigned long type_len)
{
	static int un=1;
	unsigned char Type = *typex;
	int MN= (*m)*(*n),lr,it,lw1;
	char *fname = Get_Iname();
	lw1 = *number + Top - Rhs;
	switch ( Type )
		{
			case 'd' :
				if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE ) return FALSE;
				C2F(dcopy)(&MN,*((double **) iptr),&un, stk(lr),&un);
				break;
			case 'i' :
			case 'b' :
				if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE ) return FALSE;
				C2F(icopy)(&MN,*((int **) iptr), &un, istk(lr), &un);
				break;
			case 'r' :
				if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE ) return FALSE;
				C2F(rcopy)(&MN,*((float **)iptr), &un, sstk(lr), &un);
				break;
			case 'c' :
				if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE ) return FALSE;
				strcpy(cstk(lr),*((char **) iptr));
				break;
			case 'I' :
				/* on entry lr must gives the int type */
				it = lr = ((SciIntMat *) iptr)->it;
				if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE ) return FALSE;
				C2F(tpconv)(&it,&it,&MN,((SciIntMat *) iptr)->D, &un,istk(lr), &un);
				break;
			case 'p' :
				if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE ) return FALSE;
				*stk(lr) = (double) ((unsigned long int) iptr);
				break;
			case 'S' :
				/* special case: not taken into account in createvar */
				Nbvars = Max(*number,Nbvars);
				if ( !cre_smat_from_str(fname,&lw1, m, n, (char **) iptr, nlgh))
					return FALSE;
				C2F(intersci).iwhere[*number - 1] = *Lstk(lw1);
				C2F(intersci).ntypes[*number - 1] = '$';
				break;
			case 's' :
				/* special case: not taken into account in createvar */
				Nbvars = Max(*number,Nbvars);
				if ( !cre_sparse_from_ptr(fname,&lw1, m, n, (SciSparse *) iptr, nlgh))
					return FALSE;
				C2F(intersci).iwhere[*number - 1] = *Lstk(lw1);
				C2F(intersci).ntypes[*number - 1] = '$';
				break;
			default :
				Scierror(999,_("%s: (%s) bad second argument!\n"),fname,"createvarfromptr");

				return FALSE;
		}
	/*     this object will be copied with a vcopyobj in putlhsvar */
	return TRUE;
}



/*---------------------------------------------------------------------
 *     for complex
 *---------------------------------------------------------------------*/

int C2F(createcvarfromptr)(int *number,char *typex,int *it,int *m,int *n,double *iptr,double *iptc,unsigned long type_len)
{
  unsigned char Type = *typex;
  char *fname = Get_Iname();
  int lw1, lcs, lrs, ix1;

  Nbvars = Max(Nbvars,*number);
  if (*number > intersiz) {
    Scierror(999,_("%s: createcvarfromptr: too many arguments on the stack, enlarge intersiz.\n"),fname);
    return FALSE;
  }
  lw1 = *number + Top - Rhs;
  switch ( Type ) {
  case 'd' :
    if (! C2F(cremat)(fname, &lw1, it, m, n, &lrs, &lcs, nlgh))
      return FALSE;
    ix1 = *m * *n;
    C2F(cdouble)(&ix1, (double **) iptr, stk(lrs));
    if (*it == 1) {
      ix1 = *m * *n;
      C2F(cdouble)(&ix1,(double **) iptc, stk(lcs));
    }
    break;
  case 'i' :
    if (! C2F(cremat)(fname, &lw1, it, m, n, &lrs, &lcs, nlgh))
      return FALSE;
    ix1 = *m * *n;
    C2F(cint)(&ix1, (int **) iptr, stk(lrs));
    if (*it == 1) {
      ix1 = *m * *n;
      C2F(cint)(&ix1,(int **) iptc, stk(lcs));
    }
    break;
  default :
	Scierror(999,_("%s: (%s) bad second argument!\n"),fname,"createcvarfromptr");
				
    return FALSE;
  }
  /*     this object will be copied with a vcopyobj in putlhsvar */
  C2F(intersci).ntypes[*number - 1] = '$';
  return  TRUE;
}

/*---------------------------------------------------------------------
 * mklistfromvars :
 *     replace the last n variables created at postions pos:pos-1+n
 *     by a list of these variables at position pos
 *---------------------------------------------------------------------*/

int C2F(mklistfromvars)(int *pos,int *n)
{
  int tops =  Top;
  int k;
  for ( k= *pos; k < *pos+*n; k++) C2F(convert2sci)(&k);
  Top = Top - Rhs + *pos - 1 + *n;
  C2F(mklist)(n);
  Top = tops;
  C2F(intersci).ntypes[*pos - 1] = '$';
  return  TRUE;
}
/*---------------------------------------------------------------------
 * mktlistfromvars :
 *     similar to mklistfromvars but create a tlist
 *---------------------------------------------------------------------*/

int C2F(mktlistfromvars)(int *pos,int *n)
{
  int type=16;
  int tops =  Top;
  int k;
  for ( k= *pos; k < *pos+*n; k++) C2F(convert2sci)(&k);
  Top = Top - Rhs + *pos - 1 + *n;
  C2F(mklistt)(n,&type);
  Top = tops;
  C2F(intersci).ntypes[*pos - 1] = '$';
  return  TRUE;
}
/*---------------------------------------------------------------------
 * mktlistfromvars :
 *     similar to mklistfromvars but create a mlist
 *---------------------------------------------------------------------*/

int C2F(mkmlistfromvars)(int *pos,int *n)
{
  int type=sci_mlist;
  int tops =  Top;
  int k;
  for ( k= *pos; k < *pos+*n; k++) C2F(convert2sci)(&k);
  Top = Top - Rhs + *pos - 1 + *n;
  C2F(mklistt)(n,&type);
  Top = tops;
  C2F(intersci).ntypes[*pos - 1] = '$';
  return  TRUE;
}

/*---------------------------------------------------------------------
 * call a Scilab function given its name
 *---------------------------------------------------------------------*/

int C2F(callscifun)(char *string,unsigned long string_len)
{
  int id[nsiz];
  C2F(cvname)(id, string, &cx0, string_len);
  C2F(putid)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], id);
  C2F(com).fun = -1;
  return 0;
}

/*---------------------------------------------------------------------
 * scifunction(number,ptr,mlhs,mrhs) >
 *     execute scilab function with mrhs input args and mlhs output
 *     variables
 *     input args are supposed to be stored in the top of the stack
 *     at positions top-mrhs+1:top
 *---------------------------------------------------------------------*/

int C2F(scifunction)(int *number,int *ptr,int *mlhs,int *mrhs)
{
  int cx26 = 26;
  int ix1, ix, k, intop, lw;
  int imode,ireftop;

  if ( intersci_push() == 0 )
    {
      Scierror(999,_("%s: No more memory.\n"),"scifunction");
      goto L9999;
    }

  /*     macro execution inside a builtin gateway*/
  intop = Top;
  Top = Top - Rhs + *number + *mrhs - 1;
  ++C2F(recu).pt;
  if (C2F(recu).pt > psiz) {
    Error(cx26);
    goto L9999;
  }
  C2F(recu).ids[C2F(recu).pt * nsiz - nsiz] = Lhs;
  C2F(recu).ids[C2F(recu).pt * nsiz - (nsiz-1)] = Rhs;
  C2F(recu).rstk[C2F(recu).pt - 1] = 1001;
  Lhs = *mlhs;
  Rhs = *mrhs;
  ++C2F(recu).niv;
  C2F(com).fun = 0;
  C2F(com).fin = *ptr;
  C2F(recu).icall = 5;
  C2F(recu).krec = -1;
  /* ************************** copied from callinter.h */
 L60:
  C2F(parse)();
  /* parse has exited for a built-in evaluation */

  if (C2F(com).fun == 99) {
    if( Err>0 ||C2F(errgst).err1>0) {
      imode=abs(C2F(errgst).errct)/100000 % 8;
      if (imode !=3) {
	goto L97;
      }
    }
    C2F(com).fun = 0;
    goto L200;
  }
  if (Err > 0)  goto L97;

  if ( isRecursionCallToFunction() )
  {
	  int gw = getRecursionGatewayToCall();
	  if (gw == END_OVERLOAD)
	  {
		  goto L96;
	  } 
	  else if (gw == ERROR_GW_ID)
	  {
		  goto L89;
	  }
	  else
	  {
		  k = gw;
	  }
	  goto L95;
  }
  if ( isRecursionCallToFunction() )
  {
	  int gw = getRecursionGatewayToCall();
	  if (gw == END_OVERLOAD)
	  {
		  goto L96;
	  } 
	  else if (gw == ERROR_GW_ID)
	  {
		  goto L89;
	  }
	  else
	  {
		  k = gw;
	  }
	  goto L95;
  }

 L89:
  if (Top < Rhs) {
    Scierror(22,_("%s: Recursion problems. Sorry ...\n"),"scifunction");
    goto L97;
  }
  if (Top - Rhs + Lhs + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),"scifunction");
    goto L97;
  }
  /*     ireftop used to reset top if an error occurs during 
	 the function evaluation*/
  ireftop=Top-Max(0,Rhs);

  goto L91;
 L90:
  if (Err > 0) goto L97;
  /**/
 L91:
  k = C2F(com).fun;
  C2F(com).fun = 0;
  if (k == C2F(recu).krec) {
    Scierror(22,_("%s: Recursion problems. Sorry ...\n"),"scifunction");
    goto L97;
  }
  C2F(recu).krec = -1;
  if (k == 0) goto L60;
 L95:
  if (! C2F(allowptr)(&k)) C2F(ref2val)();
  C2F(recu).krec = k;
  C2F(callinterf)(&k);

  C2F(recu).krec = -1;
  if (C2F(com).fun >= 0) {
    if (Top - Lhs + 1 > 0) {
      C2F(iset)(&Lhs, &cx0, &C2F(vstk).infstk[Top - Lhs], &cx1);
    }
    if(C2F(recu).paus > 0) goto L91;
    if (C2F(errgst).err1 > 0) Top=ireftop;
    goto L90;
  }
  /*    called interface ask for a scilab function to perform the function (fun=-1)
   *     the function name is given in ids(1,pt+1)
   */
  C2F(ref2val)();
  C2F(com).fun = 0;
  C2F(funs)(&C2F(recu).ids[(C2F(recu).pt + 1)* nsiz - nsiz]);
  if (Err > 0) goto L97;

  if (C2F(com).fun > 0) {
    if (C2F(isbyref)(&C2F(com).fun)==0) C2F(ref2val)();
    goto L91;
  }
  if (Fin == 0) {
    int cx4 = 246;
    Error(cx4);
    if (Err > 0) goto L97;
    goto L90;
  }
  ++C2F(recu).pt;
  Fin = *Lstk(C2F(com).fin);
  C2F(recu).rstk[C2F(recu).pt - 1] = 910;
  C2F(recu).icall = 5;
  C2F(com).fun = 0;
  /*     *call*  macro */
  goto L60;
 L96:
  --C2F(recu).pt;
  goto L90;

 L97: /* error handling */
  if((C2F(recu).niv > 0) && (C2F(recu).paus > 0)) {
    C2F(com).fun=0;
    goto L60;
  }
  goto L9999;
  /* ************************** end of copy*/
  L200:
  Lhs = C2F(recu).ids[C2F(recu).pt * nsiz -nsiz ];
  Rhs = C2F(recu).ids[C2F(recu).pt * nsiz -(nsiz-1)];
  --C2F(recu).pt;
  --C2F(recu).niv;
  /* + */
  Top = intop;
  ix1 = *mlhs;
  intersci_pop();
  for (ix = 1; ix <= ix1; ++ix) {
    lw = Top - Rhs + *number + ix - 1;
    C2F(intersci).ntypes[lw - 1] = '$';
  }
  return TRUE;

 L9999:
  Top = intop;
  --C2F(recu).niv;
  intersci_pop();
  return FALSE;
}

/*---------------------------------------------------------------------
 * scistring :
 *   executes scilab string (name of a scilab function) with mrhs
 *     input args and mlhs output variables
 *     input args are supposed to be indexed by ifirst,ifirst+1,...
 *     thestring= string made of the name of a Scilab function
 *     mlhs,mlhs = number of lhs and rhs parameters of the function
 *     ifisrt,thestring,mlhs and mrhs are input parameters.
 *---------------------------------------------------------------------*/

int C2F(scistring)(int *ifirst,char *thestring,int *mlhs,int *mrhs,unsigned long thestring_len)
{
  int ret = FALSE;
  int ifin, ifun, tops, moutputs, id[nsiz], lf, op, ile, ils, nnn, ninputs;
  nnn =  thestring_len;
  op = 0;
  if (nnn <= 2) {
    op = C2F(getopcode)(thestring, thestring_len);
  }
  if (op == 0) {
    C2F(cvname)(id, thestring, &cx0, nnn);
    Fin = 0;
    tops = Top;
    Top = Top - Rhs + *ifirst + *mrhs - 1;
    C2F(funs)(id);
    Top = tops;
    if (Fin == 0)
	{
      Scierror(999,_("%s: %s is not a Scilab function.\n"),"scistring",get_fname(thestring,thestring_len));
      return ret;
    }
    if (C2F(com).fun <= 0) {
      lf = *Lstk(Fin);
      ils = iadr(lf) + 1;
      moutputs = *istk(ils);
      ile = ils + moutputs * nsiz + 1;
      ninputs = *istk(ile);
      /*
       *   ninputs=actual number of inputs, moutputs=actual number of outputs
       *   of thestring: checking mlhs=ninputs and mrhs=moutputs not done.
       */
      ret = C2F(scifunction)(ifirst, &lf, mlhs, mrhs);
    } else {
      ifin = Fin;
      ifun = C2F(com).fun;
      ret = C2F(scibuiltin)(ifirst, &ifun, &ifin, mlhs, mrhs);
    }
  } else {
    ret = C2F(sciops)(ifirst, &op, mlhs, mrhs);
  }
  return ret;
}

int C2F(getopcode)(char *string,unsigned long string_len)
{
  unsigned char ch = string[0];
  int op = 0;
  if (  string_len >= 2) {
    /*     .op  or op. */
    if ( ch  == '.') ch = string[1];
    op += 51;
  }
  switch ( ch )
    {
    case  '*'  :  op += 47; break;
    case  '+'  :  op += 45; break;
    case  '-'  :  op += 46; break;
    case  '\'' :  op += 53; break;
    case  '/'  :  op += 48; break;
    case  '\\' :  op += 49; break;
    case  '^'  :  op += 62; break;
    }
  return op;
}

/*---------------------------------------------------------------------
 *     same as scifunction: executes scilab built-in function (ifin,ifun)
 *
 *     =(interface-number, function-nmber-in-interface)
 *     for the input parameters located at number, number+1, ....
 *     mlhs,mrhs = # of lhs and rhs parameters of the function.
 *---------------------------------------------------------------------*/

int C2F(scibuiltin)(int *number,int *ifun,int *ifin,int *mlhs,int *mrhs)
{
  int srhs, slhs;
  int ix, k, intop, lw, pt0;
  int imode,ireftop;
  intop = Top;

  if ( intersci_push() == 0 )
    {
      Scierror(999,_("%s: No more memory.\n"),"scifunction");
      goto L9999;
    }

  Top = Top - Rhs + *number + *mrhs - 1;
  slhs = Lhs;
  srhs = Rhs;
  Lhs = *mlhs;
  Rhs = *mrhs;
  C2F(recu).krec = -1;
  pt0 = C2F(recu).pt;
  ++C2F(recu).niv;
  goto L90;
  /* ***************************** copied from callinter.h  */

 L60:
  C2F(parse)();
  if (C2F(com).fun == 99) {
    if( Err>0 ||C2F(errgst).err1>0) {
      imode=abs(C2F(errgst).errct)/100000 % 8;
      if (imode !=3) goto L97;
    }
    C2F(com).fun = 0;
    goto L200;
  }
  if (Err > 0) goto L97;

  if ( isRecursionCallToFunction() )
  {
	  int gw = getRecursionGatewayToCall();
	  if (gw == END_OVERLOAD)
	  {
		  goto L96;
	  } 
	  else if (gw == ERROR_GW_ID)
	  {
		  goto L89;
	  }
	  else
	  {
		  k = gw;
	  }
	  goto L95;
  }


 L89:
  if (Top < Rhs) {
    Scierror(22,_("%s: Recursion problems. Sorry ...\n"),_("built in"));
    goto L97;
  }
  if (Top - Rhs + Lhs + 1 >= Bot) {
    Scierror(18,_("%s: Too many names.\n"),"");
    goto L97;
  }
  /*     ireftop used to reset top if an error occurs during 
	 the function evaluation*/
  ireftop=Top-Max(0,Rhs);

  goto L91;
 L90:
  if (Err > 0)  goto L97;
  /**/
 L91:
  k = C2F(com).fun;
  C2F(com).fun = 0;
  if (k == C2F(recu).krec) {
    Scierror(22,_("%s: Recursion problems. Sorry ...\n"),_("built in"));
    goto L9999;
  }
  C2F(recu).krec = -1;
  if (k == 0)  goto L60;
 L95:
  if (! C2F(allowptr)(&k))  C2F(ref2val)();
  C2F(recu).krec = k;
  C2F(callinterf)(&k);
  C2F(recu).krec = -1;
  if (C2F(com).fun >= 0) {
    if (Top - Lhs + 1 > 0) {
      C2F(iset)(&Lhs, &cx0, &C2F(vstk).infstk[Top - Lhs], &cx1);
    }
    if(C2F(recu).paus > 0) goto L91;
    if (C2F(errgst).err1 > 0) Top=ireftop;
    goto L90;
  }
  /*    called interface ask for a sci function to perform the function (fun=-1)*/
  /*     the function name is given in ids(1,pt+1) */
  C2F(ref2val)();
  C2F(com).fun = 0;
  C2F(funs)(&C2F(recu).ids[(C2F(recu).pt + 1)* nsiz - nsiz]);
  if (Err > 0)  goto L97;
  if (C2F(com).fun > 0) {
    if (C2F(isbyref)(&C2F(com).fun)==0) C2F(ref2val)();
    goto L91;
  }
  if (Fin == 0) {
    int cx4 = 246;
    Error(cx4);
    if (Err > 0) goto L97;
    goto L90;
  }
  ++C2F(recu).pt;
  Fin = *Lstk(C2F(com).fin);
  C2F(recu).rstk[C2F(recu).pt - 1] = 910;
  C2F(recu).icall = 5;
  C2F(com).fun = 0;
  /*     *call*  macro */
  goto L60;
 L96:
  --C2F(recu).pt;
  goto L90;

 L97: /* error handling */
  if((C2F(recu).niv > 0) && (C2F(recu).paus > 0)) {
    C2F(com).fun=0;
    goto L60;
  }
  /* ************************** end of copy */
 L200:
  Lhs = slhs;
  Rhs = srhs;
  --C2F(recu).niv;
  Top = intop;
  intersci_pop();
  for (ix = 0 ; ix < *mlhs ; ++ix) {
    lw = Top - Rhs + *number + ix ;
    C2F(intersci).ntypes[lw - 1] = '$';
  }
  return TRUE;
 L9999:
  Top = intop;
  --C2F(recu).niv;
  intersci_pop();
  return FALSE;
}

/*---------------------------------------------------------------------
 *     same as scibuiltin: executes scilab operation op
 *     for the input parameters located at number, number+1, ....
 *     mlhs,mrhs = # of lhs and rhs parameters of the operation.
 *---------------------------------------------------------------------*/

int C2F(sciops)(int *number,int *op,int *mlhs,int *mrhs)
{
  int ifin, ifun, srhs= Rhs , slhs= Lhs, ix, intop=Top , lw;

  Fin   = *op;
  Top = Top - Rhs + *number + *mrhs - 1;
  Lhs = *mlhs;
  Rhs = *mrhs;

  while (1)
    {
      C2F(allops)();
      if (Err > 0) {return FALSE;} ;
      if (C2F(com).fun == 0) break;
      Top = intop;
      ifun = C2F(com).fun;
      ifin = Fin;
      if (! C2F(scibuiltin)(number, &ifun, &ifin, mlhs, mrhs))
	{return FALSE;} ;
      if (Err > 0) {return FALSE;} ;
    }
  Lhs = slhs;
  Rhs = srhs;
  Top = intop;

  for (ix = 1 ; ix <= *mlhs ; ++ix) {
    lw = Top - Rhs + *number + ix - 1;
    C2F(intersci).ntypes[lw - 1] = '$';
  }
  C2F(com).fun = 0;
  Fin = *op;
  C2F(recu).icall = 0;
  return TRUE;
}

/*-------------------------------------------------------------
 *     test and return linear system (syslin tlist)
 *     inputs: lw = variable number
 *     outputs:
 *     N=size of A matrix (square)
 *     M=number of inputs = col. dim B matrix
 *     P=number of outputs = row. dim of C matrix
 *     ptr(A,B,C,D,X0) adresses of A,B,C,D,X0 in stk
 *     h=type   h=0.0  continuous system
 *              h=1.0  discrete time system
 *              h=h    sampled system h=sampling period
 -------------------------------------------------------------*/

int C2F(getrhssys)(int *lw,int *n,int *m,int *p,int *ptra,int *ptrb,int *ptrc,int *ptrd,int *ptrx0,double *hx)
{
  int cx2 = 2, cx3 = 3, cx4 = 4, cx5 = 5, cx6 = 6;
  int ix1, junk, msys, nsys, ix, icord;
  int ma, na, mb, nb, mc, nc, il, md, nd;
  int mx0, nx0, ptrsys, itimedomain;
  static int iwork[23] = { 10,1,7,0,1,4,5,6,7,8,10,12,21,28,28,-10,-11,
			       -12,-13,-33,0,13,29 };
  if (! C2F(getrhsvar)(lw, "t", &msys, &nsys, &ptrsys, 1L)) return FALSE;
  il = iadr(ptrsys) - msys - 1;
  /*     syslin tlist=[ chain, (A,B,C,D,X0) ,chain or scalar ]
   *                     10     1 1 1 1 1      10       1
   */
  junk = il + msys + iadr(*istk(il));
  if ( *istk(junk) != 10) return FALSE;
  if ( *istk(il + msys + iadr(*istk(il + 1))) != 1) return FALSE;
  if ( *istk(il + msys + iadr(*istk(il + 2))) != 1) return FALSE;
  if ( *istk(il + msys + iadr(*istk(il + 3))) != 1) return FALSE;
  if ( *istk(il + msys + iadr(*istk(il + 4))) != 1) return FALSE;
  if ( *istk(il + msys + iadr(*istk(il + 5))) != 1) return FALSE;
  itimedomain = *istk(il + msys + iadr(*istk(il + 6)));
  switch ( itimedomain ) {
  case sci_strings :
    /* Sys(7)='c' or 'd' */
    icord = *istk(il + msys + iadr(*istk(il + 6))+ 6);
    switch ( icord )
      {
      case 12 :  *hx = 0.; break;
      case 13 :  *hx = 1.; break;
      default :
	Scierror(999,_("Invalid time domain.\n"));
	return FALSE;
      }
    break;
  case sci_matrix :
    /*     Sys(7)=h */
    ix1 = il + msys + iadr(*istk(il + 6)) + 4;
    *hx = *stk(sadr(ix1));
    break;
  default :
    Scierror(999,_("Invalid time domain.\n"));
    return FALSE;
  }
  for (ix = 0; ix < 23; ++ix) /* @TODO : what is 23 ? */
    {
      if (iwork[ix] != *istk(junk + ix)) {
		  Scierror(999,_("Invalid system.\n"));
		  return FALSE;
      }
    }
  if (! C2F(getlistrhsvar)(lw, &cx2, "d", &ma, &na, ptra, 1L)) return FALSE;
  if (! C2F(getlistrhsvar)(lw, &cx3, "d", &mb, &nb, ptrb, 1L)) return FALSE;
  if (! C2F(getlistrhsvar)(lw, &cx4, "d", &mc, &nc, ptrc, 1L)) return FALSE;
  if (! C2F(getlistrhsvar)(lw, &cx5, "d", &md, &nd, ptrd, 1L)) return FALSE;
  if (! C2F(getlistrhsvar)(lw, &cx6, "d", &mx0, &nx0, ptrx0, 1L))  return FALSE;
  if (ma != na) {
    Scierror(999,_("A non square matrix!\n"));
    return FALSE;
  }
  if (ma != mb && mb != 0) {
    Scierror(999,_("Invalid %c,%c matrices.\n"),'A','B');
    return FALSE;
  }
  if (ma != nc && nc != 0) {
    Scierror(999,_("Invalid %c,%c matrices.\n"),'A','C');
    return FALSE;
  }
  if (mc != md && md != 0) {
    Scierror(999,_("Invalid %c,%c matrices.\n"),'C','D');
    return FALSE;
  }
  if (nb != nd && nd != 0) {
    Scierror(999,_("Invalid %c,%c matrices.\n"),'B','D');
    return FALSE;
  }
  *n = ma;
  *m = nb;
  *p = mc;
  return TRUE;
}


/*---------------------------------------------------
 * call Scilab error function (for Fortran use)
 *---------------------------------------------------*/

int C2F(errorinfo)(char *fname,int *info,unsigned long fname_len)
{
  Scierror(998,_("%s: internal error, info=%d.\n"),get_fname(fname,fname_len),*info);
  return 0;
}


/*-------------------------------------------------------------
 *  returns Maximal available size in scilab stack
 *  for variable <<number>> lw
 *  In a Fortran call
 *     lw =
 *     type= 'd','r','i','c'
 *     type_len is here for C/Fortran calling conventions
 *  This function is used for creating a working array of Maximal dimension
 *  Example :
 *     lwork=Maxvol(nb,'d')
 *     if(.not.createvar(nb,'d',lwork,1,idwork)) return
 *     call pipo(   ,stk(idwork),[lwork],...)
 *-------------------------------------------------------------*/

int C2F(maxvol)(int *lw,char *lw_type,unsigned long type_len)
{
  unsigned char Type =  *(unsigned char *)lw_type ;
  /* I like this one a lot: a kind of free jazz pattern  */
  int m = *Lstk(Bot) - sadr(iadr(*Lstk(*lw + Top - Rhs)) +4);
  switch ( Type )
    {
    case 'd' : return m; break;
    case 'i' : return iadr(m);break;
    case 'r' : return iadr(m);break;
    case 'c' : return cadr(m);break;
    case 'z' : return sadr(m)-3;break;
    }
  /* should never get there */
  return m;
}


/*---------------------------------------------
 * This function checks all the variables which
 * where references and restore their contents
 * to Scilab value.
 *---------------------------------------------*/

static int Check_references()
{
	int ivar ;
	for (ivar = 1; ivar <= Rhs ; ++ivar)
		{
			unsigned char Type = (unsigned char)C2F(intersci).ntypes[ivar - 1];
			if ( Type != '$')
				{
					int lw = ivar + Top - Rhs;
					int il = iadr(*Lstk(lw));
					if ( *istk(il) < 0)
						{
							int m,n,it,size;
							/* back conversion if necessary of a reference */
							if ( *istk(il) < 0)  il = iadr(*istk(il +1));
							m =*istk(il +1);
							n =*istk(il +2);
							it = *istk(il +3);
							switch ( Type ) {
								case 'i' :
								case 'r' :
								case 'd' :
									size  = m * n * (it + 1); 
									break;
								case 'z' :
									size  = 0;
									break; /* size is unsued for 'z' in ConvertData;*/
								case 'c' :
									size =*istk(il + 4  +1) - *istk(il + 4 ); 
									break;
								case 'b' :
									size = m*n ; 
									break;
								default:
									return FALSE;
							}
							ConvertData(&Type,size,C2F(intersci).lad[ivar - 1]);
							C2F(intersci).ntypes[ivar - 1] = '$';
						}
				}
			else
				{
				}
		}
	return TRUE;
}




/*---------------------------------------------------------------------
 * int C2F(putlhsvar)()
 *     This function put on the stack the lhs
 *     variables which are at position lhsvar(i)
 *     on the calling stack
 *     Warning : this function supposes that the last
 *     variable on the stack is at position top-rhs+nbvars
 *---------------------------------------------------------------------*/

int C2F(putlhsvar)()
{
  int ix2, ivar, ibufprec, ix, k, lcres, nbvars1;
  int plhsk;
  Check_references();

  for (k = 1; k <= Lhs; k++)
    {
      plhsk=*Lstk(LhsVar(k)+Top-Rhs);
      if (*istk( iadr(plhsk) ) < 0) {
	if (*Lstk(Bot) > *Lstk(*istk(iadr (plhsk) +2)) )
	LhsVar(k)=*istk(iadr(plhsk)+2);
	/* lcres = 0 */
      }
    }

  if (Err > 0||C2F(errgst).err1> 0)  return TRUE ;
  if (C2F(com).fun== -1 ) return TRUE ; /* execution continue with an
					    overloaded function */
  if (LhsVar(1) == 0)
    {
      Top = Top - Rhs + Lhs;
      C2F(objvide)(" ", &Top, 1L);
      Nbvars = 0;
      return TRUE;
    }
  nbvars1 = 0;
  for (k = 1; k <= Lhs ; ++k) nbvars1 = Max( nbvars1 , LhsVar(k));
  /* check if output variabe are in increasing order in the stack */
  lcres = TRUE;
  ibufprec = 0;
  for (ix = 1; ix <= Lhs ; ++ix) {
    if (LhsVar(ix) < ibufprec) {
      lcres = FALSE;
      break;
    } else {
      ibufprec = LhsVar(ix );
    }
  }
  if (! lcres) {
    /* First pass if output variables are not
     * in increasing order
     */
    for (ivar = 1; ivar <= Lhs; ++ivar) {
      ix2 = Top - Rhs + nbvars1 + ivar;
      if (! C2F(mvfromto)(&ix2, &LhsVar(ivar))) {
	return FALSE;
      }
      LhsVar(ivar) = nbvars1 + ivar;
      /* I change type of variable nbvars1 + ivar
       * in order to just perform a dcopy at next pass
       */
      if (nbvars1 + ivar > intersiz)
	  {
		Scierror(999,_("%s: intersiz is too small.\n"),"putlhsvar");
		return FALSE;
      }
      C2F(intersci).ntypes[nbvars1 + ivar - 1] = '$';
    }
  }
  /*  Second pass */
  for (ivar = 1; ivar <= Lhs ; ++ivar)
    {
      ix2 = Top - Rhs + ivar;
      if (! C2F(mvfromto)(&ix2, &LhsVar(ivar))) {
	return FALSE;
      }
    }
  Top = Top - Rhs + Lhs;
  LhsVar(1) = 0;
  Nbvars = 0;
  return TRUE;
}


/*---------------------------------------------------------------------
 * mvfromto :
 *     this routines copies the variable number i
 *     (created by getrhsvar or createvar or by mvfromto itself in a precedent call)
 *     from its position on the stack to position itopl
 *     returns false if there's no more stack space available
 *     - if type(i) # '$'  : This variable is at
 *                         position lad(i) on the stack )
 *                         and itopl must be the first free position
 *                         on the stack
 *                         copy is performed + type conversion (type(i))
 *     - if type(i) == '$': then it means that object at position i
 *                         is the result of a previous call to mvfromto
 *                         a copyobj is performed and itopl can
 *                         can be any used position on the stack
 *                         the object which was at position itopl
 *                         is replaced by object at position i
 *                         (and access to object itopl+1 can be lost if
 *                         the object at position i is <> from object at
 *                         position itopl
 *---------------------------------------------------------------------*/

static int C2F(mvfromto)(int *itopl,int *ix)
{
  int ix1=0;
  int m=0;
  int n=0;
  int it=0;
  int lcs=0;
  int lrs=0;
  int l=0;
  int size=0;
  int pointed=0;
  unsigned long int ilp=0;
  unsigned char Type;
  double wsave;

  Type = (unsigned char)C2F(intersci).ntypes[*ix - 1];
  if ( Type != '$')
    {
      /* int iwh = *ix + Top - Rhs;
	 ilp = iadr(*Lstk(iwh)); */
      int iwh = C2F(intersci).iwhere[*ix - 1];
      ilp = iadr(iwh);
      if ( *istk(ilp) < 0)  ilp = iadr(*istk(ilp +1));
      m =*istk(ilp +1);
      n =*istk(ilp +2);
      it = *istk(ilp +3);
    }

  switch ( Type ) {
  case 'i' :
    if (! C2F(cremat)("mvfromto", itopl, &it, &m, &n, &lrs, &lcs, 8L)) {
      return FALSE;
    }
    ix1 = m * n * (it + 1);
    C2F(stacki2d)(&ix1, &C2F(intersci).lad[*ix - 1], &lrs);
    C2F(intersci).lad[*ix - 1] = iadr(lrs);
    break ;
  case 'r' :
    if (! C2F(cremat)("mvfromto", itopl, &it, &m, &n, &lrs, &lcs, 8L)) {
      return FALSE;
    }
    ix1 = m * n * (it + 1);
    C2F(stackr2d)(&ix1, &C2F(intersci).lad[*ix - 1], &lrs);
    C2F(intersci).lad[*ix - 1] = iadr(lrs);
    break;
  case 'd' :
    if (! C2F(cremat)("mvfromto", itopl, &it, &m, &n, &lrs, &lcs, 8L)) {
      return FALSE;
    }
    /* no copy if the two objects are the same
     * the cremat above is kept to deal with possible size changes
     */
    if (C2F(intersci).lad[*ix - 1] != lrs) {
      ix1 = m * n * (it + 1);
      l=C2F(intersci).lad[*ix - 1];
      if (abs(l-lrs)<ix1)
	C2F(unsfdcopy)(&ix1, stk(l), &cx1, stk(lrs), &cx1);
      else
	C2F(dcopy)(&ix1, stk(l), &cx1, stk(lrs), &cx1);
      C2F(intersci).lad[*ix - 1] = lrs;
    }
    break;
  case 'z' :
    if ( *istk(ilp) == 133 ) {
      wsave=*stk(C2F(intersci).lad[*ix - 1]);
      n=*istk(m+1);
	  m=*istk(m);
	  it=1;
      if (! C2F(cremat)("mvfromto", itopl, &it, &m, &n, &lrs, &lcs, 8L)) {
      return FALSE;  }
      z2double(stk(C2F(intersci).lad[*ix - 1]),stk(lrs),m*n, m*n);
      *stk(lrs)=wsave;
      C2F(intersci).lad[*ix - 1] = lrs;
      }
    else {
      if (! C2F(cremat)("mvfromto", itopl, &it, &m, &n, &lrs, &lcs, 8L)) {
	return FALSE;
      }
    z2double(stk(C2F(intersci).lad[*ix - 1]), stk(lrs), m*n, m*n);
    C2F(intersci).lad[*ix - 1] = lrs;
    }
    break;
  case 'c' :
    m = *istk(ilp + 4  +1) - *istk(ilp + 4 );
    n = 1;
    ix1 = m * n;
    if (! C2F(cresmat2)("mvfromto", itopl, &ix1, &lrs, 8L)) {
      return FALSE;
    }
    C2F(stackc2i)(&ix1, &C2F(intersci).lad[*ix - 1], &lrs);
    C2F(intersci).lad[*ix - 1] = cadr(lrs);
    break;

  case 'b' :
    if (! C2F(crebmat)("mvfromto", itopl, &m, &n, &lrs, 8L)) {
      return FALSE;
    }
    ix1 = m * n;
    C2F(icopy)(&ix1, istk(C2F(intersci).lad[*ix - 1]), &cx1,istk(lrs), &cx1);
    C2F(intersci).lad[*ix - 1] = lrs;
    break;
  case '-' :
    /*    reference  '-' = ascii(45) */
    ilp = iadr(*Lstk(*ix));
    size = *istk(ilp+3);
    pointed = *istk(ilp+2);
    if (! C2F(cremat)("mvfromto", itopl, (it=0 ,&it), (m=1, &m), &size, &lrs, &lcs, 8L)) {
      return FALSE;
    }
    if ( C2F(vcopyobj)("mvfromto", &pointed, itopl, 8L) == FALSE)
	  return FALSE;
    break;
  case 'h' :
    if (! C2F(crehmat)("mvfromto", itopl, &m, &n, &lrs, 8L)) {
      return FALSE;
    }
    /* no copy if the two objects are the same
     * the cremat above is kept to deal with possible size changes
     */
    if (C2F(intersci).lad[*ix - 1] != lrs) {
      ix1 = m * n;
      l=C2F(intersci).lad[*ix - 1];
      if (abs(l-lrs)<ix1)
	C2F(unsfdcopy)(&ix1, stk(l), &cx1, stk(lrs), &cx1);
      else
	C2F(dcopy)(&ix1, stk(l), &cx1, stk(lrs), &cx1);
      C2F(intersci).lad[*ix - 1] = lrs;
    }
    break;
  case 'p' :   case '$' :
    /*     special case */
    if (Top - Rhs + *ix != *itopl)
      {
	ix1 = Top - Rhs + *ix;
	if ( C2F(vcopyobj)("mvfromto", &ix1, itopl, 8L) == FALSE)
	  return FALSE;
      }
  }
  return TRUE;
}



/*---------------------------------------------------------------------
 * copyref
 * copy object at position from to position to
 * without changing data.
 * The copy is only performed if object is a reference
 * and ref object is replaced by its value
 *---------------------------------------------------------------------*/

int Ref2val(int from , int to )
{
  int il,lw;
  lw = from + Top - Rhs;
  if ( from  > Rhs)
  {
    Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),"copyref","isref");
    return FALSE;
  }
  il = iadr(*Lstk(lw));
  if ( *istk(il) < 0)
    {
      int lwd ;
      /* from contains a reference */
      lw= *istk(il+2);
      lwd = to + Top -Rhs;
      C2F(copyobj)("copyref", &lw, &lwd, (unsigned long)strlen("copyref"));
    }
  return 0;
}

/*---------------------------------------------------------------------
 * convert2sci :
 *     this routine converts data of variable number num
 *     to scilab standard data code
 *     see how it is used in matdes.c
 *---------------------------------------------------------------------*/

int C2F(convert2sci)(int *ix)
{
  int ix1 = Top - Rhs + *ix;
  if (! C2F(mvfromto)(&ix1, ix)) return FALSE;
  C2F(intersci).ntypes[*ix - 1] = '$';
  return TRUE;
}



/*-----------------------------------------------------------
 * strcpy_tws : fname[0:nlgh-2]=' '
 * fname[nlgh-1] = '\0'
 * then second string is copied into first one
 * ------------------------------------------------------------*/

void strcpy_tws(char *str1,char *str2, int len)
{
  int i;
  for ( i =0 ; i  < (int)strlen(str2); i++ ) str1[i]=str2[i];
  for (i = (int)strlen(str2) ; i < len ; i++) str1[i]=' ';
  str1[len-1] ='\0';
}

/*---------------------------------------------------------------------
 *     conversion from Scilab code --> ascii
 *     + add a 0 at end of string
 *---------------------------------------------------------------------*/

int C2F(in2str)(int *n,int *line,char *str,unsigned long str_len)
{
  C2F(codetoascii)(n,line, str, str_len);
  str[*n] = '\0';
  return 0;
}

/*---------------------------------------------------------------------
 * Get_Iname:
 * Get the name (interfcae name) which was stored in ids while in checkrhs
 *---------------------------------------------------------------------*/

static char Fname[nlgh+1];

static char *Get_Iname()
{
  int i;
  C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], Fname, &cx1, nlgh);
  /** remove trailing blanks **/
  for (i=0 ; i < nlgh ; i++) if (Fname[i]==' ') { Fname[i]='\0'; break;}
  Fname[nlgh]='\0';
  return Fname;
}


/*---------------------------------------------------------------------
 * Utility for error message
 *---------------------------------------------------------------------*/

static char *pos[4] ={"first","second","third","fourth"};
static char arg_position[56]; /* @TODO WTF is 56 ? */

char *ArgPosition(int i)
{
	if ( i > 0 && i <= 4 ) {
		sprintf(arg_position,_("%s argument"),pos[i-1]);
	}else{
		sprintf(arg_position,_("argument number %d"),i);
	}
  return arg_position;
}

char *ArgsPosition(int i,int j)
{
  if ( i > 0 && i <= 4 )
    {
      if ( j > 0 && j <= 4 )
	sprintf(arg_position,_("%s and %s arguments"),pos[i-1],pos[j-1]);
      else
	sprintf(arg_position,_("%s argument and argument %d"),pos[i-1],j);
    }
  else
    {
      if ( j > 0 && j <= 4 )
	sprintf(arg_position,_("%s argument and argument %d"),pos[j-1],i);
      else
	sprintf(arg_position,_("arguments %d and %d"),i,j);
    }
  return arg_position;
}


/*---------------------------------------------------------------------
 * Utility for back convertion to Scilab format
 * (can be used with GetListRhsVar )
 *---------------------------------------------------------------------*/

static void ConvertData(unsigned char *type, int size,int l)
{
  int zero=0,mu=-1; int laddr; int prov,m,n,it;
  double wsave;
  switch (type[0]) {
  case 'c' :
    C2F(cvstr1)(&size,(int *) cstk(l),cstk(l),&zero,size);
    break;
  case 'r'  :
    C2F(rea2db)(&size,sstk(l),&mu,(double *)sstk(l),&mu);
    break;
  case 'i' :
    C2F(int2db)(&size,istk(l),&mu,(double *)istk(l),&mu);
    break;
  case 'z' :
    if (*istk( iadr(iadr(l))-2 ) == 133 ){  /* values @ even adress */
      prov=*istk( iadr(iadr(l))-1 );
      m=*istk(prov);n=*istk(prov+1);it=1;
      laddr=iadr(l);       wsave=*stk(laddr);
      /* make header */
      *istk( iadr(iadr(l))-2 ) = 1;
      *istk( iadr(iadr(l))-1 ) = m;
      *istk( iadr(iadr(l)) ) = n;
      *istk( iadr(iadr(l))+1 ) = it;
      /* convert values */
      z2double(stk(laddr),stk(laddr+1),m*n, m*n);
      *stk(laddr+1)=wsave;
    } else
      {
	F77ToSci((double *) zstk(l), size, size);
      }
  }
}

/*---------------------------------------------------------------------
 * Utility for checking properties
 *---------------------------------------------------------------------*/

static int check_prop(char *mes,int posi,int m)
{
  if ( m )
    {
      /* XXXX moduler 999 en fn des messages */
      Scierror(999,"%s: %s %s\n",Get_Iname(), ArgPosition(posi), mes);
      return FALSE;
    }
  return TRUE;
}

int check_square (int posi,int m,int n)
{
  return check_prop(_("should be square"),posi, m != n);
}

int check_vector (int posi,int m,int n)
{
  return check_prop(_("should be a vector"),posi, m != 1 && n != 1);
}

int check_row (int posi,int m,int n)
{
  return check_prop(_("should be a row vector"),posi, m != 1);
}

int check_col (int posi,int m,int n)
{
  return check_prop(_("should be a column vector"),posi, n != 1);
}

int check_scalar (int posi,int m,int n)
{
  return check_prop(_("should be a scalar"),posi, n != 1 || m != 1);
}

int check_dims(int posi,int m,int n,int m1,int n1)
{
  if ( m != m1 ||  n != n1 )
    {
      Scierror(999,_("%s: %s has wrong dimensions (%d,%d), expecting (%d,%d).\n"),Get_Iname(),ArgPosition(posi),m,n,m1,n1);
      return FALSE;
    }
  return TRUE;
}

int check_one_dim(int posi,int dim,int val,int valref)
{
  if ( val != valref)
    {
      Scierror(999,_("%s: %s has wrong %s dimension (%d), expecting (%d).\n"), Get_Iname(), ArgPosition(posi),  ( dim == 1 ) ? _("first") : _("second") , val,valref);
      return FALSE;
    }
  return TRUE;
}

int check_length(int posi,int m,int m1)
{
  if ( m != m1 )
    {
      Scierror(999,_("%s: %s has wrong length %d, expecting (%d).\n"), Get_Iname(), ArgPosition(posi), m, m1);
      return FALSE;
    }
  return TRUE;
}

int check_same_dims(int i,int j,int m1,int n1,int m2,int n2)
{
  if ( m1 == m2 && n1 == n2 ) return TRUE ;
  Scierror(999,_("%s: %s have incompatible dimensions (%dx%d) # (%dx%d)\n"),Get_Iname(), ArgsPosition(i,j),  m1,n1,m2,n2);
  return FALSE;
}

int check_dim_prop(int i,int j,int flag)
{
  if ( flag )
    {
      Scierror(999,_("%s: %s have incompatible dimensions.\n"), Get_Iname(), ArgsPosition(i,j));
      return FALSE;
    }
  return TRUE;
}


static int check_list_prop(char *mes, int lpos,int posi, int m)
{
  if ( m )
    {
      Scierror(999,_("%s: %s should be a list with %d-element being %s.\n"), Get_Iname(), ArgPosition(posi),posi,mes);
      return FALSE;
    }
  return TRUE;
}

int check_list_square(int lpos,int posi,int m,int n)
{
  return check_list_prop(_("square"),lpos,posi, (m != n));
}

int check_list_vector (int lpos,int posi,int m,int n)
{
  return check_list_prop(_("a vector"),lpos,posi, m != 1 && n != 1);
}

int check_list_row (int lpos,int posi,int m,int n)
{
  return check_list_prop(_("a row vector"),lpos,posi, m != 1);
}

int check_list_col (int lpos,int posi,int m,int n)
{
  return check_list_prop(_("a column vector"),lpos,posi, n != 1);
}

int check_list_scalar (int lpos,int posi,int m,int n)
{
  return check_list_prop(_("a scalar"),lpos, posi, n != 1 || m != 1);
}

int check_list_one_dim(int lpos,int posi,int dim,int val,int valref)
{
  if ( val != valref)
    {
      Scierror(999,_("%s: argument %d(%d) has wrong %s dimension (%d), expecting (%d).\n"),Get_Iname(),lpos,posi,( dim == 1 ) ? _("first") : _("second") , val,valref);
      return FALSE;
    }
  return TRUE;
}



/*---------------------------------------------------------------------
 * Utility for hand writen data extraction or creation
 *---------------------------------------------------------------------*/

int C2F(createdata)(int *lw, int n)
{
  int lw1;
  char *fname = Get_Iname();
  if (*lw > intersiz) {
    Scierror(999,_("%s: (%s) too many arguments in the stack edit stack.h and enlarge intersiz.\n"),fname,"createdata");
    return FALSE ;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  if (*lw < 0) {
    Scierror(999,_("%s: bad call to %s! (1rst argument).\n"),fname,"createdata");
    return FALSE ;
  }
  if (! C2F(credata)(fname, &lw1, n, nlgh))    return FALSE;
  C2F(intersci).ntypes[*lw - 1] = '$';
  C2F(intersci).iwhere[*lw - 1] = *Lstk(lw1);
  C2F(intersci).lad[*lw - 1] = *Lstk(lw1);
  return TRUE;
}

/*---------------------------------------------------------------------
 * copyvarfromsciptr
 *     copy a Scilab variable given by
 *      - its first adress l in stk
 *      - its size n
 *    to the variable position  lw
 *----------------------------------------------------------------------*/
int C2F(copyvarfromsciptr)(int lw, int n,int l)
{
  int ret,un=1;
  if ((ret=C2F(createdata)(&lw, n))==FALSE) return ret;
  C2F(unsfdcopy)(&n,stk(l),&un,stk(*Lstk(lw + Top - Rhs)),&un);
  return TRUE;
}

void * GetVarPtr(int n)
     /* return  the pointer on the first int of the n th variable  data structure  */

{
  int l1 = *Lstk(n + Top - Rhs);
  int *loci = (int *) stk(l1);
  return loci;
}


void *GetData(int lw)
     /* Usage: header = (int *) GetData(lw); header[0] = type of variable lw etc */
{
  int lw1 = lw + Top - Rhs ;
  int l1 = *Lstk(lw1);
  int *loci = (int *) stk(l1);
  if (loci[0] < 0)
    {
      l1 = loci[1];
      loci = (int *) stk(l1);
    }
  C2F(intersci).ntypes[lw - 1] = '$';
  C2F(intersci).iwhere[lw - 1] = l1;
  C2F(intersci).lad[lw - 1] = l1;
  return loci;
}

int GetDataSize(int lw)
     /* get memory used by the argument lw in double world etc */
{
  int lw1 = lw + Top - Rhs ;
  int l1 = *Lstk(lw1);
  int *loci = (int *) stk(l1);
  int n =  *Lstk(lw1+1)-*Lstk(lw1);
  if (loci[0] < 0)
    {
      l1 = loci[1];
      loci = (int *) stk(l1);
      n=loci[3];
    }
  return n;
}

void *GetRawData(int lw)
     /* same as GetData BUT does not go to the pointed variable if lw is a reference */
{
  int lw1 = lw + Top - Rhs ;
  int l1 = *Lstk(lw1);
  int *loci = (int *) stk(l1);
  C2F(intersci).ntypes[lw - 1] = '$';
  C2F(intersci).iwhere[lw - 1] = l1;
  /*  C2F(intersci).lad[lw - 1] = l1;  to be checked */
  return loci;
}

void *GetDataFromName( char *name )
     /* usage:  header = (int *) GetDataFromName("pipo"); header[0] = type of variable pipo etc... */
{
  void *header; int lw; int fin;
 if (C2F(objptr)(name,&lw,&fin,(unsigned long)strlen(name))) {
    header = istk( iadr(*Lstk(fin)));
    return (void *) header;
  }
 else
    {
      Scierror(999,_("GetDataFromName: variable %s not found.\n"),name);
      return (void *) 0;
    }
}

int C2F(createreference)(int number, int pointed)
/* variable number is created as a reference to variable pointed */
{
  int offset; int point_ed; int *header;
  CreateData( number, 4*sizeof(int) );
  header =  GetRawData(number);
  offset = Top -Rhs;
  point_ed = offset + pointed;
  header[0]= - *istk( iadr(*Lstk( point_ed )) );  /* reference : 1st entry (type) is opposite */
  header[1]= *Lstk(point_ed);  /* pointed adress */
  header[2]= point_ed; /* pointed variable */
  header[3]= *Lstk(point_ed + 1)- *Lstk(point_ed);  /* size of pointed variable */
  C2F(intersci).ntypes[number-1]= '-';
  return 1;
}

int C2F(changetoref)(int number, int pointed)
/* variable number is changed as a reference to variable pointed */
{
  int offset; int point_ed; int *header;
  header =  GetRawData(number);
  offset = Top - Rhs;
  point_ed = offset + pointed;
  header[0]= - *istk( iadr(*Lstk( point_ed )) );  /* reference : 1st entry (type) is opposite */
  header[1]= *Lstk(point_ed);  /* pointed adress */
  header[2]= pointed; /* pointed variable */
  header[3]= *Lstk(point_ed + 1) - *Lstk(point_ed);  /* size of pointed variable */
  C2F(intersci).ntypes[number-1]= '-';
  return 1;
}

int C2F(createreffromname)(int number, char *name)
     /* variable number is created as a reference pointing to variable "name" */
     /* name must be an existing Scilab variable */
{
  int *header; int lw; int fin;
  CreateData(number, 4*sizeof(int));
  header = (int *) GetData(number);
  if (C2F(objptr)(name,&lw,&fin,(unsigned long)strlen(name))) {
    header[0]= - *istk( iadr(*Lstk(fin))); /* type of reference = - type of pointed variable */
    header[1]= lw; /* pointed adress */
    header[2]= fin; /* pointed variable */
    header[3]= *Lstk(fin+1)- *Lstk(fin);  /*size of pointed variable */
    return 1;
  }
  else
    {
      Scierror(999,_("%s: Variable %s not found.\n"),"CreateRefFromName",name);
      return 0;
    }
}

/*-------------------------------------------------------
 * protect the intersci common during recursive calls
 *-------------------------------------------------------*/

typedef struct inter_s_ {
  int iwhere,nbrows,nbcols,itflag,ntypes,lad,ladc,lhsvar;
} intersci_state ;

typedef struct inter_l {
  intersci_state *state ;
  int nbvars;
  struct inter_l * next ;
} intersci_list ;

static intersci_list * L_intersci;


static int intersci_push(void)
{
  int i;
  intersci_list *loc;
  intersci_state *new ;
  new = MALLOC( Nbvars * sizeof(intersci_state) );
  if (new == 0 ) return 0;
  loc = MALLOC( sizeof(intersci_list) );
  if ( loc == NULL ) return 0;
  loc->next = L_intersci;
  loc->state = new;
  loc->nbvars =  Nbvars;
  for ( i = 0 ; i <  Nbvars ; i++ )
    {
      loc->state[i].iwhere = C2F(intersci).iwhere[i];
      loc->state[i].ntypes = C2F(intersci).ntypes[i];
      loc->state[i].lad    = C2F(intersci).lad[i];
      loc->state[i].lhsvar = C2F(intersci).lhsvar[i];
    }
  L_intersci = loc;
  return 1;
}

static void intersci_pop(void)
{
  int i;
  intersci_list *loc = L_intersci;
  if ( loc == NULL ) return ;
  Nbvars =  loc->nbvars;
  for ( i = 0 ; i <  Nbvars ; i++ )
    {
      C2F(intersci).iwhere[i] =   loc->state[i].iwhere ;
      C2F(intersci).ntypes[i] =   loc->state[i].ntypes ;
      C2F(intersci).lad[i] =   loc->state[i].lad    ;
      C2F(intersci).lhsvar[i] =   loc->state[i].lhsvar ;
    }
  L_intersci = loc->next ;
  FREE(loc->state);
  FREE(loc);
}

/*
static void intersci_show()
{
  int i;
  fprintf(stderr,"======================\n");
  for ( i = 0 ; i < C2F(intersci).nbvars ; i++ )
    {
      fprintf(stderr,"%d %d %d\n",i,
	      C2F(intersci).iwhere[i],
	      C2F(intersci).ntypes[i]);
    }
  fprintf(stderr,"======================\n");
}

*/

