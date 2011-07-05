/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    mexlib  library
 *
 *    This library emulates Matlab' API functions. It is not fully tested...
 *    -Assumes that Scilab string matrices have one column, e.g.
 *    Str=["qwerty";"123456"]; here this is a 2 x 6 matrix but Scilab
 *    considers Str as a 2 x 1 matrix. ["123";"1234"] is a valid string
 *    matrix which cannot be used here.
 *    -Assumes that sparse matrices have been converted into the Matlab
 *    format. Scilab sparse matrices are stored in the transposed Matlab
 *    format. If A is a sparse Scilab matrix, it should be converted
 *    by the command A=mtlb_sparse(A) in the calling sequence of the
 *    mex function.
 *    -Structs and Cells are Scilab mlists:
 *    Struct=mlist(["st","dims","field1",...,"fieldk"],
 *                 int32([d1,d2,...,dn]),
 *                 list(obj1,      objN),
 *                 .....
 *                 list(obj1,      objN))     k such lists
 *           N = d1 x d2    x dn
 *           obj = Scilab variable or pointer to Scilab variable.
 *     Cell = Struct with one field called "entries" and "st" <- "ce"
 *    One dimensional structs or cells are as follows:
 *    Struct=mlist(["st","dims","field1",...,"fieldk"],
 *                 int32([1,1]),
 *                 obj1,...,objk)
 *
 *    -Nd dimensional arrays are Scilab mlists (for Nd > 2):
 *     X = mlist(["hm","dims","entries"],
 *                 int32([d1,d2,...,dn]),
 *                 values)
 *     values = vector of doubles or int8-16-32 or char
 --------------------------------------------------------------------------*/
#include <stdarg.h>

#include <limits>

#include "yaspio.hxx"
#include "context.hxx"
#include "symbol.hxx"

#include "types.hxx"
#include "int8.hxx"
#include "uint8.hxx"
#include "int16.hxx"
#include "uint16.hxx"
#include "int32.hxx"
#include "uint32.hxx"
#include "int64.hxx"
#include "uint64.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "string.hxx"
#include "struct.hxx"
#include "container.hxx"
#include "cell.hxx"

extern "C"
{
#include "machine.h"
#include "mex.h"
}

#ifdef getType
#undef getType
#endif

#ifdef isComplex
#undef isComplex
#endif

static char *the_current_mex_name;

mxClassID mxGetClassID(const mxArray * ptr)
{
    types::InternalType::RealType type = ((types::InternalType *) ptr)->getType();

    switch (type)
    {
    case types::InternalType::RealInt8:
        return mxINT8_CLASS;
    case types::InternalType::RealUInt8:
        return mxUINT8_CLASS;
    case types::InternalType::RealInt16:
        return mxINT16_CLASS;
    case types::InternalType::RealUInt16:
        return mxUINT16_CLASS;
    case types::InternalType::RealInt32:
        return mxINT32_CLASS;
    case types::InternalType::RealUInt32:
        return mxUINT32_CLASS;
    case types::InternalType::RealInt64:
        return mxINT64_CLASS;
    case types::InternalType::RealUInt64:
        return mxUINT64_CLASS;
    case types::InternalType::RealString:
        return mxCHAR_CLASS;
    case types::InternalType::RealDouble:
        return mxDOUBLE_CLASS;
    case types::InternalType::RealBool:
        return mxLOGICAL_CLASS;
    case types::InternalType::RealFloat:
        return mxSINGLE_CLASS;
    default:
        return mxUNKNOWN_CLASS;
    }
}

bool mxIsInt8(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxINT8_CLASS;
}

bool mxIsInt16(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxINT16_CLASS;
}

bool mxIsInt32(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxINT32_CLASS;
}

bool mxIsInt64(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxINT64_CLASS;
}

bool mxIsUint8(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxUINT8_CLASS;
}

bool mxIsUint16(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxUINT16_CLASS;
}

bool mxIsUint32(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxUINT32_CLASS;
}

bool mxIsUint64(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxUINT64_CLASS;
}

double mxGetEps(void)
{
    types::InternalType * pITEps = symbol::Context::getInstance()->get(symbol::Symbol(L"%eps"));
    if (pITEps && pITEps->isDouble())
    {
        return pITEps->getAs < types::Double > ()->get(0);
    }

    return -1;
}

double mxGetInf(void)
{
    types::InternalType * pITInf = symbol::Context::getInstance()->get(symbol::Symbol(L"%inf"));
    if (pITInf && pITInf->isDouble())
    {
        return pITInf->getAs < types::Double > ()->get(0);
    }

    return -1;
}

double mxGetNaN(void)
{
    double x, y;

    x = mxGetInf();
    y = x / x;
    return y;
}

bool mxIsInf(double x)
{
    if (x == x + 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool mxIsFinite(double x)
{
    if (x < x + 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool mxIsNaN(double x)
{
    if (x != x)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

mxArray *mxCreateData(int m)
/* Utility fctn : create a no-header, unpopulated  *
 * mxArray of size=m*sizeof(double)                */
{
    return mxCreateDoubleMatrix(1, m, mxREAL);
}

int mxGetNumberOfElements(const mxArray * ptr)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return 0;
    }

    types::GenericType * pGT = dynamic_cast < types::GenericType * >(pIT);
    if (pGT == NULL)
    {
        return 0;
    }

    return pGT->getSize();
}

double *mxGetPr(const mxArray * ptr)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return NULL;
    }

    types::Double * pD = dynamic_cast < types::Double * >(pIT);
    if (pD == NULL)
    {
        return NULL;
    }

    return pD->get();
}

double *mxGetPi(const mxArray * ptr)
{
    return ((types::Double *) ptr)->getImg();
}

int mxGetNumberOfDimensions(const mxArray * ptr)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return 0;
    }

    types::GenericType * pGT = pIT->getAs < types::GenericType > ();
    if (pGT == NULL)
    {                           //InternalType but not GenericType, so mono dimension type.
        return 1;
    }

    return pGT->getDims();
}

int *mxGetDimensions(const mxArray * ptr)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return NULL;
    }

    switch (pIT->getType())
    {
    case types::InternalType::RealList:
    case types::InternalType::RealMList:
    case types::InternalType::RealTList:
        {
            int *piDims = (int *)MALLOC(sizeof(int));

            piDims[0] = pIT->getAs < types::Container > ()->getSize();
            return piDims;
        }
    default:
        {
            types::GenericType * pGT = pIT->getAs < types::GenericType > ();
            if (pGT == NULL)
            {
                return NULL;
            }
            return pGT->getDimsArray();
        }
    }
    return NULL;
}

int mxGetM(const mxArray * ptr)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return NULL;
    }

    types::GenericType * pGT = pIT->getAs < types::GenericType > ();
    if (pGT == NULL)
    {
        return NULL;
    }
    return pGT->getRows();
}

void mxSetM(mxArray * ptr, int M)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return;
    }

    types::GenericType * pGT = pIT->getAs < types::GenericType > ();
    if (pGT == NULL)
    {
        return;
    }

    pGT->resize(M, pGT->getCols());
}

int *mxGetJc(const mxArray * ptr)
{
    // TODO: sparse
    return NULL;
}

int *mxGetIr(const mxArray * ptr)
{
    // TODO: sparse
    return NULL;
}

void mxSetJc(mxArray * array_ptr, int *jc_data)
{
    // TODO: sparse
}

void mxSetIr(mxArray * array_ptr, int *ir_data)
{
    // TODO: sparse
}

void mxSetNzmax(mxArray * array_ptr, int nzmax)
{
    // TODO: sparse
}

int mxGetN(const mxArray * ptr)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return NULL;
    }

    types::GenericType * pGT = pIT->getAs < types::GenericType > ();
    if (pGT == NULL)
    {
        return NULL;
    }
    return pGT->getCols();
}

void mxSetN(mxArray * ptr, int N)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return;
    }

    types::GenericType * pGT = pIT->getAs < types::GenericType > ();
    if (pGT == NULL)
    {
        return;
    }

    pGT->resize(pGT->getRows(), N);
}

bool mxIsString(const mxArray * ptr)
{
    /* mxIsString is obsolete. */
    return mxIsChar(ptr);
}

bool mxIsChar(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxCHAR_CLASS;
}

bool mxIsNumeric(const mxArray * ptr)
{
    return mxIsDouble(ptr) || mxIsSingle(ptr) ||
        mxIsInt8(ptr) || mxIsUint8(ptr) ||
        mxIsInt16(ptr) || mxIsUint16(ptr) || mxIsInt32(ptr) || mxIsUint32(ptr) || mxIsInt64(ptr) || mxIsUint64(ptr);
}

bool mxIsDouble(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxDOUBLE_CLASS;
}

bool mxIsSingle(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxSINGLE_CLASS;
}

bool mxIsEmpty(const mxArray * ptr)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {                           //true or false, whatever ;)
        return true;
    }

    switch (pIT->getType())
    {
    case types::InternalType::RealDouble:
        {
            types::Double * pD = pIT->getAs < types::Double > ();
            return pD->getSize() == 0;
        }
    case types::InternalType::RealCell:
        {
            types::Cell * pC = pIT->getAs < types::Cell > ();
            return pC->getSize() == 0;
        }
    case types::InternalType::RealContainer:
    case types::InternalType::RealList:
    case types::InternalType::RealMList:
    case types::InternalType::RealTList:
        {
            types::Container * pC = pIT->getAs < types::Container > ();
            return pC->getSize() == 0;
        }
    default:
        {                       //other type can not be empty
            return false;
        }
    }
}

bool mxIsFull(const mxArray * ptr)
{
    /* mxIsFull is obsolete. */
    return !mxIsSparse(ptr);
}

bool mxIsSparse(const mxArray * ptr)
{
    // TODO: sparse
    return false;
}

bool mxIsLogical(const mxArray * ptr)
{
    // TODO: review
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return false;
    }

    return pIT->isBool();
    //return mxGetClassID(ptr)==mxLOGICAL_CLASS;
}

bool mxIsLogicalScalar(const mxArray * ptr)
{
    return mxIsLogical(ptr) && mxGetNumberOfElements(ptr) == 1;
}

void mxSetLogical(mxArray * ptr)
{
    /* obsolete */
    if (!mxIsNumeric(ptr))
    {
        return;
    }
    int *pr = (int *)mxGetPr(ptr);
    ptr = (mxArray *) new types::Bool(mxGetNumberOfDimensions(ptr), mxGetDimensions(ptr));

    ((types::Bool *) ptr)->set(pr);
}

void mxClearLogical(mxArray * ptr)
{
    /* obsolete */
    if (!mxIsLogical(ptr))
    {
        return;
    }
    int *pr = (int *)mxGetPr(ptr);
    ptr = (mxArray *) new types::Double(mxGetNumberOfDimensions(ptr), mxGetDimensions(ptr));

    ((types::Bool *) ptr)->set(pr);
}

bool mxIsComplex(const mxArray * ptr)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return false;
    }

    types::GenericType * pGT = pIT->getAs < types::GenericType > ();
    if (pGT == NULL)
    {
        return false;
    }

    return pGT->isComplex();
}

double mxGetScalar(const mxArray * ptr)
{
    // TODO: review spec
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return 0;
    }

    switch (pIT->getType())
    {
    case types::InternalType::RealDouble:
        {
            types::Double * pD = pIT->getAs < types::Double > ();
            return pD->get(0);
        }
    case types::InternalType::RealBool:
        {
            types::Bool * pB = pIT->getAs < types::Bool > ();
            return (double)pB->get(0);
        }
    case types::InternalType::RealInt8:
        {
            types::Int8 * pI = pIT->getAs < types::Int8 > ();
            return (double)pI->get(0);
        }
    case types::InternalType::RealUInt8:
        {
            types::UInt8 * pI = pIT->getAs < types::UInt8 > ();
            return (double)pI->get(0);
        }
    case types::InternalType::RealInt16:
        {
            types::Int16 * pI = pIT->getAs < types::Int16 > ();
            return (double)pI->get(0);
        }
    case types::InternalType::RealUInt16:
        {
            types::UInt16 * pI = pIT->getAs < types::UInt16 > ();
            return (double)pI->get(0);
        }
    case types::InternalType::RealInt32:
        {
            types::Int32 * pI = pIT->getAs < types::Int32 > ();
            return (double)pI->get(0);
        }
    case types::InternalType::RealUInt32:
        {
            types::UInt32 * pI = pIT->getAs < types::UInt32 > ();
            return (double)pI->get(0);
        }
    case types::InternalType::RealInt64:
        {
            types::Int64 * pI = pIT->getAs < types::Int64 > ();
            return (double)pI->get(0);
        }
    case types::InternalType::RealUInt64:
        {
            types::UInt64 * pI = pIT->getAs < types::UInt64 > ();
            return (double)pI->get(0);
        }
    default:
        return 0;
    }
}

void *mxGetData(const mxArray * ptr)
{
    return mxGetPr(ptr);
}

void *mxGetImagData(const mxArray * ptr)
{
    return mxGetPi(ptr);
}

void mexErrMsgTxt(const char *error_msg)
{
    throw new ast::ScilabException(error_msg);
}

void mxAssert(int expr, char *error_message)
{
    if (!expr)
    {
        mexErrMsgTxt(error_message);
    }
}

mxArray *mxCreateDoubleMatrix(int m, int n, mxComplexity complexFlag)
{
    types::Double * ptr = new types::Double(m, n, complexFlag == mxCOMPLEX);
    return (mxArray *) ptr;
}

mxArray *mxCreateDoubleScalar(double value)
{
    mxArray *ptr = mxCreateDoubleMatrix(1, 1, mxREAL);

    ((types::Double *) ptr)->set(0, value);
    return ptr;
}

bool mxIsClass(const mxArray * ptr, const char *name)
{
    mxClassID classID = mxGetClassID(ptr);

    if (strcmp(name, "cell") == 0)
    {
        return classID == mxCELL_CLASS;
    }
    if (strcmp(name, "char") == 0)
    {
        return classID == mxCHAR_CLASS;
    }
    if (strcmp(name, "double") == 0)
    {
        return classID == mxDOUBLE_CLASS;
    }
    if (strcmp(name, "function_handle") == 0)
    {
        return classID == mxFUNCTION_CLASS;
    }
    if (strcmp(name, "int8") == 0)
    {
        return classID == mxINT8_CLASS;
    }
    if (strcmp(name, "int16") == 0)
    {
        return classID == mxINT16_CLASS;
    }
    if (strcmp(name, "int32") == 0)
    {
        return classID == mxINT32_CLASS;
    }
    if (strcmp(name, "int64") == 0)
    {
        return classID == mxINT64_CLASS;
    }
    if (strcmp(name, "logical") == 0)
    {
        return classID == mxLOGICAL_CLASS;
    }
    if (strcmp(name, "single") == 0)
    {
        return classID == mxSINGLE_CLASS;
    }
    if (strcmp(name, "struct") == 0)
    {
        return classID == mxSTRUCT_CLASS;
    }
    if (strcmp(name, "uint8") == 0)
    {
        return classID == mxUINT8_CLASS;
    }
    if (strcmp(name, "uint16") == 0)
    {
        return classID == mxUINT16_CLASS;
    }
    if (strcmp(name, "uint32") == 0)
    {
        return classID == mxUINT32_CLASS;
    }
    if (strcmp(name, "uint64") == 0)
    {
        return classID == mxUINT64_CLASS;
    }
    // TODO: how to handle <class_name> and <class_id>?
    if (strcmp(name, "unknown") == 0)
    {
        return classID == mxUNKNOWN_CLASS;
    }
    return false;
}

mxArray *mxCreateStructArray(int ndim, const int *dims, int nfields, const char **field_names)
{
    types::Struct * ptr = new types::Struct(ndim, (int *)dims);
    for (int i = 0; i < nfields; i++)
    {
        // TODO: is this cast right?
        ptr->addField((wchar_t *) field_names[i]);
    }
    return (mxArray *) ptr;
}

mxArray *mxCreateStructMatrix(int m, int n, int nfields, const char **field_names)
{
    int dims[2] = { m, n };
    return mxCreateStructArray(2, dims, nfields, field_names);
}

void mxSetFieldByNumber(mxArray * array_ptr, int lindex, int field_number, mxArray * value)
{
    if (mxIsStruct(array_ptr))
    {
        types::SingleStruct * ptr = ((types::Struct *) array_ptr)->get(lindex);
        types::String * names = ptr->getFieldNames();
        ptr->set(names->get(field_number), (types::InternalType *) value);
    }
}

void mxSetField(mxArray * array_ptr, int lindex, const char *field_name, mxArray * value)
{
    int field_num;

    field_num = mxGetFieldNumber(array_ptr, field_name);
    mxSetFieldByNumber(array_ptr, lindex, field_num, value);
}

const char *mxGetFieldNameByNumber(const mxArray * array_ptr, int field_number)
{
    types::String * names = ((types::Struct *) array_ptr)->getFieldNames();
    // TODO: is this cast right?
    return (const char *)names->get(field_number);
}

mxChar *mxGetChars(mxArray * array_ptr)
{
    if (!mxIsChar(array_ptr))
    {
        return NULL;
    }
    wchar_t *chars = ((types::String *) array_ptr)->get(0);

    return (mxChar *) chars;
}

mxArray *mxCreateNumericArray(int ndim, const int *dims, mxClassID CLASS, mxComplexity complexFlag)
{
    types::GenericType * ptr;

    switch (CLASS)
    {
    case mxDOUBLE_CLASS:
        ptr = new types::Double(ndim, (int *)dims, complexFlag == mxCOMPLEX);
        break;
    case mxINT8_CLASS:
        ptr = new types::Int8(ndim, (int *)dims);
        break;
    case mxUINT8_CLASS:
        ptr = new types::UInt8(ndim, (int *)dims);
        break;
    case mxINT16_CLASS:
        ptr = new types::Int16(ndim, (int *)dims);
        break;
    case mxUINT16_CLASS:
        ptr = new types::UInt16(ndim, (int *)dims);
        break;
    case mxINT32_CLASS:
        ptr = new types::Int32(ndim, (int *)dims);
        break;
    case mxUINT32_CLASS:
        ptr = new types::UInt32(ndim, (int *)dims);
        break;
    case mxINT64_CLASS:
        ptr = new types::Int64(ndim, (int *)dims);
        break;
    case mxUINT64_CLASS:
        ptr = new types::UInt64(ndim, (int *)dims);
        break;
    default:
        ptr = NULL;
    }
    return (mxArray *) ptr;
}

mxArray *mxCreateNumericMatrix(int m, int n, mxClassID CLASS, mxComplexity complexFlag)
{
    int dims[2] = { m, n };
    return mxCreateNumericArray(2, dims, CLASS, complexFlag);
}

mxArray *mxCreateCharArray(int ndim, const int *dims)
{
    if (ndim == 0 || ndim == 1)
    {
        ndim = 2;
    }
    types::String * ptr = new types::String(ndim, (int *)dims);
    return (mxArray *) ptr;
}

mxArray *mxCreateCellArray(int ndim, const int *dims)
{
    types::Cell * ptr = new types::Cell(ndim, (int *)dims);
    return (mxArray *) ptr;
}

mxArray *mxCreateCellMatrix(int m, int n)
{
    int dims[2] = { m, n };
    return mxCreateCellArray(2, dims);
}

mxArray *mxGetCell(const mxArray * ptr, int lindex)
{
    types::Cell * pa = (types::Cell *) ptr;
    return (mxArray *) pa->get(lindex);
}

int mxGetFieldNumber(const mxArray * ptr, const char *string)
{
    // TODO: review
    types::String * names = ((types::Struct *) ptr)->getFieldNames();
    for (int i = 0; i < names->getSize(); i++)
    {
        if (strcmp((char *)names->get(i), string) == 0)
        {
            return i;
        }
    }
    return -1;
}

mxArray *mxGetField(const mxArray * ptr, int lindex, const char *string)
{
    // TODO: review
    types::Struct * pa = (types::Struct *) ptr;
    types::SingleStruct * s = pa->get(lindex);
    return (mxArray *) s->get((wchar_t *) string);
}

mxArray *mxGetFieldByNumber(const mxArray * ptr, int lindex, int field_number)
{
    // TODO: review
    types::Struct * pa = (types::Struct *) ptr;
    types::String * names = pa->getFieldNames();
    types::SingleStruct * s = pa->get(lindex);
    return (mxArray *) s->get(names->get(field_number));
}

int mxGetNumberOfFields(const mxArray * ptr)
{
    types::Struct * pa = (types::Struct *) ptr;
    return pa->getFieldNames()->getSize();
}

/*----------------------------------------------------
* mxCalloc is supposed to initialize data to 0
* but what does it means since size can be anythink
* we initialize to zero for double and int data types
*----------------------------------------------------*/

void *mxRealloc(void *ptr, size_t nsize)
{
    // FIXME
    return NULL;
}

void *mxCalloc(size_t n, size_t size)
{
    // FIXME
    return NULL;
}

/*  void *mxMalloc(unsigned int nsize)  */
void *mxMalloc(size_t nsize)
{
    // FIXME
    return NULL;
}

void *mxCalloc_m(unsigned int n, unsigned int size)
{
    // FIXME
    return NULL;
}

void *mxMalloc_m(unsigned int n)
{
    // FIXME
    return NULL;
}

void mexMakeMemoryPersistent(void *ptr)
{
    // FIXME
}

/* free : explicit free of a mxCalloc_m allocated space
*        except if space is protected
*/

void mxFree_m(void *ptr)
{
    // FIXME
}

/* free : explicit free of all mxCalloc_m allocated space
*        except if space is protected
*/

static void mxFree_m_all()
{
    // FIXME
}

bool mxIsCell(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxCELL_CLASS;
}

bool mxIsStruct(const mxArray * ptr)
{
    return mxGetClassID(ptr) == mxSTRUCT_CLASS;
}

/***************************************************************
* Return in str at most strl characters from first element of
* string Matrix pointed to by ptr ( ptr is assumed to be a String Matrix )
**************************************************************/

int mxGetString(const mxArray * ptr, char *str, int strl)
{
    if (!mxIsChar(ptr))
    {
        return 1;
    }
    types::String * pa = (types::String *) ptr;
    int length = Min(strl - 1, pa->getSize());

    memcpy(str, pa->get(), length);
    str[length] = '\0';
    return (length != pa->getSize())? 1 : 0;
}

char *mxArrayToString(const mxArray * array_ptr)
{
    // TODO: review
    if (mxIsChar(array_ptr))
    {
        types::String * pa = (types::String *) array_ptr;
        int buflen = pa->getSize() + 1;
        char *buf = (char *)mxCalloc(buflen, sizeof(char));

        if (mxGetString(array_ptr, buf, buflen) == 0)
        {
            return buf;
        }
    }
    return (char *)0;
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

void mxFreeMatrix(mxArray * ptr)
{
    mxDestroyArray(ptr);
}

bool mexIsGlobal(const mxArray * ptr)
{
    // TODO: really need iterate in symbols?
    symbol::Context * context = symbol::Context::getInstance();
    int size = symbol::Symbol::map_size();
    wchar_t **keys = symbol::Symbol::get_all();

    for (int i = 0; i < size; i++)
    {
        symbol::Symbol * s = new symbol::Symbol(keys[i]);
        types::InternalType * value = context->get(*s);
        if (value == (types::InternalType *) ptr)
        {
            return context->isGlobalVisible(*s);
        }
    }
    return false;
}

mxArray *mxDuplicateArray(const mxArray * ptr)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return 0;
    }

    return (mxArray *) pIT->clone();
}

mxArray *UnrefStruct(mxArray * ptr)
{
    // FIXME
    return NULL;
}

void mxDestroyArray(mxArray * ptr)
{
    if (mxIsDouble(ptr))
    {
        delete((types::Double *) ptr);
    }
    if (mxIsChar(ptr))
    {
        delete((types::String *) ptr);
    }
    if (mxIsSparse(ptr))
    {
        // TODO: sparse
    }
    if (mxIsInt8(ptr))
    {
        delete((types::Int8 *) ptr);
    }
    if (mxIsInt16(ptr))
    {
        delete((types::Int16 *) ptr);
    }
    if (mxIsInt32(ptr))
    {
        delete((types::Int32 *) ptr);
    }
    if (mxIsInt64(ptr))
    {
        delete((types::Int64 *) ptr);
    }
    if (mxIsUint8(ptr))
    {
        delete((types::UInt8 *) ptr);
    }
    if (mxIsUint16(ptr))
    {
        delete((types::UInt16 *) ptr);
    }
    if (mxIsUint32(ptr))
    {
        delete((types::UInt32 *) ptr);
    }
    if (mxIsUint64(ptr))
    {
        delete((types::UInt64 *) ptr);
    }
    if (mxIsCell(ptr))
    {
        delete((types::Cell *) ptr);
    }
    if (mxIsStruct(ptr))
    {
        delete((types::Struct *) ptr);
    }
}

void mxFree(void *ptr)
{
    // FIXME
}

int mexAtExit(mxArray * ptr)
{
    // FIXME
    return 0;
}

mxArray *mxCreateSparse(int m, int n, int nzmax, mxComplexity cmplx)
{
    // TODO: sparse
    return NULL;
}

/***************************************************************
* Create on Scilab Stack a 1x1 string matrix filled with string
**************************************************************/

mxArray *mxCreateString(const char *string)
{
    types::String * ptr = new types::String(string);
    return (mxArray *) ptr;
}

mxArray *mxCreateLogicalMatrix(int m, int n)
{
    types::Bool * ptr = new types::Bool(m, n);
    return (mxArray *) ptr;
}

mxArray *mxCreateLogicalScalar(mxLogical value)
{
    mxArray *ptr = mxCreateLogicalMatrix(1, 1);

    ((types::Bool *) ptr)->set(0, value);
    return ptr;
}

bool mxIsLogicalScalarTrue(mxArray * ptr)
{
    if (mxIsLogicalScalar(ptr) == false)
    {
        return false;
    }

    if (*mxGetLogicals(ptr) == 0)
    {
        return false;
    }

    return true;
}

bool mxIsLogicalScalar(mxArray * ptr)
{
    return mxIsLogical(ptr) && mxGetNumberOfElements(ptr) == 1;
}

/*
Print function which prints (format,args,....)
in Scilab window
*/

void mexPrintf(const char *fmt, ...)
{
    va_list args;
    char buf[2048];

    va_start(args, fmt);
    sprintf(buf, fmt, args);
    YaspWrite(fmt);
    va_end(args);
}

void mexWarnMsgTxt(const char *error_msg)
{
    YaspError(_("Warning: "));
    YaspError(error_msg);
    YaspError("\n\n");
}

int mexCallSCILAB(int nlhs, mxArray ** plhs, int nrhs, mxArray ** prhs, const char *name)
{
    // FIXME
    return 0;
}

int mexCallMATLAB(int nlhs, mxArray ** plhs, int nrhs, mxArray ** prhs, const char *name)
{
    // FIXME
    return 0;
}

int mxCalcSingleSubscript(const mxArray * ptr, int nsubs, const int *subs)
{
    int retval = 0, coeff = 1;
    int *dims = mxGetDimensions(ptr);

    for (int i = 0; i < nsubs; i++)
    {
        retval += subs[i] * coeff;
        coeff *= dims[i];
    }
    return retval;
}

int C2F(mexcallscilab) (int *nlhs, mxArray ** plhs, int *nrhs, mxArray ** prhs, char *name, int namelen)
{
    return mexCallSCILAB(*nlhs, plhs, *nrhs, prhs, name);
}

/** generic mex interface **/
const char *mexFunctionName(void)
{
    return the_current_mex_name;
}

int mxGetElementSize(const mxArray * ptr)
{
    if (mxIsDouble(ptr))
    {
        return sizeof(types::Double *);
    }
    if (mxIsChar(ptr))
    {
        return sizeof(types::String *);
    }
    if (mxIsSparse(ptr))
    {
        // TODO: sparse
    }
    if (mxIsInt8(ptr))
    {
        return sizeof(types::Int8 *);
    }
    if (mxIsInt16(ptr))
    {
        return sizeof(types::Int16 *);
    }
    if (mxIsInt32(ptr))
    {
        return sizeof(types::Int32 *);
    }
    if (mxIsInt64(ptr))
    {
        return sizeof(types::Int64 *);
    }
    if (mxIsUint8(ptr))
    {
        return sizeof(types::UInt8 *);
    }
    if (mxIsUint16(ptr))
    {
        return sizeof(types::UInt16 *);
    }
    if (mxIsUint32(ptr))
    {
        return sizeof(types::UInt32 *);
    }
    if (mxIsUint64(ptr))
    {
        return sizeof(types::UInt64 *);
    }
    if (mxIsCell(ptr))
    {
        return sizeof(types::Cell *);
    }
    if (mxIsStruct(ptr))
    {
        return sizeof(types::Struct *);
    }
    return 0;
}

mxArray *mxCreateCharMatrixFromStrings(int m, const char **str)
{
    int n = 0;

    for (int k = 0; k < m; k++)
    {
        n = Max(n, (int)strlen(str[k]));
    }
    // TODO: is wchar_t** cast right?
    types::String * ptr = new types::String(m, n, (wchar_t **) str);
    return (mxArray *) ptr;
}

int mexEvalString(const char *name)
{
    // FIXME : Better use directly YaSp engine here.
    // double *val ;
    // int rep;
    // mxArray *ppr[3];mxArray *ppl[1];
    // int nlhs;     int nrhs;
    // ppr[0] = mxCreateString(name);
    // ppr[1] = mxCreateString("errcatch");
    // ppr[2] = mxCreateString("m");
    // nrhs=3;nlhs=1;
    // rep = mexCallSCI(nlhs, ppl, nrhs, ppr, "execstr",0);
    // /* check returned value */
    // val = mxGetPr(ppl[0]);
    // mxFreeMatrix(ppl[0]);
    // mxFreeMatrix(ppr[2]);
    // mxFreeMatrix(ppr[1]);
    // mxFreeMatrix(ppr[0]);
    // if ( rep == 1 || (int) (*val) != 0 )
    // {
    //     errjump();
    // }
    return 0;
}

mxArray *mexGetArray(char *name, char *workspace)
{
    return mexGetVariable(workspace, name);
}

const mxArray *mexGetVariablePtr(const char *workspace, const char *var_name)
{
    mxArray *value = mexGetVariable(workspace, var_name);

    if (value)
    {
        return mxDuplicateArray(value);
    }
    return NULL;
}

mxArray *mexGetVariable(const char *workspace, const char *name)
{
    symbol::Context * context = symbol::Context::getInstance();
    wchar_t *dest;

    mbstowcs(dest, name, strlen(name));
    types::InternalType * value = NULL;
    if (strcmp(workspace, "base"))
    {
        value = context->get(*(new symbol::Symbol(dest)));
    }
    else if (strcmp(workspace, "caller"))
    {
        value = context->getCurrentLevel(*(new symbol::Symbol(dest)));
    }
    else if (strcmp(workspace, "global"))
    {
        value = context->getGlobalValue(*(new symbol::Symbol(dest)));
    }
    return (mxArray *) value;
}

int mexPutArray(mxArray * array_ptr, char *workspace)
{
    /* obsolete */
    mexPrintf(_("Function mexPutArray is obsolete, use mexPutVariable!\n"));
    return 1;
}

int mexPutVariable(const char *workspace, char *var_name, mxArray * array_ptr)
{
    symbol::Context * context = symbol::Context::getInstance();
    wchar_t *dest;

    mbstowcs(dest, var_name, strlen(var_name));
    if (strcmp(workspace, "base"))
    {
        context->put(*(new symbol::Symbol(dest)), *(types::InternalType *) array_ptr);
    }
    else if (strcmp(workspace, "caller"))
    {
        context->put_in_previous_scope(*(new symbol::Symbol(dest)), *(types::InternalType *) array_ptr);
    }
    else if (strcmp(workspace, "global"))
    {
        context->setGlobalValue(*(new symbol::Symbol(dest)), *(types::InternalType *) array_ptr);
    }
    return NULL;
}

int mexPutFull(char *name, int m, int n, double *pr, double *pi)
{
    mxArray *array_ptr = mxCreateDoubleMatrix(m, n, pi == NULL ? mxREAL : mxCOMPLEX);

    mxSetPr(array_ptr, pr);
    mxSetPi(array_ptr, pi);
    mexPutVariable("caller", name, array_ptr);
    return 0;
}

void mxSetName(mxArray * array_ptr, const char *name)
{
    /* obsolete */
    // FIXME
    mexErrMsgTxt(_("Routine mxSetName not implemented !\n"));
    exit(1);
}

void mxSetData(mxArray * array_ptr, void *data_ptr)
{

    if (mxIsChar(array_ptr))
    {
        ((types::String *) array_ptr)->set((wchar_t **) data_ptr);
    }
    else if (mxIsInt8(array_ptr))
    {
        ((types::Int8 *) array_ptr)->set((char *)data_ptr);
    }
    else if (mxIsInt16(array_ptr))
    {
        ((types::Int16 *) array_ptr)->set((short *)data_ptr);
    }
    else if (mxIsInt32(array_ptr))
    {
        ((types::Int32 *) array_ptr)->set((int *)data_ptr);
    }
    else if (mxIsInt64(array_ptr))
    {
        ((types::Int64 *) array_ptr)->set((long long *)data_ptr);
    }
    else if (mxIsLogical(array_ptr))
    {
        ((types::Bool *) array_ptr)->set((int *)data_ptr);
    }
    // else if (mxIsSingle(array_ptr)) {
    //   ((types::Float *) array_ptr)->set((float *) data_ptr);
    // }
    else if (mxIsUint8(array_ptr))
    {
        ((types::UInt8 *) array_ptr)->set((unsigned char *)data_ptr);
    }
    else if (mxIsUint16(array_ptr))
    {
        ((types::UInt16 *) array_ptr)->set((unsigned short *)data_ptr);
    }
    else if (mxIsUint32(array_ptr))
    {
        ((types::UInt32 *) array_ptr)->set((unsigned int *)data_ptr);
    }
    else if (mxIsUint64(array_ptr))
    {
        ((types::UInt64 *) array_ptr)->set((unsigned long long *)data_ptr);
    }
}

void mxSetPr(mxArray * ptr, double *pr)
{
    ((types::Double *) ptr)->set(pr);
}

void mxSetPi(mxArray * ptr, double *pi)
{
    ((types::Double *) ptr)->setImg(pi);
}

const char *mxGetName(const mxArray * array_ptr)
{
    /* obsolete */
    // FIXME
    mexPrintf(_("Routine mxGetName not implemented.\n"));
    exit(1);
    return 0;
}

int mxSetDimensions(mxArray * array_ptr, const int *dims, int ndim)
{
    if (mxIsCell(array_ptr))
    {
        ((types::Cell *) array_ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsChar(array_ptr))
    {
        ((types::String *) array_ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsDouble(array_ptr))
    {
        ((types::Double *) array_ptr)->resize((int *)dims, ndim);
    }
    // else if (mxIsFunction(array_ptr)) {
    //     ((types::Function *) array_ptr)->resize((int *) dims, ndim);
    //}
    // else if (mxIsSparse(array_ptr)) {
    //     TODO: we don't have Sparse classes yet
    // }
    else if (mxIsInt8(array_ptr))
    {
        ((types::Int8 *) array_ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsInt16(array_ptr))
    {
        ((types::Int16 *) array_ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsInt32(array_ptr))
    {
        ((types::Int32 *) array_ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsInt64(array_ptr))
    {
        ((types::Int64 *) array_ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsLogical(array_ptr))
    {
        ((types::Bool *) array_ptr)->resize((int *)dims, ndim);
    }
    // else if (mxIsSingle(array_ptr)) {
    //     ((types::Float *) array_ptr)->resize((int *) dims, ndim);
    // }
    else if (mxIsStruct(array_ptr))
    {
        ((types::Struct *) array_ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsUint8(array_ptr))
    {
        ((types::UInt8 *) array_ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsUint16(array_ptr))
    {
        ((types::UInt16 *) array_ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsUint32(array_ptr))
    {
        ((types::UInt32 *) array_ptr)->resize((int *)dims, ndim);
    }
    else if (mxIsUint64(array_ptr))
    {
        ((types::UInt64 *) array_ptr)->resize((int *)dims, ndim);
    }

    return 0;
}

const char *mxGetClassName(const mxArray * ptr)
{
    if (mxIsDouble(ptr))
    {
        return "double";
    }
    if (mxIsChar(ptr))
    {
        return "char";
    }
    if (mxIsSparse(ptr))
    {
        return "sparse";
    }
    if (mxIsInt8(ptr))
    {
        return "int8";
    }
    if (mxIsInt16(ptr))
    {
        return "int16";
    }
    if (mxIsInt32(ptr))
    {
        return "int32";
    }
    if (mxIsInt64(ptr))
    {
        return "int64";
    }
    if (mxIsUint8(ptr))
    {
        return "uint8";
    }
    if (mxIsUint16(ptr))
    {
        return "uint16";
    }
    if (mxIsUint32(ptr))
    {
        return "uint32";
    }
    if (mxIsUint64(ptr))
    {
        return "uint64";
    }
    if (mxIsCell(ptr))
    {
        return "cell";
    }
    if (mxIsStruct(ptr))
    {
        return "struct";
    }
    return "unknown";
}

void mxSetCell(mxArray * array_ptr, int lindex, mxArray * value)
{
    ((types::Cell *) array_ptr)->set(lindex, (types::InternalType *) value);
}

int mxGetNzmax(const mxArray * ptr)
{
    // TODO: sparse
    return 0;
}

mxLogical *mxGetLogicals(const mxArray * ptr)
{
    types::InternalType * pIT = (types::InternalType *) ptr;
    if (pIT == NULL)
    {
        return NULL;
    }

    types::Bool * pB = pIT->getAs < types::Bool > ();
    if (pB == NULL)
    {
        return NULL;
    }

    return (mxLogical *) pB->get();
}

void mexInfo(char *str)
{
    // FIXME : Use YaSpWrite
}

int mexCheck(char *str, int nbvars)
{
    // FIXME : Where does Nbvars come from ??
    // if ( nbvars != -1 && Nbvars != nbvars)
    //     fprintf(stderr,"%s %d %d\n",str,Nbvars,nbvars);
    // return Nbvars ;
    return 0;
}

/****************************************************
* C functions for Fortran  mexfunctions
****************************************************/

double *C2F(mxgetpr) (mxArray * ptr)
{
    return mxGetPr(ptr);
}

double *C2F(mxgetpi) (mxArray * ptr)
{
    return mxGetPi(ptr);
}

int C2F(mxgetm) (mxArray * ptr)
{
    return mxGetM(ptr);
}

int C2F(mxgetn) (mxArray * ptr)
{
    return mxGetN(ptr);
}

int C2F(mxisstring) (mxArray * ptr)
{
    return mxIsString(ptr);
}

int C2F(mxisnumeric) (mxArray * ptr)
{
    return mxIsNumeric(ptr);
}

int C2F(mxisfull) (mxArray * ptr)
{
    return mxIsFull(ptr);
}

int C2F(mxissparse) (mxArray * ptr)
{
    return mxIsSparse(ptr);
}

int C2F(mxiscomplex) (mxArray * ptr)
{
    return mxIsComplex(ptr);
}

double C2F(mxgetscalar) (mxArray * ptr)
{
    return mxGetScalar(ptr);
}

void C2F(mexprintf) (char *error_msg, int len)
{
    error_msg[len] = '\0';
    mexPrintf(error_msg);
}

void C2F(mexerrmsgtxt) (char *error_msg, int len)
{
    error_msg[len] = '\0';
    mexErrMsgTxt(error_msg);
}

mxArray *C2F(mxcreatefull) (int *m, int *n, int *it)
{
    /* mxCreateFull is obsolete. Call mxCreateDoubleMatrix instead. */
    return (mxArray *) mxCreateDoubleMatrix(*m, *n, (mxComplexity) * it);
}

mxArray *C2F(mxcreatedoublematrix) (int *m, int *n, int *it)
{
    return (mxArray *) mxCreateDoubleMatrix(*m, *n, (mxComplexity) * it);
}

unsigned long int C2F(mxcalloc) (unsigned int *n, unsigned int *size)
{
    mxCalloc(*n, *size);
    return 0;
}

int C2F(mxgetstring) (mxArray * ptr, char *str, int *strl)
{
    return mxGetString(ptr, str, *strl);
}

void C2F(mxfreematrix) (mxArray * ptr)
{
    mxFreeMatrix(ptr);
}

mxArray *C2F(mxcreatestring) (char *string, long int l)
{
    string[l] = '\0';
    return mxCreateString(string);
}

int C2F(mxcopyreal8toptr) (double *y, mxArray * ptr, int *n)
{
    double *pr = mxGetPr(ptr);

    memcpy(y, pr, (*n) * sizeof(double));
    return 0;
}

int C2F(mxcopycomplex16toptr) (double *y, mxArray * ptr, mxArray * pti, int *n)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

int C2F(mxcopyptrtoreal8) (mxArray * ptr, double *y, int *n)
{
    double *pr = mxGetPr(ptr);

    memcpy(pr, y, (*n) * sizeof(double));
    return 0;
}

int C2F(mxcopyptrtocomplex16) (mxArray * ptr, mxArray * pti, double *y, int *n)
{
    // FIXME : Wrap this one to the C one
    return 0;
}

/* mxCreateLogicalArray
 * mxIsLogicalScalarTrue */

/* *mxRealloc(void *ptr, size_t size);
   mxArray *mxCreateStringFromNChars(const char *str, int n);
   int mxSetClassName(mxArray *pa, const char *classname);
   int mxAddField(mxArray *pa, const char *fieldname);
   void mxRemoveField(mxArray *pa, int field);
   void mxSetCopyInCell(mxArray *pa, int i, mxArray *value);  */
