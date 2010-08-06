/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2010 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#include <jni.h> /* JavaVM */
#endif
#include "Scierror.h"
#include "gw_jvm.h"
#include "dynlib_jvm.h"
#include "InitializeJVM.h"
#include "TerminateJVM.h"
#include "loadBackGroundClassPath.h"
#include "loadOnUseClassPath.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
JVM_IMPEXP JavaVM *getScilabJavaVM(void)
{
    return NULL;
}
/*--------------------------------------------------------------------------*/ 
JVM_IMPEXP jobject getScilabObject(void)
{
    return NULL;
}
/*--------------------------------------------------------------------------*/ 
JVM_IMPEXP JNIEnv *getScilabJNIEnv(void)
{
    return NULL;
}
#endif
/*--------------------------------------------------------------------------*/ 
int gw_jvm(void)
{
    Scierror(999,_("Scilab Java module not installed.\n"));
    return 0;
}
/*--------------------------------------------------------------------------*/ 
BOOL InitializeJVM(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/ 
BOOL TerminateJVM(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/ 
BOOL loadBackGroundClassPath(void)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL loadOnUseClassPath(char *tag)
{
    return FALSE;
}
/*--------------------------------------------------------------------------*/
