
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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SCI_MEX 
#define SCI_MEX 

#ifdef WIN32
	#include <stddef.h> /*for LCC */
#endif

#define __MEX_INCLUDE__
#include "stack-c.h"
#undef __MEX_INCLUDE__

typedef int Matrix;
typedef unsigned long int vraiptrst;
#ifndef __DEF_MXARRAY__
#define __DEF_MXARRAY__
typedef int mxArray;
typedef int Gatefunc (int nlhs,mxArray *plhs[],int nrhs,mxArray *prhs[]);
#endif

typedef int (*GatefuncH) (int nlhs,mxArray *plhs[],int nrhs, mxArray *prhs[]);

typedef int (*FGatefuncH) (int *nlhs,mxArray *plhs[],int *nrhs, mxArray *prhs[]);

typedef int (*Myinterfun) (char *, GatefuncH F);

typedef int (*GT) ();

#ifndef __DEF_TABLE_STRUCT__
#define __DEF_TABLE_STRUCT__
  typedef struct table_struct {
    Myinterfun f;    /** interface **/
    GT F;     /** function **/
    char *name;      /** its name **/
  } GenericTable;
#endif
  
#define mxLOGICAL int
#define mxLogical int
  
#define REAL    0
#define COMPLEX 1

#ifndef NULL
#define NULL 0
#endif

#define mxCreateFull mxCreateDoubleMatrix

#ifndef __cplusplus
	#ifndef bool
		#define bool int
	#endif
#endif

  /* mexGetMatrixPtr - mexGetArrayPtr : NOT IN MATLAB API - V6.4 compatible*/
#define mexGetMatrixPtr(name) mexGetArrayPtr(name, "caller")
  
  /* mexGetArray : NOT IN MATLAB API - V6.4 compatible*/
#define mexGetArrayPtr(name,type) mexGetArray(name,type) 
  
  void mexPrintf(const char *fmt,...);
  void C2F(mexprintf)(char *error_msg, int len);
  
  /* mexPutFull: NOT IN MATLAB API - V4 compatible */
  /** Put a matrix in Scilab Workspace */ 
#define mexPutFull(name,m,n,ptrM,tag)					\
  if ( ! C2F(cwritemat)(name,(c_local=m,&c_local),(c1_local=n,&c1_local),ptrM,strlen(name))) { \
      mexErrMsgTxt(_("mexPutFull failed\n"));return; }
  
  /* prototypes */
  
  /* mexInfo: NOT IN MATLAB API - SCILAB SPECIFIC ?*/
  void mexInfo (char *); 

  /* mexCheck: NOT IN MATLAB API - SCILAB SPECIFIC ?*/
  int mexCheck (char *,int ); 

  mxArray *mxCreateCharMatrixFromStrings (int m, const char **str);
  mxArray *mxCreateString (const char *string);
  mxArray *C2F(mxcreatestring)  (char *string, long int l);
  /* mxcreatefull : NOT IN MATLAB API - v4 compatible */
  mxArray *C2F(mxcreatefull)  (int *m, int *n, int *it);
  mxArray *mxCreateCharArray (int ND, const int *size);

  mxArray *mxCreateCellArray (int ND, const int *size);
  mxArray *mxCreateCellMatrix (int m, int n);

  mxArray *mxCreateStructArray (int ndim, const int *dims, int nfields, const char **field_names);
  mxArray *mxCreateStructMatrix (int m, int n, int nfields, const char **field_names);

  mxArray *mxGetCell (const mxArray *ptr, int index);

  double mxGetScalar (const mxArray *ptr);
  double C2F(mxgetscalar)  (mxArray *ptr);
  double *mxGetPi (const mxArray *ptr);
  double * C2F(mxgetpi)  (mxArray *ptr);
  double *mxGetPr (const mxArray *ptr);
  double * C2F(mxgetpr)  (mxArray *ptr);
  double mxGetInf (void);
  double mxGetNaN (void);
  double mxGetEps (void);
  bool mxIsNaN (double x);
  bool mxIsInf (double x);
  bool mxIsFinite (double x);
  int *mxGetDimensions (const mxArray *ptr);
  int mxCalcSingleSubscript (const mxArray *ptr, int nsubs, const int *subs);
  int mxGetNumberOfElements (const mxArray *ptr);
  int mxGetNumberOfDimensions(const mxArray *ptr);
  int mxGetNumberOfFields(const mxArray *ptr);
  void *mxGetData(const mxArray *ptr);
  void *mxGetImagData(const mxArray *ptr);

  void clear_mex(int nlhs, mxArray **plhs, int nrhs, mxArray **prhs);

  extern void errjump ();

  int mxGetM (const mxArray *ptr);
  int  C2F(mxgetm)  (mxArray *ptr);
  int mxGetN (const mxArray *ptr);
  int  C2F(mxgetn)  (mxArray *ptr);
  int mxGetString (const mxArray *ptr, char *str, int strl);
  int  C2F(mxgetstring)  (mxArray *ptr, char *str, int *strl);
  bool mxIsComplex (const mxArray *ptr);
  int  C2F(mxiscomplex)  (mxArray *ptr);
  /* mxisfull : NOT IN MATLAB API - v4 compatible */
  bool mxIsFull (const mxArray *ptr);
  int  C2F(mxisfull)  (mxArray *ptr);
  bool mxIsNumeric (const mxArray *ptr);
  int  C2F(mxisnumeric)  (mxArray *ptr);
  bool mxIsSparse (const mxArray *ptr);
  int  C2F(mxissparse)  (mxArray *ptr);
  /* mxisstring : NOT IN MATLAB API - v6.0 compatible */
  bool mxIsString (const mxArray *ptr);
  int  C2F(mxisstring)  (mxArray *ptr);
  int *mxGetIr (const mxArray *ptr);
  int *mxGetJc (const mxArray *ptr);

  /**
   * TODO : comment
   * @param type
   * @param m
   * @param n
   * @param it
   * @param lr
   * @param ptr
   * @param type_len
   * @return
   */
  int C2F(createptr)  (char *type, int *m, int *n, int *it, int *lr, int *ptr, long int type_len);
  
  /**
   * TODO : comment
   * @param m
   * @param ptr
   * @return
   */
  int C2F(createstkptr)  (int *m, vraiptrst *ptr);
  
  
  /**
   * TODO : comment
   * @param nlhs
   * @param plhs
   * @param nrhs
   * @param prhs
   * @return
   */
  int C2F(endmex)  (int *nlhs, mxArray **plhs, int *nrhs, mxArray **prhs);
  
  
  /**
   * TODO : comment
   * @param nlhs
   * @param plhs
   * @param nrhs
   * @param prhs
   * @return
   */
  int C2F(initmex)  (int *nlhs, mxArray **plhs, int *nrhs, mxArray **prhs);
  
  int C2F(mexcallscilab)  (int *nlhs, mxArray **plhs, int *nrhs, mxArray **prhs, char *name, int namelen);
  int C2F(mxcopyptrtoreal8)  (mxArray *ptr, double *y, int *n);
  int C2F(mxcopyreal8toptr)  (double *y, mxArray *ptr, int *n);
  int fortran_mex_gateway (char *fname, FGatefuncH F);
  int mexAtExit (mxArray *ptr);
  int mexCallSCILAB (int nlhs, mxArray **plhs, int nrhs, mxArray **prhs, const char *name);
  int mexCallMATLAB (int nlhs, mxArray **plhs, int nrhs, mxArray **prhs, const char *name);
  int mex_gateway (char *fname, GatefuncH F);
  int mxGetElementSize (const mxArray *ptr);
  int mxGetNzmax (const mxArray *ptr);
  char *mxArrayToString (const mxArray *array_ptr);
  bool mxIsDouble (const mxArray *ptr);
  bool mxIsSingle (const mxArray *ptr);
  bool mxIsLogical (const mxArray *ptr);
  bool mexIsGlobal(const mxArray *ptr);
  /* mxsetlogical: NOT IN MATLAB API - v6.5 compatible */
  void mxSetLogical (mxArray *ptr);
  /* mxclearlogical: NOT IN MATLAB API - v6.5 compatible */
  void mxClearLogical (mxArray *ptr);
  bool mxIsChar (const mxArray *ptr);
  bool mxIsEmpty (const mxArray *ptr);
  bool mxIsClass (const mxArray *ptr, const char *name);
  bool mxIsCell (const mxArray *ptr);
  bool mxIsStruct (const mxArray *ptr);

  bool mxIsInt8(const mxArray *ptr);
  bool mxIsInt16(const mxArray *ptr);
  bool mxIsInt32(const mxArray *ptr);
  bool mxIsUint8(const mxArray *ptr);
  bool mxIsUint16(const mxArray *ptr);
  bool mxIsUint32(const mxArray *ptr);
  /* mxIsUint64 missing */

  void mxSetM(mxArray *ptr, int m);
  void mxSetN(mxArray *ptr, int n);
  void mxSetJc(mxArray *array_ptr, int *jc_data);
  void mxSetIr(mxArray *array_ptr, int *ir_data);
  void mxSetNzmax(mxArray *array_ptr, int nzmax);
  void mxSetCell(mxArray *pa, int i, mxArray *value);

  /* mexGetArray: NOT IN MATLAB API - v6.5 compatible */
  mxArray *mexGetArray(char *name, char *workspace);
  mxArray *mexGetVariable(const char *workspace, const char *name);
  const mxArray *mexGetVariablePtr(const char *workspace, const char *name);

  unsigned long int C2F(mxcalloc)(unsigned int *n, unsigned int *size);
  void *mxCalloc(size_t n, size_t size);
  void *mxMalloc(size_t nsize);
  void mxFree(void *ptr);
  void mxDestroyArray(mxArray *ptr);

  /* mxCalloc_m : NOT IN MATLAB API - SCILAB SPECIFIC ?*/
  void *mxCalloc_m(unsigned int n, unsigned int size);
  /* mxMalloc_m : NOT IN MATLAB API - SCILAB SPECIFIC ?*/
  void *mxMalloc_m(unsigned int nsize);
  /* mxFree_m : NOT IN MATLAB API - SCILAB SPECIFIC ?*/
  void  mxFree_m(void *);
  /* mxfreematrix: NOT IN MATLAB API - v6.0 compatible */
  void mxFreeMatrix(mxArray *ptr);
  void C2F(mxfreematrix)(mxArray *ptr);

  void mexErrMsgTxt(const char *error_msg);
  void C2F(mexerrmsgtxt)(char *error_msg, int len);
  int  mexEvalString(const char *name);
  void mexWarnMsgTxt(const char *error_msg);
  /* mexprint: NOT IN MATLAB API - SCILAB SPECIFIC ?*/
  void mexprint(char* fmt,...);

  int mxGetFieldNumber(const mxArray *ptr, const char *string);
  mxArray *mxGetField(const mxArray *pa, int i, const char *fieldname);
  void  mxSetFieldByNumber(mxArray *array_ptr, int index, int field_number, mxArray *value);
  void mxSetField (mxArray *pa, int i, const char *fieldname, mxArray *value);

  mxArray *mxGetFieldByNumber(const mxArray *ptr, int index, int field_number);
  const char *mxGetFieldNameByNumber(const mxArray *array_ptr, int field_number);
  mxLOGICAL *mxGetLogicals (const mxArray *array_ptr);

  /**
   * TODO : comment
   * @param x
   * @return
   */
  vraiptrst C2F(locptr)(void *x);
  
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

  mxClassID mxGetClassID(const mxArray *ptr);
  /* mxGetName: NOT IN MATLAB API - v6.4 compatible */
  const char *mxGetName(const mxArray *array_ptr);
  /* mxSetName: NOT IN MATLAB API - v6.4 compatible */
  void mxSetName( mxArray    *pa,    const char *s );
  void mxSetPr(mxArray *array_ptr, double *pr);
  void mxSetPi(mxArray *array_ptr, double *pi);
  void mxSetData(mxArray *array_ptr, void *pr);
  mxArray *mxCreateNumericArray(int ndim, const int *dims, mxClassID classid, mxComplexity flag);
  mxArray *mxCreateNumericMatrix(int m, int n, mxClassID classid, int cmplx_flag);
  int mxSetDimensions(mxArray *array_ptr, const int *dims, int ndim);
  mxArray *mxCreateDoubleMatrix(int m, int n, mxComplexity it);
  mxArray *mxCreateDoubleScalar(double value);
  mxArray *mxCreateSparse(int m, int n, int nzmax, mxComplexity cmplx);
  mxArray *mxDuplicateArray(const mxArray *ptr);

  /* typedef uint16_T mxChar; */

  /* typedef short int mxChar; */
  typedef unsigned short mxChar;

#define INT8_T   char
#define UINT8_T  unsigned char
#define INT16_T  short
#define UINT16_T unsigned short
#define INT32_T  int
#define UINT32_T unsigned int
#define REAL32_T float

typedef INT8_T   int8_T;
typedef UINT8_T  uint8_T;
typedef INT16_T  int16_T;
typedef UINT16_T uint16_T;
typedef INT32_T  int32_T;
typedef UINT32_T uint32_T;
typedef REAL32_T real32_T;

typedef int       mwSize;
typedef int       mwIndex;
typedef mxArray * mwPointer;

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
