/*
 * JIMS ( http://forge.scilab.org/index.php/p/JIMS/ ) - This file is a part of JIMS
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <jni.h>
#include "ScilabJavaObject.hxx"
//#include "ScilabJavaObjectHelper.hxx"
/*--------------------------------------------------------------------------*/
#define SCILABJAVAOBJECT "org/scilab/forge/jims/ScilabJavaObject"
/*--------------------------------------------------------------------------*/
void ScilabJavaObjectHelper::releaseDirectBuffer(JavaVM * jvm_, void** ref, int* javaID, int len)
{
    JNIEnv * curEnv = NULL;
    jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
    jclass cls = curEnv->FindClass(SCILABJAVAOBJECT);

    int i = 0;
    for (; i < len; i++)
    {
        curEnv->DeleteGlobalRef((jobject)(ref[i]));
        org_scilab_modules_external_objects_java::ScilabJavaObject::limitDirectBuffer(jvm_, javaID[i]);
        org_scilab_modules_external_objects_java::ScilabJavaObject::removeScilabJavaObject(jvm_, javaID[i]);
    }
}
/*--------------------------------------------------------------------------*/
