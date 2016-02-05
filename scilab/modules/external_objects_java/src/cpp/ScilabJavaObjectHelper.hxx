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

#include <jni.h>
#include "GiwsException.hxx"
#include "NoMoreScilabMemoryException.hxx"
#include "ScilabObjects.hxx"
#include "OptionsHelper.hxx"

#define SCILABJAVAOBJECT "org/scilab/forge/jims/ScilabJavaObject"

#define __getFunctionNameForDB__(Type,type) static const char* getFunctionNameForDB(type *addr) \
    {                                                                   \
        return "wrapAsDirect" #Type "Buffer";                           \
    }

struct ScilabJavaObjectHelper
{

    __getFunctionNameForDB__(Double, double);
    __getFunctionNameForDB__(Int, int);
    __getFunctionNameForDB__(Char, char);
    __getFunctionNameForDB__(Short, short);
    __getFunctionNameForDB__(Long, long);
    __getFunctionNameForDB__(Byte, byte);
    __getFunctionNameForDB__(Float, float);

    template <typename U> static void* wrapAsDirectBuffer(JavaVM * jvm_, U* address, long size, int *javaID);
    static void releaseDirectBuffer(JavaVM * jvm_, void** ref, int* javaID, int len);

    static void getMethodResult(JavaVM * jvm_, const char * const methodName, int id, int pos)
    {
        JNIEnv * curEnv = NULL;
        jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
        jclass cls = curEnv->FindClass(SCILABJAVAOBJECT);

        jmethodID jobjectArray_getAccessibleMethodsjintID = curEnv->GetStaticMethodID(cls, methodName, "(I)[Ljava/lang/String;");
        if (jobjectArray_getAccessibleMethodsjintID == NULL)
        {
            throw GiwsException::JniMethodNotFoundException(curEnv, methodName);
        }

        jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(cls, jobjectArray_getAccessibleMethodsjintID, id));
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }
        jint lenRow = curEnv->GetArrayLength(res);
        jboolean isCopy = JNI_FALSE;

        char **addr = new char*[lenRow];
        jstring *resString = new jstring[lenRow];

        for (jsize i = 0; i < lenRow; i++)
        {
            resString[i] = reinterpret_cast<jstring>(curEnv->GetObjectArrayElement(res, i));
            addr[i] = const_cast<char *>(curEnv->GetStringUTFChars(resString[i], &isCopy));
        }

        int lenCol = lenRow == 0 ? 0 : 1;
        SciErr err = createMatrixOfString(pvApiCtx, pos, lenCol, lenRow, addr);

        for (jsize i = 0; i < lenRow; i++)
        {
            curEnv->ReleaseStringUTFChars(resString[i], addr[i]);
            curEnv->DeleteLocalRef(resString[i]);
        }
        delete[] addr;
        delete[] resString;

        if (err.iErr)
        {
            throw org_scilab_modules_external_objects_java::NoMoreScilabMemoryException();
        }

        curEnv->DeleteLocalRef(res);
        curEnv->DeleteLocalRef(cls);
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }
    };

    static void unwrapString(JavaVM * jvm_, int id, int pos)
    {
        JNIEnv * curEnv = NULL;
        jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
        jclass cls = curEnv->FindClass(SCILABJAVAOBJECT);

        jmethodID jstringunwrapStringjintID = curEnv->GetStaticMethodID(cls, "unwrapString", "(I)Ljava/lang/String;" ) ;
        if (jstringunwrapStringjintID == NULL)
        {
            throw GiwsException::JniMethodNotFoundException(curEnv, "unwrapString");
        }

        jstring res = static_cast<jstring>(curEnv->CallStaticObjectMethod(cls, jstringunwrapStringjintID , id));
        char *addr = const_cast<char *>(curEnv->GetStringUTFChars(res, 0));
        SciErr err = createMatrixOfString(pvApiCtx, pos, 1, 1, &addr);
        if (err.iErr)
        {
            throw org_scilab_modules_external_objects_java::NoMoreScilabMemoryException();
        }

        curEnv->ReleaseStringUTFChars(res, addr);
        curEnv->DeleteLocalRef(res);
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }
    }

    static void unwrapRowString(JavaVM * jvm_, int id, int pos)
    {
        JNIEnv * curEnv = NULL;
        jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
        jclass cls = curEnv->FindClass(SCILABJAVAOBJECT);

        jmethodID jobjectArray_unwrapRowStringjintID = curEnv->GetStaticMethodID(cls, "unwrapRowString", "(I)[Ljava/lang/String;" );
        if (jobjectArray_unwrapRowStringjintID == NULL)
        {
            throw GiwsException::JniMethodNotFoundException(curEnv, "unwrapRowString");
        }

        jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(cls, jobjectArray_unwrapRowStringjintID , id));
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }

        jint lenRow = curEnv->GetArrayLength(res);
        jboolean isCopy = JNI_FALSE;
        char **addr = new char*[lenRow];
        jstring *resString = new jstring[lenRow];

        for (jsize i = 0; i < lenRow; i++)
        {
            resString[i] = reinterpret_cast<jstring>(curEnv->GetObjectArrayElement(res, i));
            addr[i] = const_cast<char *>(curEnv->GetStringUTFChars(resString[i], &isCopy));
        }

        SciErr err = createMatrixOfString(pvApiCtx, pos, 1, lenRow, addr);

        for (jsize i = 0; i < lenRow; i++)
        {
            curEnv->ReleaseStringUTFChars(resString[i], addr[i]);
            curEnv->DeleteLocalRef(resString[i]);
        }
        delete[] addr;
        delete[] resString;

        if (err.iErr)
        {
            throw org_scilab_modules_external_objects_java::NoMoreScilabMemoryException();
        }

        curEnv->DeleteLocalRef(res);
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }
    }

    static void unwrapMatString(JavaVM * jvm_, int id, int pos)
    {
        JNIEnv * curEnv = NULL;
        jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
        jclass cls = curEnv->FindClass(SCILABJAVAOBJECT);

        jmethodID jobjectArray__unwrapMatStringjintID = curEnv->GetStaticMethodID(cls, "unwrapMatString", "(I)[[Ljava/lang/String;" ) ;
        if (jobjectArray__unwrapMatStringjintID == NULL)
        {
            throw GiwsException::JniMethodNotFoundException(curEnv, "unwrapMatString");
        }

        jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(cls, jobjectArray__unwrapMatStringjintID , id));
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }
        jint lenRow = curEnv->GetArrayLength(res);
        jboolean isCopy = JNI_FALSE;

        jobjectArray oneDim = reinterpret_cast<jobjectArray>(curEnv->GetObjectArrayElement(res, 0));
        jint lenCol = curEnv->GetArrayLength(oneDim);
        char **addr = new char*[lenRow * lenCol];
        jstring *resString = new jstring[lenRow * lenCol];

        for (int i = 0; i < lenRow; i++)
        {
            oneDim = reinterpret_cast<jobjectArray>(curEnv->GetObjectArrayElement(res, i));
            if (getMethodOfConv())
            {
                for (int j = 0; j < lenCol; j++)
                {
                    resString[j * lenRow + i] = reinterpret_cast<jstring>(curEnv->GetObjectArrayElement(oneDim, j));
                    addr[j * lenRow + i] = const_cast<char *>(curEnv->GetStringUTFChars(resString[j * lenRow + i], &isCopy));
                }
            }
            else
            {
                for (int j = 0; j < lenCol; j++)
                {
                    resString[i * lenCol + j] = reinterpret_cast<jstring>(curEnv->GetObjectArrayElement(oneDim, j));
                    addr[i * lenCol + j] = const_cast<char *>(curEnv->GetStringUTFChars(resString[i * lenCol + j], &isCopy));
                }
            }
            curEnv->DeleteLocalRef(oneDim);
        }

        SciErr err;
        if (getMethodOfConv())
        {
            err = createMatrixOfString(pvApiCtx, pos, lenRow, lenCol, addr);
        }
        else
        {
            err = createMatrixOfString(pvApiCtx, pos, lenCol, lenRow, addr);
        }

        for (int i = 0; i < lenRow * lenCol; i++)
        {
            curEnv->ReleaseStringUTFChars(resString[i], addr[i]);
            curEnv->DeleteLocalRef(resString[i]);
        }
        delete[] addr;
        delete[] resString;

        if (err.iErr)
        {
            throw org_scilab_modules_external_objects_java::NoMoreScilabMemoryException();
        }

        curEnv->DeleteLocalRef(res);
        if (curEnv->ExceptionCheck())
        {
            throw GiwsException::JniCallMethodException(curEnv);
        }
    };
};
