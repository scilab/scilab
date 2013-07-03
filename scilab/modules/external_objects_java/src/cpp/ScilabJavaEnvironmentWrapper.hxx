/*
 * PIMS ( http://forge.scilab.org/index.php/p/pims ) - This file is part of PIMS
 * Copyright (C) 2010 - Baozeng DING
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCILABPYTHONENVIRONMENTWRAPPER_HXX__
#define __SCILABPYTHONENVIRONMENTWRAPPER_HXX__

#include "ExternalObjects.h"
#include "JavaOptionsHelper.hxx"

#include "ScilabAbstractEnvironmentWrapper.hxx"
#include "ScilabJavaException.hxx"

#include "GiwsException.hxx"

extern "C" {
#include "getScilabJavaVM.h"
}

#define SCILABJAVAOBJECT "org/scilab/modules/external_objects_java/ScilabJavaObject"

using namespace org_modules_external_objects;

namespace org_scilab_modules_external_objects_java
{

typedef struct
{
    double re;
    double im;
} complex;

class ScilabJavaEnvironmentWrapper : public ScilabAbstractEnvironmentWrapper
{

    //    JavaVariablesScope & scope;
    JavaOptionsHelper & helper;
    jclass ScilabJavaObjectClass_;

    jmethodID unwrapBooleanID_;
    jmethodID unwrapRowBooleanID_;
    jmethodID unwrapMatBooleanID_;

    jmethodID unwrapByteID_;
    jmethodID unwrapRowByteID_;
    jmethodID unwrapMatByteID_;

    jmethodID unwrapShortID_;
    jmethodID unwrapRowShortID_;
    jmethodID unwrapMatShortID_;

    jmethodID unwrapIntID_;
    jmethodID unwrapRowIntID_;
    jmethodID unwrapMatIntID_;

    jmethodID unwrapLongID_;
    jmethodID unwrapRowLongID_;
    jmethodID unwrapMatLongID_;

    jmethodID unwrapDoubleID_;
    jmethodID unwrapRowDoubleID_;
    jmethodID unwrapMatDoubleID_;

public:

    ScilabJavaEnvironmentWrapper(JavaOptionsHelper & _helper) : helper(_helper)
    {
        JavaVM * vm = getScilabJavaVM();
        JNIEnv * curEnv = NULL;

        vm->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
        ScilabJavaObjectClass_ = curEnv->FindClass(SCILABJAVAOBJECT);

        unwrapBooleanID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapBoolean", "(I)Z");
        unwrapRowBooleanID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowBoolean", "(I)[Z");
        unwrapMatBooleanID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatBoolean", "(I)[[Z");

        unwrapByteID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapByte", "(I)B");
        unwrapRowByteID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowByte", "(I)[B");
        unwrapMatByteID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatByte", "(I)[[B");

        unwrapShortID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapShort", "(I)S");
        unwrapRowShortID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowShort", "(I)[S");
        unwrapMatShortID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatShort", "(I)[[S");

        unwrapIntID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapInt", "(I)I");
        unwrapRowIntID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowInt", "(I)[I");
        unwrapMatIntID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatInt", "(I)[[I");

        unwrapLongID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapLong", "(I)J");
        unwrapRowLongID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowLong", "(I)[J");
        unwrapMatLongID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatLong", "(I)[[J");

        unwrapDoubleID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapDouble", "(I)D");
        unwrapRowDoubleID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowDouble", "(I)[D");
        unwrapMatDoubleID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatDouble", "(I)[[D");
    }

    ~ScilabJavaEnvironmentWrapper() { }

    VariableType isunwrappable(int id) const;

    int wrap(double * x, const bool isRef) const;

    int wrap(double * x, int xSize, const bool isRef) const;

    int wrap(double * x, int xSize, int xSizeCol, const bool isRef) const;

    int wrap(double * re, double * im, const bool isRef) const;

    int wrap(double * re, double * im, int xSize, const bool isRef) const;

    int wrap(double * re, double * im, int xSize, int xSizeCol, const bool isRef) const;

    int wrap(char * x, const bool isRef) const;

    int wrap(char * x, int xSize, const bool isRef) const;

    int wrap(char * x, int xSize, int xSizeCol, const bool isRef) const;


    int wrap(unsigned char * x, const bool isRef) const;

    int wrap(unsigned char * x, int xSize, const bool isRef) const;

    int wrap(unsigned char * x, int xSize, int xSizeCol, const bool isRef) const;


    int wrap(short * x, const bool isRef) const;

    int wrap(short * x, int xSize, const bool isRef) const;

    int wrap(short * x, int xSize, int xSizeCol, const bool isRef) const;


    int wrap(unsigned short * x, const bool isRef) const;

    int wrap(unsigned short * x, int xSize, const bool isRef) const;

    int wrap(unsigned short * x, int xSize, int xSizeCol, const bool isRef) const;


    int wrap(int * x, const bool isRef) const;

    int wrap(int * x, int xSize, const bool isRef) const;

    int wrap(int * x, int xSize, int xSizeCol, const bool isRef) const;


    int wrap(unsigned int * x, const bool isRef) const;

    int wrap(unsigned int * x, int xSize, const bool isRef) const;

    int wrap(unsigned int * x, int xSize, int xSizeCol, const bool isRef) const;

#ifdef __SCILAB_INT64__

    int wrap(long long * x, const bool isRef) const;

    int wrap(long long * x, int xSize, const bool isRef) const;

    int wrap(long long * x, int xSize, int xSizeCol, const bool isRef) const;


    int wrap(unsigned long long * x, const bool isRef) const;

    int wrap(unsigned long long * x, int xSize, const bool isRef) const;

    int wrap(unsigned long long * x, int xSize, int xSizeCol, const bool isRef) const;

#endif

    int wrap(char ** x, const bool isRef) const;

    int wrap(char ** x, int xSize, const bool isRef) const;

    int wrap(char ** x, int xSize, int xSizeCol, const bool isRef) const;


    int wrap(float * x, const bool isRef) const;

    int wrap(float * x, int xSize, const bool isRef) const;

    int wrap(float * x, int xSize, int xSizeCol, const bool isRef) const;


    int wrapBool(int * x, const bool isRef) const;

    int wrapBool(int * x, int xSize, const bool isRef) const;

    int wrapBool(int * x, int xSize, int xSizeCol, const bool isRef) const;

    /**
     * Unwrap a the external object with the given id into a double
     * @param id the value to wrap
     * @param pos the position in the Scilab stack where to put the result
     */
    void unwrapdouble(int id, const ScilabDoubleStackAllocator & allocator) const;

    void unwraprowdouble(int id, const ScilabDoubleStackAllocator & allocator) const;

    void unwrapmatdouble(int id, const ScilabDoubleStackAllocator & allocator) const;

    void unwrapcomplex(int id, const ScilabComplexStackAllocator & allocator) const;

    void unwraprowcomplex(int id, const ScilabComplexStackAllocator & allocator) const;

    void unwrapmatcomplex(int id, const ScilabComplexStackAllocator & allocator) const;

    void unwrapchar(int id, const ScilabCharStackAllocator & allocator) const;

    void unwraprowchar(int id, const ScilabCharStackAllocator & allocator) const;

    void unwrapmatchar(int id, const ScilabCharStackAllocator & allocator) const;

    void unwrapuchar(int id, const ScilabUCharStackAllocator & allocator) const;

    void unwraprowuchar(int id, const ScilabUCharStackAllocator & allocator) const;

    void unwrapmatuchar(int id, const ScilabUCharStackAllocator & allocator) const ;

    void unwrapshort(int id, const ScilabShortStackAllocator & allocator) const;

    void unwraprowshort(int id, const ScilabShortStackAllocator  & allocator) const;

    void unwrapmatshort(int id, const ScilabShortStackAllocator & allocator) const;

    void unwrapushort(int id, const ScilabUShortStackAllocator & allocator) const;

    void unwraprowushort(int id, const ScilabUShortStackAllocator & allocator) const;

    void unwrapmatushort(int id, const ScilabUShortStackAllocator & allocator) const;

    void unwrapint(int id, const ScilabIntStackAllocator & allocator) const;

    void unwraprowint(int id, const ScilabIntStackAllocator & allocator) const;

    void unwrapmatint(int id, const ScilabIntStackAllocator & allocator) const;

    void unwrapuint(int id, const ScilabUIntStackAllocator & allocator) const;

    void unwraprowuint(int id, const ScilabUIntStackAllocator & allocator) const;

    void unwrapmatuint(int id, const ScilabUIntStackAllocator & allocator) const;

    void unwraplong(int id, const ScilabLongStackAllocator & allocator) const;

    void unwraprowlong(int id, const ScilabLongStackAllocator & allocator) const;

    void unwrapmatlong(int id, const ScilabLongStackAllocator & allocator) const;

    void unwrapulong(int id, const ScilabULongStackAllocator & allocator) const;

    void unwraprowulong(int id, const ScilabULongStackAllocator & allocator) const;

    void unwrapmatulong(int id, const ScilabULongStackAllocator & allocator) const;

    void unwrapboolean(int id, const ScilabBooleanStackAllocator & allocator) const;

    void unwraprowboolean(int id, const ScilabBooleanStackAllocator & allocator) const;

    void unwrapmatboolean(int id, const ScilabBooleanStackAllocator & allocator) const;

    void unwrapstring(int id, const ScilabStringStackAllocator & allocator) const;

    void unwraprowstring(int id, const ScilabStringStackAllocator & allocator) const;

    void unwrapmatstring(int id, const ScilabStringStackAllocator & allocator) const;

    void unwrapfloat(int id, const ScilabFloatStackAllocator & allocator) const;

    void unwraprowfloat(int id, const ScilabFloatStackAllocator & allocator) const;

    void unwrapmatfloat(int id, const ScilabFloatStackAllocator & allocator) const;

private:

    inline const jmethodID getSingleMethod(const char x) const
    {
        return unwrapByteID_;
    }
    inline const jmethodID getSingleMethod(const unsigned char x) const
    {
        return unwrapByteID_;
    }
    inline const jmethodID getSingleMethod(const short x) const
    {
        return unwrapShortID_;
    }
    inline const jmethodID getSingleMethod(const unsigned short x) const
    {
        return unwrapShortID_;
    }
    inline const jmethodID getSingleMethod(const int x) const
    {
        return unwrapIntID_;
    }
    inline const jmethodID getSingleMethod(const unsigned int x) const
    {
        return unwrapIntID_;
    }
    inline const jmethodID getSingleMethod(const long x) const
    {
        return unwrapLongID_;
    }
    inline const jmethodID getSingleMethod(const unsigned long x) const
    {
        return unwrapLongID_;
    }
    inline const jmethodID getSingleMethod(const double x) const
    {
        return unwrapDoubleID_;
    }
    inline const jmethodID getSingleMethod(const bool x) const
    {
        return unwrapBooleanID_;
    }

    inline const jmethodID getRowMethod(const char x) const
    {
        return unwrapRowByteID_;
    }
    inline const jmethodID getRowMethod(const unsigned char x) const
    {
        return unwrapRowByteID_;
    }
    inline const jmethodID getRowMethod(const short x) const
    {
        return unwrapRowShortID_;
    }
    inline const jmethodID getRowMethod(const unsigned short x) const
    {
        return unwrapRowShortID_;
    }
    inline const jmethodID getRowMethod(const int x) const
    {
        return unwrapRowIntID_;
    }
    inline const jmethodID getRowMethod(const unsigned int x) const
    {
        return unwrapRowIntID_;
    }
    inline const jmethodID getRowMethod(const long x) const
    {
        return unwrapRowLongID_;
    }
    inline const jmethodID getRowMethod(const unsigned long x) const
    {
        return unwrapRowLongID_;
    }
    inline const jmethodID getRowMethod(const double x) const
    {
        return unwrapRowDoubleID_;
    }
    inline const jmethodID getRowMethod(const bool x) const
    {
        return unwrapRowBooleanID_;
    }

    inline const jmethodID getMatMethod(const char x) const
    {
        return unwrapMatByteID_;
    }
    inline const jmethodID getMatMethod(const unsigned char x) const
    {
        return unwrapMatByteID_;
    }
    inline const jmethodID getMatMethod(const short x) const
    {
        return unwrapMatShortID_;
    }
    inline const jmethodID getMatMethod(const unsigned short x) const
    {
        return unwrapMatShortID_;
    }
    inline const jmethodID getMatMethod(const int x) const
    {
        return unwrapMatIntID_;
    }
    inline const jmethodID getMatMethod(const unsigned int x) const
    {
        return unwrapMatIntID_;
    }
    inline const jmethodID getMatMethod(const long x) const
    {
        return unwrapMatLongID_;
    }
    inline const jmethodID getMatMethod(const unsigned long x) const
    {
        return unwrapMatLongID_;
    }
    inline const jmethodID getMatMethod(const double x) const
    {
        return unwrapMatDoubleID_;
    }
    inline const jmethodID getMatMethod(const bool x) const
    {
        return unwrapMatBooleanID_;
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const char x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticByteMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const unsigned char x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticByteMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const short x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticShortMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const unsigned short x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticShortMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const int x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticIntMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const unsigned int x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticIntMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const long x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticLongMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const unsigned long x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticLongMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const double x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticDoubleMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const bool x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticBooleanMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T, typename U, class V>
    inline void unwrapSingle(JavaVM * jvm_, const int javaID, const V & allocator) const
    {
        unwrapSingle<T, U, U>(jvm_, javaID, allocator);
    }

    template <typename T, typename U, typename V, class W>
    inline void unwrapSingle(JavaVM * jvm_, const int javaID, const W & allocator) const
    {
        JNIEnv * curEnv = NULL;
        U * addr = NULL;

        jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);

        T res = CallStatic<T>(curEnv, (V) * addr, javaID);
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }

        addr = allocator.allocate(1, 1, 0);
        *addr = static_cast<U>(res);
    }

    template <typename T, typename U, class V>
    inline void unwrapRow(JavaVM * jvm_, const int javaID, const V & allocator) const
    {
        unwrapRow<T, U, U>(jvm_, javaID, allocator);
    }

    template <typename T, typename U, typename V, class W>
    inline void unwrapRow(JavaVM * jvm_, const int javaID, const W & allocator) const
    {
        jint lenRow;
        jboolean isCopy = JNI_FALSE;
        JNIEnv * curEnv = NULL;
        U * addr = NULL;

        jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);

        jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(ScilabJavaObjectClass_, getRowMethod((V) * addr), javaID));
        if (curEnv->ExceptionCheck())
        {
            curEnv->DeleteLocalRef(res);
            throw GiwsException::JniCallMethodException(curEnv);
        }

        lenRow = curEnv->GetArrayLength(res);

        try
        {
            addr = allocator.allocate(1, lenRow, 0);
        }
        catch (const ScilabAbstractEnvironmentException & e)
        {
            curEnv->DeleteLocalRef(res);
            throw;
        }

        T * resultsArray = static_cast<T *>(curEnv->GetPrimitiveArrayCritical(res, &isCopy));
        for (int i = 0; i < lenRow; i++)
        {
            addr[i] = static_cast<U>(resultsArray[i]);
        }

        curEnv->ReleasePrimitiveArrayCritical(res, resultsArray, JNI_ABORT);
        curEnv->DeleteLocalRef(res);
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }
    }

    template <typename T, typename U, class V>
    inline void unwrapMat(JavaVM * jvm_, const int javaID, const V & allocator) const
    {
        unwrapMat<T, U, U>(jvm_, javaID, allocator);
    }

    template <typename T, typename U, typename V, class W>
    inline void unwrapMat(JavaVM * jvm_, const int javaID, const W & allocator) const
    {
        jint lenRow, lenCol;
        jboolean isCopy = JNI_FALSE;
        jarray oneDim;
        JNIEnv * curEnv = NULL;
        U * addr = NULL;

        jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);

        jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(ScilabJavaObjectClass_, getMatMethod((V) * addr), javaID));
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }

        lenRow = curEnv->GetArrayLength(res);
        oneDim = reinterpret_cast<jarray>(curEnv->GetObjectArrayElement(res, 0));
        lenCol = curEnv->GetArrayLength(oneDim);
        curEnv->DeleteLocalRef(oneDim);

        try
        {
            if (helper.getMethodOfConv())
            {
                addr = allocator.allocate(lenRow, lenCol, 0);
            }
            else
            {
                addr = allocator.allocate(lenCol, lenRow, 0);
            }
        }
        catch (const ScilabAbstractEnvironmentException & e)
        {
            curEnv->DeleteLocalRef(res);
            throw;
        }

        T * resultsArray;
        for (int i = 0; i < lenRow; i++)
        {
            oneDim = reinterpret_cast<jarray>(curEnv->GetObjectArrayElement(res, i));
            resultsArray = static_cast<T *>(curEnv->GetPrimitiveArrayCritical(oneDim, &isCopy));
            if (helper.getMethodOfConv())
            {
                for (int j = 0; j < lenCol; j++)
                {
                    addr[j * lenRow + i] = static_cast<U>(resultsArray[j]);
                }
            }
            else
            {
                for (int j = 0; j < lenCol; j++)
                {
                    addr[i * lenCol + j] = static_cast<U>(resultsArray[j]);
                }
            }
            curEnv->ReleasePrimitiveArrayCritical(oneDim, resultsArray, JNI_ABORT);
            curEnv->DeleteLocalRef(oneDim);
        }

        curEnv->DeleteLocalRef(res);
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }
    }


    /*
      inline static PyObject * pyWrap(double x)
      {
      return PyFloat_FromDouble(x);
      }
      inline static PyObject * pyWrap(float x)
      {
      return PyFloat_FromDouble((double)x);
      }
      inline static PyObject * pyWrap(bool x)
      {
      return PyBool_FromLong((long)x);
      }
      inline static PyObject * pyWrap(char x)
      {
      return PyInt_FromLong((long)x);
      }
      inline static PyObject * pyWrap(unsigned char x)
      {
      return PyInt_FromLong((long)x);
      }
      inline static PyObject * pyWrap(short x)
      {
      return PyInt_FromLong((long)x);
      }
      inline static PyObject * pyWrap(unsigned short x)
      {
      return PyInt_FromLong((long)x);
      }
      inline static PyObject * pyWrap(int x)
      {
      return PyInt_FromLong((long)x);
      }
      inline static PyObject * pyWrap(unsigned int x)
      {
      return PyInt_FromLong((long)x);
      }
      inline static PyObject * pyWrap(long long x)
      {
      return PyInt_FromLong(x);
      }
      inline static PyObject * pyWrap(unsigned long long x)
      {
      return PyInt_FromLong((long)x);
      }
      inline static PyObject * pyWrap(char * x)
      {
      return PyString_FromString(const_cast<const char *>(x));
      }
      inline static PyObject * pyWrap(double r, double i)
      {
      return PyComplex_FromDoubles(r, i);
      }

      inline static void pyUnwrap(double & x, PyObject * obj)
      {
      x = PyFloat_AS_DOUBLE(obj);
      }
      inline static void pyUnwrap(double & re, double & im, PyObject * obj)
      {
      re = PyComplex_RealAsDouble(obj);
      im = PyComplex_ImagAsDouble(obj);
      }
      inline static void pyUnwrap(float & x, PyObject * obj)
      {
      x = (float)PyFloat_AS_DOUBLE(obj);
      }
      inline static void pyUnWrap(bool & x, PyObject * obj)
      {
      x = (bool)PyInt_AS_LONG(obj);
      }
      inline static void pyUnwrap(char & x, PyObject * obj)
      {
      x = (char)PyInt_AS_LONG(obj);
      }
      inline static void pyUnwrap(unsigned char & x, PyObject * obj)
      {
      x = (unsigned char)PyInt_AS_LONG(obj);
      }
      inline static void pyUnwrap(short & x, PyObject * obj)
      {
      x = (short)PyInt_AS_LONG(obj);
      }
      inline static void pyUnwrap(unsigned short & x, PyObject * obj)
      {
      x = (unsigned short)PyInt_AS_LONG(obj);
      }
      inline static void pyUnwrap(int & x, PyObject * obj)
      {
      x = (int)PyInt_AS_LONG(obj);
      }
      inline static void pyUnwrap(unsigned int & x, PyObject * obj)
      {
      x = (unsigned int)PyInt_AS_LONG(obj);
      }
      inline static void pyUnwrap(long long & x, PyObject * obj)
      {
      x = (long long)PyInt_AS_LONG(obj);
      }
      inline static void pyUnwrap(unsigned long long & x, PyObject * obj)
      {
      x = (unsigned long long)PyInt_AS_LONG(obj);
      }
      inline static void pyUnwrap(char* & x, PyObject * obj)
      {
      x = PyString_AS_STRING(obj);
      }

      inline static PyArray_Descr * pyGetDescr(double x)
      {
      return PyArray_DescrFromType(PyArray_DOUBLE);
      }
      inline static PyArray_Descr * pyGetDescr(float x)
      {
      return PyArray_DescrFromType(PyArray_FLOAT);
      }
      inline static PyArray_Descr * pyGetDescr(char x)
      {
      return PyArray_DescrFromType(PyArray_BYTE);
      }
      inline static PyArray_Descr * pyGetDescr(unsigned char x)
      {
      return PyArray_DescrFromType(PyArray_UBYTE);
      }
      inline static PyArray_Descr * pyGetDescr(short x)
      {
      return PyArray_DescrFromType(PyArray_SHORT);
      }
      inline static PyArray_Descr * pyGetDescr(unsigned short x)
      {
      return PyArray_DescrFromType(PyArray_USHORT);
      }
      inline static PyArray_Descr * pyGetDescr(int x)
      {
      return PyArray_DescrFromType(PyArray_INT);
      }
      inline static PyArray_Descr * pyGetDescr(unsigned int x)
      {
      return PyArray_DescrFromType(PyArray_UINT);
      }
      inline static PyArray_Descr * pyGetDescr(long long x)
      {
      return PyArray_DescrFromType(PyArray_LONG);
      }
      inline static PyArray_Descr * pyGetDescr(unsigned long long x)
      {
      return PyArray_DescrFromType(PyArray_ULONG);
      }
      inline static PyArray_Descr * pyGetDescr(char * x)
      {
      return PyArray_DescrFromType(PyArray_STRING);
      }
      inline static PyArray_Descr * pyGetDescr(bool x)
      {
      return PyArray_DescrFromType(PyArray_BOOL);
      }

      template <typename T>
      inline void pyUnwrapSingle(int id, const ScilabSingleTypeStackAllocator<T> & allocator) const
      {
      PyObject * obj = scope.getObject(id);
      T * data = allocator.allocate(1, 1, static_cast<T *>(0));
      pyUnwrap(*data, obj);
      }

      template <typename T>
      inline void pyUnwrapRow(int id, const ScilabSingleTypeStackAllocator<T> & allocator) const
      {
      PyObject * obj = scope.getObject(id);
      if (PyList_Check(obj))
      {
      // List case
      int col = PyList_Size(obj);
      T * data = allocator.allocate(1, col, static_cast<T *>(0));
      for (int i = 0; i < col; i++)
      {
      PyObject * item = PyList_GET_ITEM(obj, i);
      pyUnwrap(data[i], item);
      }
      }
      else
      {
      // Numpy array case
      PyArrayObject * arr = (PyArrayObject *)obj;
      npy_intp * dims = PyArray_DIMS(arr);
      npy_intp * strides = PyArray_STRIDES(arr);

      if (strides[0] == sizeof(T))
      {
      allocator.allocate(1, static_cast<int>(dims[0]), reinterpret_cast<T*>(PyArray_DATA(arr)));
      }
      else
      {
      T * data = allocator.allocate(1, static_cast<int>(dims[0]), static_cast<T *>(0));
      char * pyData = (char *)PyArray_DATA(arr);
      for (int i = 0; i < dims[0]; i++)
      {
      data[i] = *(T*)(pyData + i * strides[0]);
      }
      }
      }
      }

      template <typename T>
      inline void pyUnwrapMat(int id, const ScilabSingleTypeStackAllocator<T> & allocator) const
      {
      PyObject * obj = scope.getObject(id);
      if (PyList_Check(obj))
      {
      // List case
      int row = PyList_Size(obj);
      if (row > 0)
      {
      PyObject * f = PyList_GET_ITEM(obj, 0);
      int col = PyList_Size(f);
      T * data = allocator.allocate(row, col, static_cast<T *>(0));
      for (int i = 0; i < row; i++)
      {
      PyObject * sublist = PyList_GET_ITEM(obj, i);
      for (int j = 0; j < col; j++)
      {
      PyObject * item = PyList_GET_ITEM(sublist, j);
      pyUnwrap(data[i + row * j], item);
      }
      }
      }
      }
      else
      {
      // Numpy array case
      PyArrayObject * arr = (PyArrayObject *)obj;
      npy_intp * dims = PyArray_DIMS(arr);
      npy_intp * strides = PyArray_STRIDES(arr);

      if (PyArray_ISFORTRAN(arr) && strides[0] == sizeof(T) && strides[1] == sizeof(T) * dims[0])
      {
      // the allocator use a memcpy (it is faster than two loops)
      allocator.allocate(static_cast<int>(dims[0]), static_cast<int>(dims[1]), (T*)PyArray_DATA(arr));
      }
      else
      {
      T * data = allocator.allocate(static_cast<int>(dims[0]), static_cast<int>(dims[1]), static_cast<T*>(0));
      char * pyData = (char*)PyArray_DATA(arr);
      for (int i = 0; i < dims[0]; i++)
      {
      for (int j = 0; j < dims[1]; j++)
      {
      data[i + dims[0] * j] = *(T*)(pyData + i * strides[0] + j * strides[1]);
      }
      }
      }
      }
      }


      template <typename T>
      inline int wrapData(T * x, const bool isRef) const
      {
      if (helper.getWrapSingleWithNumpy())
      {
      return wrapData<T, T, T>(x, 1, isRef);
      }
      else
      {
      return wrapData<T, T>(x, isRef);
      }
      }

      template <typename T>
      inline int wrapData(T * x, int xSize, const bool isRef) const
      {
      return wrapData<T, T, T>(x, xSize, isRef);
      }

      template <typename T>
      inline int wrapData(T * x, int xSize, int xSizeCol, const bool isRef) const
      {
      return wrapData<T, T, T>(x, xSize, xSizeCol, isRef);
      }

      template <typename T, typename U>
      inline int wrapData(T * x, const bool isRef) const
      {
      PyObject * obj = pyWrap(static_cast<U>(*x));
      return scope.addObject(obj);
      }

      template <typename T, typename U, typename V>
      inline int wrapData(T * x, int xSize, const bool isRef) const
      {
      if (helper.getUseNumpy())
      {
      PyObject * array;
      V * matnp = 0;
      npy_intp dim[] = { xSize };
      npy_intp strides[] = { 0 };
      npy_intp * stridesAddr = strides;

      if (helper.getUseCopy() && !isRef)
      {
      matnp = new V[xSize];
      for (int i = 0; i < xSize; i++)
      {
      matnp[i] = static_cast<V>(x[i]);
      }
      stridesAddr = 0;
      }
      else
      {
      if (sizeof(T) < sizeof(V))
      {
      throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot pass the data by reference"));
      }

      matnp = (V*)x;
      strides[0] = static_cast<npy_intp>(sizeof(T));
      }

      array = PyArray_NewFromDescr(&PyArray_Type, pyGetDescr(static_cast<U>(*x)), 1, dim, stridesAddr, reinterpret_cast<void *>(matnp), NPY_OWNDATA | NPY_FARRAY, 0);
      if (helper.getUseCopy() && !isRef)
      {
      NumpyDeallocator::attach_deallocator(array, reinterpret_cast<void *>(matnp));
      }

      return scope.addObject(array);
      }
      else
      {
      PyObject * list = PyList_New(xSize);
      for (int i = 0; i < xSize; i++)
      {
      PyList_SetItem(list, i, pyWrap(static_cast<U>(x[i])));
      }

      return scope.addObject(list);
      }
      }

      template <typename T, typename U, typename V>
      inline int wrapData(T * x, int xSize, int xSizeCol, const bool isRef) const
      {
      if (helper.getUseNumpy())
      {
      PyObject * array;
      V * matnp = 0;
      npy_intp dim[] = { xSize, xSizeCol };
      npy_intp strides[] = { 0, 0 };
      npy_intp * stridesAddr = strides;

      if (helper.getUseCopy() && !isRef)
      {
      matnp = new V[xSize * xSizeCol];
      for (int i = 0; i < xSize * xSizeCol; i++)
      {
      matnp[i] = static_cast<V>(x[i]);
      }
      stridesAddr = 0;
      }
      else
      {
      if (sizeof(T) < sizeof(V))
      {
      throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot pass the data by reference"));
      }

      matnp = (V*)x;
      strides[0] = static_cast<npy_intp>(sizeof(T));
      strides[1] = static_cast<npy_intp>(sizeof(T) * xSize);
      }

      array = PyArray_NewFromDescr(&PyArray_Type, pyGetDescr(static_cast<U>(*x)), 2, dim, stridesAddr, reinterpret_cast<void *>(matnp), NPY_OWNDATA | NPY_FARRAY, 0);
      if (helper.getUseCopy() && !isRef)
      {
      NumpyDeallocator::attach_deallocator(array, reinterpret_cast<void *>(matnp));
      }

      return scope.addObject(array);
      }
      else
      {
      PyObject * list = PyList_New(xSize);
      for (int i = 0; i < xSize; i++)
      {
      PyObject * sublist = PyList_New(xSizeCol);
      PyList_SetItem(list, i, sublist);
      for (int j = 0; j < xSizeCol; j++)
      {
      PyList_SetItem(sublist, j, pyWrap(static_cast<U>(x[i + j * xSize])));
      }
      }

      return scope.addObject(list);
      }
      }

      inline int wrapData(char ** x, int xSize, const bool isRef) const
      {
      if (helper.getUseNumpy())
      {
      PyObject * array;
      npy_intp dim[] = { xSize };
      npy_intp strides[] = { 0 };
      char * matnp = 0;
      int maxlen = 0;

      if (helper.getUseCopy() && !isRef)
      {
      int * lens = new int[xSize];
      for (int i = 0; i < xSize; i++)
      {
      lens[i] = strlen(x[i]);
      if (lens[i] > maxlen)
      {
      maxlen = lens[i];
      }
      }
      maxlen++;

      matnp = new char[xSize * maxlen];
      memset(matnp, 0, xSize * maxlen);
      for (int i = 0; i < xSize; i++)
      {
      memcpy(matnp + i * maxlen, x[i], lens[i]);
      }
      delete[] lens;

      strides[0] = static_cast<npy_intp>(maxlen);
      }
      else
      {
      throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot pass Scilab strings by reference"));
      }

      PyArray_Descr * descr = pyGetDescr(*x);
      descr->elsize = maxlen - 1;
      array = PyArray_NewFromDescr(&PyArray_Type, descr, 1, dim, strides, reinterpret_cast<void *>(matnp), NPY_OWNDATA | NPY_FARRAY, 0);
      NumpyDeallocator::attach_deallocator(array, reinterpret_cast<void *>(matnp));

      return scope.addObject(array);
      }
      else
      {
      PyObject * list = PyList_New(xSize);
      for (int i = 0; i < xSize; i++)
      {
      PyList_SetItem(list, i, pyWrap(x[i]));
      }

      return scope.addObject(list);
      }
      }

      inline int wrapData(char ** x, int xSize, int xSizeCol, const bool isRef) const
      {
      if (helper.getUseNumpy())
      {
      PyObject * array;
      char * matnp = 0;
      npy_intp dim[] = { xSize, xSizeCol };
      npy_intp strides[] = { 0, 0 };
      int maxlen = 0;

      if (helper.getUseCopy() && !isRef)
      {
      int * lens = new int[xSize * xSizeCol];
      for (int i = 0; i < xSize * xSizeCol; i++)
      {
      lens[i] = strlen(x[i]);
      if (lens[i] > maxlen)
      {
      maxlen = lens[i];
      }
      }

      matnp = new char[xSize * xSizeCol * maxlen];
      memset(matnp, 0, xSize * xSizeCol * maxlen);
      for (int i = 0; i < xSize * xSizeCol; i++)
      {
      memcpy(matnp + i * maxlen, x[i], lens[i]);
      }
      delete[] lens;

      strides[0] = static_cast<npy_intp>(maxlen);
      strides[1] = static_cast<npy_intp>(xSize * maxlen);
      }
      else
      {
      throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot pass Scilab strings by reference"));
      }


      PyArray_Descr * descr = pyGetDescr(*x);
      descr->elsize = maxlen;

      array = PyArray_NewFromDescr(&PyArray_Type, pyGetDescr(*x), 2, dim, strides, reinterpret_cast<void *>(matnp), NPY_OWNDATA | NPY_FARRAY, 0);
      NumpyDeallocator::attach_deallocator(array, reinterpret_cast<void *>(matnp));

      return scope.addObject(array);
      }
      else
      {
      PyObject * list = PyList_New(xSize);
      for (int i = 0; i < xSize; i++)
      {
      PyObject * sublist = PyList_New(xSizeCol);
      PyList_SetItem(list, i, sublist);
      for (int j = 0; j < xSizeCol; j++)
      {
      PyList_SetItem(sublist, j, pyWrap(x[i + j * xSize]));
      }
      }

      return scope.addObject(list);
      }
      }

      inline int wrapData(double * re, double * im, const bool isRef) const
      {
      PyObject * obj = pyWrap(*re, *im);
      return scope.addObject(obj);
      }

      inline int wrapData(double * re, double * im, int xSize, const bool isRef) const
      {
      if (helper.getUseNumpy())
      {
      PyObject * array;
      npy_intp dim[] = { xSize };
      double * matnp = 0;

      if (helper.getUseCopy() && !isRef)
      {
      matnp = new double[2 * xSize];
      for (int i = 0; i < xSize; i++)
      {
      matnp[2 * i] = re[i];
      matnp[2 * i  + 1] = im[i];
      }
      }
      else
      {
      throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot pass Scilab Complex by reference"));
      }

      array = PyArray_NewFromDescr(&PyArray_Type, PyArray_DescrFromType(PyArray_CDOUBLE), 1, dim, 0, reinterpret_cast<void *>(matnp), NPY_OWNDATA | NPY_FARRAY, 0);
      NumpyDeallocator::attach_deallocator(array, reinterpret_cast<void *>(matnp));

      return scope.addObject(array);
      }
      else
      {
      PyObject * list = PyList_New(xSize);
      for (int i = 0; i < xSize; i++)
      {
      PyList_SetItem(list, i, pyWrap(re[i], im[i]));
      }

      return scope.addObject(list);
      }
      }

      inline int wrapData(double * re, double * im, int xSize, int xSizeCol, const bool isRef) const
      {
      if (helper.getUseNumpy())
      {
      PyObject * array;
      double * matnp = 0;
      npy_intp dim[] = { xSize, xSizeCol };

      if (helper.getUseCopy() && !isRef)
      {
      matnp = new double[2 * xSize * xSizeCol];
      for (int i = 0; i < xSize * xSizeCol; i++)
      {
      matnp[2 * i] = re[i];
      matnp[2 * i  + 1] = im[i];
      }
      }
      else
      {
      throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot pass Scilab Complex by reference"));
      }


      array = PyArray_NewFromDescr(&PyArray_Type, PyArray_DescrFromType(PyArray_CDOUBLE), 2, dim, 0, reinterpret_cast<void *>(matnp), NPY_OWNDATA | NPY_FARRAY, 0);
      NumpyDeallocator::attach_deallocator(array, reinterpret_cast<void *>(matnp));

      return scope.addObject(array);
      }
      else
      {
      PyObject * list = PyList_New(xSize);
      for (int i = 0; i < xSize; i++)
      {
      PyObject * sublist = PyList_New(xSizeCol);
      PyList_SetItem(list, i, sublist);
      for (int j = 0; j < xSizeCol; j++)
      {
      PyList_SetItem(sublist, j, pyWrap(re[i + j * xSize], im[i + j * xSize]));
      }
      }

      return scope.addObject(list);
      }
      }
    */
};
}

#endif // __SCILABJAVAENVIRONMENTWRAPPER_HXX__
