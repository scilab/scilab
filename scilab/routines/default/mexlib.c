#include "../mex.h"
#include <string.h>
#include <stdio.h>
#if defined  sgi && ! defined  __STDC__
#define __STDC__
#endif
 
#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif 

extern void cerro __PARAMS((char *str));
extern int C2F(dcopy) __PARAMS((int*, double *, int *, double *, int *));
extern int C2F(xscion)  __PARAMS((int *i));
extern void C2F (xscisncr) __PARAMS((char *str, integer *n, integer dummy)); 
extern int  C2F(mxgetm) __PARAMS((    Matrix *ptr));
extern int  C2F(mxgetn) __PARAMS((    Matrix *ptr));
extern void C2F(xscisrn) __PARAMS((char *str,int *n,int  dummy));
extern int  C2F(erro)  __PARAMS((char *str,int dummy));
extern int  C2F(hmcreate)  __PARAMS((int *lw,int *nz,int *sz,int *typv,int *iflag,int *retval));
extern int  C2F(structcreate)  __PARAMS((int *lw1,int *ndim,int *dims, int *nfields, char **field_names, int *retval));
extern double C2F(dlamch)  __PARAMS((char *CMACH, unsigned long int));

#define DOUBLEMATRIX 1
#define INTMATRIX 8
#define STRINGMATRIX  10
#define SPARSEMATRIX  7
#define LOGICAL 4
#define MLIST   17
#define AsIs  '$'
#define NDARRAY 1
#define CELL 2
#define STRUCT 3

/* s2iadr: loc adress to loci adress */
#define s2iadr(l) ((l)+(l)-1)
/* i2sadr: loci adress to loc adress */
#define i2sadr(l) ( ( (l)/2 ) +1 )

vraiptrst  stkptr(long int ptr_lstk)
{
  vraiptrst ptr =  C2F(locptr)(stk((long int)(ptr_lstk))); 
  return ptr;
}

Matrix *loci2ptr(int *loci)
{
  Matrix ptr;
  ptr=(Matrix) ((double *)loci-(double *)stkptr(C2F(vstk).Lstk[0])+C2F(vstk).Lstk[0]) ;
  return (Matrix *) ptr;
}
/*    ix1 = il + 3 + *n;
 *ili = sadr(ix1) + *istk(il + 2 + (*ix - 1) ) - 1; */



int *listentry(int *loci, int i)
{
  int n, *ptr; /* , lll;*/
  n = loci[1]; 
  if (i <= n) {
    if ( n%2 == 0 ) n++;
    /* jpc lll = loci + 3 + n + 2*( loci[2 + (i - 1) ] -1); 
       ptr = (int *) (lll); */
    ptr = (int *) loci + 3 + n + 2*( loci[2 + (i - 1) ] -1);
  return ptr;
  } else {
    return NULL;
  }
}

double *listentryold(int *loci, int i)
{
  int n;
  double *ptr;

  n = loci[1];
  if (i <= n) {
    ptr = (double *)(loci+3+n) + loci[2 + (i - 1) ] - 1;
    return  ptr;
  } else {
    return NULL;
  }
}

int theMLIST(int *loci)
{
  if (loci[0]==17 && loci[1]==3 && loci[6]==10) {
    if (loci[14]==12 && loci[15]==14) return 2;  /* CELL   */
    if (loci[14]==17 && loci[15]==22) return 1;  /* NDARRAY   */
  }
   if (loci[17]==28 && loci[18]==29 && loci[19]==27) return 3;  
   else return 0;
}


/*----------------------------------------------------------------
 *                       DOUBLEMATRIX                            
 *   loci[0]= DOUBLEMATRIX  =1                                   
 *   loci[1]= M (number of rows)                                 
 *   loci[2]= N (number of cols)                                 
 *   loci[3]= mxComplexity  (0 real, 1 complex)                  
 *   loc[2] = real part                                          
 *   loc[2+M*N] = imaginary part  (if mxComplexity =1)           
 *    
 *-----------------------------------------------------------------
 *                       INTMATRIX                               
 *   loci[0]= INTMATRIX =8                                       
 *   loci[1]= M (number of rows)                                 
 *   loci[2]= N (number of cols)                                 
 *   loci[3]= TYPE                                               
 *      TYPE=   01    02   04    11     12    14                      
 *             int8 int16 int32 uint8 uint16 uint32                   
 *   loc[2] = real part                                               
 *
 *------------------------------------------------------------------
 *                    SPARSEMATRIX                                    
 *   loci[0]= SPARSEMATRIX  =7 (converted from 8)                     
 *   loci[1]= M (number of rows)                                      
 *   loci[2]= N (number of cols)                                      
 *   loci[3]= mxComplexity  (0 real, 1 complex)                       
 *   loci[4]=nzmax                                                    
 *   loci[5]=Jc[0]                                                    
 *   loci[5+loci[2]+1]=Ir[0]                                         
 *   loc[(5+loci[2]+loci[4])/2 + 1] = real part                       
 *   loc[(5+loci[2]+loci[4])/2 + 1 + M*N] = imag part                 
 *
 *-------------------------------------------------------------------
 *                    STRINGMATRIX                                        
 *    loci[0]= STRINGMATRIX   = 10                                        
 *    loci[1]=  nrows                                                     
 *    loci[2]=  ncols  (MUST be 1 since Matlab string matrices are seen   
 *                           as Scilab  column matrices, all columns      
 *                           having  the same length                      
 *    loci[3]=  0                                                         
 *    loci[4:...]= 1, ptr1, ..., ptrnrows,                                
 *    loci[5]=loci[4] + length of string in row 1                         
 *    loci[6]=loci[5] + length of string in row 2                         
 *    loci[5+loci[1]:...] = code of strings  (int)                        
 *
 *--------------------------------------------------------------------
 *
 *                   MLIST                                      
 *           <=>  ndim matlab numeric or char array    
 *           <=>  mlist(["hm","dims","entries"],[dim1,...,dimk], vector)  
 *
 *            or
 *           <=>  matlab cell array
 *           <=> mlist(["ce","dims","entries"],[dim1,...,dimk], list())
 *
 *   loci[0]=   MLIST  =  17                                      
 *   loci[1]= 3   (3 elements in the mlist)                           
 *   loci[2,3,4,5]= 1,12,*,*  <-> 4 pointers                          
 * 
 * ->loci[6:27]  <-> ["hm","dims","entries"] = ...
 *                     HYPERMATRIX
 *             10,1,3,0,1,3,7,14,17,22,13,18,22,28,14,23,29,27,18,14,28,* 
 *                                h  m  d  i  m  s  e  n  t  r  i  e  s   
 *               <-> ["ce","dims","entries"] = ...
 *                     CELL
 *             10,1,3,0,1,3,7,14,12,14,13,18,22,28,14,23,29,27,18,14,28,* 
 *                                c  e  d  i  m  s  e  n  t  r  i  e  s   
 *
 * ->loci[28:...] <-> [dim1, ..., dimk] = ...
 *                1,ndims,1,0,****** (dimensions)  28=6+2*(loci[3]-1)
 *                            ^                                           
 *                            |                                           
 *                         loci[32]                                       
 *
 * ->loci[6+2*(loci[4]-1):...] <-> vector
 *                            = 1,m,n,it,*  for double  array               
 *                            = 8,m,n,it,* for int array   with n=1 
 *                            = 10,m,n,0,1,len+1 for char array with m=1, n=1 
 *                            = 15,n,...         for cell array
 *                              ^                                         
 *                              |                                         
 *                     loci[6+2*(loci[4]-1)]   ( = 1, 8, 10)
 *   loci[6+2*(loci[4]-1)+4:...] = ***** (entries) for double or int array
 *   
 *   loci[6+2*(loci[4]-1)+4:6+2*(loci[4]-1)+5] = [1,length+1]
 *   loci[6+2*(loci[4]-1)+6:...] = entries = code of string  (int)
 *
 *----------------------------------------------------------------------
 **************************************************************************/

mxClassID mxGetClassID(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: 
    return mxDOUBLE_CLASS;
  case STRINGMATRIX:
    return mxCHAR_CLASS;
  case SPARSEMATRIX:
    return mxSPARSE_CLASS;
  case INTMATRIX:
    /*[8,m,n,it] it=01    02   04    11     12    14
      int8 int16 int32 uint8 uint16 uint32    */
    switch (loci[3]){
    case 1:
      return mxINT8_CLASS;
    case 2:
      return mxINT16_CLASS;
    case 4:
      return mxINT32_CLASS;
    case 11:
      return mxUINT8_CLASS;
    case 12:
      return mxUINT16_CLASS;
    case 14:
      return mxUINT32_CLASS;
    default:
      return mxUNKNOWN_CLASS;
    }
  case MLIST:
    /* to be done return mxCELL_CLASS or mxCHAR_CLASS or mxSTRUCT_CLASS */
    /* loci[6+2*(loci[4]-1)]   ( = 1, 10, 8)  */
    switch (loci[6+2*(loci[4]-1)]) {
      /* listentry(loci,3) */
    case DOUBLEMATRIX:
      return mxDOUBLE_CLASS;
    case STRINGMATRIX:
      return mxCHAR_CLASS;
    case INTMATRIX:
      switch (loci[6+2*(loci[4]-1)+3]) {
      case 1:
	return mxINT8_CLASS;
      case 2:
	return mxINT16_CLASS;
      case 4:
	return mxINT32_CLASS;
      case 11:
	return mxUINT8_CLASS;
      case 12:
	return mxUINT16_CLASS;
      case 14:
	return mxUINT32_CLASS;
      default:
	return mxUNKNOWN_CLASS;
      }
    }
  default:
    return mxUNKNOWN_CLASS;
  }
}

double mxGetEps(void)
{
  double eps;
  eps=C2F(dlamch)("e",1L);
  return eps;
}


double mxGetInf(void)
{
  double big;
  big=C2F(dlamch)("o",1L);
  return big*big;
}

double mxGetNaN(void)
{
double x,y;
x=mxGetInf();
y=x/x;
  return y;
}

int mxIsInf(double x)
{
  if (x == x+1)
    return 1;
  else return 0;
}

int mxIsFinite(double x)
{
  if (x < x+1)
    return 1;
  else return 0;
}

int mxIsNaN(double x)
{
  if ( x != x ) 
    return 1;
  else return 0;
}
/*******************   C functions              ***************************/
int *mxGetHeader(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  return &loci[0];
}
int mxGetNumberOfElements(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int) ptr);
  int m, commonlength;
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX:
    return loci[1]*loci[2];
  case MLIST:
    return loci[6+2*(loci[4]-1)+1];
    /* debut=listentry(loci,3); (debut)... */
  case STRINGMATRIX:
    m=loci[1];
    commonlength=loci[5]-loci[4];
    return m*commonlength;
    /*case SPARSEMATRIX to be done */
  default:
    return 0;
  }
}


double *mxGetPr(Matrix *ptr)
{
  double *loc = (double *) stkptr((long int)ptr);
  int *loci = (int *) stkptr((long int)ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX:
    return (loci[1]==0 || loci[2] == 0) ? NULL : &loc[2];
  case MLIST:
    switch (loci[6+2*(loci[4]-1)]){
    case DOUBLEMATRIX: case INTMATRIX:
      return (double *) &loci[6+2*(loci[4]-1)+4];
    case STRINGMATRIX:
      return (double *) &loci[6+2*(loci[4]-1)+6];
    default:
      return 0;
    }
  case STRINGMATRIX:
    return &loc[i2sadr(5 + loci[2])];
  case SPARSEMATRIX:
    return &loc[ i2sadr(5+loci[2]+loci[4]) ];
  default:
    return 0;
  }
}


double *mxGetPi(Matrix *ptr)
{ 
  int debut; int m,n,it;
  double *loc = (double *) stkptr((long int)ptr);
  int *loci = (int *) stkptr((long int)ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX:
    if (loci[3]==0 || loci[1]==0 || loci[2] == 0) return NULL;
    return  &loc[2];&loc[2 + loci[1] * loci[2]];
  case MLIST:
    debut=6+2*(loci[4]-1);
    switch (loci[debut]){
    case DOUBLEMATRIX: case INTMATRIX:
      m=loci[debut+1];n=loci[debut+2];
      it=loci[debut+3];  /* it should be 1 */
      return (double *) &loci[debut+4+2*m*n];
    default:
      return 0;
    }
  case SPARSEMATRIX:
    if (loci[3]==0) return NULL;
    return &loc[ i2sadr(5+loci[2] +loci[4]) + loci[1]*loci[2]];
  default:
    return 0;
 }
}

int mxGetNumberOfDimensions(Matrix *ptr)
{
  int *loci1;
  int *loci = (int *) stkptr((long int)ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX: case SPARSEMATRIX:
    return 2;
  case MLIST:
    switch (theMLIST(loci)) {
    case NDARRAY:
      return loci[29]*loci[30]; /* loci[29] or loci[30] = 1  */
    case CELL:
      loci1 = (int *) listentry(loci,2)  ;
      return loci1[1]*loci1[2];
    case STRUCT:
      loci1 = (int *) listentry(loci,2)  ;
      return loci1[1]*loci1[2];
    default:
      return 0;
	}
  default:
    return 0;
  }
}

int *mxGetDimensions(Matrix *ptr)
{
  int *loci1;
  int *loci = (int *) stkptr((long int) ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX: case STRINGMATRIX:
    return &loci[1];
  case SPARSEMATRIX: /* to be done  */
    break;
  case MLIST:
    switch (theMLIST(loci)) {
      /*  case NDARRAY       return &loci[32];       break;  */
    case NDARRAY: case CELL: case STRUCT:
      loci1 = (int *) listentry(loci,2);
      return &loci1[4];
  default:
    return 0;
    }
  }
  return 0;
}

int mxGetM(Matrix *ptr)
{
  int *loci1;
  int *loci= (int *) stkptr((long int)ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX: case STRINGMATRIX: case SPARSEMATRIX:
    return loci[1];
  case MLIST:
    switch (theMLIST(loci)) {
    case NDARRAY:
      return loci[32]; 
    case CELL: case STRUCT:
      loci1 = (int *) listentry(loci,2);
      return loci1[4];
    default:
      return 0;
    }
  default:
    return 0;
  }
}

void mxSetM(Matrix *ptr, int m)
{
  int *loci = (int *) stkptr((long int)ptr);
  int oldM; int commonlength; int j;
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX: case SPARSEMATRIX:
    loci[1]=m;
    break;
  case STRINGMATRIX:
    /* string: copy loci[5+oldM] to loci[5+mnew]  */    
    oldM=loci[1];
    commonlength=loci[5]-loci[4];
    for (j=0; j<=m*commonlength; ++j) {
      loci[5+j+m]=loci[5+j+oldM];
    }
    loci[1]=m;
    break;
  default:
    break;
  }
}

int *mxGetJc(Matrix *ptr)
{
  int *loci= (int *) stkptr((long int)ptr);
  if (loci[0] != SPARSEMATRIX) {
    return 0;}
  /*  loci[0]=7;  M=loci[1];  N=loci[2];  it =loci[3];  nzmax=loci[4];
      Jc[0]=loci[5];  Jc[1]=loci[6]; etc; Jc[N]=nnz; */
  return &loci[5];
}

int *mxGetIr(Matrix *ptr)
{
  int *loci= (int *) stkptr((long int)ptr);
  /* ... N=loci[2],  nzmax=loci[4]; then Jc,  then Ir   */
  return &loci[5+loci[2]+1];
}


int mxGetN(Matrix *ptr)
{
  int ret;int j;
  int *loci1;
  int *loci= (int *) stkptr((long int)ptr);
  int numberofdim;

  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX: case SPARSEMATRIX:
    return loci[2];
  case STRINGMATRIX:
    /*  for strings, N=length of first (and unique, loci[2]=1) column 
        (all rows have same Matlab-length) */
    return loci[5]-loci[4];
  case MLIST:
    switch (theMLIST(loci)) {
    case NDARRAY: case CELL: case STRUCT:
      loci1 = (int *) listentry(loci,2);
      numberofdim = loci1[1]*loci1[2];
      if (numberofdim==2)
      return loci1[5];
      else
	ret=loci1[5];
      for (j=0; j < numberofdim-2; ++j) ret=ret*loci1[6+j]; 
      return ret;
    default:
      return 0;
	}
  default:
    return 0;
  }
}


void mxSetN(Matrix *ptr, int n)
{
  int *loci = (int *) stkptr((long int)ptr);
  int i,m;
  switch (loci[0]) {
  case STRINGMATRIX:
    m=loci[1];
    /* oldN = loci[5]-loci[4];  */
    for (i=0; i<m; ++i) {
      loci[5+i]=loci[4+i]+n;
      /* to be done: compress loci[5+m]  */
    }
    break;
  case DOUBLEMATRIX: case INTMATRIX: 
    loci[2]=n;
    break;
  default:
    break;
  }
}

int mxIsString(Matrix *ptr)
{
  int *loci=(int *) stkptr((long int)ptr);
  if (loci[0] == STRINGMATRIX) 
    return 1;
  else return 0;
} 

int mxIsChar(Matrix *ptr)
{
  int *loci=(int *) stkptr((long int)ptr);
  switch (loci[0]) {
  case STRINGMATRIX:
    return 1;
  case MLIST:
    switch (loci[6+2*(loci[4]-1)]){
    case STRINGMATRIX:
      return 1;
    default:
      return 0;
    }
  default:
    return 0;
  } 
}

int mxIsNumeric(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  if (loci[0] != STRINGMATRIX)
    return 1;
  else
    return 0;
}

int mxIsDouble(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  if ((loci[0] == DOUBLEMATRIX) | (loci[0] == SPARSEMATRIX) | ( (loci[0] == MLIST) && (loci[6+2*(loci[4]-1)] == DOUBLEMATRIX)))
    return 1;
  else 
    return 0;
}

int mxIsEmpty(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  if ( (loci[0] == DOUBLEMATRIX) | (loci[0] == SPARSEMATRIX) ) 
    return 1-loci[1]*loci[2];
  else 
    return 0;
}

int mxIsFull(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  if ( loci[0] != SPARSEMATRIX)
    return 1;
  else
    return 0;
}

int mxIsSparse(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  if (loci[0] == SPARSEMATRIX) 
    return 1;
  else
    return 0;
}

int mxIsLogical(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  if (loci[0] == LOGICAL) 
    return 1;
  else
    return 0;
}

void mxSetLogical(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  loci[0] = LOGICAL;
}

void mxClearLogical(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  if (loci[0] != LOGICAL)
    mexErrMsgTxt("Variable is not logical");
  loci[0] = DOUBLEMATRIX;
}

int mxIsComplex(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX:
    return loci[3];
  case MLIST:
    if (loci[6+2*(loci[4]-1)] == DOUBLEMATRIX) {
      return loci[6+2*(loci[4]-1)+3];
    }
  default:
    return 0;
  }
}


double mxGetScalar(Matrix *ptr)
{ 
  double *loc = (double *) stkptr((long int)ptr);
  int *loci = (int *) stkptr((long int)ptr);
  if (loci[0] == DOUBLEMATRIX) {
    return  loc[2];
  }
  else if (loci[0] == SPARSEMATRIX) {
    /* nnz = loci[4];    N = loci[2]; */
    return loc[ i2sadr(5+loci[2]+loci[4]) ];
  }
  else {
    return 0;
  }
}


void *mxGetData(Matrix *ptr)
{
  return mxGetPr(ptr);
  /*  double *loc = (double *) stkptr(ptr);
  int *loci = (int *) stkptr(ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX:
    return &loc[2];
  case MLIST:
    return &loc[loci[4]+4];
  case STRINGMATRIX:
    return &loci[5+loci[1]];
  case SPARSEMATRIX:
    return &loc[i2sadr(5+loci[2]+loci[4])];
  default:
  return 0;
  }  */
}

void *mxGetImagData(Matrix *ptr)
{
  return mxGetPi(ptr);
}

void mexErrMsgTxt(char *error_msg)
{
  cerro(error_msg);
  errjump();
}
void mxAssert(int expr, char *error_message)
{
  if (!expr) mexErrMsgTxt(error_message);
}

Matrix *mxCreateFull(int m, int n, int it)
{
  static int lw, lr, lc;
  int k;
  lw = Nbvars + 1;
  /* sciprint("mxCreateFull XXXX  %d\n\r",lw); */
  if (! C2F(createcvar)(&lw, "d", &it, &m, &n, &lr, &lc, 1L)) {
    mexErrMsgTxt("No more memory available: increase stacksize");
  }
  for ( k=0; k<m*n*(it+1); k++ ) {
   *stk(lr+k)=0;
  }
   return (Matrix *) C2F(vstk).Lstk[lw + Top - Rhs - 1];  /* C2F(intersci).iwhere[lw-1]);  */
}

int mxIsClass(Matrix *ptr, char *name)
{
  int *loci = (int *) stkptr((long int)ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: 
    if ( strcmp(name,"double") == 0) return 1;
    break;
  case MLIST:
    switch (loci[6+2*(loci[4]-1)]){
    case DOUBLEMATRIX:
      if ( strcmp(name,"double") == 0) return 1;
      break;
    case INTMATRIX:
      if ( strcmp(name,"double") == 0) return 1;
      break;
    }
    break;
  case STRINGMATRIX:
    if ( strcmp(name,"char") == 0) return 1;
    break;
  case SPARSEMATRIX:
    if ( strcmp(name,"sparse") == 0) return 1;
    break;
  default:
    return 0;
  }
  return 0;
}
mxArray *mxCreateStructArray(int ndim, int *dims, int nfields, char **field_names)
{
  static int lw,lw1;
  int retval;
  Nbvars++;
  lw = Nbvars;
  lw1 = lw + Top - Rhs;
  /* int C2F(structcreate)(lw, nz, sz, nf, fnames,retval) */
  /*  C2F(hmcreate)(&lw1, &ndim, dims, &CLASS, &cmplx,&retval); */
  C2F(structcreate)(&lw1, &ndim, dims, &nfields, field_names, &retval);
  if( !retval) {
    return (Matrix *) 0;
  }
  C2F(intersci).ntypes[lw-1]=AsIs;
  return (Matrix *) C2F(vstk).Lstk[lw + Top - Rhs - 1 ];
}

Matrix *mxCreateNumericArray(int ndim, int *dims, int CLASS, int cmplx)
{
  static int lw,lw1;
  int retval;
  Nbvars++;
  lw = Nbvars;
  lw1 = lw + Top - Rhs;
  C2F(hmcreate)(&lw1, &ndim, dims, &CLASS, &cmplx,&retval);
  if( !retval) {
    return (Matrix *) 0;
  }
  C2F(intersci).ntypes[lw-1]=AsIs;
  return (Matrix *) C2F(vstk).Lstk[lw + Top - Rhs - 1 ];  /* C2F(intersci).iwhere[lw-1])  */
}

Matrix *mxCreateCharArray(int ndim, int *dims)
{
  static int lw,lw1, CLASS, cmplx;
  int retval;
  Nbvars++;
  lw = Nbvars;
  lw1 = lw + Top - Rhs;
  C2F(hmcreate)(&lw1, &ndim, dims, (CLASS=1, &CLASS),(cmplx=0, &cmplx),&retval);
  if( !retval) {
    return (Matrix *) 0;
  }
  C2F(intersci).ntypes[lw-1]=AsIs;
  return (Matrix *) C2F(vstk).Lstk[lw+ Top - Rhs - 1];  /* C2F(intersci).iwhere[lw-1])  */
}

Matrix *mxCreateCellArray(int ndim, int *dims)
{
  Matrix *ppr[1];Matrix *ppl[1];
  int k, nlhs, nrhs;
  double  *start_of_pr;
  ppr[0] = mxCreateDoubleMatrix( ndim , 1, 0);
  start_of_pr = (double *) mxGetPr(ppr[0]);
  for ( k=0; k<ndim; k++ ) {
    start_of_pr[k]= (double) dims[k];
  }
  nrhs=1;nlhs=1;
  /* cell = mlist(["ce","dims","entries"],[dim1,...,dimk],list([],...,[])) */
  /* w=makecell(dims) w=mlist(["ce","dims","entries"],dims,list())  */
  mexCallSCILAB(nlhs, ppl, nrhs, ppr, "makecell");    
  C2F(intersci).ntypes[Nbvars + Top - Rhs -1]=AsIs;
  return ppl[0];
}



Matrix *mxGetCell(Matrix *ptr, int index)
{
  int *locilist,*lociobj,*lociobjcopy;
  int *loci = (int *) stkptr((long int)ptr);
  int kk,lw,isize;
  locilist = listentry(loci,3);
  lociobj = listentry(locilist,index);
  isize=2*(locilist[index+2]-locilist[index+1]);
  Nbvars++;
  lw=Nbvars;
  CreateData(lw,4*isize);
  lociobjcopy=GetData(lw);
  for (kk = 0; kk < isize; ++kk) lociobjcopy[kk]=lociobj[kk];
  C2F(intersci).ntypes[lw-1]=AsIs;
  C2F(intersci).iwhere[lw-1]=C2F(vstk).Lstk[lw+ Top - Rhs - 1];
  /* TO BE REDONE! */
  return (Matrix *) C2F(vstk).Lstk[lw+ Top - Rhs - 1];  /* C2F(intersci).iwhere[lw-1])  */
}

void *mxCalloc(unsigned int n, unsigned int size)
{
  int m;  vraiptrst lrd;
  m = (n * size) /sizeof(double) + 1;
  if (! C2F(createstkptr)( &m, &lrd)) {
    return 0;
  }
  return (void *) lrd;
}

void *mxMalloc(unsigned int nsize)
{
  int m;  vraiptrst lrd;
  m = (nsize) /sizeof(double) + 1;
  if (! C2F(createstkptr)( &m, &lrd)) {
    return 0;
  }
  return (void *) lrd;
}

int mxIsCell(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  if (loci[0]==17 && loci[1]==3 && loci[6]==10 && loci[14]==12 && loci[15]==14)
    return 1;
  else return 0;
}

int mxIsStruct(Matrix *ptr)
{
  int *loci = (int *) stkptr((long int)ptr);
  /* mlist(["struct","dims","fields"],
           [d1,..,dk],["f1",...,"fp"],list1(...),listp(...)) */
  if (loci[0]==17 && loci[17]==28 && loci[18]==29 && loci[19]==27)
    return 1;
  else return 0;
}
      
/***************************************************************
 * Return in str at most strl characters from first element of 
 * string Matrix pointed to by ptr ( ptr is assumed to be a String Matrix )
 **************************************************************/

int mxGetString(Matrix *ptr, char *str, int strl)
{
  int *loci = (int *) stkptr((long int)ptr);
  int commonlength, firstchain; 
  int nrows = loci[1];
  /*  int ncols = loci[2]; This is 1 */
  /* commonlength=nrows*(loci[5]-loci[4]); */
  commonlength=loci[5]-loci[4];
  firstchain=5+nrows;
  C2F(in2str)(&commonlength, &loci[firstchain], str,0L);
  return 0;
}

/*-------------------------------------------------
 * mxFreeMatrix : 
 * mxFreeMatrix will do something only if it is called 
 * in the reverse order of variable allocation 
 * Thus after calling mexCallScilab in order to 
 * free the stack, one has to call mxFree as in the 
 * following example :
 *
 * int nlhs = 3; int nrhs = 2;
 * rhs[0] = mxCreateString(filename);
 * rhs[1] = mxCreateFull(1,1,REAL);
 * mexCallMATLAB(nlhs, lhs, nrhs, rhs, "gm_ifile");
 *  free space in reverse order 
 * for (i= nlhs -1 ; i >=0 ; i--) mxFreeMatrix(lhs[i]);
 * for (i= rhs -1 ; i >=0 ; i--) mxFreeMatrix(rhs[i]);
 *-------------------------------------------------*/

void mxFreeMatrix(Matrix *ptr)
{
  /* If we free the last stored object we can decrement Nbvars */
  if ( (int)ptr == C2F(vstk).Lstk[Top - Rhs + Nbvars - 1]) {
    /* sciprint("XXXX OK %dvar %d \r\n",(int)ptr,Nbvars); */
    Nbvars--;
  }
  else {
    /* sciprint("XXXX Fail %d var %d\r\n",(int)ptr,Nbvars); */
  }
  /* Automatically freed when return from mexfunction */
  return ;
}


void mxDestroyArray(Matrix *ptr)
{   /* No need */
  return; 
}

void mxFree(void *ptr)
{
  /* No need to free */
  return ;
}

int mexAtExit(Matrix *ptr)
{
  /* XXXXX To be done....*/
  return 0;
}

Matrix *mxCreateSparse(int m, int n, int nzmax, int cmplx)
{
  static int lw, iprov;
  Nbvars++;
  lw=Nbvars;
  iprov = lw + Top - Rhs;
  if( ! C2F(mspcreate)(&iprov, &m, &n, &nzmax, &cmplx)) {
    return (Matrix *) 0;
  }
  C2F(intersci).ntypes[lw-1]=AsIs;
  return (Matrix *) C2F(vstk).Lstk[lw + Top - Rhs - 1];     /* C2F(intersci).iwhere[lw-1])  */
}

/***************************************************************
 * Create on Scilab Stack a 1x1 string matrix filled with string
 **************************************************************/

Matrix *mxCreateString(const char *string)
{
  static int i, lw;
  static int one=1;
  i = strlen(string);
  lw=Nbvars+1;
  /* we do not increment Nbvars since it is done inside createvar */
  if ( ! C2F(createvarfromptr)(&lw, "c", &one, &i, (double *) &string, 1L) ) {
    return (Matrix *) 0;
  }
  /* back conversion to Scilab coding */
  Convert2Sci(lw);
  C2F(intersci).ntypes[lw-1]=AsIs;
  return (Matrix *) C2F(vstk).Lstk[lw + Top - Rhs - 1];
}

/* 
  Print function which prints (format,args,....) 
  in Scilab window
*/


/*
#ifdef __STDC__ 
void  C2F(mexprintf)(char *fmt,...) 
#else 
void  C2F(mexprintf)(va_alist) va_dcl
#endif 
{
  int i;  integer lstr;  va_list ap;  char s_buf[1024];
#ifdef __STDC__
  va_start(ap,fmt);
#else
  char *fmt;
  va_start(ap);
  fmt = va_arg(ap, char *);
#endif
  C2F(xscion)(&i);
  if (i == 0) 
    {
      (void)  vfprintf(stdout, fmt, ap );
    }
  else 
    {
      (void ) vsprintf(s_buf, fmt, ap );
      lstr=strlen(s_buf);
      C2F(xscisrn)(s_buf,&lstr,0L);
    }
  va_end(ap);
}
*/


void mexWarnMsgTxt(char *error_msg)
{
  mexPrintf("Warning: ");
  mexPrintf(error_msg);
  mexPrintf("\n\n");
  /*  mexPrintf(strcat("Warning: ",error_msg)); */
}

/* 1 is returned in case of failure */

static int mexCallSCI(int nlhs, Matrix **plhs, int nrhs, Matrix **prhs, char *name,int jumpflag)
{
  static int i1, i2;
  static int k, kk, nv;
  nv = Nbvars;
  for (k = 1; k <= nrhs; ++k) 
    {
      for (kk = 1; kk <= nv; ++kk) 
	if ((int) prhs[k-1] == C2F(vstk).Lstk[kk + Top - Rhs - 1]) break;
      if (kk == nv + 1) 
	{
	  mexErrMsgTxt("mexCallSCILAB: invalid pointer passed to called function");
	  return 1;
	} 
      else 
	{
	  ++Nbvars;
	  i1 = Top - Rhs + kk;
	  i2 = Top - Rhs + Nbvars;
	  C2F(vcopyobj)("mexCallSCILAB", &i1, &i2, 13L);
	}
    }
  i1 = nv + 1;
  if (! C2F(scistring)(&i1, name, &nlhs, &nrhs, strlen(name) )) { 
    if ( jumpflag == 1) 
      {
	mexErrMsgTxt("mexCallSCILAB: evaluation failed ");
      }
    return 1; 
    /*	      return 0;  */
  }
  for (k = 1; k <= nlhs; ++k) {
    plhs[k-1] = (Matrix *) C2F(vstk).Lstk[nv + k + Top - Rhs - 1];
  }
  Nbvars = nv+nlhs;
  return 0;
}


int mexCallSCILAB(int nlhs, Matrix **plhs, int nrhs, Matrix **prhs, char *name) {
  return mexCallSCI(nlhs,plhs,nrhs,prhs,name,1); 
}

int mxCalcSingleSubscript(Matrix *ptr, int nsubs, int *subs)
{
  int k, retval, coeff;
  int *dims = mxGetDimensions(ptr);
  retval=0;coeff=1;
  for  (k=0; k<nsubs; k++) {
    retval=retval+subs[k]*coeff;
    coeff=coeff*dims[k];
  }
  return retval;
}

int C2F(mexcallscilab)(integer *nlhs, Matrix **plhs, integer *nrhs, Matrix **prhs, char *name, int namelen)
{
  return mexCallSCILAB(*nlhs, plhs, *nrhs, prhs, name);
}

/** generic mex interface **/

int mex_gateway(char *fname, GatefuncH F)
{
  int nlhs,nrhs;
  Matrix * plhs[intersiz];  Matrix * prhs[intersiz];
  C2F(initmex)(&nlhs, plhs, &nrhs, prhs);
  (*F)(nlhs, plhs, nrhs, prhs);
  C2F(endmex)(&nlhs, plhs, &nrhs, prhs);
  return 0;
}

int fortran_mex_gateway(char *fname, FGatefuncH F)
{
  int nlhs,nrhs;
  Matrix * plhs[intersiz];  Matrix * prhs[intersiz];
  C2F(initmex)(&nlhs, plhs, &nrhs, prhs);
  (*F)( &nlhs, plhs, &nrhs, prhs);
  C2F(endmex)(&nlhs, plhs, &nrhs, prhs);
  return 0;
}


/** generic scilab interface **/

int sci_gateway(char *fname, GatefuncS F)
{
  (*F)(fname,strlen(fname));
  if (!C2F(putlhsvar)()) {return 0;}
  return 0;
}

int mxGetElementSize(Matrix *ptr)
{ int k, it; 
 int *loci = (int *) stkptr((long int)ptr);
 switch (loci[0]) {
 case DOUBLEMATRIX: case SPARSEMATRIX:
   return sizeof(double);
 case STRINGMATRIX:
   return 2*sizeof(char);    /* ? Matlab returns 2 */
 case MLIST:
   k=loci[4];
   /*   loci[6+2*(k-1)]  =  1 if double matrix, 8 if integer  */
   if (loci[6+2*(k-1)]==DOUBLEMATRIX)  return sizeof(double);
   if (loci[6+2*(k-1)]==INTMATRIX) {
     it= loci[6+2*(k-1)+3]; return( it%10 );   
   }
   break;
 case INTMATRIX:
   /*[8,m,n,it] it=01    02   04    11     12    14
     int8 int16 int32 uint8 uint16 uint32    */
   it=loci[3];
   return( it%10 );
 default:
   mexErrMsgTxt("GetElementSize error");
   return 0;
 }
 return 0;
}

Matrix *mxCreateCharMatrixFromStrings(int m, char **str)
{
  static int i, lr1, lw, ilocal, n, k,kk, nmax;
  nmax = strlen(str[0]);
  for (k=1; k<m; ++k) {
    n=strlen(str[k]);
    if (n > nmax) nmax=n;
  }
  Nbvars++;
  lw = Nbvars + Top -Rhs;
  /*      logical function cresmat3(fname,lw,m,n,nchar,lr) */
  if( ! C2F(cresmat4)("mxCreateString:", &lw, &m, &nmax, &lr1, 15L)) {
    return (Matrix *) 0;
  }
  /*    Convert string to integer code    */
  /*   subroutine cvstr(n,line,str,job) */
  for (k=0; k<m; ++k) {
    n=strlen(str[k]);
    /* fill with blanks (40) if n<nmax !!*/
    C2F(cvstr)(&n, istk(lr1), str[k], (ilocal=0, &ilocal),i);
    if (nmax>n) {
      for (kk=0; kk<nmax-n; ++kk) *istk(lr1+n+kk)=40;
    };
    lr1=lr1+nmax;
  }
  /*    ichar($) = 36 : variable returned as is to scilab */
  C2F(intersci).ntypes[lw-1]=AsIs;
  return (Matrix *) C2F(vstk).Lstk[lw - 1];
}

int mexEvalString(char *name)
{
  int rep;
  Matrix *ppr[1];Matrix *ppl[1];
  int nlhs;     int nrhs;
  *ppr = mxCreateString(name);
  nrhs=1;nlhs=0;
  if (( rep = mexCallSCI(nlhs, ppl, nrhs, ppr, "execstr",0))==1) 
    {
      sciprintf("Error in mexEvalString %s\r\n",name);
      errjump();
    }
  return rep;
}

mxArray *mexGetArray(char *name, char *workspace)
{
  int lw;
  C2F(objptr)(name,&lw,strlen(name));
  return (mxArray *) lw;
}



int C2F(initmex)(integer *nlhs, Matrix **plhs, integer *nrhs, Matrix **prhs)
{
  int *loci;int *loci1;
  static int output, k, RhsVar;
  static int m, commonlength, line;
  if (Rhs==-1) Rhs++;
  Nbvars = 0;
  *nlhs = Lhs;  *nrhs = Rhs;
  for (output = 1; output <= *nlhs ; ++output) plhs[output-1]=0;
  for (k = 1; k <= *nrhs ; ++k) 
    {
      RhsVar = k  + Top - Rhs;
      prhs[k-1] = (Matrix *) C2F(vstk).Lstk[RhsVar - 1];
      C2F(intersci).ntypes[k-1]=AsIs;
      loci = (int *) stkptr((long int)prhs[k-1]);
      switch (loci[0]) {
      case DOUBLEMATRIX: case INTMATRIX: case SPARSEMATRIX:
	break;
      case STRINGMATRIX:
	if (loci[2] !=1) mexErrMsgTxt("Invalid string matrix (at most one column!)");
	m=loci[1];
	commonlength=loci[5]-loci[4];
	if (m > 1) {
 	  for (line = 1; line < m; line++)
	    {
	      if (loci[5+line] - loci[4+line] != commonlength)
		mexErrMsgTxt("Column length of string matrix must agree!");
	    }
	}
	break;
      case 5:
	mexErrMsgTxt("Use mtlb_sparse(sparse( ))!");
	/*   scilab sparse  should be matlabified  */
	return 0;
      case MLIST:
	loci1 = (int *) listentry(loci,2);
	m = loci1[1]*loci1[2];
	C2F(entier)(&m, (double *) &loci1[4], &loci1[4]);
	break;
      default:
	mexErrMsgTxt("Invalid input");
	return 0;
      }
    }
  Nbvars = Rhs ;
  return 0;
}


/****************************************************
 * A set of utility functions 
 ****************************************************/

vraiptrst C2F(locptr)(void * x)
{
  return((vraiptrst) x);
}

int C2F(createstkptr)(integer *m, vraiptrst *ptr)
{
  int lr, n=1, lw;
  Nbvars++;
  lw = Nbvars;
  if (! C2F(createvar)(&lw, "d", m, &n, &lr, 1L)) {
    return 0;
  }
  *ptr = C2F(locptr)(stk(lr));
  return 1;
} 

int C2F(createptr)(char *type, int *m, int *n, int *it, int *lr, int *ptr, long int type_len)
{
  static int lc, lw;
  Nbvars++;
  lw = Nbvars;
  sciprint("createptr XXXX  %d\n\r",lw);
  if (! C2F(createcvar)(&lw, type, it, m, n, lr, &lc, 1L)) {
    return 0;
  }
  *ptr = C2F(vstk).Lstk[lw + Top - Rhs - 1];
  return 1;
} 

int C2F(endmex)(integer *nlhs, Matrix **plhs, integer *nrhs, Matrix **prhs)
{
  int nv=Nbvars ,kk,k; 
  for (k = 1; k <= *nlhs; k++)
    {
      LhsVar(k) = 0;
      for (kk = 1; kk <= nv; kk++)
	{
	  if ((int) plhs[k-1] == C2F(vstk).Lstk[kk+ Top - Rhs -1]) 
	    {
	      LhsVar(k) = kk; 
	      break;
	    }
	}
    }
  C2F(putlhsvar)();
  return 0;
}

/* a utility function to recover available Vars position */ 

void clear_mex(integer nlhs, Matrix **plhs, integer nrhs, Matrix **prhs)
{
  int nv=Nbvars ,kk,k; 
  int max = (int) plhs[0] ; 
  for (k = 1; k <= nlhs; k++)
    if (  (int)plhs[k-1] > max ) max =  (int)plhs[k-1];
  for (k = 1; k <= nrhs; k++)
    if ( (int)  prhs[k-1] > max ) max =  (int) prhs[k-1];
  for (k = 1; k <= nv; k++)
    if ( max <  C2F(vstk).Lstk[k + Top - Rhs -1]) Nbvars--;
}

void mexInfo(char *str) {
  sciprint("%s %d\r\n",str,Nbvars);
}

/****************************************************
 * C functions for Fortran  mexfunctions 
 ****************************************************/

double * C2F(mxgetpr)(Matrix *ptr)
{
  double *loc = (double *) stkptr(*ptr);
  int *loci = (int *) stkptr(*ptr);
  if (loci[0]==DOUBLEMATRIX) {
    return &loc[2];
  }
  else if ( loci[0]==SPARSEMATRIX ) { 
    /*    nnz = loci[4];  N = loci[2];  */
    return &loc[ i2sadr(5+loci[2]+loci[4]) ];
  }
  else {
    return 0;
  }
}


double * C2F(mxgetpi)(Matrix *ptr)
{
  double *loc = (double *) stkptr(*ptr);
  return &loc[2 +  C2F(mxgetm)(ptr) *  C2F(mxgetn)(ptr)];
}

int  C2F(mxgetm)(Matrix *ptr)
{
  int *loc= (int *) stkptr(*ptr);
  return loc[1] ;
}

int  C2F(mxgetn)(Matrix *ptr)
{
  int *loc = (int *) stkptr(*ptr);
  if ( loc[0] == STRINGMATRIX) {
    return loc[5]-1;
  }
  return loc[2] ;
}

int  C2F(mxisstring)(Matrix *ptr)
{
  int *loc=(int *) stkptr(*ptr);
  if (loc[0] == STRINGMATRIX) 
    return 1;
  else return 0;
} 

int  C2F(mxisnumeric)(Matrix *ptr)
{
  int *loc = (int *) stkptr(*ptr);
  if ( loc[0] == DOUBLEMATRIX) 
    return 1;
  else 
    return 0;
}

int  C2F(mxisfull)(Matrix *ptr)
{
  int *loc = (int *) stkptr(*ptr);
  if ( loc[0] == DOUBLEMATRIX) {
    return 1;
  }
  return 0;
}

int  C2F(mxissparse)(Matrix *ptr)
{
  int *loc = (int *) stkptr(*ptr);
  if (loc[0] == SPARSEMATRIX) {
    return 1;
  }
  return 0;
}


int  C2F(mxiscomplex)(Matrix *ptr)
{
  int *loc = (int *) stkptr(*ptr);
  if (loc[3] == DOUBLEMATRIX) {
    return 1;
  }
  return 0;
}

double  C2F(mxgetscalar)(Matrix *ptr)
{ static int N,nnz;
 double *loc = (double *) stkptr(*ptr);
 int *loci = (int *) stkptr(*ptr);
 if (loci[0] == DOUBLEMATRIX) {
   return  loc[2];
 }
 else if (loci[0] == SPARSEMATRIX) {
   nnz = loci[4];
   N = loci[2];
   return loc[(5+nnz+N)/2+1];
 }
 else {
   return 0;
 }
}

void  C2F(mexprintf)(char *error_msg, int len)
{
  char * buf;
  if ((buf = (char *)malloc((unsigned)sizeof(char)*(len+1)))
      == NULL) {
    cerro("Running out of memory");
    return;
  }
  buf[len]='\0';
  strncpy(buf, error_msg, (size_t)len);
  sciprint("%s\n\r",buf);
  free(buf);
}

void C2F(mexerrmsgtxt)(char *error_msg, int len)
{
  C2F(erro)(error_msg,len);
  errjump();
}

Matrix *C2F(mxcreatefull)(int *m, int *n, int *it)
{
  static int lr1;
  int lrd;
  if (! C2F(createptr)("d", m, n, it, &lr1, &lrd,1L)) {
    return  (Matrix *) 0;
  }
  return (Matrix *) lrd ;
} 


unsigned long int C2F(mxcalloc)(unsigned int *n, unsigned int *size)
{
  int m;
  vraiptrst lrd;
  m = (*n * *size) /sizeof(double) + 1;
  if (! C2F(createstkptr)(&m, &lrd)) {
    return 0;
  }
  return (unsigned long int) lrd;
}

int  C2F(mxgetstring)(Matrix *ptr, char *str, int *strl)
{
  int *loc = (int *) stkptr(*ptr);
  int commonlength;  int nrows = loc[1];
  /*  int ncols = loc[2]; This is 1 */
  commonlength=nrows*(loc[5]-loc[4]);
  C2F(in2str)(&commonlength, &loc[5+nrows], str,0L);
  *strl = Min(*strl,commonlength);
  return 0;
}

void C2F(mxfreematrix)(Matrix *ptr)
{
  return ;
}


Matrix * C2F(mxcreatestring)(char *string, long int l)
{
  static int i, lr1, lw, iprov, ilocal; 
  /** i = strlen(string); **/
  i= l;
  Nbvars++;
  lw=Nbvars;
  iprov = lw + Top -Rhs;
  if( ! C2F(cresmat2)("mxCreateString:", &iprov, &i, &lr1, 15L)) {
    return (Matrix *) 0;
  }
  C2F(cvstr)(&i, istk(lr1), string, (ilocal=0, &ilocal),l);
  C2F(intersci).ntypes[lw-1]=36;
  return (Matrix *) C2F(vstk).Lstk[Top -Rhs + lw - 1];
}


int C2F(mxcopyreal8toptr)(double *y, Matrix *ptr, integer *n)
{
  int one=1;
  double *loc = ((double *) *ptr); 
  C2F(dcopy)(n,y,&one,loc,&one);
  return 0;
}

int C2F(mxcopycomplex16toptr)(double *y, Matrix *ptr, Matrix *pti, integer *n)
{
  int one=1; int two=2;
  double *loc = ((double *) *ptr);  
  double *locim = ((double *) *pti);
  C2F(dcopy)(n,y,&two,loc,&one);
  C2F(dcopy)(n,++y,&two,locim,&one);
  return 0;
}

int C2F(mxcopyptrtoreal8)(Matrix *ptr, double *y, integer *n)
{
  int one=1;
  double *loc = ((double *) *ptr); 
  C2F(dcopy)(n,loc,&one,y,&one);
  return 0;
}

int C2F(mxcopyptrtocomplex16)(Matrix *ptr, Matrix *pti, double *y, integer *n)
{
  int one=1; int two=2;
  double *loc = ((double *) *ptr);
  double *locim = ((double *) *pti);
  C2F(dcopy)(n,loc,&one,y,&two);
  C2F(dcopy)(n,locim,&one,++y,&two);
  return 0;
}






