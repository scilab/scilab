#ifndef SCI_MEX 
#define SCI_MEX 

#include "stack-c.h"

typedef int Matrix;
typedef unsigned long int vraiptrst;
typedef int mxArray;

typedef int (*GatefuncH) __PARAMS((int nlhs,Matrix *plhs[],int nrhs,
                                 Matrix *prhs[]));

typedef int (*FGatefuncH) __PARAMS((int *nlhs,Matrix *plhs[],int *nrhs,
                                 Matrix *prhs[]));

typedef int Gatefunc __PARAMS((int nlhs,Matrix *plhs[],int nrhs,
                                 Matrix *prhs[]));
typedef int (*GatefuncS) __PARAMS((char *fname, int l));
typedef int (*Myinterfun) __PARAMS((char *, GatefuncH F));

typedef struct table_struct {
  Myinterfun f;    /** interface **/
  GatefuncH F;     /** function **/
  char *name;      /** its name **/
} GenericTable;

#define REAL 0
#define COMPLEX 1

#ifndef NULL
#define NULL 0
#endif

#define mxCreateDoubleMatrix mxCreateFull
#define bool int
#define mexCallMATLAB mexCallSCILAB
#define mexGetMatrixPtr(name) mexGetArrayPtr(name, "caller")
/** XXXX A finir **/
#define mexGetArrayPtr(name,type) mexGetArray(name,type) 
#define mexPrintf sciprint

/** Put a matrix in Scilab Workspace */ 

#define mexPutFull(name,m,n,ptrM,tag) \
  if ( ! C2F(cwritemat)(name,m,n,ptrM,strlen(name))) {	\
      mexErrMsgTxt("mexPutFull failed\r\n");return; }

/* prototypes */

Matrix *mxCreateCharMatrixFromStrings __PARAMS((int m, char **str));
Matrix *mxCreateString __PARAMS((char *string));
Matrix *C2F(mxcreatestring)  __PARAMS((char *string, long int l));
Matrix *C2F(mxcreatefull)  __PARAMS((int *m, int *n, int *it));
Matrix *mxCreateCharMatrixFromStrings __PARAMS((int m, char **str));
Matrix *mxCreateFull __PARAMS((int m, int n, int it));
Matrix *mxCreateSparse __PARAMS((int m, int n, int nzmax, int cmplx));
Matrix *mxCreateNumericArray __PARAMS((int ND, int *size,int CLASS,int cmplx));
Matrix *mxCreateCharArray __PARAMS((int ND, int *size));
Matrix *mxCreateCellArray __PARAMS((int ND,int *size));
Matrix *mxGetCell __PARAMS((Matrix *ptr,int index));


double C2F(mxgetscalar)  __PARAMS((Matrix *ptr));
double * C2F(mxgetpi)  __PARAMS((Matrix *ptr));
double * C2F(mxgetpr)  __PARAMS((Matrix *ptr));
double *mxGetPi __PARAMS((Matrix *ptr));
double *mxGetPr __PARAMS((Matrix *ptr));
double mxGetScalar __PARAMS((Matrix *ptr));
double mxGetInf __PARAMS(());
double mxGetNaN __PARAMS(());
double mxGetEps __PARAMS(());
int mxIsNaN __PARAMS((double x));
int mxIsInf __PARAMS((double x));
int mxIsFinite __PARAMS((double x));
int *mxGetDimensions __PARAMS((Matrix *ptr));
int mxCalcSingleSubscript __PARAMS((Matrix *ptr, int nsubs, int *subs));
int mxGetNumberOfElements __PARAMS((Matrix *ptr));
int mxGetNumberOfDimensions __PARAMS((Matrix *ptr));
void *mxGetData __PARAMS((Matrix *ptr));
void *mxGetImagData __PARAMS((Matrix *ptr));

extern int C2F(createcvar) __PARAMS(());
extern int C2F(createptr) __PARAMS((char *type,integer * m,integer * n, integer *it,integer * lr,integer *ptr, long int type_len));
extern int C2F(createstkptr) __PARAMS((integer *m, vraiptrst *ptr));
extern int C2F(endmex)  __PARAMS((integer *nlhs,Matrix *plhs[],integer *nrhs,Matrix *prhs[]));
extern int C2F(getrhsvar) __PARAMS((integer *, char *, integer *, integer *, integer *, long unsigned int));
extern int C2F(initmex) __PARAMS((integer *nlhs,Matrix *plhs[],integer *nrhs,Matrix *prhs[]));
extern int C2F(putlhsvar) __PARAMS((void));
extern void errjump __PARAMS((void));
extern void sciprint __PARAMS((char *fmt,...));
extern vraiptrst C2F(locptr) __PARAMS(( void *x));
int  C2F(mxgetm)  __PARAMS((Matrix *ptr));
int  C2F(mxgetn)  __PARAMS((Matrix *ptr));
int  C2F(mxgetstring)  __PARAMS((Matrix *ptr, char *str, int *strl));
int  C2F(mxiscomplex)  __PARAMS((Matrix *ptr));
int  C2F(mxisfull)  __PARAMS((Matrix *ptr));
int  C2F(mxisnumeric)  __PARAMS((Matrix *ptr));
int  C2F(mxissparse)  __PARAMS((Matrix *ptr));
int  C2F(mxisstring)  __PARAMS((Matrix *ptr));
int *mxGetIr __PARAMS((Matrix *ptr));
int *mxGetJc __PARAMS((Matrix *ptr));
int C2F(createptr)  __PARAMS((char *type, int *m, int *n, int *it, int *lr, int *ptr, long int type_len));
int C2F(createstkptr)  __PARAMS((integer *m, vraiptrst *ptr));
int C2F(endmex)  __PARAMS((integer *nlhs, Matrix **plhs, integer *nrhs, Matrix **prhs));
int C2F(initmex)  __PARAMS((integer *nlhs, Matrix **plhs, integer *nrhs, Matrix **prhs));
int C2F(mexcallscilab)  __PARAMS((integer *nlhs, Matrix **plhs, integer *nrhs, Matrix **prhs, char *name, int namelen));
int C2F(mxcopyptrtoreal8)  __PARAMS((Matrix *ptr, double *y, integer *n));
int C2F(mxcopyreal8toptr)  __PARAMS((double *y, Matrix *ptr, integer *n));
int C2F(mxgetheader)  __PARAMS((int lw, void **header));
int C2F(mxgetheader2)  __PARAMS((Matrix *ptr, int **header));
int fortran_mex_gateway __PARAMS((char *fname, FGatefuncH F));
int mexAtExit __PARAMS((Matrix *ptr));
int mexCallSCILAB __PARAMS((int nlhs, Matrix **plhs, int nrhs, Matrix **prhs, char *name));
int mex_gateway __PARAMS((char *fname, GatefuncH F));
int mxGetElementSize __PARAMS((Matrix *ptr));
int *mxGetHeader __PARAMS((Matrix *ptr));
int mxGetHeader2 __PARAMS((Matrix *ptr, void **header));
int mxGetM __PARAMS((Matrix *ptr));
int mxGetN __PARAMS((Matrix *ptr));
int mxGetString __PARAMS((Matrix *ptr, char *str, int strl));
int mxIsComplex __PARAMS((Matrix *ptr));
int mxIsDouble __PARAMS((Matrix *ptr));
int mxIsFull __PARAMS((Matrix *ptr));
int mxIsNumeric __PARAMS((Matrix *ptr));
int mxIsSparse __PARAMS((Matrix *ptr));
int mxIsLogical __PARAMS((Matrix *ptr));
void mxSetLogical __PARAMS((Matrix *ptr));
void mxClearLogical __PARAMS((Matrix *ptr));
int mxIsString __PARAMS((Matrix *ptr));
int mxIsChar __PARAMS((Matrix *ptr));
int mxIsEmpty __PARAMS((Matrix *ptr));
int mxIsClass __PARAMS((Matrix *ptr, char *name));

void mxSetM __PARAMS((Matrix *ptr, int m));
void mxSetN __PARAMS((Matrix *ptr, int n));
int sci_gateway __PARAMS((char *fname, GatefuncS F));
mxArray *mexGetArray __PARAMS((char *name, char *workspace));
mxArray *mexGetArray __PARAMS((char *name, char *workspace));
unsigned long int C2F(mxcalloc)  __PARAMS((unsigned int *n, unsigned int *size));
/*void  C2F(mexprintf)   __PARAMS((char *fmt,...) );*/
void  C2F(mexprintf)  __PARAMS((char *error_msg, int len));
void *mxCalloc __PARAMS((unsigned int n, unsigned int size));
void *mxMalloc __PARAMS((unsigned int nsize));

void C2F(mexerrmsgtxt)  __PARAMS((char *error_msg, int len));
void C2F(mxfreematrix)  __PARAMS((Matrix *ptr));
void mexErrMsgTxt __PARAMS((char *error_msg));
void mexEvalString __PARAMS((char *name));
void mexWarnMsgTxt __PARAMS((char *error_msg));
void mexprint __PARAMS((char* fmt,...));
void mxFree __PARAMS((Matrix *ptr));
void mxFreeMatrix __PARAMS((Matrix *ptr));
void mxDestroyArray __PARAMS((Matrix *ptr));

vraiptrst C2F(locptr) __PARAMS((void *x));

typedef enum {
	mxCELL_CLASS = 1,
	mxSTRUCT_CLASS,
	mxOBJECT_CLASS,
	mxCHAR_CLASS,
	mxSPARSE_CLASS,
	mxDOUBLE_CLASS,
	mxSINGLE_CLASS,
	mxINT8_CLASS,
	mxUINT8_CLASS,
	mxINT16_CLASS,
	mxUINT16_CLASS,
	mxINT32_CLASS,
	mxUINT32_CLASS,
	mxUNKNOWN_CLASS = -1
} mxClassID;

mxClassID mxGetClassID __PARAMS((Matrix *ptr));

typedef enum {
  mxREAL,
  mxCOMPLEX
  } mxComplexity; 

/* typedef uint16_T mxChar; */

typedef short int mxChar;

#define mxREAL 0
#define mxCOMPLEX 1

#define INT8_T char
#define UINT8_T unsigned char
#define INT16_T short
#define UINT16_T unsigned short
#define INT32_T int
#define UINT32_T unsigned int
#define REAL32_T float


typedef INT8_T int8_T;
typedef UINT8_T uint8_T;
typedef INT16_T int16_T;
typedef UINT16_T uint16_T;
typedef INT32_T int32_T;
typedef UINT32_T uint32_T;
typedef REAL32_T real32_T;

#endif SCI_MEX 
