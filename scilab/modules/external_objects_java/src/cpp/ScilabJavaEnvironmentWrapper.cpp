/*
 * PIMS ( http://forge.scilab.org/index.php/p/pims ) - This file is part of PIMS
 * Copyright (C) 2010 - Baozeng DING
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef _MSC_VER
#include "windows.h"
#endif

#include "ScilabJavaEnvironmentWrapper.hxx"
#include "ScilabJavaObject.hxx"

extern "C" {
#include "getScilabJavaVM.h"
}

namespace org_scilab_modules_external_objects_java
{

VariableType ScilabJavaEnvironmentWrapper::isunwrappable(int id) const
{
    JavaVM *vm = getScilabJavaVM ();
    int type = ScilabJavaObject::isUnwrappable(vm, id);
    return (VariableType)type;
}

// template <class T>
// int ScilabJavaEnvironmentWrapper::wrap(T* x, const bool isRef) const
// {
//     JavaVM *vm = getScilabJavaVM ();
//     return ScilabJavaObject::wrapDouble(vm, x, isRef);
// }


int ScilabJavaEnvironmentWrapper::wrap(double * x, const bool isRef) const
{
    JavaVM *vm = getScilabJavaVM ();
    return ScilabJavaObject::wrapDouble(vm, *x);
}

int ScilabJavaEnvironmentWrapper::wrap(double * x, int xSize, const bool isRef) const
{
    //  return wrapData<double>(x, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(double * x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData<double>(x, xSize, xSizeCol, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(double * re, double * im, const bool isRef) const
{
    //if (helper.getWrapSingleWithNumpy())
    //{
    //    return wrapData(re, im, 1, isRef);
    //}
    //else
    //{
    //    return wrapData(re, im, isRef);
    //}
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(double * re, double * im, int xSize, const bool isRef) const
{
    //    return wrapData(re, im, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(double * re, double * im, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData(re, im, xSize, xSizeCol, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(char * x, const bool isRef) const
{
    JavaVM *vm = getScilabJavaVM ();
    return ScilabJavaObject::wrapChar(vm, *x);
}

int ScilabJavaEnvironmentWrapper::wrap(char * x, int xSize, const bool isRef) const
{
    //    return wrapData<char>(x, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(char * x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData<char>(x, xSize, xSizeCol, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned char * x, const bool isRef) const
{
    //    return wrapData<unsigned char>(x, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned char * x, int xSize, const bool isRef) const
{
    //  return wrapData<unsigned char>(x, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned char * x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData<unsigned char>(x, xSize, xSizeCol, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(short * x, const bool isRef) const
{
    JavaVM *vm = getScilabJavaVM ();
    return ScilabJavaObject::wrapShort(vm, *x);
}

int ScilabJavaEnvironmentWrapper::wrap(short * x, int xSize, const bool isRef) const
{
    //    return wrapData<short>(x, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(short * x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData<short>(x, xSize, xSizeCol, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned short * x, const bool isRef) const
{
    JavaVM *vm = getScilabJavaVM ();
    return ScilabJavaObject::wrapUShort(vm, *x);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned short * x, int xSize, const bool isRef) const
{
    //    return wrapData<unsigned short>(x, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned short * x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData<unsigned short>(x, xSize, xSizeCol, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(int * x, const bool isRef) const
{
    JavaVM *vm = getScilabJavaVM ();
    return ScilabJavaObject::wrapInt(vm, *x);
}

int ScilabJavaEnvironmentWrapper::wrap(int * x, int xSize, const bool isRef) const
{
    //    return wrapData<int>(x, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(int * x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData<int>(x, xSize, xSizeCol, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned int * x, const bool isRef) const
{
    JavaVM *vm = getScilabJavaVM ();
    return ScilabJavaObject::wrapUInt(vm, *x);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned int * x, int xSize, const bool isRef) const
{
    //    return wrapData<unsigned int>(x, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned int * x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData<unsigned int>(x, xSize, xSizeCol, isRef);
    return 0;
}

#ifdef __SCILAB_INT64__

int ScilabJavaEnvironmentWrapper::wrap(long long * x, const bool isRef) const
{
    JavaVM *vm = getScilabJavaVM ();
    return ScilabJavaObject::wrapLong(vm, *x);
}

int ScilabJavaEnvironmentWrapper::wrap(long long * x, int xSize, const bool isRef) const
{
    //    return wrapData<long>(x, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(long long * x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData<long>(x, xSize, xSizeCol, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned long long * x, const bool isRef) const
{
    //    return wrapData<unsigned long>(x, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned long long * x, int xSize, const bool isRef) const
{
    //    return wrapData<unsigned long>(x, xSize, isRef);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned long long * x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData<unsigned long>(x, xSize, xSizeCol, isRef);
    return 0;
}

#endif

int ScilabJavaEnvironmentWrapper::wrap(char ** x, const bool isRef) const
{
    JavaVM *vm = getScilabJavaVM ();
    return ScilabJavaObject::wrapString(vm, (char const*) * x);
}

int ScilabJavaEnvironmentWrapper::wrap(char ** x, int xSize, const bool isRef) const
{
    //    return wrapData(x, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(char ** x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData(x, xSize, xSizeCol, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrapBool(int * x, const bool isRef) const
{
    JavaVM *vm = getScilabJavaVM ();
    return ScilabJavaObject::wrapBoolean(vm, *x != 0);
}

int ScilabJavaEnvironmentWrapper::wrapBool(int * x, int xSize, const bool isRef) const
{
    //    return wrapData<int, bool, npy_bool>(x, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrapBool(int * x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData<int, bool, npy_bool>(x, xSize, xSizeCol, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(float * x, const bool isRef) const
{
    JavaVM *vm = getScilabJavaVM ();
    return ScilabJavaObject::wrapFloat(vm, *x);
}

int ScilabJavaEnvironmentWrapper::wrap(float * x, int xSize, const bool isRef) const
{
    //    return wrapData<float>(x, xSize, isRef);
    return 0;
}

int ScilabJavaEnvironmentWrapper::wrap(float * x, int xSize, int xSizeCol, const bool isRef) const
{
    //    return wrapData<float>(x, xSize, xSizeCol, isRef);
    return 0;
}

void ScilabJavaEnvironmentWrapper::unwrapdouble(int id, const ScilabDoubleStackAllocator & allocator) const
{
    JavaVM *vm = getScilabJavaVM ();
    double data = ScilabJavaObject::unwrapDouble(vm, id);
    allocator.allocate(1, 1, &data);
}

void ScilabJavaEnvironmentWrapper::unwraprowdouble(int id, const ScilabDoubleStackAllocator & allocator) const
{
    printf("unwraprowdouble %d...", id);
    //  pyUnwrapRow<double>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatdouble(int id, const ScilabDoubleStackAllocator & allocator) const
{
    printf("unwrapmatdouble %d...", id);
    //    pyUnwrapMat<double>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapchar(int id, const ScilabCharStackAllocator & allocator) const
{
    //    pyUnwrapSingle<char>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowchar(int id, const ScilabCharStackAllocator & allocator) const
{
    //    pyUnwrapRow<char>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatchar(int id, const ScilabCharStackAllocator & allocator) const
{
    //    pyUnwrapMat<char>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapuchar(int id, const ScilabUCharStackAllocator & allocator) const
{
    //    pyUnwrapSingle<unsigned char>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowuchar(int id, const ScilabUCharStackAllocator & allocator) const
{
    //    pyUnwrapRow<unsigned char>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatuchar(int id, const ScilabUCharStackAllocator & allocator) const
{
    //    pyUnwrapMat<unsigned char>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapshort(int id, const ScilabShortStackAllocator & allocator) const
{
    //    pyUnwrapSingle<short>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowshort(int id, const ScilabShortStackAllocator & allocator) const
{
    //    pyUnwrapRow<short>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatshort(int id, const ScilabShortStackAllocator & allocator) const
{
    //    pyUnwrapMat<short>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapushort(int id, const ScilabUShortStackAllocator & allocator) const
{
    //    pyUnwrapSingle<unsigned short>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowushort(int id, const ScilabUShortStackAllocator & allocator) const
{
    //    pyUnwrapRow<unsigned short>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatushort(int id, const ScilabUShortStackAllocator & allocator) const
{
    //    pyUnwrapMat<unsigned short>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapint(int id, const ScilabIntStackAllocator & allocator) const
{
    //    pyUnwrapSingle<int>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowint(int id, const ScilabIntStackAllocator & allocator) const
{
    //    pyUnwrapRow<int>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatint(int id, const ScilabIntStackAllocator & allocator) const
{
    //    pyUnwrapMat<int>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapuint(int id, const ScilabUIntStackAllocator & allocator) const
{
    //    pyUnwrapSingle<unsigned int>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowuint(int id, const ScilabUIntStackAllocator & allocator) const
{
    //    pyUnwrapRow<unsigned int>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatuint(int id, const ScilabUIntStackAllocator & allocator) const
{
    //    pyUnwrapMat<unsigned int>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraplong(int id, const ScilabLongStackAllocator & allocator) const
{
    //    pyUnwrapSingle<long long>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowlong(int id, const ScilabLongStackAllocator & allocator) const
{
    //    pyUnwrapRow<long long>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatlong(int id, const ScilabLongStackAllocator & allocator) const
{
    //    pyUnwrapMat<long long>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapulong(int id, const ScilabULongStackAllocator & allocator) const
{
    //    pyUnwrapSingle<unsigned long long>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowulong(int id, const ScilabULongStackAllocator & allocator) const
{
    //    pyUnwrapRow<unsigned long long>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatulong(int id, const ScilabULongStackAllocator & allocator) const
{
    //    pyUnwrapMat<unsigned long long>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapboolean(int id, const ScilabBooleanStackAllocator & allocator) const
{
    //    pyUnwrapSingle<int>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowboolean(int id, const ScilabBooleanStackAllocator & allocator) const
{
    //    pyUnwrapRow<int>(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatboolean(int id, const ScilabBooleanStackAllocator & allocator) const
{
    //    pyUnwrapMat<int >(id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapstring(int id, const ScilabStringStackAllocator & allocator) const
{
    JavaVM *vm = getScilabJavaVM ();
    char * data = ScilabJavaObject::unwrapString(vm, id);
    allocator.allocate(1, 1, &data);
}

void ScilabJavaEnvironmentWrapper::unwraprowstring(int id, const ScilabStringStackAllocator & allocator) const
{
    /*    PyObject * obj = scope.getObject(id);

        if (PyList_Check(obj))
        {
            // List case
            int col = PyList_Size(obj);
            char ** data = new char*[col];
            for (int i = 0; i < col; i++)
            {
                PyObject * item = PyList_GET_ITEM(obj, i);
                data[i] = PyString_AsString(item);
            }

            allocator.allocate(1, col, data);
            delete[] data;
        }
        else
        {
            // Numpy array case
            PyArrayObject * arr = reinterpret_cast<PyArrayObject *>(obj);
            npy_intp * dims = PyArray_DIMS(arr);
            npy_intp * strides = PyArray_STRIDES(arr);
            char * pyData = reinterpret_cast<char *>(PyArray_DATA(arr));
            char ** data = new char*[dims[0]];

            for (int i = 0; i < dims[0]; i++)
            {
                data[i] = pyData + i * strides[0];
            }

            allocator.allocate(1, dims[0], data);
            delete[] data;
        }
    */
}

void ScilabJavaEnvironmentWrapper::unwrapmatstring(int id, const ScilabStringStackAllocator & allocator) const
{
    /*    PyObject * obj = scope.getObject(id);

        if (PyList_Check(obj))
        {
            // List case
            int row = PyList_Size(obj);
            if (row > 0)
            {
                PyObject * f = PyList_GET_ITEM(obj, 0);
                int col = PyList_Size(f);
                char ** data = new char*[row * col];
                for (int i = 0; i < row; i++)
                {
                    PyObject * sublist = PyList_GetItem(obj, i);
                    for (int j = 0; j < col; j++)
                    {
                        PyObject * item = PyList_GET_ITEM(sublist, j);
                        data[i + row * j] = PyString_AsString(item);
                    }
                }

                allocator.allocate(row, col, data);
                delete[] data;
            }
        }
        else
        {
            // Numpy array case
            PyArrayObject * arr = reinterpret_cast<PyArrayObject *>(obj);
            npy_intp * dims = PyArray_DIMS(arr);
            npy_intp * strides = PyArray_STRIDES(arr);
            char * pyData = reinterpret_cast<char *>(PyArray_DATA(arr));
            char ** data = new char*[dims[0] * dims[1]];

            for (int i = 0; i < dims[0]; i++)
            {
                for (int j = 0; j < dims[1]; j++)
                {
                    data[i + dims[0] * j] = pyData + i * strides[0] + j * strides[1];
                }
            }

            allocator.allocate(dims[0], dims[1], data);
            delete[] data;
        }
    */
}

void ScilabJavaEnvironmentWrapper::unwrapcomplex(int id, const ScilabComplexStackAllocator & allocator) const
{
    /*    PyObject * obj = scope.getObject(id);
        ComplexDataPointers cdp = allocator.allocate(1, 1, static_cast<double *>(0), static_cast<double *>(0));
        pyUnwrap(*cdp.realPtr, *cdp.imagPtr, obj);*/
}

void ScilabJavaEnvironmentWrapper::unwraprowcomplex(int id, const ScilabComplexStackAllocator & allocator) const
{
    /*    PyObject * obj = scope.getObject(id);

        if (PyList_Check(obj))
        {
            // List case
            int col = PyList_Size(obj);
            ComplexDataPointers cdp = allocator.allocate(1, col, static_cast<double *>(0), static_cast<double *>(0));
            for (int i = 0; i < col; i++)
            {
                PyObject * item = PyList_GET_ITEM(obj, i);
                pyUnwrap(cdp.realPtr[i], cdp.imagPtr[i], obj);
            }
        }
        else
        {
            // Numpy array case
            PyArrayObject * arr = reinterpret_cast<PyArrayObject *>(obj);
            npy_intp * dims = PyArray_DIMS(arr);
            npy_intp * strides = PyArray_STRIDES(arr);

            ComplexDataPointers cdp = allocator.allocate(1, static_cast<int>(dims[0]), static_cast<double *>(0), static_cast<double *>(0));
            char * pyData = reinterpret_cast<char *>(PyArray_DATA(arr));
            for (int i = 0; i < dims[0]; i++)
            {
                complex c = *reinterpret_cast<complex *>(pyData + i * strides[0]);
                cdp.realPtr[i] = c.re;
                cdp.imagPtr[i] = c.im;
            }
        }
    */
}

void ScilabJavaEnvironmentWrapper::unwrapmatcomplex(int id, const ScilabComplexStackAllocator & allocator) const
{
    /*    PyObject * obj = scope.getObject(id);
        if (PyList_Check(obj))
        {
            // List case
            int row = PyList_Size(obj);
            if (row > 0)
            {
                PyObject * f = PyList_GET_ITEM(obj, 0);
                int col = PyList_Size(f);
                ComplexDataPointers cdp = allocator.allocate(row, col, static_cast<double *>(0), static_cast<double *>(0));
                for (int i = 0; i < row; i++)
                {
                    PyObject * sublist = PyList_GET_ITEM(obj, i);
                    for (int j = 0; j < col; j++)
                    {
                        PyObject * item = PyList_GET_ITEM(sublist, j);
                        pyUnwrap(cdp.realPtr[i + row * j], cdp.imagPtr[i + row * j], item);
                    }
                }
            }
        }
        else
        {
            // Numpy array case
            PyArrayObject * arr = reinterpret_cast<PyArrayObject *>(obj);
            npy_intp * dims = PyArray_DIMS(arr);
            npy_intp * strides = PyArray_STRIDES(arr);

            ComplexDataPointers cdp = allocator.allocate(static_cast<int>(dims[0]), static_cast<int>(dims[1]), static_cast<double *>(0), static_cast<double *>(0));
            char * pyData = reinterpret_cast<char*>(PyArray_DATA(arr));

            for (int i = 0; i < dims[0]; i++)
            {
                for (int j = 0; j < dims[1]; j++)
                {
                    complex c = *reinterpret_cast<complex *>(pyData + i * strides[0] + j * strides[1]);
                    cdp.realPtr[i + dims[0] * j] = c.re;
                    cdp.imagPtr[i + dims[0] * j] = c.im;
                }
            }
            }*/
}

void ScilabJavaEnvironmentWrapper::unwrapfloat(int id, const ScilabFloatStackAllocator & allocator) const { }

void ScilabJavaEnvironmentWrapper::unwraprowfloat(int id, const ScilabFloatStackAllocator & allocator) const { }

void ScilabJavaEnvironmentWrapper::unwrapmatfloat(int id, const ScilabFloatStackAllocator & allocator) const { }
}
