#include "../mex.h"
#include <string.h>
#include <stdio.h>
#ifdef sgi
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
extern double C2F(dlamch)  __PARAMS((char *CMACH, unsigned long int));

#define DOUBLEMATRIX 1
#define INTMATRIX 8
#define STRINGMATRIX  10
#define SPARSEMATRIX  7
#define LOGICAL 4
#define MLIST   17
#define AsIs  '$'

/* s2iadr: loc adress to loci adress */
#define s2iadr(l) ((l)+(l)-1)
/* i2sadr: loci adress to loc adress */
#define i2sadr(l) ( ( (l)/2 ) +1 )

vraiptrst  stkptr(ptr_lstk)
     Matrix *ptr_lstk;
{
  vraiptrst ptr;
  ptr = C2F(locptr)(stk((int)(ptr_lstk))); 
  return ptr;
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

mxClassID mxGetClassID( ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: 
    return mxDOUBLE_CLASS;
    break;
  case STRINGMATRIX:
    return mxCHAR_CLASS;
    break;
  case SPARSEMATRIX:
    return mxSPARSE_CLASS;
    break;
  case INTMATRIX:
    /*[8,m,n,it] it=01    02   04    11     12    14
      int8 int16 int32 uint8 uint16 uint32    */
    switch (loci[3]){
    case 1:
      return mxINT8_CLASS;
      break;
    case 2:
      return mxINT16_CLASS;
      break;
    case 4:
      return mxINT32_CLASS;
      break;
    case 11:
      return mxUINT8_CLASS;
      break;
    case 12:
      return mxUINT16_CLASS;
      break;
    case 14:
      return mxUINT32_CLASS;
      break;
    default:
      return mxUNKNOWN_CLASS;
    }
  case MLIST:
    /* to be done return mxCELL_CLASS or mxCHAR_CLASS or mxSTRUCT_CLASS */
    /* loci[6+2*(loci[4]-1)]   ( = 1, 10, 8)  */
    switch (loci[6+2*(loci[4]-1)]) {
    case DOUBLEMATRIX:
      return mxDOUBLE_CLASS;
      break;
    case STRINGMATRIX:
      return mxCHAR_CLASS;
      break;
    case INTMATRIX:
      switch (loci[6+2*(loci[4]-1)+3]) {
      case 1:
	return mxINT8_CLASS;
	break;
      case 2:
	return mxINT16_CLASS;
	break;
      case 4:
	return mxINT32_CLASS;
	break;
      case 11:
	return mxUINT8_CLASS;
	break;
      case 12:
	return mxUINT16_CLASS;
	break;
      case 14:
	return mxUINT32_CLASS;
	break;
      default:
	return mxUNKNOWN_CLASS;
      }
    }
  default:
    return mxUNKNOWN_CLASS;
  }
}

double mxGetEps()
{
  double eps;
  eps=C2F(dlamch)("e",1L);
  return eps;
}


double mxGetInf()
{
  double big;
  big=C2F(dlamch)("o",1L);
  return big*big;
}

double mxGetNaN()
{
double x,y;
x=mxGetInf();
y=x/x;
  return y;
}

int mxIsInf(x)
     double x;
{
  if (x == x+1)
    return 1;
  else return 0;
}

int mxIsFinite(x)
     double x;
{
  if (x < x+1)
    return 1;
  else return 0;
}

int mxIsNaN(x)
     double x;
{
  if ( x != x ) 
    return 1;
  else return 0;
}
/*******************   C functions              ***************************/
int *mxGetHeader(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  return &loci[0];
}
int mxGetNumberOfElements(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  int m, commonlength;
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX:
    return loci[1]*loci[2];
    break;
  case MLIST:
    return loci[6+2*(loci[4]-1)+1];
    break;
  case STRINGMATRIX:
    m=loci[1];
    commonlength=loci[5]-loci[4];
    return m*commonlength;
    break;
    /*case SPARSEMATRIX to be done */
  default:
    return 0;
  }
}


double *mxGetPr(ptr)
     Matrix *ptr;
{
  double *loc = (double *) stkptr(ptr);
  int *loci = (int *) stkptr(ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX:
    return &loc[2];
    break;
  case MLIST:
    switch (loci[6+2*(loci[4]-1)]){
    case DOUBLEMATRIX: case INTMATRIX:
      return (double *) &loci[6+2*(loci[4]-1)+4];
      break;
    case STRINGMATRIX:
      return (double *) &loci[6+2*(loci[4]-1)+6];
      break;
    default:
      return 0;
      break;
    }
  case STRINGMATRIX:
    return &loc[i2sadr(5 + loci[2])];
    break;
  case SPARSEMATRIX:
    return &loc[ i2sadr(5+loci[2]+loci[4]) ];
    break;
  default:
    return 0;
  }
}


double *mxGetPi(ptr)
     Matrix *ptr;
{ int debut; int m,n,it;
 double *loc = (double *) stkptr(ptr);
 int *loci = (int *) stkptr(ptr);
 switch (loci[0]) {
 case DOUBLEMATRIX: case INTMATRIX:
   if (loci[3]==0) return NULL;
   return &loc[2 + loci[1] * loci[2]];
   break;
 case MLIST:
   debut=6+2*(loci[4]-1);
    switch (loci[debut]){
    case DOUBLEMATRIX: case INTMATRIX:
      m=loci[debut+1];n=loci[debut+2];
      it=loci[debut+3];  /* it should be 1 */
      return (double *) &loci[debut+4+2*m*n];
      break;
    default:
      return 0;
      break;
    }
 case SPARSEMATRIX:
   if (loci[3]==0) return NULL;
   return &loc[ i2sadr(5+loci[2] +loci[4]) + loci[1]*loci[2]];
   break;
 default:
   return 0;
 }
}

int mxGetNumberOfDimensions(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX: case SPARSEMATRIX:
    return 2;
    break;
  case MLIST:
    return loci[29]*loci[30];  /* loci[29] or loci[30] = 1  */
    break;
  default:
    return 0;
  }
}

int *mxGetDimensions(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX: case STRINGMATRIX:
    return &loci[1];
    break;
  case MLIST:
    return &loci[32];
    break;
    /* case SPARSEMATRIX: to be done  */
  default:
    return 0;
  }
}

int mxGetM(ptr)
     Matrix *ptr;
{
  int *loci= (int *) stkptr(ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX: case STRINGMATRIX: case SPARSEMATRIX:
    return loci[1];
    break;
  case MLIST:
    return loci[32];
    break;
  }
  return 0;
}

void mxSetM(ptr,m)
     Matrix *ptr; int m;
{
  int *loci = (int *) stkptr(ptr);
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

int *mxGetJc(ptr)
     Matrix *ptr;
{
  int *loci= (int *) stkptr(ptr);
  if (loci[0] != SPARSEMATRIX) {
    return 0;}
  /*  loci[0]=7;  M=loci[1];  N=loci[2];  it =loci[3];  nzmax=loci[4];
      Jc[0]=loci[5];  Jc[1]=loci[6]; etc; Jc[N]=nnz; */
  return &loci[5];
}

int *mxGetIr(ptr)
     Matrix *ptr;
{
  int *loci= (int *) stkptr(ptr);
  /* ... N=loci[2],  nzmax=loci[4]; then Jc,  then Ir   */
  return &loci[5+loci[2]+1];
}


int mxGetN(ptr)
     Matrix *ptr;
{
  int *loci= (int *) stkptr(ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX: case SPARSEMATRIX:
    return loci[2];
    break;
  case STRINGMATRIX:
    /*  for strings, N=length of first (and unique, loci[2]=1) column 
        (all rows have same Matlab-length) */
    return loci[5]-loci[4];
    break;
  case MLIST:
    return loci[33];
    break;
  }
  return 0;
}


void mxSetN(ptr,n)
     Matrix *ptr; int n; 
{
  int *loci = (int *) stkptr(ptr);
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

int mxIsString(ptr)
     Matrix *ptr;
{
  int *loci=(int *) stkptr(ptr);
  if (loci[0] == STRINGMATRIX) 
    return 1;
  else return 0;
} 

int mxIsChar(ptr)
     Matrix *ptr;
{
  int *loci=(int *) stkptr(ptr);
  switch (loci[0]) {
  case STRINGMATRIX:
    return 1;
    break;
  case MLIST:
    switch (loci[6+2*(loci[4]-1)]){
    case STRINGMATRIX:
      return 1;
      break;
    default:
      return 0;
      break;
    }
  default:
    return 0;
    break;
  } 
}

int mxIsNumeric(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  if (loci[0] != STRINGMATRIX)
    return 1;
  else
    return 0;
}

int mxIsDouble(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  if ((loci[0] == DOUBLEMATRIX) | (loci[0] == SPARSEMATRIX) | ( (loci[0] == MLIST) && (loci[6+2*(loci[4]-1)] == DOUBLEMATRIX)))
    return 1;
  else 
    return 0;
}

int mxIsEmpty(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  if ( (loci[0] == DOUBLEMATRIX) | (loci[0] == SPARSEMATRIX) ) 
    return 1-loci[1]*loci[2];
  else 
    return 0;
}

int mxIsFull(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  if ( loci[0] != SPARSEMATRIX)
    return 1;
  else
    return 0;
}

int mxIsSparse(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  if (loci[0] == SPARSEMATRIX) 
    return 1;
  else
    return 0;
}

int mxIsLogical(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  if (loci[0] == LOGICAL) 
    return 1;
  else
    return 0;
}

void mxSetLogical(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  loci[0] = LOGICAL;
}

void mxClearLogical(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  if (loci[0] != LOGICAL)
    mexErrMsgTxt("Variable is not logical");
  loci[0] = DOUBLEMATRIX;
}

int mxIsComplex(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX:
    return loci[3];
    break;
  case MLIST:
    if (loci[6+2*(loci[4]-1)] == DOUBLEMATRIX) {
      return loci[6+2*(loci[4]-1)+3];
      break;
    }
  default:
    return 0;
  }
}


double mxGetScalar(ptr)
     Matrix *ptr;
{ 
  double *loc = (double *) stkptr(ptr);
  int *loci = (int *) stkptr(ptr);
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


void *mxGetData(ptr)
     Matrix *ptr;
{
  return mxGetPr(ptr);
  /*  double *loc = (double *) stkptr(ptr);
  int *loci = (int *) stkptr(ptr);
  switch (loci[0]) {
  case DOUBLEMATRIX: case INTMATRIX:
    return &loc[2];
    break;
  case MLIST:
    return &loc[loci[4]+4];
    break;
  case STRINGMATRIX:
    return &loci[5+loci[1]];
    break;
  case SPARSEMATRIX:
    return &loc[i2sadr(5+loci[2]+loci[4])];
    break;
  default:
  return 0;
  }  */
}

void *mxGetImagData(ptr)
     Matrix *ptr;
{
  return mxGetPi(ptr);
}

void mexErrMsgTxt(error_msg)
     char *error_msg;
{
  cerro(error_msg);
  errjump();
}

Matrix *mxCreateFull(m, n, it)
     int m, n, it;
{
  static int lw, lr, lc;
  int k;
  lw = Nbvars + 1;
  if (! C2F(createcvar)(&lw, "d", &it, &m, &n, &lr, &lc, 1L)) {
    mexErrMsgTxt("No more memory available: increase stacksize");
  }
  for ( k=0; k<m*n*(it+1); k++ ) {
   *stk(lr+k)=0;
  }
  return (Matrix *) C2F(vstk).Lstk[lw + Top - Rhs - 1];
}

int mxIsClass( ptr, name)
     Matrix *ptr; char *name;
{
  int *loci = (int *) stkptr(ptr);
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

Matrix *mxCreateNumericArray(ND, size, CLASS, cmplx) 
     int ND; int *size; int CLASS; int cmplx;
{
  static int lw,lw1,w;
  int retval;
  Nbvars++;
  lw = Nbvars;
  lw1 = lw + Top - Rhs;
  C2F(hmcreate)(&lw1, &ND, size, &CLASS, &cmplx,&retval);
  if( !retval) {
    return (Matrix *) 0;
  }
  C2F(intersci).ntypes[lw-1]=AsIs;
  return (Matrix *) C2F(vstk).Lstk[lw + Top - Rhs - 1 ];
}

Matrix *mxCreateCharArray(ND, size)
     int ND; int *size;
{
  static int lw,lw1, CLASS, cmplx;
  int retval;
  Nbvars++;
  lw = Nbvars;
  lw1 = lw + Top - Rhs;
  C2F(hmcreate)(&lw1, &ND, size, (CLASS=4, &CLASS),(cmplx=0, &cmplx),&retval);
  if( !retval) {
    return (Matrix *) 0;
  }
  C2F(intersci).ntypes[lw-1]=AsIs;
  return (Matrix *) C2F(vstk).Lstk[lw+ Top - Rhs - 1];

}

Matrix *mxCreateCellArray(ND, size)
     int ND; int *size;
{
  Matrix *ppr[1];Matrix *ppl[1];
  int k, nlhs, nrhs;
  double  *start_of_pr;
  ppr[0] = mxCreateDoubleMatrix( ND , 1, 0);
  start_of_pr = (double *) mxGetPr(ppr[0]);
  for ( k=0; k<ND; k++ ) {
    start_of_pr[k]= (double) size[k];
  }
  nrhs=1;nlhs=1;
  /* cell = mlist(["ce","dims","entries"],[dim1,...,dimk],list([],...,[])) */
  /* w=makecell(dims) w=mlist(["ce","dims","entries"],dims,list())  */
  mexCallSCILAB(nlhs, ppl, nrhs, ppr, "makecell");    
  C2F(intersci).ntypes[Nbvars + Top - Rhs -1]=AsIs;
  return ppl[0];
}



Matrix *mxGetCell(ptr, index)
     Matrix *ptr; int index;
{
  /* TO BE DONE */
return (Matrix *) 0;
}

void *mxCalloc(n, size)
     unsigned int n, size;
{
  int m;  vraiptrst lrd;
  m = (n * size) /sizeof(double) + 1;
  if (! C2F(createstkptr)( &m, &lrd)) {
    return 0;
  }
  return (void *) lrd;
}

void *mxMalloc(nsize)
     unsigned int nsize;
{
  int m;  vraiptrst lrd;
  m = (nsize) /sizeof(double) + 1;
  if (! C2F(createstkptr)( &m, &lrd)) {
    return 0;
  }
  return (void *) lrd;
}

int mxIsCell(ptr)
     Matrix *ptr;
{
  int *loci = (int *) stkptr(ptr);
  if (loci[0]==17 && loci[1]==3 && loci[6]==10 && loci[14]==12 && loci[15]==14)
    return 1;
  else return 0;
}

int mxIsStruct(ptr)
     Matrix *ptr;
{
/*  TO BE DONE  */
return 1;
}
      
/***************************************************************
 * Return in str at most strl characters from first element of 
 * string Matrix pointed to by ptr ( ptr is assumed to be a String Matrix )
 **************************************************************/

int mxGetString(ptr,str,strl)
     Matrix *ptr;     int  strl;     char *str;
{
  int *loci = (int *) stkptr(ptr);
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

void mxFreeMatrix(ptr)
     Matrix *ptr;
{
  /* If we free the last stored object we can decrement Nbvars */
  if ( (int)ptr == C2F(vstk).Lstk[Top - Rhs + Nbvars - 1]) 
    Nbvars--;
  /* Automatically freed when return from mexfunction */
  return ;
}


void mxDestroyArray(ptr)
     Matrix *ptr;
{   /* No need */
  return; 
}

void mxFree(ptr)
     Matrix *ptr;
{
  /* No need to free */
  return ;
}

int mexAtExit(ptr)
     Matrix *ptr;
{
  /* To be done....*/
  return 0;
}

Matrix *mxCreateSparse(m,n,nzmax,cmplx)
     int m, n, nzmax, cmplx;
{
  static int lw, iprov;
  Nbvars++;
  lw=Nbvars;
  iprov = lw + Top - Rhs;
  if( ! C2F(mspcreate)(&iprov, &m, &n, &nzmax, &cmplx)) {
    return (Matrix *) 0;
  }
  C2F(intersci).ntypes[lw-1]=AsIs;
  return (Matrix *) C2F(vstk).Lstk[lw + Top - Rhs - 1];
}

/***************************************************************
 * Create on Scilab Stack a 1x1 string matrix filled with string
 **************************************************************/

Matrix *mxCreateString(string)
     char *string;
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


void mexWarnMsgTxt(error_msg)
     char *error_msg;
{
  mexPrintf("Warning: ");
  mexPrintf(error_msg);
  mexPrintf("\n\n");
  /*  mexPrintf(strcat("Warning: ",error_msg)); */
}

int mexCallSCILAB(nlhs, plhs, nrhs, prhs, name)
     int nlhs, nrhs;
     Matrix *prhs[];Matrix *plhs[];
     char *name;
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
	  return 0;
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
    errjump();
    /*	      return 0;  */
  }
  for (k = 1; k <= nlhs; ++k) {
    plhs[k-1] = (Matrix *) C2F(vstk).Lstk[nv + k + Top - Rhs - 1];
  }
  Nbvars = nv+nlhs;
  return 1;
}

int mxCalcSingleSubscript(ptr, nsubs, subs)
Matrix *ptr; int nsubs; int *subs;

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

int C2F(mexcallscilab)(nlhs, plhs, nrhs, prhs, name, namelen)
     integer *nlhs,*nrhs;
     Matrix *plhs[];     Matrix *prhs[];
     char *name;     int namelen;
{
  return mexCallSCILAB(*nlhs, plhs, *nrhs, prhs, name);
}

/** generic mex interface **/

int mex_gateway(fname,F)
     char *fname;
     GatefuncH F;
{
  int nlhs,nrhs;
  Matrix * plhs[intersiz];  Matrix * prhs[intersiz];
  C2F(initmex)(&nlhs, plhs, &nrhs, prhs);
  (*F)(nlhs, plhs, nrhs, prhs);
  C2F(endmex)(&nlhs, plhs, &nrhs, prhs);
  return 0;
}

int fortran_mex_gateway(fname,F)
     char *fname;
     FGatefuncH F;
{
  int nlhs,nrhs;
  Matrix * plhs[intersiz];  Matrix * prhs[intersiz];
  C2F(initmex)(&nlhs, plhs, &nrhs, prhs);
  (*F)( &nlhs, plhs, &nrhs, prhs);
  C2F(endmex)(&nlhs, plhs, &nrhs, prhs);
  return 0;
}


/** generic scilab interface **/

int sci_gateway(fname,F)
     char *fname;
     GatefuncS F;
{
  (*F)(fname,strlen(fname));
  if (!C2F(putlhsvar)()) {return 0;}
  return 0;
}

int mxGetElementSize(ptr)
     Matrix *ptr;
{ int k, it; 
 int *loci = (int *) stkptr(ptr);
 switch (loci[0]) {
 case DOUBLEMATRIX: case SPARSEMATRIX:
   return sizeof(double);
   break;
 case STRINGMATRIX:
   return 2*sizeof(char);    /* ? Matlab returns 2 */
   break;
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
   break;
 default:
   mexErrMsgTxt("GetElementSize error");
   return 0;
 }
 return 0;
}

Matrix *mxCreateCharMatrixFromStrings(m, str)
     int m;
     char **str;
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

void mexEvalString(name)
     char *name;
{
  Matrix *ppr[1];Matrix *ppl[1];
  int nlhs;     int nrhs;
  *ppr = mxCreateString(name);
  nrhs=1;nlhs=0;
  mexCallSCILAB(nlhs, ppl, nrhs, ppr, "execstr");    
  return;
}

mxArray *mexGetArray(name, workspace)
     char *name; char *workspace;
{
  int lw;
  C2F(objptr)(name,&lw,strlen(name));
  return (mxArray *) lw;
}



int C2F(initmex)(nlhs, plhs, nrhs, prhs)
     integer *nlhs,*nrhs;
     Matrix *plhs[];     Matrix *prhs[]; 
{
  int *loci;
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
      loci = (int *) stkptr(prhs[k-1]);
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
	break;
      case MLIST:
	m=loci[29]*loci[30];
	C2F(entier)(&m, (double *) &loci[32], &loci[32]);
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

vraiptrst C2F(locptr)(x)
     void *x;
{
  return((vraiptrst) x);
}

int C2F(createstkptr)(m, ptr)
     integer *m;
     vraiptrst *ptr;
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

int C2F(createptr)(type, m, n, it, lr, ptr, type_len)
     char *type;
     int *m, *n, *it, *lr, *ptr;
     long int type_len;
{
  static int lc, lw;
  Nbvars++;
  lw = Nbvars;
  if (! C2F(createcvar)(&lw, type, it, m, n, lr, &lc, 1L)) {
    return 0;
  }
  *ptr = C2F(vstk).Lstk[lw + Top - Rhs - 1];
  return 1;
} 

int C2F(endmex)(nlhs, plhs, nrhs, prhs)
     integer *nlhs,*nrhs;
     Matrix *plhs[];     Matrix *prhs[];
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


/****************************************************
 * C functions for Fortran  mexfunctions 
 ****************************************************/

double * C2F(mxgetpr)(ptr)
     Matrix *ptr;
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


double * C2F(mxgetpi)(ptr)
     Matrix *ptr;
{
  double *loc = (double *) stkptr(*ptr);
  return &loc[2 +  C2F(mxgetm)(ptr) *  C2F(mxgetn)(ptr)];
}

int  C2F(mxgetm)(ptr)
     Matrix *ptr;
{
  int *loc= (int *) stkptr(*ptr);
  return loc[1] ;
}

int  C2F(mxgetn)(ptr)
     Matrix *ptr;
{
  int *loc = (int *) stkptr(*ptr);
  if ( loc[0] == STRINGMATRIX) {
    return loc[5]-1;
  }
  return loc[2] ;
}

int  C2F(mxisstring)(ptr)
     Matrix *ptr;
{
  int *loc=(int *) stkptr(*ptr);
  if (loc[0] == STRINGMATRIX) 
    return 1;
  else return 0;
} 

int  C2F(mxisnumeric)(ptr)
     Matrix *ptr;
{
  int *loc = (int *) stkptr(*ptr);
  if ( loc[0] == DOUBLEMATRIX) 
    return 1;
  else 
    return 0;
}

int  C2F(mxisfull)(ptr)
     Matrix *ptr;
{
  int *loc = (int *) stkptr(*ptr);
  if ( loc[0] == DOUBLEMATRIX) {
    return 1;
  }
  return 0;
}

int  C2F(mxissparse)(ptr)
     Matrix *ptr;
{
  int *loc = (int *) stkptr(*ptr);
  if (loc[0] == SPARSEMATRIX) {
    return 1;
  }
  return 0;
}


int  C2F(mxiscomplex)(ptr)
     Matrix *ptr;
{
  int *loc = (int *) stkptr(*ptr);
  if (loc[3] == DOUBLEMATRIX) {
    return 1;
  }
  return 0;
}

double  C2F(mxgetscalar)(ptr)
     Matrix *ptr;
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

void  C2F(mexprintf)(error_msg,len) 
     char *error_msg;
     int len;
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

void C2F(mexerrmsgtxt)(error_msg,len)
     char *error_msg;
     int len;
{
  C2F(erro)(error_msg,len);
  errjump();
}

Matrix *C2F(mxcreatefull)(m, n, it)
     int *m, *n, *it;
{
  static int lr1;
  int lrd;
  if (! C2F(createptr)("d", m, n, it, &lr1, &lrd,1L)) {
    return  (Matrix *) 0;
  }
  return (Matrix *) lrd ;
} 


unsigned long int C2F(mxcalloc)(n, size)
     unsigned int *n, *size;
{
  int m;
  vraiptrst lrd;
  m = (*n * *size) /sizeof(double) + 1;
  if (! C2F(createstkptr)(&m, &lrd)) {
    return 0;
  }
  return (unsigned long int) lrd;
}

int  C2F(mxgetstring)(ptr,str,strl)
     Matrix *ptr;     int  *strl;     char *str;
{
  int *loc = (int *) stkptr(*ptr);
  int commonlength;  int nrows = loc[1];
  /*  int ncols = loc[2]; This is 1 */
  commonlength=nrows*(loc[5]-loc[4]);
  C2F(in2str)(&commonlength, &loc[5+nrows], str,0L);
  *strl = Min(*strl,commonlength);
  return 0;
}

void C2F(mxfreematrix)(ptr)
     Matrix *ptr;
{
  return ;
}


Matrix * C2F(mxcreatestring)(string,l)
     char *string;
     long int l;
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


int C2F(mxcopyreal8toptr)(y,ptr,n)
     Matrix *ptr;     double y[];     integer *n;
{
  int one=1;
  double *loc = ((double *) *ptr); 
  C2F(dcopy)(n,y,&one,loc,&one);
  return 0;
}

int C2F(mxcopycomplex16toptr)(y,ptr,pti,n)
     Matrix *ptr;     Matrix *pti;   double y[];     integer *n;
{
  int one=1; int two=2;
  double *loc = ((double *) *ptr);  
  double *locim = ((double *) *pti);
  C2F(dcopy)(n,y,&two,loc,&one);
  C2F(dcopy)(n,++y,&two,locim,&one);
  return 0;
}

int C2F(mxcopyptrtoreal8)(ptr,y,n)
     Matrix *ptr;     double y[];     integer *n;
{
  int one=1;
  double *loc = ((double *) *ptr); 
  C2F(dcopy)(n,loc,&one,y,&one);
  return 0;
}

int C2F(mxcopyptrtocomplex16)(ptr,pti,y,n)
     Matrix *ptr;     Matrix *pti;   double y[];     integer *n;
{
  int one=1; int two=2;
  double *loc = ((double *) *ptr);
  double *locim = ((double *) *pti);
  C2F(dcopy)(n,loc,&one,y,&two);
  C2F(dcopy)(n,locim,&one,++y,&two);
  return 0;
}






