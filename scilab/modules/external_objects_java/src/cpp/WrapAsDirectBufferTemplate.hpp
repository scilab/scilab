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
#include <jni.h>
#include "GiwsException.hxx"
/*--------------------------------------------------------------------------*/
#define SCILABJAVAOBJECT "org/scilab/forge/jims/ScilabJavaObject"
/*--------------------------------------------------------------------------*/
template <typename U>
void* ScilabJavaObjectHelper::wrapAsDirectBuffer(JavaVM * jvm_, U* address, long size, int *javaID)
{
    JNIEnv * curEnv = NULL;
    jobject dbuffer = NULL;
    jobject ref = NULL;

    jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
    jclass cls = curEnv->FindClass(SCILABJAVAOBJECT);

    jmethodID id = curEnv->GetStaticMethodID(cls, ScilabJavaObjectHelper::getFunctionNameForDB(address), "(Ljava/nio/ByteBuffer;)I");
    dbuffer = curEnv->NewDirectByteBuffer((void*)address, (jlong)size);

    ref = curEnv->NewGlobalRef(dbuffer);
    *javaID = static_cast<jint>(curEnv->CallStaticIntMethod(cls, id, ref));

    return ref;
}
/*--------------------------------------------------------------------------*/
