/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
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

#ifdef _MSC_VER
#include "windows.h"
#endif

#include "ScilabJavaEnvironmentWrapper.hxx"

extern "C" {
#include "getScilabJavaVM.h"
}

namespace org_scilab_modules_external_objects_java
{

VariableType ScilabJavaEnvironmentWrapper::isunwrappable(int id) const
{
    JavaVM * vm = getScilabJavaVM();
    int type = ScilabJavaObject::isUnwrappable(vm, id);

    return (VariableType)type;
}

int ScilabJavaEnvironmentWrapper::wrap(double * x, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, 1);
    }

    return wrap(vm, x);
}

int ScilabJavaEnvironmentWrapper::wrap(double * x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize);
    }

    return wrap(vm, x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrap(double * x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize * xSizeCol);
    }

    return wrap(vm, x, xSize, xSizeCol);
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
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, 1);
    }

    return wrap(vm, (byte *)x);
}

int ScilabJavaEnvironmentWrapper::wrap(char * x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize);
    }

    return wrap(vm, (byte *)x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrap(char * x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize * xSizeCol);
    }

    return wrap(vm, (byte *)x, xSize, xSizeCol);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned char * x, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (char *)x, 1);
    }

    return wrap(vm, (byte *)x);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned char * x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (char *)x, xSize);
    }

    return wrap(vm, (byte *)x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned char * x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (char *)x, xSize * xSizeCol);
    }

    return wrap(vm, (byte *)x, xSize, xSizeCol);
}

int ScilabJavaEnvironmentWrapper::wrap(short * x, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, 1);
    }

    return wrap(vm, x);
}

int ScilabJavaEnvironmentWrapper::wrap(short * x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize);
    }

    return wrap(vm, x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrap(short * x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize * xSizeCol);
    }

    return wrap(vm, x, xSize, xSizeCol);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned short * x, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (short *)x, 1);
    }

    return wrap(vm, (short *)x);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned short * x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (short *)x, xSize);
    }

    return wrap(vm, (short *)x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned short * x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (short *)x, xSize * xSizeCol);
    }

    return wrap(vm, (short *)x, xSize, xSizeCol);
}

int ScilabJavaEnvironmentWrapper::wrap(int * x, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, 1);
    }

    return wrap(vm, x);
}

int ScilabJavaEnvironmentWrapper::wrap(int * x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize);
    }

    return wrap(vm, x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrap(int * x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize * xSizeCol);
    }

    return wrap(vm, x, xSize, xSizeCol);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned int * x, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (int *)x, 1);
    }

    return wrap(vm, (int *)x);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned int * x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (int *)x, xSize);
    }

    return wrap(vm, (int *)x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned int * x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (int *)x, xSize * xSizeCol);
    }

    return wrap(vm, (int *)x, xSize, xSizeCol);
}

#ifdef __SCILAB_INT64__

int ScilabJavaEnvironmentWrapper::wrap(long long * x, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, 1);
    }

    return wrap(vm, x);
}

int ScilabJavaEnvironmentWrapper::wrap(long long * x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize);
    }

    return wrap(vm, x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrap(long long * x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize * xSizeCol);
    }

    return wrap(vm, x, xSize, xSizeCol);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned long long * x, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (long long *)x, 1);
    }

    return wrap(vm, (long long *)x);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned long long * x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (long long *)x, xSize);
    }

    return wrap(vm, (long long *)x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrap(unsigned long long * x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, (long long *)x, xSize * xSizeCol);
    }

    return wrap(vm, (long long *)x, xSize, xSizeCol);
}

#endif

int ScilabJavaEnvironmentWrapper::wrap(char ** x, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    return wrap(vm, (char const**)x);
}

int ScilabJavaEnvironmentWrapper::wrap(char ** x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    return wrap(vm, (char const**)x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrap(char ** x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    return wrap(vm, (char const**)x, xSize, xSizeCol);
}

int ScilabJavaEnvironmentWrapper::wrapBool(int * x, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, 1);
    }

    return wrap<int, bool>(vm, x);
}

int ScilabJavaEnvironmentWrapper::wrapBool(int * x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize);
    }

    return wrap<int, bool>(vm, x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrapBool(int * x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    if (isRef)
    {
        return wrapAsDirectBuffer(vm, x, xSize * xSizeCol);
    }

    return wrap<int, bool>(vm, x, xSize, xSizeCol);
}

int ScilabJavaEnvironmentWrapper::wrapList(int len, const int * const ids) const
{
    JavaVM * vm = getScilabJavaVM();
    return ScilabJavaObject::wrapList(vm, ids, len);
}

int ScilabJavaEnvironmentWrapper::wrapPoly(int len, const double * const coefs) const
{
    JavaVM * vm = getScilabJavaVM();
    return ScilabJavaObject::wrapPoly(vm, coefs, len);
}

int ScilabJavaEnvironmentWrapper::wrapFloat(double * x, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    return wrap<double, float>(vm, x);
}

int ScilabJavaEnvironmentWrapper::wrapFloat(double * x, int xSize, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    return wrap<double, float>(vm, x, xSize);
}

int ScilabJavaEnvironmentWrapper::wrapFloat(double * x, int xSize, int xSizeCol, const bool isRef) const
{
    JavaVM * vm = getScilabJavaVM();
    return wrap<double, float>(vm, x, xSize, xSizeCol);
}

void ScilabJavaEnvironmentWrapper::unwrapdouble(int id, const ScilabDoubleStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapSingle<jdouble, double>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowdouble(int id, const ScilabDoubleStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapRow<jdouble, double>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatdouble(int id, const ScilabDoubleStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapMat<jdouble, double>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapchar(int id, const ScilabCharStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapSingle<jbyte, char>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowchar(int id, const ScilabCharStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapRow<jbyte, char>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatchar(int id, const ScilabCharStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapMat<jbyte, char>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapuchar(int id, const ScilabUCharStackAllocator & allocator) const
{
    // Not called
}

void ScilabJavaEnvironmentWrapper::unwraprowuchar(int id, const ScilabUCharStackAllocator & allocator) const
{
    // Not called
}

void ScilabJavaEnvironmentWrapper::unwrapmatuchar(int id, const ScilabUCharStackAllocator & allocator) const
{
    // Not called
}

void ScilabJavaEnvironmentWrapper::unwrapshort(int id, const ScilabShortStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapSingle<jshort, short>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowshort(int id, const ScilabShortStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapRow<jshort, short>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatshort(int id, const ScilabShortStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapMat<jshort, short>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapushort(int id, const ScilabUShortStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapSingle<jchar, unsigned short>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowushort(int id, const ScilabUShortStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapRow<jchar, unsigned short>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatushort(int id, const ScilabUShortStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapMat<jchar, unsigned short>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapint(int id, const ScilabIntStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapSingle<jint, int>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwraprowint(int id, const ScilabIntStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapRow<jint, int>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatint(int id, const ScilabIntStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapMat<jint, int>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapuint(int id, const ScilabUIntStackAllocator & allocator) const
{
    // Not called
}

void ScilabJavaEnvironmentWrapper::unwraprowuint(int id, const ScilabUIntStackAllocator & allocator) const
{
    // Not called
}

void ScilabJavaEnvironmentWrapper::unwrapmatuint(int id, const ScilabUIntStackAllocator & allocator) const
{
    // Not called
}

void ScilabJavaEnvironmentWrapper::unwraplong(int id, const ScilabLongStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapSingle<jlong, long long, long long>(vm, id, allocator, true);
}

void ScilabJavaEnvironmentWrapper::unwraprowlong(int id, const ScilabLongStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapRow<jlong, long long>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapmatlong(int id, const ScilabLongStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapMat<jlong, long long>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapulong(int id, const ScilabULongStackAllocator & allocator) const
{
    // Not called
}

void ScilabJavaEnvironmentWrapper::unwraprowulong(int id, const ScilabULongStackAllocator & allocator) const
{
    // Not called
}

void ScilabJavaEnvironmentWrapper::unwrapmatulong(int id, const ScilabULongStackAllocator & allocator) const
{
    // Not called
}

void ScilabJavaEnvironmentWrapper::unwrapboolean(int id, const ScilabBooleanStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapSingle<jboolean, int, bool>(vm, id, allocator, true);
}

void ScilabJavaEnvironmentWrapper::unwraprowboolean(int id, const ScilabBooleanStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapRow<jboolean, int, bool>(vm, id, allocator, true);
}

void ScilabJavaEnvironmentWrapper::unwrapmatboolean(int id, const ScilabBooleanStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapMat<jboolean, int, bool>(vm, id, allocator);
}

void ScilabJavaEnvironmentWrapper::unwrapstring(int id, const ScilabStringStackAllocator & allocator) const
{
    JNIEnv * curEnv = NULL;
    JavaVM * vm = getScilabJavaVM();
    vm->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);

    jstring res = static_cast<jstring>(curEnv->CallStaticObjectMethod(ScilabJavaObjectClass_, unwrapStringID_ , id));
    char * addr = const_cast<char *>(curEnv->GetStringUTFChars(res, 0));

    allocator.allocate(1, 1, &addr);

    curEnv->ReleaseStringUTFChars(res, addr);
    curEnv->DeleteLocalRef(res);
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }
}

void ScilabJavaEnvironmentWrapper::unwraprowstring(int id, const ScilabStringStackAllocator & allocator) const
{
    JNIEnv * curEnv = NULL;
    JavaVM * vm = getScilabJavaVM();
    vm->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);

    jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(ScilabJavaObjectClass_, unwrapRowStringID_ , id));
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }

    jint lenRow = curEnv->GetArrayLength(res);
    jboolean isCopy = JNI_FALSE;
    char ** addr = new char*[lenRow];
    jstring * resString = new jstring[lenRow];
    char empty = '\0';

    for (jsize i = 0; i < lenRow; i++)
    {
        resString[i] = reinterpret_cast<jstring>(curEnv->GetObjectArrayElement(res, i));
        if (resString[i])
        {
            addr[i] = const_cast<char *>(curEnv->GetStringUTFChars(resString[i], &isCopy));
        }
        else
        {
            addr[i] = &empty;
        }
    }

    allocator.allocate(1, lenRow, addr);

    for (jsize i = 0; i < lenRow; i++)
    {
        if (resString[i])
        {
            curEnv->ReleaseStringUTFChars(resString[i], addr[i]);
            curEnv->DeleteLocalRef(resString[i]);
        }
    }
    delete[] addr;
    delete[] resString;

    curEnv->DeleteLocalRef(res);
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }
}

void ScilabJavaEnvironmentWrapper::unwrapmatstring(int id, const ScilabStringStackAllocator & allocator) const
{
    JNIEnv * curEnv = NULL;
    JavaVM * vm = getScilabJavaVM();
    vm->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);

    jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(ScilabJavaObjectClass_, unwrapMatStringID_, id));
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }
    jint lenRow = curEnv->GetArrayLength(res);
    jboolean isCopy = JNI_FALSE;

    jobjectArray oneDim = reinterpret_cast<jobjectArray>(curEnv->GetObjectArrayElement(res, 0));
    jint lenCol = curEnv->GetArrayLength(oneDim);
    char ** addr = new char*[lenRow * lenCol];
    jstring * resString = new jstring[lenRow * lenCol];
    char empty = '\0';

    for (int i = 0; i < lenRow; i++)
    {
        oneDim = reinterpret_cast<jobjectArray>(curEnv->GetObjectArrayElement(res, i));
        if (helper.getMethodOfConv())
        {
            for (int j = 0; j < lenCol; j++)
            {
                const unsigned int pos = j * lenRow + i;
                resString[pos] = reinterpret_cast<jstring>(curEnv->GetObjectArrayElement(oneDim, j));
                if (resString[pos])
                {
                    addr[pos] = const_cast<char *>(curEnv->GetStringUTFChars(resString[pos], &isCopy));
                }
                else
                {
                    addr[pos] = &empty;
                }
            }
        }
        else
        {
            for (int j = 0; j < lenCol; j++)
            {
                const unsigned int pos = i * lenCol + j;
                resString[pos] = reinterpret_cast<jstring>(curEnv->GetObjectArrayElement(oneDim, j));
                if (resString[pos])
                {
                    addr[pos] = const_cast<char *>(curEnv->GetStringUTFChars(resString[pos], &isCopy));
                }
                else
                {
                    addr[pos] = &empty;
                }
            }
        }
        curEnv->DeleteLocalRef(oneDim);
    }

    if (helper.getMethodOfConv())
    {
        allocator.allocate(lenRow, lenCol, addr);
    }
    else
    {
        allocator.allocate(lenCol, lenRow, addr);
    }

    for (int i = 0; i < lenRow * lenCol; i++)
    {
        if (resString[i])
        {
            curEnv->ReleaseStringUTFChars(resString[i], addr[i]);
            curEnv->DeleteLocalRef(resString[i]);
        }
    }
    delete[] addr;
    delete[] resString;

    curEnv->DeleteLocalRef(res);
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }
}

void ScilabJavaEnvironmentWrapper::unwrapcomplex(int id, const ScilabComplexStackAllocator & allocator) const
{
    // never called
}

void ScilabJavaEnvironmentWrapper::unwraprowcomplex(int id, const ScilabComplexStackAllocator & allocator) const
{
    // never called
}

void ScilabJavaEnvironmentWrapper::unwrapmatcomplex(int id, const ScilabComplexStackAllocator & allocator) const
{
    // never called
}

void ScilabJavaEnvironmentWrapper::unwrapfloat(int id, const ScilabFloatStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapSingle<jfloat, double, float>(vm, id, allocator, true);
}

void ScilabJavaEnvironmentWrapper::unwraprowfloat(int id, const ScilabFloatStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapRow<jfloat, double, float>(vm, id, allocator, true);
}

void ScilabJavaEnvironmentWrapper::unwrapmatfloat(int id, const ScilabFloatStackAllocator & allocator) const
{
    JavaVM * vm = getScilabJavaVM();
    unwrapMat<jfloat, double, float>(vm, id, allocator);
}
}
