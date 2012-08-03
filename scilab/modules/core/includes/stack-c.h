/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/* Common Block Declarations */

#ifndef STACK_SCI
#define STACK_SCI

#if defined(__SCILAB_TOOLBOX__) && !defined(__INTERNAL_API_SCILAB__) && !defined(__USE_DEPRECATED_STACK_FUNCTIONS__) && !defined(__MEX_INCLUDE__)
    #error Using stack-c.h is deprecated. Please use api_scilab instead (try 'help api_scilab'). Note the stack-c.h API will be removed after Scilab 6.0. You can define __USE_DEPRECATED_STACK_FUNCTIONS__ to bypass this error.
#endif

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#if  ( !(defined __MATH__)  || (defined __APPLE__) || (defined aix) )
#include "core_math.h"
#else
#include "machine.h"
#endif

#include "stack-def.h"
#include "stackTypeVariable.h"

#include "BOOL.h"
#include "doublecomplex.h" /* define doublecomplex type */
#include "stack1.h"
#include "stack2.h"
#include "stack3.h"
#include "sci_types.h"

/*-------------------------------------------------
 * structure used for int matrix
 * -------------------------------------------------*/

/* a set of define to decode it argument */

#define I_CHAR 1
#define I_INT16 2
#define I_INT32 4
#define I_UCHAR 11
#define I_UINT16 12
#define I_UINT32 14

/* a set of define for the associated casts */

#define IC_CHAR(x) ((char *) (x))
#define IC_INT16(x) ((short int *) (x))
#define IC_INT32(x)   ((int *) (x))
#define IC_UCHAR(x)   ((unsigned char *) (x))
#define IC_UINT16(x)   ((unsigned short int *) (x))
#define IC_UINT32(x)  ((unsigned int *) (x))

/**
 * sciintmat
 */
typedef struct sciintmat {
	int m,n;
	int it ; /**< it : 1,2,4,11,12,14  */
	int l;   /**< if l != -1 then istk(l) == D */
	void *D;     /**< data : should be casted according to it */
} SciIntMat ;



/*-------------------------------------------------
 * set of defines for interface simplication
 * -------------------------------------------------*/

static int c1_local=0;
static int c_local=0;

static void initial_c1_local(void);
static void initial_c_local(void);

/* Correction Warning variable "c_local" was declared but never referenced */
/* initial_c_local not used */
static void initial_c_local(void)
{
	initial_c1_local();
	c_local=0;
}
/* Correction Warning variable "c1_local" was declared but never referenced */
/* initial_c1_local not used */
static void initial_c1_local(void)
{
	initial_c_local();
	c1_local=0;
}



#define iadr(l) ((l)+(l)-1)
#define sadr(l) (((l)/2)+1)
#define cadr(l) ((l)+(l)+(l)+(l)-3)

/* Top is a variable used in Windows.h */
#ifdef Top
#undef Top
#endif
#define Top C2F(vstk).top
#define Fin C2F(com).fin
#define Rhs C2F(com).rhs
#define Lhs C2F(com).lhs
#define Bot C2F(vstk).bot
#define Err C2F(iop).err
#define Leps_sci  *stk(C2F(vstk).leps)

#define stk(x)  ( C2F(stack).Stk + x-1 )
#define istk(x) (((int *) C2F(stack).Stk) + x-1 )
#define sstk(x) (((float *) C2F(stack).Stk) + x-1 )
#define cstk(x) (((char *) C2F(stack).Stk) + x-1 )

#define zstk(x) (((doublecomplex *) C2F(stack).Stk) + x-1 )


#define Lstk(x) (((int *) C2F(vstk).lstk) + x-1 )
#define Infstk(x) (((int *) C2F(vstk).infstk) + x-1 )

/* to retreive handles on the stack */
#define hstk(x) (((long long *) C2F(stack).Stk) + x-1 )

#define CheckOpt(first) if ( C2F(checkopt)(first) ) {return 0;}

#define FirstOpt() C2F(firstopt)()

#define FindOpt(str,opts) C2F(findopt)(str,opts)

#define NumOpt() C2F(numopt)()

#define IsOpt(k,name) C2F(isopt)((c_local=k,&c_local),name,nlgh)

#define Maxvol(n,ct)  C2F(maxvol)((c_local=n,&c_local),ct,1L)

#define CreateVarFromPtr(n,ct,mx,nx,lx) if ( ! C2F(createvarfromptr)((c_local=n,&c_local),ct,mx,nx,(void *)lx,1L)) \
					     { return 0;}

#define CreateCVarFromPtr(n,ct,it,mx,nx,lrx,lcx) if ( ! C2F(createcvarfromptr)((c_local=n,&c_local),ct,it,mx,nx,(double *)lrx,(double *) lcx,1L)) \
					     { return 0;}
#define CreateRefFromName(n,nx) if(! C2F(createreffromname)(n,nx)){return 0;}

#define CreateRef(num, point) if(! C2F(createreference)(num,point)){return 0;}

#define ChangeToRef(num, point) C2F(changetoref)(num,point)

#define FreePtr(lx) C2F(freeptr)((double **) lx)

#define GetType(n)   C2F(gettype)((c_local = n +Top - Rhs, &c_local))

#define VarType(n)   C2F(vartype)((c_local=n,&c_local))
#define ElementType(n,m)   C2F(elementtype)((c_local=n,&c_local),(c1_local=m,&c1_local))

#define OverLoad(n) C2F(overload)((c_local = (n == 0) ? n : n + Top - Rhs, &c_local),fname,(unsigned long)strlen(fname))

/**
 * <long-description>
 *
 * @param n
 * @param ct
 * @param mx
 * @param nx
 * @param lx
 * @return <ReturnValue>
 */
#define GetRhsVar(n,ct,mx,nx,lx) if (! C2F(getrhsvar)((c_local=n,&c_local),ct,mx,nx,(int *) lx,1L))\
        { return 0;  }

#define CreateVar(n,ct,mx,nx,lx) if(! C2F(createvar)((c_local=n,&c_local),ct,mx,nx,(int *)lx, 1L))\
        { return 0;  }

#define CreateWork(n,mx,lx) if(! C2F(creatework)((c_local=n,&c_local),mx,(int *)lx))\
        { return 0;  }

#define SetWorkSize(n,mx) if(! C2F(setworksize)((c_local=n,&c_local),mx))\
        { return 0;  }

#define CreateData(n,nx) if(! C2F(createdata)((c_local=n,&c_local),nx))\
        { return 0;  }

#define GetRhsCVar(n,ct,it,mx,nx,lrx,lcx) if (! C2F(getrhscvar)((c_local=n,&c_local),ct,it,mx,nx,lrx,lcx,1L))\
        { return 0;  }

#define CreateCVar(n,ct,it,mx,nx,lrx,lcx) if(! C2F(createcvar)((c_local=n,&c_local),ct,it,mx,nx,lrx,lcx, 1L))\
        { return 0;  }

#define Errorinfo(routinename,info) C2F(errorinfo)(routinename,(c_local=info, &c_local), strlen(routinename));

#define PutLhsVar()  if (! C2F(putlhsvar)()) {	return 0; }

#define ReadMatrix(ct,mx,nx,w)  if (! C2F(creadmat)(ct,mx,nx,w,(unsigned long)strlen(ct) )) {	return 0; }

#define WriteMatrix(ct,mx,nx,w)  if (! C2F(cwritemat)(ct,mx,nx,w,(unsigned long)strlen(ct) )) {	return 0; }

#define PutVar(num, nam)  if (! C2F(putvar)( (c_local=num, &c_local), nam, (unsigned long)strlen(nam) )) {	return 0; }

#define ReadString(ct,mx,w)  if (! C2F(creadchain)(ct,mx,w,strlen(ct),strlen(w) )) {	return 0; }

#define WriteString(ct,mx,w)  if (! C2F(cwritechain)(ct,mx,w,strlen(ct),strlen(w) )) {	return 0; }

#define GetMatrixptr(ct,mx,nx,lx)  if (! C2F(cmatptr)(ct,mx,nx,lx,(unsigned long)strlen(ct) )) {	return 0; }

#define GetMatrixdims(n,mx,nx)  if (! C2F(getmatdims)((c_local=n,&c_local),mx,nx)) {	return 0; }

#define CreateVarFrom(n,ct,mx,nx,lx,lx1) if (!C2F(createvarfrom)((c_local=n,&c_local),ct,mx,nx,lx,lx1,1L))  { return 0;}
#define CreateCVarFrom(n,ct,it,mx,nx,lx,lc,lx1,lc1) if (!C2F(createcvarfrom)((c_local=n,&c_local),ct,it,mx,nx,lx,lc,lx1,lc1,1L))  { return 0;}

#define Createlist(m,n) C2F(createlist)((c_local=m,&c_local),(c1_local=n,&c1_local))

#define CreateListVarFrom(n,m,ct,mx,nx,lx,lx1) if (!C2F(createlistvarfrom)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,mx,nx,(int *)lx,(int *) lx1,1L))  { return 0;}

#define CreateListCVarFrom(n,m,ct,it,mx,nx,lx,lc,lx1,lc1) if (!C2F(createlistcvarfrom)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,it,mx,nx,(int *)lx,(int *) lc,(int *) lx1,(int *)lc1,1L))  { return 0;}

#define CreateListVarFromPtr(n,m,ct,mx,nx,lx1) if (!C2F(createlistvarfromptr)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,mx,nx,(void *) lx1,1L))  { return 0;}

#define CreateListCVarFromPtr(n,m,ct,it,mx,nx,lx1,lc1) if (!C2F(createlistcvarfromptr)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,it,mx,nx,(void *) lx1,(void *) lc1,1L))  { return 0;}



#define  GetListRhsVar(n,m,ct,m1e1,n1e1,l1e1)  if(!C2F(getlistrhsvar)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,m1e1,n1e1,(int *) l1e1,1L)) {return 0;}

#define  GetListRhsCVar(n,m,ct,it,m1e1,n1e1,l1e1,l1e2)  if(!C2F(getlistrhscvar)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,it,m1e1,n1e1,(int *) l1e1,(int *) l1e2,1L)) {return 0;}

#ifdef _MSC_VER
#define CheckRhs(minrhs,maxrhs)  \
  if (! C2F(checkrhs)(fname,(c_local = minrhs,&c_local),(c1_local=maxrhs,&c1_local),\
		      (unsigned long)strlen(fname))) { \
      return 0;\
  }

#define CheckLhs(minlhs,maxlhs)  \
  if (! C2F(checklhs)(fname,(c_local = minlhs,&c_local),(c1_local=maxlhs,&c1_local),\
		      (unsigned long)strlen(fname))) { \
      return 0;\
  }
#else
#define CheckRhs(minrhs,maxrhs)  \
  if (! C2F(checkrhs)(fname,(c_local = minrhs,&c_local),(c1_local=maxrhs,&c1_local),\
		      strlen(fname))) { \
      return 0;\
  }

#define CheckLhs(minlhs,maxlhs)  \
  if (! C2F(checklhs)(fname,(c_local = minlhs,&c_local),(c1_local=maxlhs,&c1_local),\
		      strlen(fname))) { \
      return 0;\
  }

#endif

/** used for conversion to Scilab internal format **/

#define Convert2Sci(x)  C2F(convert2sci)((c_local=x,&c_local))


/** Used for calling a scilab function by its name  **/

#define SciString(ibegin,name,mlhs,mrhs) \
    if( ! C2F(scistring)(ibegin,name,mlhs,mrhs,(unsigned long)strlen(name))) return 0;

/** Used for calling a scilab function given as argument **/

#define SciFunction(ibegin,lf,mlhs,mrhs) \
    if( ! C2F(scifunction)(ibegin,lf,mlhs,mrhs)) return 0;

/** used for protecting a call to a Scilab function **/

#define PExecSciFunction(n,mx,nx,lx,name,fsqpenv) \
  if(! C2F(scifunction)((c_local=n,&c_local),mx,nx,lx))\
{ sciprint(_(" results may be inaccurate. rcond = %s1\n"),name);  longjmp(fsqpenv,-1); }

#define Nbvars C2F(intersci).nbvars

/**
 * TODO : need a comment !!!!
 *
 */
#define LhsVar(x) C2F(intersci).lhsvar[x-1]


/* used to access data associated to a variable
 * for example when a variable is created with CreateVarFromPtr(...)
 */

#define VarPtr(x) C2F(intersci).lad[x-1]

typedef int (*interfun)(char *fname,unsigned long l);

typedef struct tagTabF {
  interfun f;
  char *name;
} TabF;


/*-------------------------------------------------
 * checks properties
 * -------------------------------------------------*/

#define CheckSquare(pos,m,n) if (! check_square(pos,m,n)) return 0;
#define CheckVector(pos,m,n) if (! check_vector(pos,m,n)) return 0;
#define CheckRow(pos,m,n) if (! check_row(pos,m,n)) return 0;
#define CheckColumn(pos,m,n) if (! check_col(pos,m,n)) return 0;
#define CheckScalar(pos,m,n) if (! check_scalar(pos,m,n)) return 0;
#define CheckDims(pos,m,n,m1,n1) if (! check_dims(pos,m,n,m1,n1)) return 0;
#define CheckLength(pos,m,m1) if (! check_length(pos,m,m1)) return 0;
#define CheckSameDims(pos,pos1,m,n,m1,n1) if (!  check_same_dims(pos,pos1,m,n,m1,n1)) return 0;
#define CheckDimProp(pos,pos1,m) if (!  check_dim_prop(pos,pos1,m)) return 0;
#define CheckOneDim(pos,dim,val,valref) if (! check_one_dim(pos,dim,val,valref)) return 0;

#define CheckListSquare(lpos,pos,m,n) if (! check_list_square(lpos,pos,m,n)) return 0;
#define CheckListVector(lpos,pos,m,n) if (! check_list_vector(lpos,pos,m,n)) return 0;
#define CheckListRow(lpos,pos,m,n) if (! check_list_row(lpos,pos,m,n)) return 0;
#define CheckListColumn(lpos,pos,m,n) if (! check_list_column(lpos,pos,m,n)) return 0;
#define CheckListScalar(lpos,pos,m,n) if (! check_list_scalar(lpos,pos,m,n)) return 0;
#define CheckListOneDim(lpos,pos,dim,val,valref) if (! check_list_one_dim(lpos,pos,dim,val,valref)) return 0;

/*-------------------------------------------------
 * structure used for optional arguments in interfaces
 * -------------------------------------------------*/

typedef struct rhs_opts__ {
  int position ; /** stack position : -1 if not present */
  char *name   ;
  char *type;
  int m,n;
  unsigned long int l;
} rhs_opts;

int get_optionals(char *name,rhs_opts opts[]);


/*-------------------------------------
 * get infos without being dependent
 * of stack structure !!! (! WOW !)
 *------------------------------------*/
#define getType(il)			*istk(il)
#define getNumberOfLines(il)		*istk(il+1)
#define getNumberOfColumns(il)		*istk(il+2)
#define getPrecision(il)		((getType(il) == sci_ints) ? (*istk(il+3)) : (0))
#define getDoubleDataAddress(il)	sadr(il + 4)
#define isComplex(il)			((getType(il) == sci_ints) ? (0) : (*istk(il+3))) == 0
#define getElementByAddress(ea)		*stk(ea)


/*------------------------------
 * prototypes 
 *-----------------------------*/

/* functions defined in stack-2.c */

extern int C2F(firstopt)(void);
extern int C2F(findopt)(char *, rhs_opts *);
extern int C2F(isopt)(int *,char *,unsigned long);
extern int C2F(checkrhs)(char *fname, int *imin, int *imax, unsigned long fname_len);
extern int C2F(checklhs)(char *fname, int *imin, int *imax, unsigned long fname_len);
extern void C2F(freeptr)(double *ip[]);

#ifdef __cplusplus
}
#endif
#endif /*  STACK_SCI  */


