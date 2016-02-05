
/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) ENPC
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SCI_MEX
#define SCI_MEX

#include <stdio.h>
#include "machine.h"
#include <assert.h>

//define mxAssert in order to benefit user compilation flag ( debug or not )
#define mxAssert(/* int */expr, /*char* */error_message) if (!expr){mexPrintf("Assertion Error: %s\n", error_message); assert(expr);}
#define mxAssertS(/* int */expr, /*char* */error_msesage) assert(expr);


#ifndef __cplusplus
typedef int bool;
#endif

typedef int mxArray;

typedef int mwSize;
typedef int mwIndex;
typedef int mwSignedIndex;

typedef char mxChar;
typedef int mxLogical;

typedef enum
{
    mxUNKNOWN_CLASS,
    mxCELL_CLASS,
    mxSTRUCT_CLASS,
    mxLOGICAL_CLASS,
    mxCHAR_CLASS,
    mxVOID_CLASS,
    mxDOUBLE_CLASS,
    mxSINGLE_CLASS,
    mxINT8_CLASS,
    mxUINT8_CLASS,
    mxINT16_CLASS,
    mxUINT16_CLASS,
    mxINT32_CLASS,
    mxUINT32_CLASS,
    mxINT64_CLASS,
    mxUINT64_CLASS,
    mxFUNCTION_CLASS
} mxClassID;

typedef enum
{
    mxREAL = 0, mxCOMPLEX
} mxComplexity;

//Create or Delete Array
mxArray *mxCreateDoubleMatrix(int m, int n, mxComplexity it);
mxArray *mxCreateDoubleScalar(double value);
mxArray *mxCreateNumericMatrix(int m, int n, mxClassID classid, mxComplexity flag);
mxArray *mxCreateNumericArray(int ndim, const int *dims, mxClassID classid, mxComplexity flag);
mxArray *mxCreateUninitNumericMatrix(size_t m, size_t n, mxClassID classid, mxComplexity ComplexFlag);
mxArray *mxCreateUninitNumericArray(size_t ndim, size_t *dims, mxClassID classid, mxComplexity ComplexFlag);

mxArray *mxCreateString(const char *string);
mxArray *mxCreateCharMatrixFromStrings(int m, const char **str);
mxArray *mxCreateCharArray(int ND, const int *size);

mxArray *mxCreateLogicalScalar(mxLogical value);
mxArray *mxCreateLogicalMatrix(int m, int n);
mxArray *mxCreateLogicalArray(mwSize ndim, const mwSize *dims);
mxArray *mxCreateSparseLogicalMatrix(mwSize m, mwSize n, mwSize nzmax);

mxArray *mxCreateSparse(int m, int n, int nzmax, mxComplexity cmplx);

mxArray *mxCreateStructMatrix(int m, int n, int nfields, const char **field_names);
mxArray *mxCreateStructArray(int ndim, const int *dims, int nfields, const char **field_names);

mxArray *mxCreateCellArray(int ND, const int *size);
mxArray *mxCreateCellMatrix(int m, int n);

void mxDestroyArray(mxArray *ptr);
mxArray *mxDuplicateArray(const mxArray *ptr);

void *mxCalloc(size_t n, size_t size);
void *mxMalloc(size_t nsize);
void *mxRealloc(void *ptr, mwSize size);
void mxFree(void *ptr);

//Validate Data
int mxIsDouble(const mxArray *ptr);
int mxIsSingle(const mxArray *ptr);
int mxIsComplex(const mxArray *ptr);
int mxIsNumeric(const mxArray *ptr);
int mxIsInt64(const mxArray *ptr);
int mxIsUint64(const mxArray *ptr);
int mxIsInt32(const mxArray *ptr);
int mxIsUint32(const mxArray *ptr);
int mxIsInt16(const mxArray *ptr);
int mxIsUint16(const mxArray *ptr);
int mxIsInt8(const mxArray *ptr);
int mxIsUint8(const mxArray *ptr);
int mxIsScalar(const mxArray *array_ptr);

int mxIsChar(const mxArray *ptr);

int mxIsLogical(const mxArray *ptr);
int mxIsLogicalScalar(const mxArray *ptr);
int mxIsLogicalScalarTrue(const mxArray *ptr);

int mxIsStruct(const mxArray *ptr);

int mxIsCell(const mxArray *ptr);

int mxIsClass(const mxArray *ptr, const char *name);

int mxIsInf(double x);
int mxIsFinite(double x);
int mxIsNaN(double x);
int mxIsEmpty(const mxArray *ptr);
int mxIsSparse(const mxArray *ptr);

int mxIsFromGlobalWS(const mxArray *pm);

//Convert Data Types
char *mxArrayToString(const mxArray *array_ptr);
char *mxArrayToUTF8String(const mxArray *array_ptr);
int mxGetString(const mxArray *ptr, char *str, int strl);

int mxSetClassName(mxArray *array_ptr, const char *classname);

//Access Data
int mxGetNumberOfDimensions(const mxArray *ptr);
int mxGetElementSize(const mxArray *ptr);
mwSize *mxGetDimensions(const mxArray *pm);
int mxSetDimensions(mxArray *array_ptr, const int *dims, int ndim);
int mxGetNumberOfElements(const mxArray *ptr);
int mxCalcSingleSubscript(const mxArray *ptr, int nsubs, const int *subs);
int mxGetM(const mxArray *ptr);
void mxSetM(mxArray *ptr, int m);
int mxGetN(const mxArray *ptr);
void mxSetN(mxArray *ptr, int n);

double mxGetScalar(const mxArray *ptr);
double *mxGetPr(const mxArray *ptr);
void mxSetPr(mxArray *array_ptr, double *pr);
double *mxGetPi(const mxArray *ptr);
void mxSetPi(mxArray *array_ptr, double *pi);
void *mxGetData(const mxArray *ptr);
void mxSetData(mxArray *array_ptr, void *pr);
void *mxGetImagData(const mxArray *ptr);
void mxSetImagData(mxArray *array_ptr, void *data_ptr);

mxChar *mxGetChars(mxArray *array_ptr);

mxLogical *mxGetLogicals(const mxArray *ptr);

mxClassID mxGetClassID(const mxArray *ptr);
const char *mxGetClassName(const mxArray *ptr);
//mxSetClassName is already defined early

mxArray *mxGetProperty(const mxArray *pa, mwIndex index, const char *propname);
void mxSetProperty(mxArray *pa, mwIndex index, const char *propname, const mxArray *value);

mxArray *mxGetField(const mxArray *pa, int i, const char *fieldname);
void mxSetField(mxArray *pa, int i, const char *fieldname, mxArray *value);
int mxGetNumberOfFields(const mxArray *ptr);
const char *mxGetFieldNameByNumber(const mxArray *array_ptr, int field_number);
int mxGetFieldNumber(const mxArray *ptr, const char *string);
mxArray *mxGetFieldByNumber(const mxArray *ptr, int index, int field_number);
void mxSetFieldByNumber(mxArray *array_ptr, int index, int field_number, mxArray *value);
int mxAddField(mxArray *pm, const char *fieldname);
void mxRemoveField(mxArray *pm, int fieldnumber);

mxArray *mxGetCell(const mxArray *ptr, int index);
void mxSetCell(mxArray *pa, int i, mxArray *value);

int mxGetNzmax(const mxArray *ptr);
void mxSetNzmax(mxArray *array_ptr, int nzmax);
int *mxGetIr(const mxArray *ptr);
void mxSetIr(mxArray *array_ptr, int *ir_data);
int *mxGetJc(const mxArray *ptr);
void mxSetJc(mxArray *array_ptr, int *jc_data);

//MEX Library
#ifdef __cplusplus
extern "C" void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
#endif
//not a real mex function, just used to set caller name
void setmexFunctionName(const char* name);
const char *mexFunctionName(void);
int mexAtExit(void(*func)(void));

int mexCallMATLAB(int nlhs, mxArray **plhs, int nrhs, mxArray **prhs, const char *name);
mxArray *mexCallMATLABWithTrap(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[], const char *functionName);
int mexEvalString(const char *name);
mxArray *mexEvalStringWithTrap(const char *command);

const mxArray *mexGet(double handle, const char *property);
int mexSet(double handle, const char *property, mxArray *value);
mxArray *mexGetVariable(const char *workspace, const char *name);
const mxArray *mexGetVariablePtr(const char *workspace, const char *name);
int mexPutVariable(const char *workspace, const char *varname, const mxArray *pm);
int mexIsGlobal(const mxArray *ptr);

int mexPrintf(const char *format, ...);

void mexSetTrapFlag(int trapflag);
void mexErrMsgIdAndTxt(const char *errorid, const char *errormsg, ...);
void mexWarnMsgIdAndTxt(const char *warningid, const char *warningmsg, ...);
void mexErrMsgTxt(const char *error_msg);
void mexWarnMsgTxt(const char *error_msg);

int mexIsLocked(void);
void mexLock(void);
void mexUnlock(void);
void mexMakeArrayPersistent(mxArray *pm);
void mexMakeMemoryPersistent(void *ptr);

//Data Types
double mxGetInf(void);
double mxGetNaN(void);
double mxGetEps(void);
#endif
#ifdef __cplusplus
}
#endif
