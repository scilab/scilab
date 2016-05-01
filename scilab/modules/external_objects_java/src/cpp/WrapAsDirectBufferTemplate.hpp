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
/*--------------------------------------------------------------------------*/
#include <jni.h>
#include "GiwsException.hxx"
#include "ScilabJavaObjectHelper.hxx"
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
