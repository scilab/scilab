/* Common Block Declarations */
/* Copyright INRIA/ENPC */

#ifndef STACK_SCI 
#define STACK_SCI 

#if  !(defined __LCC__) && ( !(defined __MATH__)  || (defined __APPLE__) || (defined aix) ) 
#include "graphics/Math.h"
#else
#include "machine.h"
#define Min(x,y)	(((x)<(y))?(x):(y))
#define Max(x,y)	(((x)>(y))?(x):(y))
#endif

#include "stack-def.h"

/*-------------------------------------------------
 * types 
 * -------------------------------------------------*/

typedef enum {
  sci_matrix= 1 ,
  sci_poly= 2 ,
  sci_boolean= 4 ,
  sci_sparse= 5  ,
  sci_boolean_sparse= 6,
  sci_ints= 8,
  sci_handles=9,
  sci_strings= 10,
  sci_u_function= 11,
  sci_c_function= 13 ,
  sci_lib=  14,
  sci_list= 15,
  sci_tlist= 16,
  sci_mlist= 17
} sci_types; 

/*-------------------------------------------------
 * structure used for sparse matrix 
 * -------------------------------------------------*/

typedef struct scisparse { 
  integer m,n,it,nel ; /* nel : number of non nul elements */
  integer *mnel,*icol; /* mnel[i]: number of non nul elements of row i, size m 
		    * icol[j]: column of the j-th non nul element, size nel 
		    */
  double *R,*I ; /* R[j]: real value of the j-th non nul element, size nel 
		  * I[j]: imag value of the j-th non nul element, size nel 
		  */
} SciSparse ; 

extern SciSparse* NewSparse __PARAMS((int *it,int *m,int *n,int*nel));
extern void FreeSparse __PARAMS((     SciSparse *x));

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

typedef struct sciintmat { 
  integer m,n,it ; /* it : 1,2,4,11,12,14  */
  integer l;       /* if l != -1 then istk(l) == D */
  void *D;         /* data : should be casted according to it */
} SciIntMat ; 


#include "interf/stack1.h" 
#include "interf/stack2.h" 
#include "interf/stack3.h" 


/*-------------------------------------------------
 * set of defines for interface simplication 
 * -------------------------------------------------*/

static int c1_local;
static int c_local;

#define TRUE_ 1
#define FALSE_ 0

#ifndef NULL
#define NULL 0
#endif

#define iadr(l) ((l)+(l)-1)
#define sadr(l) (((l)/2)+1)
#define cadr(l) ((l)+(l)+(l)+(l)-3)

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
typedef struct { double r, i; } doublecomplex;
#define zstk(x) (((doublecomplex *) C2F(stack).Stk) + x-1 )


#define Lstk(x) (((integer *) C2F(vstk).lstk) + x-1 )
#define Infstk(x) (((integer *) C2F(vstk).infstk) + x-1 )

#ifndef FTable_H
extern  void * GetFuncPtr __PARAMS((char *,int,void *,void (*f)(),int *,int*,int*));  
#endif 
#define CheckOpt(first) if ( C2F(checkopt)(first) ) {return 0;}

#define FirstOpt() C2F(firstopt)()

#define FindOpt(str,opts) C2F(findopt)(str,opts)

#define NumOpt() C2F(numopt)() 

#define IsOpt(k,name) C2F(isopt)((c_local=k,&c_local),name,nlgh)

#define Maxvol(n,ct)  C2F(maxvol)((c_local=n,&c_local),ct,1L)

#define CreateVarFromPtr(n,ct,mx,nx,lx) if ( ! C2F(createvarfromptr)((c_local=n,&c_local),ct,mx,nx,(double *)lx,1L)) \
					     { return 0;} 

#define CreateCVarFromPtr(n,ct,it,mx,nx,lrx,lcx) if ( ! C2F(createcvarfromptr)((c_local=n,&c_local),ct,it,mx,nx,(double *)lrx,(double *) lcx,1L)) \
					     { return 0;} 
#define CreateRefFromName(n,nx) if(! C2F(createreffromname)(n,nx)){return 0;}

#define CreateRef(num, point) if(! C2F(createreference)(num,point)){return 0;}

#define ChangeToRef(num, point) C2F(changetoref)(num,point)

#define FreePtr(lx) C2F(freeptr)((double **) lx)

#define FreeRhsSVar(S) { c_local =0; while ( S[c_local] != 0) { FREE(S[c_local]);c_local++;}; FREE(S);} 

#define GetType(n)   C2F(gettype)((c_local = n +Top - Rhs, &c_local))

#define VarType(n)   C2F(vartype)((c_local=n,&c_local))

#define OverLoad(n) C2F(overload)((c_local = n +Top - Rhs, &c_local),fname,strlen(fname))

#define GetRhsVar(n,ct,mx,nx,lx) if (! C2F(getrhsvar)((c_local=n,&c_local),ct,mx,nx,(integer *) lx,1L))\
        { return 0;  }

#define CreateVar(n,ct,mx,nx,lx) if(! C2F(createvar)((c_local=n,&c_local),ct,mx,nx,(void *)lx, 1L))\
        { return 0;  }


#define CreateWork(n,mx,lx) if(! C2F(creatework)((c_local=n,&c_local),mx,(void *)lx))\
        { return 0;  }

#define SetWorkSize(n,mx) if(! C2F(setworksize)((c_local=n,&c_local),mx))\
        { return 0;  }

#define CreateData(n,nx) if(! C2F(createdata)((c_local=n,&c_local),nx))\
        { return 0;  }

#define GetRhsCVar(n,ct,it,mx,nx,lrx,lcx) if (! C2F(getrhscvar)((c_local=n,&c_local),ct,it,mx,nx,lrx,lcx,1L))\
        { return 0;  }

#define CreateCVar(n,ct,it,mx,nx,lrx,lcx) if(! C2F(createcvar)((c_local=n,&c_local),ct,it,mx,nx,lrx,lcx, 1L))\
        { return 0;  }

#define Error(x) C2F(error)((c_local=x,&c_local))

#define Errorinfo(routinename,info) C2F(errorinfo)(routinename,(c_local=info, &c_local), strlen(routinename));

#define PutLhsVar()  if (! C2F(putlhsvar)()) {	return 0; }

#define ReadMatrix(ct,mx,nx,w)  if (! C2F(creadmat)(ct,mx,nx,w,(unsigned long)strlen(ct) )) {	return 0; }

#define WriteMatrix(ct,mx,nx,w)  if (! C2F(cwritemat)(ct,mx,nx,w,strlen(ct) )) {	return 0; }

#define PutVar(num, nam)  if (! C2F(putvar)( (c_local=num, &c_local), nam, strlen(nam) )) {	return 0; }

#define ReadString(ct,mx,w)  if (! C2F(creadchain)(ct,mx,w,strlen(ct),strlen(w) )) {	return 0; }

#define WriteString(ct,mx,w)  if (! C2F(cwritechain)(ct,mx,w,strlen(ct),strlen(w) )) {	return 0; }

#define GetMatrixptr(ct,mx,nx,lx)  if (! C2F(cmatptr)(ct,mx,nx,lx,(unsigned long)strlen(ct) )) {	return 0; }

#define GetMatrixdims(n,mx,nx)  if (! C2F(getmatdims)((c_local=n,&c_local),mx,nx)) {	return 0; }

#define CreateVarFrom(n,ct,mx,nx,lx,lx1) if (!C2F(createvarfrom)((c_local=n,&c_local),ct,mx,nx,lx,lx1,1L))  { return 0;} 
#define CreateCVarFrom(n,ct,it,mx,nx,lx,lc,lx1,lc1) if (!C2F(createcvarfrom)((c_local=n,&c_local),ct,it,mx,nx,lx,lc,lx1,lc1,1L))  { return 0;} 

#define Createlist(m,n) C2F(createlist)((c_local=m,&c_local),(c1_local=n,&c1_local))

#define CreateListVarFrom(n,m,ct,mx,nx,lx,lx1) if (!C2F(createlistvarfrom)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,mx,nx,(void *)lx,(void *) lx1,1L))  { return 0;} 

#define CreateListCVarFrom(n,m,ct,it,mx,nx,lx,lc,lx1,lc1) if (!C2F(createlistcvarfrom)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,it,mx,nx,(void *)lx,(void *) lc,(void *) lx1,(void *)lc1,1L))  { return 0;} 

#define CreateListVarFromPtr(n,m,ct,mx,nx,lx1) if (!C2F(createlistvarfromptr)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,mx,nx,(void *) lx1,1L))  { return 0;} 

#define CreateListCVarFromPtr(n,m,ct,it,mx,nx,lx1,lc1) if (!C2F(createlistcvarfromptr)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,it,mx,nx,(void *) lx1,(void *) lc1,1L))  { return 0;} 



#define  GetListRhsVar(n,m,ct,m1e1,n1e1,l1e1)  if(!C2F(getlistrhsvar)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,m1e1,n1e1,(integer *) l1e1,1L)) {return 0;}

#define  GetListRhsCVar(n,m,ct,it,m1e1,n1e1,l1e1,l1e2)  if(!C2F(getlistrhscvar)((c_local=n,&c_local),(c1_local=m,&c1_local),ct,it,m1e1,n1e1,(integer *) l1e1,(integer *) l1e2,1L)) {return 0;}

#ifdef WIN32
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
    if( ! C2F(scistring)(ibegin,name,mlhs,mrhs,strlen(name))) return 0;

/** Used for calling a scilab function given as argument **/

#define SciFunction(ibegin,lf,mlhs,mrhs) \
    if( ! C2F(scifunction)(ibegin,lf,mlhs,mrhs)) return 0;

/** used for protecting a call to a Scilab function **/

#define PExecSciFunction(n,mx,nx,lx,name,fsqpenv) \
  if(! C2F(scifunction)((c_local=n,&c_local),mx,nx,lx))\
{ sciprint("Error in function %s\r\n",name);  longjmp(fsqpenv,-1); }

#define Nbvars C2F(intersci).nbvars 

#define LhsVar(x) C2F(intersci).lhsvar[x-1]


/* used to access data associated to a variable 
 * for example when a variable is created with CreateVarFromPtr(...) 
 */

#define VarPtr(x) C2F(intersci).lad[x-1] 

typedef int (*interfun) __PARAMS((char *fname,unsigned long l));

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

int get_optionals __PARAMS((char *name,rhs_opts opts[]));

/*------------------------------
 * prototypes 
 *-----------------------------*/

extern int C2F(error) __PARAMS((int *));
extern int C2F(getmatdims) __PARAMS((integer *, integer *, integer *));
extern int C2F(getrhsvar) __PARAMS((integer *, char *type, integer *, integer *, integer *, unsigned long));
extern int C2F(getrhscvar) __PARAMS((integer *, char *type,integer*,integer*, integer *, integer *, integer *, unsigned long));

extern int C2F(createvar) __PARAMS((integer *, char *, integer *, integer *, integer *, unsigned long ));
extern int C2F(createcvar) __PARAMS((integer *, char *,integer *,integer*, integer *, integer *, integer *, unsigned long ));

extern int C2F(putlhsvar) __PARAMS((void));
extern int C2F(cmatptr) __PARAMS((char *,integer *,integer *,integer *,unsigned long));
extern int C2F(createcvarfromptr) __PARAMS((integer *,char *,integer*, integer *, integer *,double *,double *, unsigned long));

extern int C2F(scifunction) __PARAMS ((integer *,integer *,integer *,integer*));

extern int C2F(firstopt) __PARAMS((void));
extern int C2F(findopt) __PARAMS((char *, rhs_opts *));

extern int C2F(numopt) __PARAMS((void));
extern int C2F(isopt) __PARAMS((integer *,char *,unsigned long));
extern int C2F(gettype) __PARAMS((integer *pos));

extern int C2F(checkrhs)  __PARAMS((char *fname, integer *imin, integer *imax, unsigned long fname_len));
extern int C2F(checklhs)  __PARAMS((char *fname, integer *imin, integer *imax, unsigned long fname_len));

extern void C2F(freeptr) __PARAMS((double *ip[]));

extern void *GetData __PARAMS((int lw));

extern void *GetRawData __PARAMS((int lw));

extern void *GetDataFromName __PARAMS(( char *name));

extern int GetDataSize __PARAMS((int lw));

#endif /*  STACK_SCI  */


