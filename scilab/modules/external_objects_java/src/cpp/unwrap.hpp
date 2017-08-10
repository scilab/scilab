/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
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

/*
 * Template to handle several unwrapping operations
 */

#include <jni.h>
#include "api_scilab.h"
#include "GiwsException.hxx"
#include "NoMoreScilabMemoryException.hxx"
#include "JIMSwrapunwrap.h"

#define SCILABJAVAOBJECT "org/scilab/modules/external_objects_java/ScilabJavaObject"

template <typename T, typename U, class V>
void unwrapMat(JavaVM * jvm_, const bool methodOfConv, const int javaID, const int pos)
{
    SciErr err;
    jint lenRow, lenCol;
    jboolean isCopy = JNI_FALSE;
    jarray oneDim;
    JNIEnv * curEnv = NULL;
    U *addr = NULL;

    jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
    jclass cls = curEnv->FindClass(SCILABJAVAOBJECT);

    jmethodID id = curEnv->GetStaticMethodID(cls, V::getMatMethodName(), V::getMatMethodSignature()) ;
    if (id == NULL)
    {
        throw GiwsException::JniMethodNotFoundException(curEnv, V::getMatMethodName());
    }

    jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(cls, id, javaID));
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }

    lenRow = curEnv->GetArrayLength(res);
    oneDim = reinterpret_cast<jarray>(curEnv->GetObjectArrayElement(res, 0));
    lenCol = curEnv->GetArrayLength(oneDim);
    curEnv->DeleteLocalRef(oneDim);

    if (getMethodOfConv())
    {
        err = V::allocMatrix(pvApiCtx, pos, lenRow, lenCol, (void**) &addr);
    }
    else
    {
        err = V::allocMatrix(pvApiCtx, pos, lenCol, lenRow, (void**) &addr);
    }

    if (err.iErr)
    {
        curEnv->DeleteLocalRef(res);
        throw org_scilab_modules_external_objects_java::NoMoreScilabMemoryException();
    }

    T *resultsArray;
    for (int i = 0; i < lenRow; i++)
    {
        oneDim = reinterpret_cast<jarray>(curEnv->GetObjectArrayElement(res, i));
        resultsArray = static_cast<T *>(curEnv->GetPrimitiveArrayCritical(oneDim, &isCopy));
        if (getMethodOfConv())
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

template <typename T, typename U, class V>
void unwrapRow(JavaVM * jvm_, const bool methodOfConv, const int javaID, const int pos)
{
    SciErr err;
    jint lenRow;
    jboolean isCopy = JNI_FALSE;
    JNIEnv * curEnv = NULL;
    U *addr = NULL;

    jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
    jclass cls = curEnv->FindClass(SCILABJAVAOBJECT);

    jmethodID id = curEnv->GetStaticMethodID(cls, V::getRowMethodName(), V::getRowMethodSignature());
    if (id == NULL)
    {
        throw GiwsException::JniMethodNotFoundException(curEnv, V::getRowMethodName());
    }

    jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(cls, id, javaID));
    if (curEnv->ExceptionCheck())
    {
        curEnv->DeleteLocalRef(res);
        throw GiwsException::JniCallMethodException(curEnv);
    }

    lenRow = curEnv->GetArrayLength(res);
    err = V::allocMatrix(pvApiCtx, pos, 1, lenRow, (void**) &addr);

    if (err.iErr)
    {
        curEnv->DeleteLocalRef(res);
        throw org_scilab_modules_external_objects_java::NoMoreScilabMemoryException();
    }

    T *resultsArray = static_cast<T *>(curEnv->GetPrimitiveArrayCritical(res, &isCopy));
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
void unwrapSingle(JavaVM * jvm_, const bool methodOfConv, const int javaID, const int pos)
{
    SciErr err;
    JNIEnv * curEnv = NULL;
    U *addr = NULL;

    jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
    jclass cls = curEnv->FindClass(SCILABJAVAOBJECT);

    jmethodID id = curEnv->GetStaticMethodID(cls, V::getMethodName(), V::getMethodSignature()) ;
    if (id == NULL)
    {
        throw GiwsException::JniMethodNotFoundException(curEnv, V::getMethodName());
    }

    err = V::allocMatrix(pvApiCtx, pos, 1, 1, (void**) &addr);

    if (err.iErr)
    {
        throw org_scilab_modules_external_objects_java::NoMoreScilabMemoryException();
    }

    *addr = static_cast<U>(V::getSingleVar(curEnv, cls, id, javaID));
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }
}
