/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Inria 
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
/*---------------------------------------------------------------------
 * Interface Library:   ilib 
 *---------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include "../stack-c.h"
#include "../sun/men_Sutils.h" 

 
/* Table of constant values */

static integer cx1 = 1;
static integer cx0 = 0;

static char *Get_Iname __PARAMS((void));
static int C2F(mvfromto) __PARAMS((integer *itopl,integer *));

static int rhs_opt_find __PARAMS((char *name,rhs_opts opts[]));
static void rhs_opt_print_names __PARAMS((rhs_opts opts[]));

/*------------------------------------------------
 * checkrhs: checks right hand side arguments 
 *-----------------------------------------------*/

int C2F(checkrhs)(fname, iMin, iMax, fname_len)
     char *fname;
     integer *iMin, *iMax;
     unsigned long fname_len;
{
  /*
   * store the name in recu array, fname can be a non null terminated char array  
   * Get_Iname() can be used in other function to get the interface name 
   */

  C2F(cvname)(&C2F(recu).ids[(C2F(recu).pt + 1) * nsiz - nsiz], fname, &cx0, fname_len);
  /* sciprint("checkrhs test : [%s] \r\n",Get_Iname()); */
  if ( Rhs < *iMin || Rhs > *iMax) 
    {
      Scierror(77,"%s: wrong number of rhs arguments\r\n",get_fname(fname,fname_len));
      return FALSE_; 
    }
  return TRUE_;
} 

/*------------------------------------------------
 * checkrhs: checks left hand side arguments 
 *-----------------------------------------------*/

int C2F(checklhs)(fname, iMin, iMax, fname_len)
     char *fname;
     integer *iMin, *iMax;
     unsigned long fname_len;
{
  if ( Lhs < *iMin || Lhs > *iMax) 
    {
      Scierror(78,"%s: wrong number of lhs arguments\r\n",get_fname(fname,fname_len));
      return FALSE_;
    }
  return TRUE_;
} 

/*---------------------------------------------------------------------
 * isopt:
 * returns the status of the variable number k
 * if its an optional variable f(x=...) 
 * returns .true. and variable name in namex
 * namex must have a size of nlgh + 1
 *---------------------------------------------------------------------*/

int C2F(isopt)(k, namex, name_len)
     integer *k;
     char *namex;
     unsigned long name_len;
{
  integer i1 =  *k + Top - Rhs;
  if ( C2F(isoptlw)(&Top, &i1, namex, name_len) == FALSE_) return FALSE_ ;
  /* add a '\0' at the end of the string removing trailing blanks */
  for ( i1 = nlgh-1 ; i1 >=0 ; i1--) { if ( namex[i1] != ' ') break ;} 
  namex[i1+1]='\0';
  return TRUE_;
} 

/*--------------------------------------- 
 * isoptlw :
 * returns the status of the variable at position lw in the stack 
 * if its an optional variable f(x=...) 
 * returns .true. and variable name in namex
 *--------------------------------------- */

int C2F(isoptlw)(topk, lw, namex, name_len)
     integer *topk, *lw;
     char *namex;
     unsigned long name_len;
{
  if (*infstk(*lw ) != 1) return FALSE_ ;
  C2F(cvname)(&C2F(vstk).idstk[(*lw) * nsiz - nsiz], namex, &cx1, name_len);
  return TRUE_;
}

/*--------------------------------------- 
 * firstopt :
 * return the position of the first optionnal argument 
 * given as xx=val in the calling sequence. 
 * If no such argument it returns Rhs+1.
 *--------------------------------------- */
integer C2F(firstopt)()

{
  integer k;
  for (k = 1; k <= Rhs ; ++k) 
    if (*infstk(k + Top - Rhs )==1)
      return k;
  return(Rhs+1);
}


/*--------------------------------------- 
 * findopt :
 * checks if option str has been passed. 
 * If yes returns the position of the variable 
 * If no  returns 0
 *--------------------------------------- */

int C2F(findopt)(str,opts)
     char * str;
     rhs_opts opts[];
{
  int i, pos;

  pos = 0;
  i = rhs_opt_find(str,opts);
  if ( i>=0 ) 
    if (opts[i].position>0) 
      pos = opts[i].position;
  
  return(pos);
}


/*--------------------------------------- 
 * numopt :
 *  returns the number of optional variables 
 *  given as xx=val in the caling sequence 
 *  top must have a correct value when using this function 
 *--------------------------------------- */

integer C2F(numopt)()
{
  integer k, ret=0;
  for (k = 1; k <= Rhs ; ++k) 
    if ( *infstk(k + Top - Rhs) == 1 ) ret++;
  return ret;
} 

/*---------------------------------------------------------------------
 * vartype:
 *   type of variable number number in the stack 
 *---------------------------------------------------------------------*/

integer C2F(vartype)(number)
     integer *number;
{
  integer ix1=  *number + Top - Rhs;
  return  C2F(gettype)(&ix1);
} 

/*------------------------------------------------
 * gettype: 
 *    returns the type of object at position lw in the stack 
 *------------------------------------------------*/

integer C2F(gettype)(lw)
     integer *lw;
{
  integer il;
  il = iadr(*lstk(*lw ));
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  return *istk(il);
}

/*------------------------------------------------
 * overloadtype:
 *    set mechanism to overloaded function fname if object type
 *    does not fit given type
 *------------------------------------------------*/

integer C2F(overloadtype)(lw,fname,typ)
     integer *lw;
     char *fname, *typ;
{
  integer il,ityp;
  il = iadr(*lstk(*lw ));
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  switch (*typ) {
  case 'c' : /* string */
  case 'S' : /* string Matrix */
    ityp=10;
    break;
  case 'd' :
  case 'i' :
  case 'r' :
  case 'z' :
    ityp=1;
    break ;
  case 'b' : /* boolean */
    ityp=4;
    break;
  case 'l' : /* list */
    ityp=15;
    break;
  case 't' : /* tlist */
    ityp=16;
    break;
  case 'm' : /* mlist */
    ityp=17;
    break;
  case 'f' : /* external */
    ityp=13;
    break;
  case 'p' : /* pointer */
    ityp=128;
    break;
  case 's' : /* sparse */
    ityp= 5;
    break;
  case 'I' : /* int matrix */
    ityp=8;
    break;

  }
  if (*istk(il ) != ityp) {
    return C2F(overload)(lw,fname,strlen(fname));
  }
  return 1;
}


/*------------------------------------------------
 * overload
 *    set mechanism to overloaded function fname for object lw
 *------------------------------------------------*/

integer C2F(overload)(lw,fname,l)
     integer *lw;
     char *fname;
     unsigned long l;
{
  C2F(putfunnam)(fname,lw,l);
  C2F(com).fun=-1;
  return 0;
}



/*------------------------------------------------
 * ogettype : unused 
 *------------------------------------------------*/

integer C2F(ogettype)(lw)
     integer *lw;
{
  return  *istk(iadr(*lstk(*lw )) );
}


/*----------------------------------------------------
 * Optional arguments f(....., arg =val,...) 
 *          in interfaces 
 * function get_optionals : example is provided in 
 *    examples/addinter-examples/intex2c.c
 *----------------------------------------------------*/


int get_optionals(fname,opts)
     char *fname ;   /* function name */
     rhs_opts opts[];/* optional arguments */
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
	  Scierror(999,"%s: optional arguments name=val must be at the end\r\n",fname);
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
	      sciprint("%s: unrecognized optional arguments %s\r\n",fname,name);
	      rhs_opt_print_names(opts) ;
	      Error(999); 
	      return(0);
	    }
	}
    }
  return 1;
}

/* Is name in opts */

int rhs_opt_find(name,opts) 
     char *name ;     /** name to be searched **/
     rhs_opts opts[]; /* array of optinal names (in alphabetical order) 
		       * the array is null terminated */
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

void rhs_opt_print_names(opts) 
     rhs_opts opts[]; /* array of optinal names (in alphabetical order) 
		       * the array is null terminated */
{
  int i=0;
  if ( opts[i].name == NULL )
    {
      sciprint("optional argument list is empty\r\n");
      return;
    }
  sciprint("optional arguments list: ");
  while ( opts[i+1].name != NULL ) 
    {
      sciprint("%s, ",opts[i].name);
      i++;
    }
  sciprint("and %s.\r\n",opts[i].name);
  return ;
}

/*---------------------------------------------------------------------
 * isref :
 *   checks if variable number lw is on the stack 
 *   or is just a reference to a variable on the stack 
 *---------------------------------------------------------------------*/

int C2F(isref)(lw)
     integer *lw;
{
  int retval;
  integer  lw1;
  retval = FALSE_;
  if (*lw > intersiz) {
    Scierror(999,"isref: too many arguments in the stack edit stack.h and enlarge intersiz\r\n");
    return retval;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  if (*lw < 0) {
    Scierror(999,"isref: bad call! (1rst argument)\r\n");
    return retval;
  }
  if ( *istk(iadr(lw1)) < 0)   retval = TRUE_;
  return retval;
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

int C2F(createvar)(lw, typex, m, n, lr, type_len)
     integer *lw;
     char *typex;
     integer *m, *n, *lr;
     unsigned long type_len;
{
  integer ix1, ix, it=0, lw1, lcs, M,N,IT,LR;
  unsigned char Type = *typex , Type1;
  char *fname = Get_Iname();
  if (*lw > intersiz) {
    Scierror(999,"%s: (createvar) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_ ;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  if (*lw < 0) {
    Scierror(999,"%s: bad call to createvar! (1rst argument)\r\n",
	     fname);
    return FALSE_ ;
  }
  switch (Type ) 
    {
    case 'c' : 
      ix1 = *m * *n;
      if (! C2F(cresmat2)(fname, &lw1, &ix1, lr, nlgh)) return FALSE_; 
      *lr = cadr(*lr);
      for (ix = 0; ix < (*m)*(*n) ; ++ix) *cstk(*lr+ix)= ' ';
      *cstk(*lr+ (*m)*(*n) )= '\0';
      Type1 = Type ; M =  *m * *n; N = 1; LR = *lr ;
      break;
    case 'd' : 
      if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh))    return FALSE_;
      Type1 = Type ; M =  *m ; N = *n; IT= 0; LR = *lr ;
      break;
    case 'z' : 
      IT = 1;
      if (! C2F(cremat)(fname, &lw1, &IT, m, n, lr, &lcs, nlgh))    return FALSE_;
      Type1 = Type ; M =  *m ; N = *n; LR = *lr ;
      C2F(intersci).ladc[*lw - 1] = *lr + M*N;
      break;
    case 'l' : 
      C2F(crelist)(&lw1, m, lr);
      Type1 = '$' ; M =  *m ; N = 1; IT= 0; LR = *lr ;
      break;
    case 't' : 
      C2F(cretlist)(&lw1, m, lr);
      Type1 = '$' ; M =  *m ; N = 1; IT= 0; LR = *lr ;
      break;
    case 'm' : 
      C2F(cremlist)(&lw1, m, lr);
      Type1 = '$' ; M =  *m ; N = 1; IT= 0; LR = *lr ;
      break;
    case 'r' : 
      if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh)) return FALSE_;
      *lr = iadr(*lr);
      Type1 = Type ; M =  *m ; N = *n; IT= 0; LR = *lr ;
      break ;
    case 'i' :
      if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh)) return FALSE_;
      *lr = iadr(*lr) ;
      Type1 = Type ; M =  *m ; N = *n; IT= 0; LR = *lr ;
      break ;
    case 'b' :
      if (! C2F(crebmat)(fname, &lw1, m, n, lr, nlgh))  return FALSE_; 
      Type1 = Type ; M =  *m ; N = *n; IT= 0; LR = *lr ;
      break;
    case 'p' : 
      if (! C2F(crepointer)(fname, &lw1, lr, nlgh))    return FALSE_;
      Type1 = '$' ; M =  1 ; N = 1; IT= 0; LR = *lr ;
      break;
    case 'I' : 
      it = *lr ; /* on entry lr gives the int type */ 
      if (! C2F(creimat)(fname, &lw1, &it, m, n, lr, nlgh))    return FALSE_;
      Type1 = '$' ; M =  *m ; N = *n; IT= 0; LR = *lr ;
      break;
    }
  C2F(intersci).ntypes[*lw - 1] = Type1;
  C2F(intersci).iwhere[*lw - 1] = *lstk(lw1);
  C2F(intersci).nbrows[*lw - 1] = M; 
  C2F(intersci).nbcols[*lw - 1] = N;
  C2F(intersci).itflag[*lw - 1] = IT;
  C2F(intersci).lad[*lw - 1] = LR;
  return TRUE_; 
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

int C2F(createcvar)(lw, typex, it, m, n, lr, lc, type_len)
     integer *lw;
     char *typex;
     integer *it, *m, *n, *lr, *lc;
     unsigned long type_len;
{
  unsigned char Type = *typex ;
  integer lw1;
  char *fname = Get_Iname();
  if (*lw > intersiz) {
    Scierror(999,"%s: (createcvar) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  if (*lw < 0) {
    Scierror(999,"%s: bad call to createcvar! (1rst argument)\r\n",
	     fname);
    return FALSE_;
  }
  switch ( Type )  {
  case 'd' : 
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, lc, nlgh))  return FALSE_;
    break;
  case 'z' : 
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, lc, nlgh))    return FALSE_;
    break;
  case 'r' : 
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, lc, nlgh))  return FALSE_;
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  case 'i' : 
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, lc, nlgh))  return FALSE_;
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  }
  C2F(intersci).ntypes[*lw - 1] = Type;
  C2F(intersci).iwhere[*lw - 1] = *lstk(lw1);
  C2F(intersci).nbrows[*lw - 1] = *m;
  C2F(intersci).nbcols[*lw - 1] = *n;
  C2F(intersci).itflag[*lw - 1] = *it;
  C2F(intersci).lad[*lw - 1] = *lr;
  C2F(intersci).ladc[*lw - 1] = *lc;
  return TRUE_;
} 

/*---------------------------------------------------------------------
 *     create a variable number lw on the stack of type 
 *     list with nel elements 
 *---------------------------------------------------------------------*/

int C2F(createlist)(lw, nel)
     integer *lw, *nel;
{
  char *fname = Get_Iname();
  integer lr, lw1;
  if (*lw > intersiz) {
    Scierror(999,"%s: (createlist) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  if (*lw < 0) {
    Scierror(999,"%s: bad call to createlist! (1rst argument)\r\n",fname);
    return FALSE_;
  }
  C2F(crelist)(&lw1, nel, &lr);
  C2F(intersci).ntypes[*lw - 1] = '$';
  C2F(intersci).iwhere[*lw - 1] = *lstk(lw1);
  C2F(intersci).nbrows[*lw - 1] = *nel;
  C2F(intersci).nbcols[*lw - 1] = 1;
  C2F(intersci).lad[*lw - 1] = lr;
  return TRUE_;
} 

/*---------------------------------------------------------------------
 *     create a variable number lw on the stack of type 
 *     type and size m,n 
 *     the argument must be of type type ('c','d','r','i','b') 
 *     return values m,n,lr,lar 
 *     lar is also an input value 
 *     if lar != -1 var is filled with data stored at lar 
 *---------------------------------------------------------------------*/

int C2F(createvarfrom)(lw, typex, m, n, lr, lar, type_len)
     integer *lw;
     char *typex;
     integer *m, *n, *lr, *lar;
     unsigned long type_len;
{
  int M=*m,N=*n,MN=M*N;
  unsigned char Type = *typex;
  integer inc=1;
  integer it=0, lw1, lcs;
  char *fname = Get_Iname();
  if (*lw > intersiz) {
    Scierror(999,"%s: (createvarfrom) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  if (*lw < 0) {
    Scierror(999,"%s: bad call to createvarfrom! (1rst argument)\r\n",
	     fname);
    return FALSE_;
  }
  switch ( Type ) {
  case 'c' :
    if (! C2F(cresmat2)(fname, &lw1, &MN, lr, nlgh)) return FALSE_;
    if (*lar != -1)  C2F(cvstr1)(&MN, istk(*lr), cstk(*lar), &cx0,  MN + 1);
    *lar = *lr;
    *lr = cadr(*lr);
    M=MN; N= 1;
    break;
  case 'd' :
    if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh))  return FALSE_;
    if (*lar != -1)  C2F(dcopy)(&MN, stk(*lar), &cx1, stk(*lr), &cx1);
    *lar = *lr;
    break;
  case 'r' :
    if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh)) return FALSE_;
    if (*lar != -1)   C2F(rea2db)(&MN, sstk(*lar), &cx1, stk(*lr), & cx1);
    *lar = *lr;
    *lr = iadr(*lr);
    break;
  case 'i' :
    if (! C2F(cremat)(fname, &lw1, &it, m, n, lr, &lcs, nlgh)) return FALSE_;
    if (*lar != -1) C2F(int2db)(&MN,istk(*lar), &cx1, stk(*lr), &cx1);
    *lar = *lr;
    *lr = iadr(*lr);
    break;
  case 'b' :
    if (! C2F(crebmat)(fname, &lw1, m, n, lr, nlgh)) return FALSE_;
    if (*lar != -1) C2F(icopy)(&MN, istk(*lar), &cx1, istk(*lr), &cx1);
    *lar = *lr;
    break;
  case 'I' :
    it = *lr;
    if (! C2F(creimat)(fname, &lw1, &it, m, n, lr,  nlgh))  return FALSE_;
    if (*lar != -1) 
      C2F(tpconv)(&it,&it,&MN,istk(*lar), &inc,istk(*lr), &inc);
    *lar = *lr;
    break;
  case 'p' :
    MN=1;
    if (! C2F(crepointer)(fname, &lw1, lr, nlgh))    return FALSE_;
    if (*lar != -1)  C2F(dcopy)(&MN, stk(*lar), &cx1, stk(*lr), &cx1);
    *lar = *lr;
    break;
  }
  C2F(intersci).ntypes[*lw - 1] = '$';
  C2F(intersci).iwhere[*lw - 1] = *lstk(lw1);
  C2F(intersci).nbrows[*lw - 1] = M;
  C2F(intersci).nbcols[*lw - 1] = N;
  C2F(intersci).itflag[*lw - 1] = 0;
  C2F(intersci).lad[*lw - 1] = *lr;
  return TRUE_;
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

int C2F(createcvarfrom)(lw, typex, it, m, n, lr, lc, lar, lac, type_len)
     integer *lw;
     char *typex;
     integer *it, *m, *n, *lr, *lc, *lar, *lac;
     unsigned long type_len;
{
  unsigned char Type = *typex;
  int MN;
  integer lw1, lcs;
  char *fname =     Get_Iname();
  if (*lw > intersiz) {
    Scierror(999,"%s: (createcvarfrom) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  MN = (*m)*(*n);
  if (*lw < 0) {
    Scierror(999,"%s: bad call to createcvarfrom! (1rst argument)\r\n",
	     fname);
    return FALSE_;
  }
  switch ( Type ) {
  case 'd' : 
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, lc, nlgh)) return FALSE_;
    if (*lar != -1) C2F(dcopy)(&MN, stk(*lar), &cx1,stk(*lr) , &cx1);
    if (*lac != -1 && *it == 1) C2F(dcopy)(&MN, stk(*lac), &cx1,stk(*lc) , &cx1);
    *lar = *lr;
    *lac = *lc;
    break;
  case 'r' :
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, lc, nlgh)) return FALSE_;
    if (*lar != -1) C2F(rea2db)(&MN, sstk(*lar), &cx1, stk(*lr), &cx1);
    if (*lac != -1 && *it==1) C2F(rea2db)(&MN, sstk(*lac), &cx1, stk(*lc), &cx1);
    *lar = *lr;
    *lac = *lc;
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break ;
  case 'i' :
    if (! C2F(cremat)(fname, &lw1, it, m, n, lr, &lcs, nlgh)) return FALSE_;
    if (*lar != -1) C2F(int2db)(&MN, istk(*lar), &cx1, stk(*lr), & cx1);
    if (*lac != -1 && (*it==1)) C2F(int2db)(&MN, istk(*lac), &cx1, stk(*lc), &cx1);
    *lar = *lr;
    *lac = *lc;
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  }
  C2F(intersci).ntypes[*lw - 1] = '$';
  C2F(intersci).iwhere[*lw - 1] = *lstk(lw1);
  C2F(intersci).nbrows[*lw - 1] = *m;
  C2F(intersci).nbcols[*lw - 1] = *n;
  C2F(intersci).itflag[*lw - 1] = *it;
  C2F(intersci).lad[*lw - 1] = *lr;
  C2F(intersci).ladc[*lw - 1] = *lc;
  return TRUE_;
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

int C2F(createlistvarfrom)(lnumber, number, typex, m, n, lr, lar, type_len)
     integer *lnumber, *number;
     char *typex;
     integer *m, *n, *lr, *lar;
     unsigned long type_len;
{
  unsigned Type = *typex;
  integer lc, ix1, it = 0, mn = (*m)*(*n),inc=1;
  char *fname = Get_Iname();
  if (*lnumber > intersiz) {
    Scierror(999,"%s: (createlistvar) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }
  switch ( Type ) {
  case 'c' :
    *n = 1;
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcrestring)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1], m, lr, nlgh)) {
      return FALSE_;
    }
    if (*lar != -1) C2F(cvstr1)(m, istk(*lr), cstk(*lar), &cx0,  *m * *n + 1);
    *lar = *lr;
    *lr = cadr( *lr);
    break;
  case 'd' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, lr, &lc, nlgh)) {
      return FALSE_;
    }
    if (*lar != -1) C2F(dcopy)(&mn, stk(*lar), &cx1,stk(*lr) , &cx1);
    *lar = *lr;
    break;
  case 'r' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, lr, &lc, nlgh)) {
      return FALSE_;
    }
    if (*lar != -1) 	C2F(rea2db)(&mn, sstk(*lar), &cx1, stk(*lr), &cx1);
    *lar = *lr;
    *lr = iadr(*lr);
    break;
  case 'i' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, lr, &lc, nlgh)) {
      return FALSE_;
    }
    if (*lar != -1) 	C2F(int2db)(&mn, istk(*lar), &cx1, stk(*lr), &cx1);
    *lar = *lr;
    *lr = iadr(*lr);
    break;
  case 'b' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcrebmat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1]
			   , m, n, lr, nlgh)) {
      return FALSE_;
    }
    if (*lar != -1) C2F(icopy)(&mn, istk(*lar), &cx1, istk(*lr), &cx1);
    *lar = *lr;
    break;
  case 'I' : 
    it = *lr ; /* it gives the type on entry */
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcreimat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, lr, nlgh)) {
      return FALSE_;
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
	return FALSE_;
      }
    if (*lar != -1) *stk(*lr)= *stk(*lar);
    *lar = *lr;
    break;
  default :
    Scierror(999,"%s: (createlistvar) bad third argument!\r\n",fname);
    return FALSE_;
    break;
  }
  return TRUE_;
}



/*---------------------------------------------------------------------
 * create a complex list variable from data 
 *---------------------------------------------------------------------*/

int C2F(createlistcvarfrom)(lnumber, number, typex, it, m, n, lr, lc, lar, lac, 
			type_len)
     integer *lnumber, *number;
     char *typex;
     integer *it, *m, *n, *lr, *lc, *lar, *lac;
     unsigned long type_len;
{
  integer ix1;
  int mn = (*m)*(*n);
  unsigned char Type = *typex;
  char *fname = Get_Iname();

  if (*lnumber > intersiz) {
    Scierror(999,"%s: (createlistcvar) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname) ;
    return FALSE_;
  }

  switch ( Type ) { 
  case 'd' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],it, m, n, lr, lc, nlgh)) 
      return FALSE_;
    if (*lar != -1) C2F(dcopy)(&mn,  stk(*lar), &cx1, stk(*lr), &cx1);
    if (*lac != -1 && *it==1) C2F(dcopy)(&mn, stk(*lac), &cx1,stk(*lc) , &cx1);
    *lar = *lr;
    *lac = *lc;
    break;
  case 'r' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  it, m, n, lr, lc, nlgh)) 
      return FALSE_;
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
      return FALSE_;
    if (*lar != -1) C2F(int2db)(&mn,istk(*lar), &cx1, stk(*lr), &cx1);
    if (*lac != -1 && *it==1) C2F(int2db)(&mn, istk(*lac), &cx1, stk(*lc), &cx1);
    *lar = *lr;
    *lac = *lc;
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  default :
    Scierror(999,"%s:  createlistcvar called with bad third argument!\r\n",fname);
    return FALSE_;
  }
  return TRUE_;
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

int C2F(createlistvarfromptr)(lnumber, number, typex, m, n, iptr, type_len)
     integer *lnumber, *number;
     char *typex;
     integer *m, *n;
     void *iptr;
     unsigned long type_len;
{
  unsigned Type = *typex;
  integer lc, ix1, it = 0, lr,inc=1;
  char *fname = Get_Iname();
  if (*lnumber > intersiz) {
    Scierror(999,"%s: (createlistvarfromptr) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }
  switch ( Type ) {
  case 'c' :
    *n = 1;
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcrestring)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1], m, &lr, nlgh)) {
      return FALSE_;
    }
    C2F(cchar)(m,(char **) iptr, istk(lr));
    break;
  case 'd' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, &lr, &lc, nlgh)) {
      return FALSE_;
    }
    ix1= (*m)*(*n);
    C2F(cdouble) (&ix1,(double **) iptr, stk(lr));
    break;
  case 'r' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, &lr, &lc, nlgh)) {
      return FALSE_;
    }
    ix1= (*m)*(*n);
    C2F(cfloat) (&ix1,(float **) iptr, stk(lr));
    break;
  case 'i' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, &lr, &lc, nlgh)) {
      return FALSE_;
    }
    ix1 = *m * *n;
    C2F(cint)(&ix1,(int **) iptr, stk(lr));
    break;
  case 'b' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcrebmat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1]
			   , m, n, &lr, nlgh)) {
      return FALSE_;
    }
    ix1 = *m * *n;
    C2F(cbool)(&ix1,(int **) iptr, istk(lr));
    break;
  case 'S' : 
    ix1 = *lnumber + Top - Rhs;
    if ( !cre_listsmat_from_str(fname,&ix1, number, &C2F(intersci).lad[*lnumber - 1]
				, m, n, (char **) iptr, nlgh)) /* XXX */
      return FALSE_;
    break;
  case 's' :
    ix1 = *lnumber + Top - Rhs;
    if ( !cre_listsparse_from_ptr(fname,&ix1,number,
			      &C2F(intersci).lad[*lnumber - 1]
			      , m, n, (SciSparse *) iptr, nlgh))
      return FALSE_;
    break;
  case 'I' :
    it = ((SciIntMat *) iptr)->it;
    if (! C2F(listcreimat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  &it, m, n, &lr, nlgh)) {
      return FALSE_;
    }
    ix1= (*m)*(*n);
    C2F(tpconv)(&it,&it,&ix1,((SciIntMat *) iptr)->D, &inc,istk(lr), &inc);
    break;
  case 'p' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcrepointer)(fname, &ix1, number, 
			      &C2F(intersci).lad[*lnumber - 1],&lr,nlgh)) 
      {
	return FALSE_;
      }
    *stk(lr) = (double) ((unsigned long int) iptr);
    break;
  default :
    Scierror(999,"%s: (createlistvarfromptr) bad third argument!\r\n",fname);
    return FALSE_;
    break;
  }
  return TRUE_;
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

int C2F(createlistcvarfromptr)(lnumber, number, typex,it, m, n, iptr, iptc, type_len)
     integer *lnumber, *number;
     char *typex;
     integer *m, *n, *it;
     void *iptr,*iptc;
     unsigned long type_len;
{
  unsigned Type = *typex;
  integer lr,lc, ix1;
  char *fname = Get_Iname();
  if (*lnumber > intersiz) {
    Scierror(999,"%s: (createlistvarfromptr) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }
  switch ( Type ) {
  case 'd' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  it, m, n, &lr, &lc, nlgh)) {
      return FALSE_;
    }
    ix1= (*m)*(*n);
    C2F(cdouble) (&ix1,(double **) iptr, stk(lr));
    if ( *it == 1) C2F(cdouble) (&ix1,(double **) iptc, stk(lc));
    break;
  case 'r' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  it, m, n, &lr, &lc, nlgh)) {
      return FALSE_;
    }
    ix1= (*m)*(*n);
    C2F(cfloat) (&ix1,(float **) iptr, stk(lr));
    if ( *it == 1)     C2F(cfloat) (&ix1,(float **) iptc, stk(lc));
    break;
  case 'i' :
    ix1 = *lnumber + Top - Rhs;
    if (! C2F(listcremat)(fname, &ix1, number, &C2F(intersci).lad[*lnumber - 1],
			  it, m, n, &lr, &lc, nlgh)) {
      return FALSE_;
    }
    ix1 = *m * *n;
    C2F(cint)(&ix1,(int **) iptr, stk(lr));
    if ( *it == 1)     C2F(cint)(&ix1,(int **) iptc, stk(lc));
    break;
  default :
    Scierror(999,"%s: (createlistcvarfromptr) bad third argument!\r\n",fname);
    return FALSE_;
    break;
  }
  return TRUE_;
}


/*---------------------------------------------------------------------
 * getmatdims :
 *     check if argument number <<number>> is a matrix and 
 *     returns its dimensions
 *---------------------------------------------------------------------*/
int C2F(getmatdims)(number, m, n)
     integer *number, *m, *n;
{
  char *fname = Get_Iname();
  integer il,lw,typ;

  lw = *number + Top - Rhs;
  if ( *number > Rhs) {
    Scierror(999,"%s: bad call to getmatdims! (1rst argument)\r\n",fname);
    return FALSE_;
  }

  il = iadr(*lstk(lw));
  if (*istk(il ) < 0) il = iadr(*istk(il +1));
  typ = *istk(il );
  if (typ > 10) {
    Scierror(201,"%s: argument %d should be a matrix\r\n", fname,*number);
    return  FALSE_;    
  }
  *m = *istk(il + 1);
  *n = *istk(il + 2);
  return TRUE_;
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

int C2F(getrhsvar)(number, typex, m, n, lr, type_len)
     integer *number;
     char *typex;
     integer *m, *n, *lr;
     unsigned long type_len;
{
  int ierr=0, il1,ild1,nn;
  int lrr;
  char *fname = Get_Iname();
  char **items, namex[nlgh+1];
  unsigned char Type = *(unsigned char *) typex;
  integer topk,ltype, m1, n1, lc,lr1, it=0, lw, ile, ils, ix1,ix2;
  integer mnel,icol;
  SciSparse *Sp;
  SciIntMat *Im;
  /* we accept a call to getrhsvar after a createvarfromptr call */
  if ( *number > Rhs && *number > Nbvars ) {
    Scierror(999,"%s: bad call to getrhsvar! (1rst argument)\r\n",fname);
    return FALSE_;
  }

  Nbvars = Max(Nbvars,*number);
  lw = *number + Top - Rhs;

  if (*number > intersiz) {
    Scierror(999,"%s: (getrhsvar) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }

  if (  C2F(overloadtype)(&lw,fname,&Type) == 0) return FALSE_;
  
  topk = Top;
  switch ( Type ) 
    {
    case 'c' : 
      *n = 1;
      if (! C2F(getsmat)(fname,&topk,&lw,&m1,&n1,&cx1,&cx1,lr,m, nlgh))
	return FALSE_;
      ix2 = *m * *n;
      /* in case where ix2 is 0 in2str adds the \0 char after the end of
         the storage of the variable, so it writes over the next variable 
         tp avoid this pb we shift up by one the location where the 
         data is written*/
      lrr=*lr;
      if (ix2==0) lrr--;

      C2F(in2str)(&ix2, istk(*lr), cstk(cadr(*lr)), ix2 + 1);
      *lr = cadr(*lr);
      break;

    case 'd' :
      if (! C2F(getmat)(fname, &topk, &lw, &it, m, n, lr, &lc, nlgh)) return FALSE_;
      break ;
    case 'z' :
      if (! C2F(getmat)(fname, &topk, &lw, &it, m, n, lr, &lc, nlgh)) return FALSE_;
      ix2 = *m * *n;
      if (it != 1) {
	Scierror(999," Waiting for a complex argument (z)"); return FALSE_;
      };
      SciToF77(stk(*lr), ix2, ix2);
      break ;
    case 'r' :
      if (! C2F(getmat)(fname, &topk, &lw, &it, m, n, lr, &lc, nlgh))  return FALSE_;
      ix1 = *m * *n;
      C2F(simple)(&ix1, stk(*lr), sstk(iadr(*lr)));
      *lr = iadr(*lr);
      break;
    case 'i' :
      if (! C2F(getmat)(fname, &topk, &lw, &it, m, n, lr, &lc, nlgh)) return FALSE_;
      ix1 = *m * *n;
      C2F(entier)(&ix1, stk(*lr), istk(iadr(*lr)));
      *lr = iadr(*lr) ;
      break;
    case 'b' : 
      if (! C2F(getbmat)(fname, &topk, &lw, m, n, lr, nlgh))  return FALSE_;
      break;
    case 'l' :
    case 't' :
    case 'm' :
      *n = 1;
      if (! C2F(getilist)(fname, &topk, &lw, m, n, lr, nlgh))  return FALSE_;
      /* No data conversion for list members ichar(type)='$' */
      Type = '$' ;
      break;
    case 'S' :
      /** getwsmat : must be back in stack1.c from xawelm.f */
      if (! C2F(getwsmat)(fname,&topk,&lw,m,n,&il1,&ild1, nlgh)) return FALSE_;
      nn= (*m)*(*n);
      ScilabMStr2CM(istk(il1),&nn,istk(ild1),&items,&ierr);
      if ( ierr == 1) return FALSE_;
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
      if (! C2F(getsparse)(fname,&topk,&lw,&it,m,n,&(Sp->nel),&mnel,&icol,&lr1,&lc,nlgh))
	return FALSE_;
      Sp->m = *m ; Sp->n = *n ; Sp->it = it; 
      Sp->mnel = istk(mnel);
      Sp->icol = istk(icol);
      Sp->R = stk(lr1);
      Sp->I = (it==1) ? stk(lc): NULL;
      Type = '$';
      break;
    case 'I' :
      /* int matrices */ 
      Im = (SciIntMat *) lr ;
      if (! C2F(getimat)(fname,&topk,&lw,&it,m,n,&lr1,nlgh))
	return FALSE_;
      Im->m = *m ; Im->n = *n ; Im->it = it; Im->l = lr1;
      Im->D = istk(lr1);
      Type = '$';
      break;
    case 'f' :
      /* XXXXXX : gros bug ici car getexternal a besoin de savoir 
	 pour quelle fonction on recupere un external 
	 or ici on presuppose que c'est setfeval 
	 de plus on ne sait pas exactement de quel type d'external il s'agit
      */
	 
      /*      int function getrhsvar(number,type,m,n,lr) */
      *lr = *lstk(lw);
      ils = iadr(*lr) + 1;
      *m = *istk(ils);
      ile = ils + *m * nsiz + 1;
      *n = *istk(ile);
      if (! C2F(getexternal)(fname, &topk, &lw, namex, &ltype, C2F(setfeval), nlgh, 
			     nlgh)) {
	return FALSE_;
      }
      Type = '$';
      break;
    case 'p' : 
      if (! C2F(getpointer)(fname, &topk, &lw,lr, nlgh)) return FALSE_;
      break;
    }
  C2F(intersci).ntypes[*number - 1] = Type ;
  C2F(intersci).iwhere[*number - 1] = *lstk(lw);
  C2F(intersci).nbrows[*number - 1] = *m;
  C2F(intersci).nbcols[*number - 1] = *n;
  C2F(intersci).itflag[*number - 1] = it;
  C2F(intersci).lad[*number - 1] = *lr;
  return TRUE_;
} 


/*---------------------------------------------------------------------
 * getrhsvcar :
 *     get the argument number <<number>> 
 *     the argument must be of type type ('d','r','i') 
 *     like getrhsvar but for complex matrices 
 *---------------------------------------------------------------------*/

int C2F(getrhscvar)(number, typex, it, m, n, lr, lc, type_len)
     integer *number;
     char *typex;
     integer *it, *m, *n, *lr, *lc;
     unsigned long type_len;
{
  integer ix1, lw, topk, ix2;
  unsigned char Type = *typex;
  char *fname = Get_Iname();
  
  Nbvars = Max(Nbvars,*number);
  lw = *number + Top - Rhs;
  if (*number > Rhs) {
    Scierror(999,"%s: bad call to getrhscvar! (1rst argument)\r\n", fname);
    return FALSE_;
  }
  if (*number > intersiz) {
    Scierror(999,"%s: (getrhscvar) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }
  topk = Top;
  switch ( Type ) {
  case 'd' :
    if (! C2F(getmat)(fname, &topk, &lw, it, m, n, lr, lc, nlgh)) return FALSE_;
    break;
  case 'z' :
    if (! C2F(getmat)(fname, &topk, &lw, it, m, n, lr, lc, nlgh)) return FALSE_;
    ix2 = *m * *n;
    if (*it != 1) {
      Scierror(999," Waiting for a complex argument (z)"); return FALSE_;
    };
    SciToF77(stk(*lr), ix2, ix2);
    break ;
  case 'r' :
    if (! C2F(getmat)(fname, &topk, &lw, it, m, n, lr, lc, nlgh)) return FALSE_;
    ix1 = *m * *n * (*it + 1);
    C2F(simple)(&ix1, stk(*lr), sstk(iadr(*lr)));
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  case 'i' :
    if (! C2F(getmat)(fname, &topk, &lw, it, m, n, lr, lc, nlgh)) return FALSE_;
    ix1 = *m * *n * (*it + 1);
    C2F(entier)(&ix1, stk(*lr), istk(iadr(*lr)));
    *lr = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  }
  C2F(intersci).ntypes[*number - 1] = Type;
  C2F(intersci).iwhere[*number - 1] = *lstk(lw);
  C2F(intersci).nbrows[*number - 1] = *m;
  C2F(intersci).nbcols[*number - 1] = *n;
  C2F(intersci).itflag[*number - 1] = *it;
  C2F(intersci).lad[*number - 1] = *lr;
  C2F(intersci).ladc[*number - 1] = *lc;
  return TRUE_;
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

int C2F(getlistrhsvar)(lnumber, number, typex, m, n, lr, type_len)
     integer *lnumber, *number;
     char *typex;
     integer *m, *n, *lr;
     unsigned long type_len;
{
  int lr1;
  char **items;
  int il1,ild1,nn,ierr=0;
  char *fname = Get_Iname();
  integer m1, n1, lc, it, lw, topk = Top, ix1,ix2;
  unsigned char Type = *typex;   
  integer mnel,icol;
  SciSparse *Sp;
  SciIntMat *Im;

  Nbvars = Max(Nbvars,*lnumber);
  lw = *lnumber + Top - Rhs;
  if (*lnumber > Rhs) {
    Scierror(999,"%s: bad call to getlistrhsvar! (1rst argument)\r\n",fname);
    return FALSE_;
  }
  if (*lnumber > intersiz) {
    Scierror(999,"%s: (getlistrhsvar) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }

  switch ( Type ) { 
  case 'c' : 
    *n = 1;
    if (! C2F(getlistsimat)(fname, &topk, &lw, number, &m1, &n1, &cx1, &cx1,lr, m, nlgh)) 
      return FALSE_;
    ix2 = *m * *n;
    C2F(in2str)(&ix2, istk(*lr), cstk(cadr(*lr)), ix2 + 1);
    *lr = cadr(*lr);
    break;
  case 'd' : 
    if (! C2F(getlistmat)(fname, &topk, &lw, number, &it, m, n, lr, &lc, nlgh))
      return FALSE_;
    break;
  case 'r' :
    if (! C2F(getlistmat)(fname, &topk, &lw, number, &it, m, n, lr, &lc, nlgh))
      return FALSE_;
    ix1 = *m * *n;
    C2F(simple)(&ix1, stk(*lr), sstk(iadr(*lr)));
    *lr = iadr(*lr);
    break;
  case 'i' :
    if (! C2F(getlistmat)(fname, &topk, &lw, number, &it, m, n, lr, &lc, nlgh))
      return FALSE_;
    ix1 = *m * *n;
    C2F(entier)(&ix1, stk(*lr), istk(iadr(*lr)));
    *lr = iadr(*lr);
    break;
  case 'b' :
    if (! C2F(getlistbmat)(fname, &topk, &lw, number, m, n, lr, nlgh)) 
      return FALSE_;
    *lr = *lr;
    break;
  case 'z' :
    if (! C2F(getlistmat)(fname, &topk, &lw,number, &it, m, n, lr, &lc, nlgh)) return FALSE_;
    ix1 = *m * *n;
    if (it != 1) {
      Scierror(999,"%s: argument %d >(%d) should be a complex matrix\r\n",
	       fname, Rhs + (lw -topk) , *number);
      return FALSE_;
    };
    SciToF77(stk(*lr), ix1, ix1);
    break;
  case 'S' :
    /** getwsmat : must be back in stack1.c from xawelm.f */
    if (! C2F(getlistwsmat)(fname,&topk,&lw,number,m,n,&il1,&ild1, nlgh)) return FALSE_;
    nn= (*m)*(*n);
    ScilabMStr2CM(istk(il1),&nn,istk(ild1),&items,&ierr);
    if ( ierr == 1) return FALSE_;
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
      return FALSE_;
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
      return FALSE_;
    Im->m = *m ; Im->n = *n ; Im->it = it; Im->l = lr1;
    Im->D = istk(lr1);
    Type = '$';
    break;
  case 'p' :
    if (! C2F(getlistpointer)(fname, &topk, &lw, number, lr,  nlgh))
      return FALSE_;
    break;
  default :
    Scierror(999,"%s: getlistrhsvar was called with bad third argument (%c)\r\n",fname,Type);
    return FALSE_;
  }
  /* can't perform back data conversion with lists */
  C2F(intersci).ntypes[*number - 1] = '$';
  return TRUE_ ; 
}
  
/*---------------------------------------------------------------------
 * for complex 
 *---------------------------------------------------------------------*/

int C2F(getlistrhscvar)(lnumber, number, typex, it, m, n, lr, lc, type_len)
     integer *lnumber, *number;
     char *typex;
     integer *it, *m, *n, *lr, *lc;
     unsigned long type_len;
{
  integer ix1, topk= Top, lw;
  char *fname = Get_Iname();
  unsigned  char    Type = * typex;

  Nbvars = Max(Nbvars,*lnumber);
  lw = *lnumber + Top - Rhs;
  if (*lnumber > Rhs) {
    Scierror(999,"%s: bad call to getlistrhscvar! (1rst argument)\r\n",fname);
    return FALSE_;
  }
  if (*lnumber > intersiz) {
    Scierror(999,"%s: (getlistrhscvar) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }
  switch ( Type ) {
  case 'd' :
    if (! C2F(getlistmat)(fname, &topk, &lw, number, it, m, n, lr, lc, nlgh)) return FALSE_;
    break;
  case 'r' :
    if (! C2F(getlistmat)(fname, &topk, &lw, number, it, m, n, lr, lc, nlgh)) 
      return FALSE_;
    ix1 = *m * *n * (*it + 1);
    C2F(simple)(&ix1, stk(*lr), sstk(iadr(*lr)));
    *lr  = iadr(*lr);
    *lc = *lr + *m * *n;
    break;
  case 'i' :
    if (! C2F(getlistmat)(fname, &topk, &lw, number, it, m, n, lr, lc, nlgh)) 
      return FALSE_;
    ix1 = *m * *n * (*it + 1);
    C2F(entier)(&ix1, stk(*lr), istk(iadr(*lr)));
    *lr = iadr(*lr) ;
    *lc = *lr + *m * *n;
    break;
  default :
    Scierror(999,"%s: getlistrhscvar was called with  bad third argument!\r\n",fname);
    return FALSE_;
  }
  /* can't perform back data conversion with lists */
  C2F(intersci).ntypes[*number - 1] = '$';
  return TRUE_;
}

/*---------------------------------------------------------------------
 *     creates variable number number of type "type" and dims m,n 
 *     from pointer ptr 
 *     
 *---------------------------------------------------------------------*/

int C2F(createvarfromptr)(number, typex, m, n, iptr, type_len)
     integer *number;
     char *typex;
     integer *m, *n;
     void *iptr;
     unsigned long type_len;
{
  static int un=1;
  unsigned char Type = *typex;
  int MN= (*m)*(*n),lr,it,lw1;
  char *fname = Get_Iname();
  lw1 = *number + Top - Rhs;
  switch ( Type ) 
    {
    case 'd' :
      if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE_ ) return FALSE_;
      C2F(dcopy)(&MN,*((double **) iptr),&un, stk(lr),&un);
      break;
    case 'i' :
    case 'b' :
      if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE_ ) return FALSE_;
      C2F(icopy)(&MN,*((int **) iptr), &un, istk(lr), &un);
      break;
    case 'r' :
      if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE_ ) return FALSE_;
      C2F(rcopy)(&MN,*((float **)iptr), &un, sstk(lr), &un);
      break;
    case 'c' :
      if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE_ ) return FALSE_;
      strcpy(cstk(lr),*((char **) iptr));
      break;
    case 'I' :
      /* on entry lr must gives the int type */
      it = lr = ((SciIntMat *) iptr)->it;
      if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE_ ) return FALSE_;
      C2F(tpconv)(&it,&it,&MN,((SciIntMat *) iptr)->D, &un,istk(lr), &un);
      break;
    case 'p' :
      if ( C2F(createvar)(number, typex, m, n, &lr, type_len) == FALSE_ ) return FALSE_;
      *stk(lr) = (double) ((unsigned long int) iptr);
      break;
    case 'S' :
      /* special case: not taken into account in createvar */
      Nbvars = Max(*number,Nbvars);
      if ( !cre_smat_from_str(fname,&lw1, m, n, (char **) iptr, nlgh)) 
	return FALSE_;
      C2F(intersci).ntypes[*number - 1] = '$';
      break;
    case 's' :
      /* special case: not taken into account in createvar */
      Nbvars = Max(*number,Nbvars);
      if ( !cre_sparse_from_ptr(fname,&lw1, m, n, (SciSparse *) iptr, nlgh))
	return FALSE_;
      C2F(intersci).ntypes[*number - 1] = '$';
      break;
    default :
      Scierror(999,"%s: createvarfromptr was called with bad second argument!\r\n",fname);
      return FALSE_;
    }
  /*     this object will be copied with a vcopyobj in putlhsvar */
  return TRUE_; 
} 



/*---------------------------------------------------------------------
 *     for complex 
 *---------------------------------------------------------------------*/

int C2F(createcvarfromptr)(number, typex, it, m, n, iptr, iptc, type_len)
     integer *number;
     char *typex;
     integer *it, *m, *n;
     double *iptr, *iptc;
     unsigned long type_len;
{
  unsigned char Type = *typex;
  char *fname = Get_Iname();
  integer lw1, lcs, lrs, ix1;

  Nbvars = Max(Nbvars,*number);
  if (*number > intersiz) {
    Scierror(999,"%s: createcvarfromptr: too many arguments on the stack, enlarge intersiz\r\n",
	     fname);
    return FALSE_;
  }
  lw1 = *number + Top - Rhs;
  switch ( Type ) {
  case 'd' : 
    if (! C2F(cremat)(fname, &lw1, it, m, n, &lrs, &lcs, nlgh)) 
      return FALSE_;
    ix1 = *m * *n;
    C2F(cdouble)(&ix1, (double **) iptr, stk(lrs));
    if (*it == 1) {
      ix1 = *m * *n;
      C2F(cdouble)(&ix1,(double **) iptc, stk(lcs));
    }
    break;
  case 'i' :
    if (! C2F(cremat)(fname, &lw1, it, m, n, &lrs, &lcs, nlgh)) 
      return FALSE_;
    ix1 = *m * *n;
    C2F(cint)(&ix1, (int **) iptr, stk(lrs));
    if (*it == 1) {
      ix1 = *m * *n;
      C2F(cint)(&ix1,(int **) iptc, stk(lcs));
    }
    break;
  default :
    Scierror(999,"%s: createcvarfromptr was called with bad second argument!\r\n",fname);
    return FALSE_;
  }
  /*     this object will be copied with a vcopyobj in putlhsvar */
  C2F(intersci).ntypes[*number - 1] = '$';
  return  TRUE_;
} 

/*---------------------------------------------------------------------
 * mklistfromvars : 
 *     replace the last n variables created at postions pos:pos-1+n 
 *     by a list of these variables at position pos 
 *---------------------------------------------------------------------*/

int C2F(mklistfromvars)(pos, n)
     integer *pos, *n;
{
  integer tops =  Top;
  int k;
  for ( k= *pos; k < *pos+*n; k++) C2F(convert2sci)(&k);
  Top = Top - Rhs + *pos - 1 + *n;
  C2F(mklist)(n);
  Top = tops;
  C2F(intersci).ntypes[*pos - 1] = '$';
  return  TRUE_;
} 
/*---------------------------------------------------------------------
 * mktlistfromvars : 
 *     similar to mklistfromvars but create a tlist 
 *---------------------------------------------------------------------*/

int C2F(mktlistfromvars)(pos, n)
     integer *pos, *n;
{
  integer type=16;
  integer tops =  Top;
  int k;
  for ( k= *pos; k < *pos+*n; k++) C2F(convert2sci)(&k);
  Top = Top - Rhs + *pos - 1 + *n;
  C2F(mklistt)(n,&type);
  Top = tops;
  C2F(intersci).ntypes[*pos - 1] = '$';
  return  TRUE_;
} 
/*---------------------------------------------------------------------
 * mktlistfromvars : 
 *     similar to mklistfromvars but create a mlist 
 *---------------------------------------------------------------------*/

int C2F(mkmlistfromvars)(pos, n)
     integer *pos, *n;
{
  integer type=17;
  integer tops =  Top;
  int k;
  for ( k= *pos; k < *pos+*n; k++) C2F(convert2sci)(&k);
  Top = Top - Rhs + *pos - 1 + *n;
  C2F(mklistt)(n,&type);
  Top = tops;
  C2F(intersci).ntypes[*pos - 1] = '$';
  return  TRUE_;
} 

/*---------------------------------------------------------------------
 * call a Scilab function given its name 
 *---------------------------------------------------------------------*/

int C2F(callscifun)(string, string_len)
     char *string;
     unsigned long string_len;
{
  integer id[nsiz];
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

int C2F(scifunction)(number, ptr, mlhs, mrhs)
     integer *number, *ptr, *mlhs, *mrhs;
{
  integer cx26 = 26;
  integer ix1, krec, iflagint, ix, k, intop, il, ir, lw;

  /*     macro execution */
  intop = Top;
  Top = Top - Rhs + *number + *mrhs - 1;
  ++C2F(recu).pt;
  if (C2F(recu).pt > psiz) {
    C2F(error)(&cx26);
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
  krec = -1;

 L60:
  C2F(parse)();
  if (C2F(com).fun == 99) {
    C2F(com).fun = 0;
    goto L200;
  }
  if (Err > 0)  goto L9999;
  if (C2F(recu).rstk[C2F(recu).pt - 1] / 100 == 9) {
    ir = C2F(recu).rstk[C2F(recu).pt - 1] - 900;
    if (ir == 1) {
      /*     .     back to matsys */
      k = 13;
    } else if (ir >= 2 && ir <= 9) {
      /*     .     back to matio */
      k = 5;
      /*<          elseif(ir.eq.10) then >*/
    } else if (ir == 10) {
      /*     .     end of overloaded function */
      goto L96;
      /*<          elseif(ir.gt.40) then >*/
    } else if (ir > 40) {
      /*     .     back to matus2 */
      k = 24;
    } else if (ir > 20) {
      /*     .     back to matusr */
      k = 14;
    } else {
      goto L89;
    }
    iflagint = 0;
    goto L95;
  }

 L89:
  if (Top < Rhs) {
    Scierror(22,"scifunction: recursion problems. Sorry....\r\n");
    goto L9999;
  }
  if (Top - Rhs + Lhs + 1 >= Bot) {
    Scierror(18,"scifunction: too many names\r\n");
    goto L9999;
  }
  goto L91;
 L90:
  if (Err > 0) {
    goto L9999;
  }
  if (Top - Lhs + 1 > 0) {
    C2F(iset)(&Lhs, &cx0, &C2F(vstk).Infstk[Top - Lhs], &cx1);
  }
 L91:
  k = C2F(com).fun;
  C2F(com).fun = 0;
  if (k == krec) {
    Scierror(22,"scifunction: recursion problems. Sorry....\r\n");
    goto L9999;
  }
  if (k == 0) {
    goto L60;
  }
  if (k == 2) {
    il = iadr( *lstk(Top + 1 - Rhs));
    iflagint = *istk(il + 3);
  }
 L95:
  if (! C2F(allowptr)(&k)) {
    C2F(ref2val)();
  }
  C2F(callinterf)(&k, &iflagint);
  if (C2F(com).fun >= 0) {
    goto L90;
  }
  /*    called interface ask for a scilab function to perform the function (fun=-1)
   *     the function name is given in ids(1,pt+1) 
   */
  C2F(ref2val)();
  C2F(com).fun = 0;
  C2F(funs)(&C2F(recu).ids[(C2F(recu).pt + 1)* nsiz - nsiz]);
  if (Err > 0) {
    goto L9999;
  }
  if (C2F(com).fun > 0) {
    goto L91;
  }
  if (C2F(com).fin == 0) {
    integer cx4 = 4;
    C2F(error)(&cx4);
    if (Err > 0) {
      goto L9999;
    }
    goto L90;
  }
  ++C2F(recu).pt;
  C2F(com).fin = *lstk(C2F(com).fin);
  C2F(recu).rstk[C2F(recu).pt - 1] = 910;
  C2F(recu).icall = 5;
  C2F(com).fun = 0;
  /*     *call*  macro */
  goto L60;
 L96:
  --C2F(recu).pt;
  goto L90;
 L200:
  Lhs = C2F(recu).ids[C2F(recu).pt * nsiz -nsiz ];
  Rhs = C2F(recu).ids[C2F(recu).pt * nsiz -(nsiz-1)];
  --C2F(recu).pt;
  --C2F(recu).niv;
  /* + */
  Top = intop;
  ix1 = *mlhs;
  for (ix = 1; ix <= ix1; ++ix) {
    lw = Top - Rhs + *number + ix - 1;
    C2F(intersci).ntypes[lw - 1] = '$';
  }
  return TRUE_;

 L9999:
  Top = intop;
  --C2F(recu).niv;
  return FALSE_;
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

int C2F(scistring)(ifirst, thestring, mlhs, mrhs, thestring_len)
     integer *ifirst;
     char *thestring;
     integer *mlhs, *mrhs;
     unsigned long thestring_len;
{
  int ret = FALSE_;
  integer ifin, ifun, tops, moutputs, id[nsiz], lf, op, ile, ils, nnn, ninputs;
  nnn =  thestring_len;
  op = 0;
  if (nnn <= 2) {
    op = C2F(getopcode)(thestring, thestring_len);
  }
  if (op == 0) {
    C2F(cvname)(id, thestring, &cx0, nnn);
    C2F(com).fin = 0;
    tops = Top;
    Top = Top - Rhs + *ifirst + *mrhs - 1;
    C2F(funs)(id);
    Top = tops;
    if (C2F(com).fin == 0) {
      Scierror(999,"scistring: %s is not a Scilab function\r\n", 
	       get_fname(thestring,thestring_len));
      return ret;
    }
    if (C2F(com).fun <= 0) {
      lf = *lstk(C2F(com).fin);
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
      ifin = C2F(com).fin;
      ifun = C2F(com).fun;
      ret = C2F(scibuiltin)(ifirst, &ifun, &ifin, mlhs, mrhs);
    }
  } else {
    ret = C2F(sciops)(ifirst, &op, mlhs, mrhs);
  }
  return ret;
}

integer C2F(getopcode)(string, string_len)
     char *string;
     unsigned long string_len;
{
  unsigned char ch = string[0];
  integer op = 0;
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

int C2F(scibuiltin)(number, ifun, ifin, mlhs, mrhs)
     integer *number, *ifun, *ifin, *mlhs, *mrhs;
{
  integer krec, srhs, slhs, iflagint;
  integer ix, k, intop, il, ir, lw, pt0;
  intop = Top;
  Top = Top - Rhs + *number + *mrhs - 1;
  slhs = Lhs;
  srhs = Rhs;
  Lhs = *mlhs;
  Rhs = *mrhs;
  krec = -1;
  pt0 = C2F(recu).pt;
  goto L90;
  /* ***************************** */
  /*<  60   call  parse >*/
 L60:
  C2F(parse)();
  if (C2F(com).fun == 99) {
    C2F(com).fun = 0;
    goto L200;
  }
  if (Err > 0) {
    goto L9999;
  }
  if (C2F(recu).rstk[C2F(recu).pt - 1] / 100 == 9) {
    ir = C2F(recu).rstk[C2F(recu).pt - 1] - 900;
    if (ir == 1) {
      /*     .     back to matsys */
      k = 13;
    } else if (ir >= 2 && ir <= 9) {
      /*     .     back to matio */
      k = 5;
    } else if (ir == 10) {
      /*     .     end of overloaded function */
      goto L96;
    } else if (ir > 40) {
      /*     .     back to matus2 */
      k = 24;
    } else if (ir > 20) {
      /*     .     back to matusr */
      k = 14;
    } else {
      goto L89;
    }
    iflagint = 0;
    goto L95;
  }
 L89:
  if (Top < Rhs) {
    Scierror(22,"scibuiltin: recursion problems. Sorry....\r\n");
    goto L9999;
  }
  if (Top - Rhs + Lhs + 1 >= Bot) {
    Scierror(18,"scibuiltin: too many names\r\n");
    goto L9999;
  }
  goto L91;
 L90:
  if (Err > 0) {
    goto L9999;
  }
  if (Top - Lhs + 1 > 0) {
    C2F(iset)(&Lhs, &cx0, &C2F(vstk).Infstk[Top - Lhs], &
	      cx1);
  }
 L91:
  k = C2F(com).fun;
  C2F(com).fun = 0;
  if (k == krec) {
    Scierror(22,"scibuiltin: recursion problems. Sorry....\r\n");
    goto L9999;
  }
  if (k == 0) {
    if (C2F(recu).pt > pt0) {
      goto L60;
    }
    goto L200;
  }
  if (k == 2) {
    il = iadr(*lstk(Top + 1 - Rhs));
    iflagint = *istk(il + 3);
  }
  if (! C2F(allowptr)(&k)) {
    C2F(ref2val)();
  }
 L95:
  C2F(callinterf)(&k, &iflagint);
  if (C2F(recu).icall != 0) {
    goto L60;
  }
  if (C2F(com).fun >= 0) {
    goto L90;
  }
  /*    called interface ask for a sci function to perform the function (fun=-1)*/
  /*     the function name is given in ids(1,pt+1) */
  C2F(ref2val)();
  C2F(com).fun = 0;
  C2F(funs)(&C2F(recu).ids[(C2F(recu).pt + 1)* nsiz - nsiz]);
  if (Err > 0) {
    goto L9999;
  }
  if (C2F(com).fun > 0) {
    goto L91;
  }
  if (C2F(com).fin == 0) {
    integer cx4 = 4;
    C2F(error)(&cx4);
    if (Err > 0) {
      goto L9999;
    }
  }
  ++C2F(recu).pt;
  C2F(com).fin = *lstk(C2F(com).fin);
  C2F(recu).rstk[C2F(recu).pt - 1] = 910;
  C2F(recu).icall = 5;
  C2F(com).fun = 0;
  /*     *call*  macro */
  goto L60;
 L96:
  --C2F(recu).pt;
  goto L90;
  /* ************************** */
 L200:
  Lhs = slhs;
  Rhs = srhs;
  Top = intop;
  for (ix = 0 ; ix < *mlhs ; ++ix) {
    lw = Top - Rhs + *number + ix ;
    C2F(intersci).ntypes[lw - 1] = '$';
  }
  return TRUE_;
 L9999:
  return FALSE_;
}

/*---------------------------------------------------------------------
 *     same as scibuiltin: executes scilab operation op 
 *     for the input parameters located at number, number+1, .... 
 *     mlhs,mrhs = # of lhs and rhs parameters of the operation. 
 *---------------------------------------------------------------------*/

int C2F(sciops)(number, op, mlhs, mrhs)
     integer *number, *op, *mlhs, *mrhs;
{
  integer ifin, ifun, srhs= Rhs , slhs= Lhs, ix, intop=Top , lw;

  Fin   = *op;
  Top = Top - Rhs + *number + *mrhs - 1;
  Lhs = *mlhs;
  Rhs = *mrhs;

  while (1) 
    {
      C2F(allops)();
      if (Err > 0) {return FALSE_;} ;
      if (C2F(com).fun == 0) break; 
      Top = intop;
      ifun = C2F(com).fun;
      ifin = C2F(com).fin;
      if (! C2F(scibuiltin)(number, &ifun, &ifin, mlhs, mrhs)) 
	{return FALSE_;} ;
      if (Err > 0) {return FALSE_;} ;
    }
  Lhs = slhs;
  Rhs = srhs;
  Top = intop;
  
  for (ix = 1 ; ix <= *mlhs ; ++ix) {
    lw = Top - Rhs + *number + ix - 1;
    C2F(intersci).ntypes[lw - 1] = '$';
  }
  C2F(com).fun = 0;
  C2F(com).fin = *op;
  C2F(recu).icall = 0;
  return TRUE_;
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

int C2F(getrhssys)(lw, n, m, p, ptra, ptrb, ptrc, ptrd, ptrx0, hx)
     integer *lw, *n, *m, *p, *ptra, *ptrb, *ptrc, *ptrd, *ptrx0;
     double *hx;
{
  integer cx2 = 2, cx3 = 3, cx4 = 4, cx5 = 5, cx6 = 6;
  integer ix1, junk, msys, nsys, ix, icord;
  integer ma, na, mb, nb, mc, nc, il, md, nd;
  integer mx0, nx0, ptrsys, itimedomain;
  static integer iwork[23] = { 10,1,7,0,1,4,5,6,7,8,10,12,21,28,28,-10,-11,
			       -12,-13,-33,0,13,29 };
  if (! C2F(getrhsvar)(lw, "t", &msys, &nsys, &ptrsys, 1L)) return FALSE_;
  il = iadr(ptrsys) - msys - 1;
  /*     syslin tlist=[ chain, (A,B,C,D,X0) ,chain or scalar ]
   *                     10     1 1 1 1 1      10       1    
   */
  junk = il + msys + iadr(*istk(il));
  if ( *istk(junk) != 10) return FALSE_;
  if ( *istk(il + msys + iadr(*istk(il + 1))) != 1) return FALSE_;
  if ( *istk(il + msys + iadr(*istk(il + 2))) != 1) return FALSE_;
  if ( *istk(il + msys + iadr(*istk(il + 3))) != 1) return FALSE_;
  if ( *istk(il + msys + iadr(*istk(il + 4))) != 1) return FALSE_;
  if ( *istk(il + msys + iadr(*istk(il + 5))) != 1) return FALSE_;
  itimedomain = *istk(il + msys + iadr(*istk(il + 6)));
  switch ( itimedomain ) {
  case 10 :
    /* Sys(7)='c' or 'd' */
    icord = *istk(il + msys + iadr(*istk(il + 6))+ 6);
    switch ( icord ) 
      {
      case 12 :  *hx = 0.; break;
      case 13 :  *hx = 1.; break;
      default : 
	Scierror(999,"invalid time domain\r\n");
	return FALSE_;
      }
    break;
  case 1 : 
    /*     Sys(7)=h */
    ix1 = il + msys + iadr(*istk(il + 6)) + 4;
    *hx = *stk(sadr(ix1));
    break;
  default : 
    Scierror(999,"invalid time domain\r\n");
    return FALSE_;
  }
  for (ix = 0; ix < 23; ++ix) 
    {
      if (iwork[ix] != *istk(junk + ix)) {
	Scierror(999,"invalid system\r\n");
	return FALSE_;
      }
    }
  if (! C2F(getlistrhsvar)(lw, &cx2, "d", &ma, &na, ptra, 1L)) return FALSE_;
  if (! C2F(getlistrhsvar)(lw, &cx3, "d", &mb, &nb, ptrb, 1L)) return FALSE_;
  if (! C2F(getlistrhsvar)(lw, &cx4, "d", &mc, &nc, ptrc, 1L)) return FALSE_;
  if (! C2F(getlistrhsvar)(lw, &cx5, "d", &md, &nd, ptrd, 1L)) return FALSE_;
  if (! C2F(getlistrhsvar)(lw, &cx6, "d", &mx0, &nx0, ptrx0, 1L))  return FALSE_;
  if (ma != na) {
    Scierror(999,"A matrix non square!\r\n");
    return FALSE_;
  }
  if (ma != mb && mb != 0) {
    Scierror(999,"Invalid A,B matrices\r\n");
    return FALSE_;
  }
  if (ma != nc && nc != 0) {
    Scierror(999,"Invalid A,C matrices\r\n");
    return FALSE_;
  }
  if (mc != md && md != 0) {
    Scierror(999,"Invalid C,D matrices\r\n");
    return FALSE_;
  }
  if (nb != nd && nd != 0) {
    Scierror(999,"Invalid B;D matrices\r\n");
    return FALSE_;
  }
  *n = ma;
  *m = nb;
  *p = mc;
  return TRUE_;
}


/*--------------------------------------------------- 
 * call Scilab error function (for Fortran use)
 *---------------------------------------------------*/

int C2F(errorinfo)(fname, info, fname_len)
     char *fname;
     integer *info;
     unsigned long fname_len;
{
  Scierror(998,"%s: internal error, info=%d\r\n",get_fname(fname,fname_len),*info);
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

integer C2F(maxvol)(lw, lw_type, type_len)
     integer *lw;
     char *lw_type;
     unsigned long type_len;
{
  unsigned char Type =  *(unsigned char *)lw_type ;
  /* I like this one a lot: a kind of free jazz pattern  */
  integer m = *lstk(Bot) - sadr(iadr(*lstk(*lw + Top - Rhs)) +4);
  switch ( Type ) 
    {
    case 'd' : return m; break;
    case 'i' : return iadr(m);break;
    case 'r' : return iadr(m);break;
    case 'c' : return cadr(m);break;
    case 'z' : return sadr(m)-1;break;
    }
  /* should never get there */
  return m; 
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
  integer ix2, ivar, ibufprec, ix, k, lcres, nbvars1;

  if (C2F(com).fun== -1 ) return TRUE_ ; /* execution continue with an 
					    overloaded function */


  if (LhsVar(1) == 0) 
    {
      Top = Top - Rhs + Lhs;
      C2F(objvide)(" ", &Top, 1L);
      Nbvars = 0;
      return TRUE_;
    }
  nbvars1 = 0;
  for (k = 1; k <= Lhs ; ++k) nbvars1 = Max( nbvars1 , LhsVar(k));
  /* check if output variabe are in increasing order in the stack */
  lcres = TRUE_;
  ibufprec = 0;
  for (ix = 1; ix <= Lhs ; ++ix) {
    if (LhsVar(ix) < ibufprec) {
      lcres = FALSE_;
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
	return FALSE_;
      }
      LhsVar(ivar) = nbvars1 + ivar;
      /* I change type of variable nbvars1 + ivar 
       * in order to just perform a dcopy at next pass 
       */
      if (nbvars1 + ivar > intersiz) {
	Scierror(999,"putlhsvar: intersiz is too small\r\n");
	return FALSE_;
      }
      C2F(intersci).ntypes[nbvars1 + ivar - 1] = '$';
    }
  }
  /*  Second pass */
  for (ivar = 1; ivar <= Lhs ; ++ivar) 
    {
      ix2 = Top - Rhs + ivar;
      if (! C2F(mvfromto)(&ix2, &LhsVar(ivar))) {
	return FALSE_;
      }
    }
  Top = Top - Rhs + Lhs;
  LhsVar(1) = 0;
  Nbvars = 0;
  return TRUE_;
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

static int C2F(mvfromto)(itopl, ix)
     integer *itopl, *ix;
{
  integer ix1, m,n,it,lcs,lrs ,il; 
  unsigned char Type ;
  int iwh;
  int izpos,spos;
/*   FD modif 
was:
  m = C2F(intersci).nbrows[*ix - 1];
  n = C2F(intersci).nbcols[*ix - 1];
  it = C2F(intersci).itflag[*ix - 1];
  Type = C2F(intersci).ntypes[*ix - 1];  */

  iwh = C2F(intersci).iwhere[*ix - 1];
  m =*istk( iadr(iwh) +1);
  n =*istk( iadr(iwh) +2);
  it = *istk( iadr(iwh) +3);
  Type = C2F(intersci).ntypes[*ix - 1];

  switch ( Type ) {
  case 'i' : 
    if (! C2F(cremat)("mvfromto", itopl, &it, &m, &n, &lrs, &lcs, 8L)) {
      return FALSE_;
    }
    ix1 = m * n * (it + 1);
    C2F(stacki2d)(&ix1, &C2F(intersci).lad[*ix - 1], &lrs);
    C2F(intersci).lad[*ix - 1] = iadr(lrs);
    break ;
  case 'r' :
    if (! C2F(cremat)("mvfromto", itopl, &it, &m, &n, &lrs, &lcs, 8L)) {
      return FALSE_;
    }
    ix1 = m * n * (it + 1);
    C2F(stackr2d)(&ix1, &C2F(intersci).lad[*ix - 1], &lrs);
    C2F(intersci).lad[*ix - 1] = iadr(lrs);
    break;
  case 'd' :
    if (! C2F(cremat)("mvfromto", itopl, &it, &m, &n, &lrs, &lcs, 8L)) {
      return FALSE_;
    }
    /* no copy if the two objects are the same 
     * the cremat above is kept to deal with possible size changes 
     */
    if (C2F(intersci).lad[*ix - 1] != lrs) {
      ix1 = m * n * (it + 1);
      C2F(dcopy)(&ix1, stk(C2F(intersci).lad[*ix - 1]), &cx1, stk(lrs), &cx1);
      C2F(intersci).lad[*ix - 1] = lrs;
    }
    break;
  case 'z' :
    if (! C2F(cremat)("mvfromto", itopl, &it, &m, &n, &lrs, &lcs, 8L)) {
      return FALSE_;
    }
    ix1 = m * n * (it + 1);
    F77ToSci(stk(C2F(intersci).lad[*ix - 1]), m*n, m*n);
    if (C2F(intersci).lad[*ix - 1] != lrs) {
      ix1 = m * n * (it + 1);
      C2F(dcopy)(&ix1, stk(C2F(intersci).lad[*ix - 1]), &cx1, stk(lrs), &cx1);
      C2F(intersci).lad[*ix - 1] = lrs;
      C2F(intersci).ladc[*ix - 1] = lrs +m*n;
    }
    break;
  case 'c' : 
    /*  FD added  next 3 */
  il = iadr(iwh);
  m = *istk(il + 4  +1) - *istk(il + 4 );
  n = 1;

    ix1 = m * n;
    if (! C2F(cresmat2)("mvfromto", itopl, &ix1, &lrs, 8L)) {
      return FALSE_;
    }
    C2F(stackc2i)(&ix1, &C2F(intersci).lad[*ix - 1], &lrs);
    C2F(intersci).lad[*ix - 1] = cadr(lrs);
    break;
  
  case 'b' :
    if (! C2F(crebmat)("mvfromto", itopl, &m, &n, &lrs, 8L)) {
      return FALSE_;
    }
    ix1 = m * n;
    C2F(icopy)(&ix1, istk(C2F(intersci).lad[*ix - 1]), &cx1,istk(lrs), &cx1);
    C2F(intersci).lad[*ix - 1] = lrs;
    break;
  case 'p' :
  case '$' :
    /*     special case */
    if (Top - Rhs + *ix != *itopl) 
      {
	ix1 = Top - Rhs + *ix;
	if ( C2F(vcopyobj)("mvfromto", &ix1, itopl, 8L) == FALSE_) 
	  return FALSE_;
      }
  }
  return TRUE_;
} 

/*---------------------------------------------------------------------
 * convert2sci : 
 *     this routine converts data of variable number num 
 *     to scilab standard data code 
 *     see how it is used in matdes.c 
 *---------------------------------------------------------------------*/

int C2F(convert2sci)(ix)
     integer *ix;
{
  integer ix1 = Top - Rhs + *ix;
  if (! C2F(mvfromto)(&ix1, ix)) return FALSE_;
  C2F(intersci).ntypes[*ix - 1] = '$';
  return TRUE_;
} 



/*---------------------------------------------------------------------
 * int tryenhaut:
 *     this routines checks if the creation of a variable at position 
 *     itopl on the stack would give an adress >= adress of the variable number i 
 *---------------------------------------------------------------------*/

int C2F(tryenhaut)(itopl, ix)
     integer *itopl, *ix;
{
  integer ix1, m,n,it,lcs,lrs ;
  unsigned char Type ;

  m = C2F(intersci).nbrows[*ix - 1];
  n = C2F(intersci).nbcols[*ix - 1];
  it = C2F(intersci).itflag[*ix - 1];
  Type = (unsigned char) C2F(intersci).ntypes[*ix - 1];
  switch ( Type ) {
  case 'i' : 
    if (! C2F(fakecremat)(itopl, &it, &m, &n, &lrs, &lcs)) return FALSE_;
    if (it == 0) {
      if ( sadr(C2F(intersci).lad[*ix - 1]) <= lrs) return FALSE_;
    } else {
      if ( sadr(C2F(intersci).ladc[*ix - 1]) <= lcs) return FALSE_;
    }
    break;
  case 'r' : 
    if (! C2F(fakecremat)(itopl, &it, &m, &n, &lrs, &lcs)) return FALSE_;
    if (it == 0) {
      if (sadr(C2F(intersci).lad[*ix - 1]) <= lrs) return  FALSE_;
    } else {
      if ( sadr(C2F(intersci).ladc[*ix - 1]) <= lcs) return  FALSE_;
    }
    break;
  case 'b' : 
    if (! C2F(fakecrebmat)(itopl, &m, &n, &lrs)) 	return FALSE_;
    if (C2F(intersci).lad[*ix - 1] <= lrs) return  FALSE_;
    break;
  case 'd' : 
    if (! C2F(fakecremat)(itopl, &it, &m, &n, &lrs, &lcs)) return FALSE_;
    if (it == 0) {
      if (C2F(intersci).lad[*ix - 1] <= lrs) return FALSE_;
    } else {
      if (C2F(intersci).ladc[*ix - 1] <= lcs) return FALSE_;
    }
    break;
  case 'c' : 
    ix1 = m * n;
    if (! C2F(fakecresmat2)(itopl, &ix1, &lrs)) return FALSE_;
    ix1 = sadr(C2F(intersci).lad[*ix - 1]);
    if ( sadr(ix1) <= lrs) return FALSE_ ;
    break;
  }
  return TRUE_;
} 

/*-----------------------------------------------------------
 * strcpy_tws : fname[0:nlgh-2]=' '
 * fname[nlgh-1] = '\0'
 * then second string is copied into first one 
 * ------------------------------------------------------------*/

void strcpy_tws(str1,str2,len) 
     char *str1,*str2;
     int len;
{
  int i; 
  for ( i =0 ; i  < strlen(str2); i++ ) str1[i]=str2[i];
  for (i = strlen(str2) ; i < len ; i++) str1[i]=' ';
  str1[len-1] ='\0';
}

/*---------------------------------------------------------------------
 *     conversion from Scilab code --> ascii 
 *     + add a 0 at end of string 
 *---------------------------------------------------------------------*/

int C2F(in2str)(n, line, str, str_len)
     integer *n, *line;
     char *str;
     unsigned long str_len;
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

static char *pos[] ={"first","second","third","fourth"};
static char arg_position[56];

char *ArgPosition(i)
     int i;
{
  if ( i > 0 && i <= 4 ) 
    sprintf(arg_position,"%s argument",pos[i-1]);
  else 
    sprintf(arg_position,"argument number %d",i);
  return arg_position;
}

char *ArgsPosition(i,j)
     int i,j;
{
  if ( i > 0 && i <= 4 ) 
    {
      if ( j > 0 && j <= 4 ) 
	sprintf(arg_position,"%s and %s arguments",pos[i-1],pos[j-1]);
      else 
	sprintf(arg_position,"%s argument and argument %d",pos[i-1],j);
    }
  else 
    {
      if ( j > 0 && j <= 4 ) 
	sprintf(arg_position,"%s argument and argument %d",pos[j-1],i);
      else 
	sprintf(arg_position,"arguments %d and %d",i,j);
    }
  return arg_position;
}


/*---------------------------------------------------------------------
 * Utility for back convertion to Scilab format 
 * (can be used with GetListRhsVar ) 
 *---------------------------------------------------------------------*/

void ConvertData(type,size,l)
     char *type;
     int size,l;
{
  int zero=0,mu=-1;
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
   F77ToSci(stk(l), size, size);
  }
}

/*---------------------------------------------------------------------
 * Utility for checking properties 
 *---------------------------------------------------------------------*/

static int check_prop(mes,pos,m)
     char *mes;
     int pos,m;
{
  if ( m ) 
    { 
      Scierror(999,"%s: %s %s\r\n",
	       Get_Iname(),
	       ArgPosition(pos),
	       mes);
      return FALSE_;
    }
  return TRUE_;
}

int check_square (pos,m,n) 
     int pos,m,n;
{
  return check_prop("should be square",pos, m != n);
}

int check_vector (pos,m,n) 
     int pos,m,n;
{
  return check_prop("should be a vector",pos, m != 1 && n != 1);
}

int check_row (pos,m,n) 
     int pos,m,n;
{
  return check_prop("should be a row vector",pos, m != 1);
}

int check_col (pos,m,n) 
     int pos,m,n;
{
  return check_prop("should be a column vector",pos, n != 1);
}

int check_scalar (pos,m,n) 
     int pos,m,n;
{
  return check_prop("should be a scalar",pos, n != 1 || m != 1);
}

int check_dims(pos,m,n,m1,n1)
     int pos,m,n,m1,n1;
{
  if ( m != m1 ||  n != n1 ) 
    { 
      Scierror(999,"%s: %s has wrong dimensions (%d,%d), expecting (%d,%d)\r\n",
	       Get_Iname(),
	       ArgPosition(pos),
	       m,n,
	       m1,n1);
      return FALSE_;
    }
  return TRUE_;
}

int check_one_dim(pos,dim,val,valref)
     int pos,dim,val,valref;
{
  if ( val != valref) 
    { 
      Scierror(999,"%s: %s has wrong %s dimension (%d), expecting (%d)\r\n",
	       Get_Iname(),
	       ArgPosition(pos),
	       ( dim == 1 ) ? "first" : "second" ,
	       val,valref);
      return FALSE_;
    }
  return TRUE_;
}

int check_length(pos,m,m1)
     int pos,m,m1;
{
  if ( m != m1 )
    { 
      Scierror(999,"%s: %s has wrong length %d, expecting (%d)\r\n",
	       Get_Iname(),
	       ArgPosition(pos),
	       m, m1);
      return FALSE_;
    }
  return TRUE_;
}

int check_same_dims(i,j,m1,n1,m2,n2) 
     int i,j,m1,n1,m2,n2;
{
  if ( m1 == m2 && n1 == n2 ) return TRUE_ ;
  Scierror(999,"%s: %s have incompatible dimensions (%dx%d) # (%dx%d)\r\n",
	   Get_Iname(),
	   ArgsPosition(i,j),
	   m1,n1,m2,n2);
  return FALSE_;
}

int check_dim_prop(i,j,flag) 
     int i,j,flag;
{
  if ( flag ) 
    {
      Scierror(999,"%s: %s have incompatible dimensions\r\n",
	       Get_Iname(),
	       ArgsPosition(i,j));
      return FALSE_;
    }
  return TRUE_;
}
 

static int check_list_prop(mes,lpos,pos,m)
     char *mes;
     int lpos,pos,m;
{
  if ( m ) 
    { 
      Scierror(999,"%s: %s should be a list with %d-element being %s \r\n",
	       Get_Iname(),
	       ArgPosition(pos),
	       pos,mes);
      return FALSE_;
    }
  return TRUE_;
}

int check_list_square (lpos,pos,m,n) 
     int lpos,pos,m,n;
{
  return check_list_prop("square",lpos,pos, m != n);
}

int check_list_vector (lpos,pos,m,n) 
     int lpos,pos,m,n;
{
  return check_list_prop("a vector",lpos,pos, m != 1 && n != 1);
}

int check_list_row (lpos,pos,m,n) 
     int lpos,pos,m,n;
{
  return check_list_prop("a row vector",lpos,pos, m != 1);
}

int check_list_col (lpos,pos,m,n) 
     int lpos,pos,m,n;
{
  return check_list_prop("a column vector",lpos,pos, n != 1);
}

int check_list_scalar (lpos,pos,m,n) 
     int pos,m,n;
{
  return check_list_prop("a scalar",lpos, pos, n != 1 || m != 1);
}

int check_list_one_dim(lpos,pos,dim,val,valref)
     int lpos,pos,dim,val,valref;
{
  if ( val != valref) 
    { 
      Scierror(999,"%s: argument %d(%d) has wrong %s dimension (%d), expecting (%d)\r\n",
	       Get_Iname(),lpos,pos,
	       ( dim == 1 ) ? "first" : "second" ,
	       val,valref);
      return FALSE_;
    }
  return TRUE_;
}



/*---------------------------------------------------------------------
 * Utility for ? 
 *---------------------------------------------------------------------*/

int C2F(createdata)(lw, n)
     integer *lw, n;
{
  integer lw1;
  char *fname = Get_Iname();
  if (*lw > intersiz) {
    Scierror(999,"%s: (createdata) too many arguments in the stack edit stack.h and enlarge intersiz\r\n",
	     fname);
    return FALSE_ ;
  }
  Nbvars = Max(*lw,Nbvars);
  lw1 = *lw + Top - Rhs;
  if (*lw < 0) {
    Scierror(999,"%s: bad call to createdata! (1rst argument)\r\n",
	     fname);
    return FALSE_ ;
  }
  if (! C2F(credata)(fname, &lw1, n, nlgh))    return FALSE_;
  C2F(intersci).ntypes[*lw - 1] = '$';
  C2F(intersci).iwhere[*lw - 1] = *lstk(lw1);
  C2F(intersci).lad[*lw - 1] = *lstk(lw1);
  return TRUE_; 
}


void *GetData(lw)
     int lw;
{
  int lw1 = lw + Top - Rhs ;
  int *loci = (int *) stk( *lstk(lw1));
  C2F(intersci).ntypes[lw - 1] = '$';
  C2F(intersci).iwhere[lw - 1] = *lstk(lw1);
  C2F(intersci).lad[lw - 1] = *lstk(lw1);
  return loci;
}

