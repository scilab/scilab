#ifdef __cplusplus
extern "C" {
#endif

#ifndef SCI_MEX 
#define SCI_MEX 


#ifdef WIN32
	#include <stddef.h> /*for LCC */
#endif

#include "stack-c.h"

typedef int Matrix;
typedef unsigned long int vraiptrst;
typedef int mxArray;

typedef int (*GatefuncH) __PARAMS((int nlhs,mxArray *plhs[],int nrhs,
                                 mxArray *prhs[]));

typedef int (*FGatefuncH) __PARAMS((int *nlhs,mxArray *plhs[],int *nrhs,
                                 mxArray *prhs[]));

typedef int Gatefunc __PARAMS((int nlhs,mxArray *plhs[],int nrhs,
                                 mxArray *prhs[]));
typedef int (*GatefuncS) __PARAMS((char *fname, int l));
typedef int (*Myinterfun) __PARAMS((char *, GatefuncH F));

typedef int (*GT) ();

typedef struct table_struct {
  Myinterfun f;    /** interface **/
  GT F;     /** function **/
  char *name;      /** its name **/
} GenericTable;

#define mxLOGICAL int
#define mxLogical int

#define REAL 0
#define COMPLEX 1

#ifndef NULL
#define NULL 0
#endif

#define mxCreateFull mxCreateDoubleMatrix
#define bool int

#define mexGetMatrixPtr(name) mexGetArrayPtr(name, "caller")
/** XXXX A finir **/
#define mexGetArrayPtr(name,type) mexGetArray(name,type) 


#ifdef __STDC__ 
void mexPrintf __PARAMS((char *fmt,...));
#else 
void  mexPrintf __PARAMS(()); 
#endif

/** Put a matrix in Scilab Workspace */ 
#define mexPutFull(name,m,n,ptrM,tag) \
  if ( ! C2F(cwritemat)(name,(c_local=m,&c_local),(c1_local=n,&c1_local),ptrM,strlen(name))) {	\
      mexErrMsgTxt("mexPutFull failed\r\n");return; }

/* prototypes */

void mexInfo __PARAMS((char *)); 
int mexCheck __PARAMS((char *,int )); 
mxArray *mxCreateCharMatrixFromStrings __PARAMS((int m, const char **str));
mxArray *mxCreateString __PARAMS((const char *string));
mxArray *C2F(mxcreatestring)  __PARAMS((char *string, long int l));
mxArray *C2F(mxcreatefull)  __PARAMS((int *m, int *n, int *it));
/* mxArray *mxCreateFull __PARAMS((int m, int n, int it)); */
mxArray *mxCreateCharArray __PARAMS((int ND, const int *size));

mxArray *mxCreateCellArray __PARAMS((int ND, const int *size));
mxArray *mxCreateCellMatrix __PARAMS((int m, int n));

mxArray *mxCreateStructArray __PARAMS((int ndim, const int *dims, int nfields, const char **field_names));
mxArray *mxCreateStructMatrix __PARAMS((int m, int n, int nfields, const char **field_names));


mxArray *mxGetCell __PARAMS((const mxArray *ptr, int index));

double C2F(mxgetscalar)  __PARAMS((mxArray *ptr));
double * C2F(mxgetpi)  __PARAMS((mxArray *ptr));
double * C2F(mxgetpr)  __PARAMS((mxArray *ptr));
double *mxGetPi __PARAMS((const mxArray *ptr));
double *mxGetPr __PARAMS((const mxArray *ptr));
double mxGetScalar __PARAMS((const mxArray *ptr));
double mxGetInf __PARAMS((void));
double mxGetNaN __PARAMS((void));
double mxGetEps __PARAMS((void));
bool mxIsNaN __PARAMS((double x));
bool mxIsInf __PARAMS((double x));
bool mxIsFinite __PARAMS((double x));
int *mxGetDimensions __PARAMS((const mxArray *ptr));
int mxCalcSingleSubscript __PARAMS((const mxArray *ptr, int nsubs, const int *subs));
int mxGetNumberOfElements __PARAMS((const mxArray *ptr));
int mxGetNumberOfDimensions __PARAMS((const mxArray *ptr));
int mxGetNumberOfFields __PARAMS((const mxArray *ptr));
void *mxGetData __PARAMS((const mxArray *ptr));
void *mxGetImagData __PARAMS((const mxArray *ptr));
extern void diary_nnl __PARAMS((char *str,int *n));
extern int getdiary __PARAMS(());

extern int C2F(createptr) __PARAMS((char *type,integer * m,integer * n, integer *it,integer * lr,integer *ptr, long int type_len));
extern int C2F(createstkptr) __PARAMS((integer *m, vraiptrst *ptr));
extern int C2F(endmex)  __PARAMS((integer *nlhs,mxArray *plhs[],integer *nrhs,mxArray *prhs[]));

void clear_mex(integer nlhs, mxArray **plhs, integer nrhs, mxArray **prhs);

extern int C2F(getrhsvar) __PARAMS((integer *, char *, integer *, integer *, integer *, long unsigned int));
extern int C2F(initmex) __PARAMS((integer *nlhs,mxArray *plhs[],integer *nrhs,mxArray *prhs[]));
extern int C2F(putlhsvar) __PARAMS((void));
extern void errjump __PARAMS((void));
extern void sciprint __PARAMS((char *fmt,...));
extern vraiptrst C2F(locptr) __PARAMS(( void *x));
int  C2F(mxgetm)  __PARAMS((mxArray *ptr));
int  C2F(mxgetn)  __PARAMS((mxArray *ptr));
int  C2F(mxgetstring)  __PARAMS((mxArray *ptr, char *str, int *strl));
int  C2F(mxiscomplex)  __PARAMS((mxArray *ptr));
int  C2F(mxisfull)  __PARAMS((mxArray *ptr));
int  C2F(mxisnumeric)  __PARAMS((mxArray *ptr));
int  C2F(mxissparse)  __PARAMS((mxArray *ptr));
int  C2F(mxisstring)  __PARAMS((mxArray *ptr));
int *mxGetIr __PARAMS((const mxArray *ptr));
int *mxGetJc __PARAMS((const mxArray *ptr));
int C2F(createptr)  __PARAMS((char *type, int *m, int *n, int *it, int *lr, int *ptr, long int type_len));
int C2F(createstkptr)  __PARAMS((integer *m, vraiptrst *ptr));
int C2F(endmex)  __PARAMS((integer *nlhs, mxArray **plhs, integer *nrhs, mxArray **prhs));
int C2F(initmex)  __PARAMS((integer *nlhs, mxArray **plhs, integer *nrhs, mxArray **prhs));
int C2F(mexcallscilab)  __PARAMS((integer *nlhs, mxArray **plhs, integer *nrhs, mxArray **prhs, char *name, int namelen));
int C2F(mxcopyptrtoreal8)  __PARAMS((mxArray *ptr, double *y, integer *n));
int C2F(mxcopyreal8toptr)  __PARAMS((double *y, mxArray *ptr, integer *n));
int fortran_mex_gateway __PARAMS((char *fname, FGatefuncH F));
int mexAtExit __PARAMS((mxArray *ptr));
int mexCallSCILAB __PARAMS((int nlhs, mxArray **plhs, int nrhs, mxArray **prhs, const char *name));
int mexCallMATLAB __PARAMS((int nlhs, mxArray **plhs, int nrhs, mxArray **prhs, const char *name));
int mex_gateway __PARAMS((char *fname, GatefuncH F));
int mxGetElementSize __PARAMS((const mxArray *ptr));
int mxGetM __PARAMS((const mxArray *ptr));
int mxGetN __PARAMS((const mxArray *ptr));
int mxGetNzmax __PARAMS((const mxArray *ptr));
int mxGetString __PARAMS((const mxArray *ptr, char *str, int strl));
char *mxArrayToString __PARAMS((const mxArray *array_ptr));
bool mxIsComplex __PARAMS((const mxArray *ptr));
bool mxIsDouble __PARAMS((const mxArray *ptr));
bool mxIsSingle __PARAMS((const mxArray *ptr));
bool mxIsFull __PARAMS((const mxArray *ptr));
bool mxIsNumeric __PARAMS((const mxArray *ptr));
bool mxIsSparse __PARAMS((const mxArray *ptr));
bool mxIsLogical __PARAMS((const mxArray *ptr));
bool mexIsGloball __PARAMS((const mxArray *ptr));
void mxSetLogical __PARAMS((mxArray *ptr));
void mxClearLogical __PARAMS((mxArray *ptr));
bool mxIsString __PARAMS((const mxArray *ptr));
bool mxIsChar __PARAMS((const mxArray *ptr));
bool mxIsEmpty __PARAMS((const mxArray *ptr));
bool mxIsClass __PARAMS((const mxArray *ptr, const char *name));
bool mxIsCell __PARAMS((const mxArray *ptr));
bool mxIsStruct __PARAMS((const mxArray *ptr));

bool mxIsInt8 __PARAMS((const mxArray *ptr));
bool mxIsInt16 __PARAMS((const mxArray *ptr));
bool mxIsInt32 __PARAMS((const mxArray *ptr));
bool mxIsUint8 __PARAMS((const mxArray *ptr));
bool mxIsUint16 __PARAMS((const mxArray *ptr));
bool mxIsUint32 __PARAMS((const mxArray *ptr));

void mxSetM __PARAMS((mxArray *ptr, int m));
void mxSetN __PARAMS((mxArray *ptr, int n));
void mxSetJc  __PARAMS((mxArray *array_ptr, int *jc_data));
void mxSetIr  __PARAMS((mxArray *array_ptr, int *ir_data));
void mxSetNzmax  __PARAMS((mxArray *array_ptr, int nzmax));
void mxSetCell __PARAMS((mxArray *pa, int i, mxArray *value));

int sci_gateway __PARAMS((char *fname, GatefuncS F));
mxArray *mexGetArray __PARAMS((char *name, char *workspace));

mxArray *mexGetVariable __PARAMS((const char *workspace, const char *name));
const mxArray *mexGetVariablePtr __PARAMS((const char *workspace, const char *name));

unsigned long int C2F(mxcalloc)  __PARAMS((unsigned int *n, unsigned int *size));
/*void  C2F(mexprintf)   __PARAMS((char *fmt,...) );*/
void  C2F(mexprintf)  __PARAMS((char *error_msg, int len));

void *mxCalloc __PARAMS((size_t n, size_t size));
void *mxMalloc __PARAMS((size_t nsize));
/*  void *mxRealloc(void *ptr, size_t size);  */

void *mxCalloc_m __PARAMS((unsigned int n, unsigned int size));
void *mxMalloc_m __PARAMS((unsigned int nsize));
void  mxFree_m __PARAMS((void *));

void C2F(mexerrmsgtxt)  __PARAMS((char *error_msg, int len));
void C2F(mxfreematrix)  __PARAMS((mxArray *ptr));
void mexErrMsgTxt __PARAMS((char *error_msg));
int  mexEvalString __PARAMS((char *name));
void mexWarnMsgTxt __PARAMS((char *error_msg));
void mexprint __PARAMS((char* fmt,...));
void mxFree __PARAMS((void *ptr));
void mxFreeMatrix __PARAMS((mxArray *ptr));
void mxDestroyArray __PARAMS((mxArray *ptr));
int mxGetFieldNumber __PARAMS((const mxArray *ptr, const char *string));
mxArray *mxGetField __PARAMS((const mxArray *pa, int i, const char *fieldname));
void  mxSetFieldByNumber __PARAMS((mxArray *array_ptr, int index, int field_number, mxArray *value));
void mxSetField __PARAMS((mxArray *pa, int i, const char *fieldname, mxArray *value));


mxArray *mxGetFieldByNumber __PARAMS((const mxArray *ptr, int index, int field_number));
const char *mxGetFieldNameByNumber __PARAMS((const mxArray *array_ptr, int field_number));
mxLOGICAL *mxGetLogicals __PARAMS((mxArray *array_ptr));

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
	mxUNKNOWN_CLASS = 0
} mxClassID;

typedef enum { mxREAL, mxCOMPLEX } mxComplexity; 



mxClassID mxGetClassID __PARAMS((const mxArray *ptr));
const char *mxGetName __PARAMS((const mxArray *array_ptr));
void mxSetName __PARAMS(( mxArray    *pa,    const char *s ));
void mxSetPr __PARAMS((mxArray *array_ptr, double *pr));
void mxSetPi __PARAMS((mxArray *array_ptr, double *pi));
void mxSetData __PARAMS((mxArray *array_ptr, void *pr));
mxArray *mxCreateNumericArray __PARAMS((int ndim, const int *dims, mxClassID classid, mxComplexity flag));
mxArray *mxCreateNumericMatrix __PARAMS((int m, int n, mxClassID classid, int cmplx_flag));
int mxSetDimensions __PARAMS((mxArray *array_ptr, const int *dims, int ndim));
mxArray *mxCreateDoubleMatrix __PARAMS((int m, int n, mxComplexity it));
mxArray *mxCreateDoubleScalar __PARAMS((double value));
mxArray *mxCreateSparse __PARAMS((int m, int n, int nzmax, mxComplexity cmplx));
mxArray *mxDuplicateArray __PARAMS((const mxArray *ptr));

/* typedef uint16_T mxChar; */

/* typedef short int mxChar; */
typedef unsigned short mxChar;

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

#endif /* SCI_MEX  */

#ifdef __cplusplus
}
#endif

/* generic mexfunction name */
#ifdef __cplusplus
extern "C" {
  void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
}
#endif
