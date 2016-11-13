/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

#include "ScilabPolynomialToJava.hxx"

namespace org_modules_types
{

jobjectArray ScilabPolynomialToJava::getJavaArrayFromPolynom(JNIEnv * curEnv, double *** data, int rows, int cols, int ** nbCoeff)
{
    jobjectArray data_ = curEnv->NewObjectArray(rows, curEnv->FindClass("[[D"), 0);
    if (!data_)
    {
        return 0;
    }

    for (int i = 0; i < rows; i++)
    {
        jobjectArray data1_ = curEnv->NewObjectArray(cols, curEnv->FindClass("[D"), 0);
        if (!data1_)
        {
            curEnv->DeleteLocalRef(data_);
            return 0;
        }
        curEnv->SetObjectArrayElement(data_, i, data1_);
        for (int j = 0; j < cols; j++)
        {
            jdoubleArray dataLocal = curEnv->NewDoubleArray(nbCoeff[i][j]);
            if (!dataLocal)
            {
                curEnv->DeleteLocalRef(data_);
                for (int k = 0; k <= i; k++)
                {
                    curEnv->DeleteLocalRef(curEnv->GetObjectArrayElement(data_, k));
                }
                return 0;
            }
            curEnv->SetDoubleArrayRegion(dataLocal, 0, nbCoeff[i][j], (jdouble*)(data[i][j]) );
            curEnv->SetObjectArrayElement(data1_, j, dataLocal);
            curEnv->DeleteLocalRef(dataLocal);
        }
    }

    return data_;
}

void ScilabPolynomialToJava::sendPolynomial(JavaVM * jvm_, char * varName, int * indexes, int indexesSize, char * polyVarName, double *** data, int rows, int cols, int ** nbCoeff, bool swaped, int handlerId)
{
    JNIEnv * curEnv = 0;
    jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), 0);
    jclass cls = curEnv->FindClass(ScilabVariables::className().c_str());

    jmethodID methodId = curEnv->GetStaticMethodID(cls, "sendPolynomial", "(Ljava/lang/String;[ILjava/lang/String;[[[DZI)V" ) ;
    if (!methodId)
    {
        curEnv->DeleteLocalRef(cls);
        throw GiwsException::JniMethodNotFoundException(curEnv, "sendPolynomial");
    }

    jstring varName_ = curEnv->NewStringUTF(varName);
    if (varName && !varName_)
    {
        curEnv->DeleteLocalRef(cls);
        throw GiwsException::JniBadAllocException(curEnv);
    }

    jintArray indexes_ = curEnv->NewIntArray(indexesSize) ;

    if (!indexes_)
    {
        curEnv->DeleteLocalRef(varName_);
        curEnv->DeleteLocalRef(cls);
        throw GiwsException::JniBadAllocException(curEnv);
    }

    curEnv->SetIntArrayRegion(indexes_, 0, indexesSize, (jint*)(indexes));

    jstring polyVarName_ = curEnv->NewStringUTF(polyVarName);
    if (polyVarName && !polyVarName_)
    {
        curEnv->DeleteLocalRef(varName_);
        curEnv->DeleteLocalRef(indexes_);
        curEnv->DeleteLocalRef(cls);
        throw GiwsException::JniBadAllocException(curEnv);
    }

    jobjectArray data_ = getJavaArrayFromPolynom(curEnv, data, rows, cols, nbCoeff);
    if (!data_)
    {
        curEnv->DeleteLocalRef(varName_);
        curEnv->DeleteLocalRef(indexes_);
        curEnv->DeleteLocalRef(polyVarName_);
        curEnv->DeleteLocalRef(cls);
        throw GiwsException::JniBadAllocException(curEnv);
    }

    jboolean swaped_ = (static_cast<bool>(swaped) ? JNI_TRUE : JNI_FALSE);

    curEnv->CallStaticVoidMethod(cls, methodId , varName_, indexes_, polyVarName_, data_, swaped_, handlerId);
    curEnv->DeleteLocalRef(varName_);
    curEnv->DeleteLocalRef(indexes_);
    curEnv->DeleteLocalRef(polyVarName_);
    for (int i = 0; i < rows; i++)
    {
        curEnv->DeleteLocalRef(curEnv->GetObjectArrayElement(data_, i));
    }
    curEnv->DeleteLocalRef(data_);
    curEnv->DeleteLocalRef(cls);
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }
}

void ScilabPolynomialToJava::sendPolynomial(JavaVM * jvm_, char * varName, int * indexes, int indexesSize, char * polyVarName, double *** real, double *** img, int rows, int cols, int ** nbCoeff, bool swaped, int handlerId)
{
    JNIEnv * curEnv = 0;
    jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), 0);
    jclass cls = curEnv->FindClass(ScilabVariables::className().c_str());

    jmethodID methodId = curEnv->GetStaticMethodID(cls, "sendPolynomial", "(Ljava/lang/String;[ILjava/lang/String;[[[D[[[DZI)V" ) ;
    if (!methodId)
    {
        curEnv->DeleteLocalRef(cls);
        throw GiwsException::JniMethodNotFoundException(curEnv, "sendPolynomial");
    }

    jstring varName_ = curEnv->NewStringUTF(varName);
    if (varName && !varName_)
    {
        curEnv->DeleteLocalRef(cls);
        throw GiwsException::JniBadAllocException(curEnv);
    }

    jintArray indexes_ = curEnv->NewIntArray(indexesSize) ;

    if (!indexes_)
    {
        curEnv->DeleteLocalRef(varName_);
        curEnv->DeleteLocalRef(cls);
        throw GiwsException::JniBadAllocException(curEnv);
    }

    curEnv->SetIntArrayRegion(indexes_, 0, indexesSize, (jint*)(indexes));

    jstring polyVarName_ = curEnv->NewStringUTF(polyVarName);
    if (polyVarName && !polyVarName_)
    {
        curEnv->DeleteLocalRef(varName_);
        curEnv->DeleteLocalRef(indexes_);
        curEnv->DeleteLocalRef(cls);
        throw GiwsException::JniBadAllocException(curEnv);
    }

    jobjectArray real_ = getJavaArrayFromPolynom(curEnv, real, rows, cols, nbCoeff);
    if (!real_)
    {
        curEnv->DeleteLocalRef(varName_);
        curEnv->DeleteLocalRef(indexes_);
        curEnv->DeleteLocalRef(polyVarName_);
        curEnv->DeleteLocalRef(cls);
        throw GiwsException::JniBadAllocException(curEnv);
    }

    jobjectArray img_ = getJavaArrayFromPolynom(curEnv, img, rows, cols, nbCoeff);
    if (!img_)
    {
        curEnv->DeleteLocalRef(varName_);
        curEnv->DeleteLocalRef(indexes_);
        curEnv->DeleteLocalRef(polyVarName_);
        curEnv->DeleteLocalRef(cls);
        for (int i = 0; i < rows; i++)
        {
            curEnv->DeleteLocalRef(curEnv->GetObjectArrayElement(real_, i));
        }
        curEnv->DeleteLocalRef(real_);
        throw GiwsException::JniBadAllocException(curEnv);
    }

    jboolean swaped_ = (static_cast<bool>(swaped) ? JNI_TRUE : JNI_FALSE);

    curEnv->CallStaticVoidMethod(cls, methodId , varName_, indexes_, polyVarName_, real_, img_, swaped_, handlerId);
    curEnv->DeleteLocalRef(varName_);
    curEnv->DeleteLocalRef(indexes_);
    curEnv->DeleteLocalRef(polyVarName_);
    for (int i = 0; i < rows; i++)
    {
        curEnv->DeleteLocalRef(curEnv->GetObjectArrayElement(real_, i));
        curEnv->DeleteLocalRef(curEnv->GetObjectArrayElement(img_, i));
    }
    curEnv->DeleteLocalRef(real_);
    curEnv->DeleteLocalRef(img_);
    curEnv->DeleteLocalRef(cls);
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }
}
}
