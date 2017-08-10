/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __SCILABPYTHONENVIRONMENTWRAPPER_HXX__
#define __SCILABPYTHONENVIRONMENTWRAPPER_HXX__

#include "ExternalObjects.h"
#include "JavaOptionsHelper.hxx"

#include "ScilabAbstractEnvironmentWrapper.hxx"
#include "ScilabJavaException.hxx"

#include "ScilabJavaObject.hxx"

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

    jmethodID unwrapStringID_;
    jmethodID unwrapRowStringID_;
    jmethodID unwrapMatStringID_;

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

    jmethodID unwrapFloatID_;
    jmethodID unwrapRowFloatID_;
    jmethodID unwrapMatFloatID_;

    jmethodID unwrapCharID_;
    jmethodID unwrapRowCharID_;
    jmethodID unwrapMatCharID_;

    jmethodID wrapAsDirectByteBufferID_;
    jmethodID wrapAsDirectDoubleBufferID_;
    jmethodID wrapAsDirectIntBufferID_;
    jmethodID wrapAsDirectCharBufferID_;
    jmethodID wrapAsDirectFloatBufferID_;
    jmethodID wrapAsDirectLongBufferID_;
    jmethodID wrapAsDirectShortBufferID_;

public:

    ScilabJavaEnvironmentWrapper(JavaOptionsHelper & _helper) : helper(_helper)
    {
        JavaVM * vm = getScilabJavaVM();
        JNIEnv * curEnv = NULL;

        vm->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
        ScilabJavaObjectClass_ = curEnv->FindClass(SCILABJAVAOBJECT);

        unwrapStringID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapString", "(I)Ljava/lang/String;");
        unwrapRowStringID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowString", "(I)[Ljava/lang/String;");
        unwrapMatStringID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatString", "(I)[[Ljava/lang/String;");

        unwrapBooleanID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapBoolean", "(I)Z");
        unwrapRowBooleanID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowBoolean", "(I)[Z");
        unwrapMatBooleanID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatBoolean", "(I)[[Z");

        unwrapByteID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapByte", "(I)B");
        unwrapRowByteID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowByte", "(I)Ljava/lang/Object;");
        unwrapMatByteID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatByte", "(I)[[B");

        unwrapShortID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapShort", "(I)S");
        unwrapRowShortID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowShort", "(I)Ljava/lang/Object;");
        unwrapMatShortID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatShort", "(I)[[S");

        unwrapIntID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapInt", "(I)I");
        unwrapRowIntID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowInt", "(I)Ljava/lang/Object;");
        unwrapMatIntID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatInt", "(I)[[I");

        unwrapLongID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapLong", "(I)J");
        unwrapRowLongID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowLong", "(I)Ljava/lang/Object;");
        unwrapMatLongID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatLong", "(I)[[J");

        unwrapDoubleID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapDouble", "(I)D");
        unwrapRowDoubleID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowDouble", "(I)Ljava/lang/Object;");
        unwrapMatDoubleID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatDouble", "(I)[[D");

        unwrapFloatID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapFloat", "(I)F");
        unwrapRowFloatID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowFloat", "(I)Ljava/lang/Object;");
        unwrapMatFloatID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatFloat", "(I)[[F");

        unwrapCharID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapChar", "(I)C");
        unwrapRowCharID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapRowChar", "(I)Ljava/lang/Object;");
        unwrapMatCharID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "unwrapMatChar", "(I)[[C");

        wrapAsDirectByteBufferID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "wrapAsDirectByteBuffer", "(Ljava/nio/ByteBuffer;)I");
        wrapAsDirectDoubleBufferID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "wrapAsDirectDoubleBuffer", "(Ljava/nio/ByteBuffer;)I");
        wrapAsDirectIntBufferID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "wrapAsDirectIntBuffer", "(Ljava/nio/ByteBuffer;)I");
        wrapAsDirectCharBufferID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "wrapAsDirectCharBuffer", "(Ljava/nio/ByteBuffer;)I");
        wrapAsDirectFloatBufferID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "wrapAsDirectFloatBuffer", "(Ljava/nio/ByteBuffer;)I");
        wrapAsDirectLongBufferID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "wrapAsDirectLongBuffer", "(Ljava/nio/ByteBuffer;)I");
        wrapAsDirectShortBufferID_ = curEnv->GetStaticMethodID(ScilabJavaObjectClass_, "wrapAsDirectShortBuffer", "(Ljava/nio/ByteBuffer;)I");
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


    int wrapFloat(double * x, const bool isRef) const;

    int wrapFloat(double * x, int xSize, const bool isRef) const;

    int wrapFloat(double * x, int xSize, int xSizeCol, const bool isRef) const;


    int wrapBool(int * x, const bool isRef) const;

    int wrapBool(int * x, int xSize, const bool isRef) const;

    int wrapBool(int * x, int xSize, int xSizeCol, const bool isRef) const;

    int wrapList(int len, const int* const ids) const;

    int wrapPoly(int len, const double * const coefs) const;

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

    template<typename T>
    inline int wrap(JavaVM * vm, T * x) const
    {
        return ScilabJavaObject::wrap(vm, *x);
    }

    template<typename T, typename U>
    inline int wrap(JavaVM * vm, T * x) const
    {
        return ScilabJavaObject::wrap(vm, (U) * x);
    }

    template<typename T>
    inline int wrap(JavaVM * vm, T * x, const int len) const
    {
        return ScilabJavaObject::wrap(vm, x, len);
    }

    template<typename T, typename U>
    inline int wrap(JavaVM * vm, T * x, const int len) const
    {
        U * xx = new U[len];
        int i;
        for (i = 0; i < len; i++)
        {
            xx[i] = (U)x[i];
        }

        i = ScilabJavaObject::wrap(vm, xx, len);
        delete[] xx;

        return i;
    }

    template<typename T>
    inline int wrap(JavaVM * vm, T * x, const int r, const int c) const
    {
        if (helper.getMethodOfConv())
        {
            T ** xx = new T*[r];
            int i, j;
            for (i = 0; i < r; i++)
            {
                xx[i] = new T[c];
                for (j = 0; j < c; j++)
                {
                    xx[i][j] = x[j * r + i];
                }
            }
            j = ScilabJavaObject::wrap(vm, xx, r, c);
            for (i = 0; i < r; delete[] xx[i++])
            {
                ;
            }
            delete[] xx;
            return j;
        }
        else
        {
            T ** xx = new T*[c];
            int i;
            xx[0] = x;
            for (i = 1; i < c; i++)
            {
                xx[i] = xx[i - 1] + r;
            }
            i = ScilabJavaObject::wrap(vm, xx, c, r);
            delete[] xx;
            return i;
        }
    }

    template<typename T, typename U>
    inline int wrap(JavaVM * vm, T * x, const int r, const int c) const
    {
        if (helper.getMethodOfConv())
        {
            U ** xx = new U*[r];
            int i, j;
            for (i = 0; i < r; i++)
            {
                xx[i] = new U[c];
                for (j = 0; j < c; j++)
                {
                    xx[i][j] = (U)(x[j * r + i]);
                }
            }
            j = ScilabJavaObject::wrap(vm, xx, r, c);
            for (i = 0; i < r; delete[] xx[i++])
            {
                ;
            }
            delete[] xx;
            return j;
        }
        else
        {
            U ** xx = new U*[c];
            int i, j;
            for (i = 0; i < c; i++)
            {
                xx[i] = new U[r];
                for (j = 0; j < r; j++)
                {
                    xx[i][j] = (U)(x[i * r + j]);
                }
            }
            j = ScilabJavaObject::wrap(vm, xx, c, r);
            for (i = 0; i < c; delete[] xx[i++])
            {
                ;
            }
            delete[] xx;
            return j;
        }
    }

    inline const jmethodID getSingleMethod(const char * x) const
    {
        return unwrapByteID_;
    }
    inline const jmethodID getSingleMethod(const short * x) const
    {
        return unwrapShortID_;
    }
    inline const jmethodID getSingleMethod(const unsigned short * x) const
    {
        return unwrapCharID_;
    }
    inline const jmethodID getSingleMethod(const int * x) const
    {
        return unwrapIntID_;
    }
    inline const jmethodID getSingleMethod(const long long * x) const
    {
        return unwrapLongID_;
    }
    inline const jmethodID getSingleMethod(const double * x) const
    {
        return unwrapDoubleID_;
    }
    inline const jmethodID getSingleMethod(const bool * x) const
    {
        return unwrapBooleanID_;
    }
    inline const jmethodID getSingleMethod(const float * x) const
    {
        return unwrapFloatID_;
    }

    inline const jmethodID getRowMethod(const char * x) const
    {
        return unwrapRowByteID_;
    }
    inline const jmethodID getRowMethod(const short * x) const
    {
        return unwrapRowShortID_;
    }
    inline const jmethodID getRowMethod(const unsigned short * x) const
    {
        return unwrapRowCharID_;
    }
    inline const jmethodID getRowMethod(const int * x) const
    {
        return unwrapRowIntID_;
    }
    inline const jmethodID getRowMethod(const long long * x) const
    {
        return unwrapRowLongID_;
    }
    inline const jmethodID getRowMethod(const double * x) const
    {
        return unwrapRowDoubleID_;
    }
    inline const jmethodID getRowMethod(const bool * x) const
    {
        return unwrapRowBooleanID_;
    }
    inline const jmethodID getRowMethod(const float * x) const
    {
        return unwrapRowFloatID_;
    }

    inline const jmethodID getMatMethod(const char * x) const
    {
        return unwrapMatByteID_;
    }
    inline const jmethodID getMatMethod(const short * x) const
    {
        return unwrapMatShortID_;
    }
    inline const jmethodID getMatMethod(const unsigned short * x) const
    {
        return unwrapMatCharID_;
    }
    inline const jmethodID getMatMethod(const int * x) const
    {
        return unwrapMatIntID_;
    }
    inline const jmethodID getMatMethod(const long long * x) const
    {
        return unwrapMatLongID_;
    }
    inline const jmethodID getMatMethod(const double * x) const
    {
        return unwrapMatDoubleID_;
    }
    inline const jmethodID getMatMethod(const bool * x) const
    {
        return unwrapMatBooleanID_;
    }
    inline const jmethodID getMatMethod(const float * x) const
    {
        return unwrapMatFloatID_;
    }

    inline const jmethodID getDBMethod(const char * x) const
    {
        return wrapAsDirectByteBufferID_;
    }
    inline const jmethodID getDBMethod(const double * x) const
    {
        return wrapAsDirectDoubleBufferID_;
    }
    inline const jmethodID getDBMethod(const int * x) const
    {
        return wrapAsDirectIntBufferID_;
    }
    inline const jmethodID getDBMethod(const float * x) const
    {
        return wrapAsDirectFloatBufferID_;
    }
    inline const jmethodID getDBMethod(const long long * x) const
    {
        return wrapAsDirectLongBufferID_;
    }
    inline const jmethodID getDBMethod(const short * x) const
    {
        return wrapAsDirectShortBufferID_;
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const char * x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticByteMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const short * x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticShortMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const unsigned short * x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticShortMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const int * x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticIntMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const long long * x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticLongMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const double * x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticDoubleMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const bool * x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticBooleanMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T>
    inline T CallStatic(JNIEnv * env_, const float * x, int javaID) const
    {
        return static_cast<T>(env_->CallStaticFloatMethod(ScilabJavaObjectClass_, getSingleMethod(x), javaID));
    }

    template <typename T, typename U, class V>
    inline void unwrapSingle(JavaVM * jvm_, const int javaID, const V & allocator) const
    {
        unwrapSingle<T, U, U>(jvm_, javaID, allocator);
    }

    template <typename T, typename U, typename V, class W>
    inline void unwrapSingle(JavaVM * jvm_, const int javaID, const W & allocator, const bool mustAlloc = false) const
    {
        JNIEnv * curEnv = NULL;
        U* addr = 0;


        jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);

        T res = CallStatic<T>(curEnv, (V *)addr, javaID);
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }

        if (mustAlloc)
        {
            addr = reinterpret_cast<U *>(allocator.allocate(1, 1, 0));
            *addr = static_cast<U>(res);
        }
        else
        {
            allocator.allocate(1, 1, reinterpret_cast<V *>(&res));
        }
    }

    template <typename T, typename U, class V>
    inline void unwrapRow(JavaVM * jvm_, const int javaID, const V & allocator) const
    {
        unwrapRow<T, U, U>(jvm_, javaID, allocator);
    }

    template <typename T, typename U, typename V, class W>
    inline void unwrapRow(JavaVM * jvm_, const int javaID, const W & allocator, const bool mustAlloc = false) const
    {
        jint lenRow;
        jboolean isCopy = JNI_FALSE;
        JNIEnv * curEnv = NULL;
        U* addr = 0;
        jobject res = 0;
        jobjectArray array = 0;
        void * buffer = 0;

        jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);

        res = curEnv->CallStaticObjectMethod(ScilabJavaObjectClass_, getRowMethod((V *)addr), javaID);
        if (curEnv->ExceptionCheck())
        {
            curEnv->DeleteLocalRef(res);
            throw GiwsException::JniCallMethodException(curEnv);
        }

        buffer = curEnv->GetDirectBufferAddress(res);
        if (buffer)
        {
            const jlong len = curEnv->GetDirectBufferCapacity(res);
            V * resultsArray = (V *)buffer;
            if (mustAlloc)
            {
                try
                {
                    addr = reinterpret_cast<U* >(allocator.allocate(1, len, 0));
                    for (jlong i = 0; i < len; i++)
                    {
                        addr[i] = static_cast<U>(resultsArray[i]);
                    }
                }
                catch (const ScilabAbstractEnvironmentException & e)
                {
                    curEnv->DeleteLocalRef(res);
                    throw;
                }
            }
            else
            {
                try
                {
                    allocator.allocate(1, len, reinterpret_cast<V *>(resultsArray));
                }
                catch (const ScilabAbstractEnvironmentException & e)
                {
                    curEnv->DeleteLocalRef(res);
                    throw;
                }
            }
        }
        else
        {
            array = static_cast<jobjectArray>(res);
            lenRow = curEnv->GetArrayLength(array);

            if (mustAlloc)
            {
                try
                {
                    addr = reinterpret_cast<U *>(allocator.allocate(1, lenRow, 0));
                }
                catch (const ScilabAbstractEnvironmentException & e)
                {
                    curEnv->DeleteLocalRef(array);
                    throw;
                }
            }

            T * resultsArray = static_cast<T *>(curEnv->GetPrimitiveArrayCritical(array, &isCopy));

            if (mustAlloc)
            {
                for (int i = 0; i < lenRow; i++)
                {
                    addr[i] = static_cast<U>(resultsArray[i]);
                }
            }
            else
            {
                try
                {
                    allocator.allocate(1, lenRow, reinterpret_cast<V *>(resultsArray));
                }
                catch (const ScilabAbstractEnvironmentException & e)
                {
                    curEnv->ReleasePrimitiveArrayCritical(array, resultsArray, JNI_ABORT);
                    curEnv->DeleteLocalRef(array);
                    throw;
                }
            }

            curEnv->ReleasePrimitiveArrayCritical(array, resultsArray, JNI_ABORT);
            curEnv->DeleteLocalRef(array);
        }
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
        U* addr = 0;

        jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);

        jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(ScilabJavaObjectClass_, getMatMethod((V *)addr), javaID));
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
                addr = (U *)allocator.allocate(lenRow, lenCol, 0);
            }
            else
            {
                addr = (U *)allocator.allocate(lenCol, lenRow, 0);
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

    template<typename T>
    inline int wrapAsDirectBuffer(JavaVM * jvm_, T * data, const int size) const
    {
        JNIEnv * curEnv = NULL;
        jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);

        jobject dbuffer = curEnv->NewDirectByteBuffer((void*)data, (jlong)(size * sizeof(T)));
        int id = static_cast<jint>(curEnv->CallStaticIntMethod(ScilabJavaObjectClass_, getDBMethod(data), dbuffer));

        curEnv->DeleteLocalRef(dbuffer);
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }

        return id;
    }
};
}

#endif // __SCILABJAVAENVIRONMENTWRAPPER_HXX__
