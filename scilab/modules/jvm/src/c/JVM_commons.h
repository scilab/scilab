
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#ifndef __JVM_COMMONS_H__
#define __JVM_COMMONS_H__
/*--------------------------------------------------------------------------*/ 
#include <jni.h>
#include "machine.h"
#include "JVM_functions.h"
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#define JRE_PATH "/java/jre"
#else
/* Always this value when we embedded JRE */
#define JRE_PATH "/java/jre"
#endif
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#define DEFAULT_SCILAB_CLASSPATH "/bin" /* SCI/bin */
#define DEFAULT_SCILAB_LIBRARYPATH "/bin" /* SCI/bin */
#else
/* not really used , java.library.path is used in fact */
/* @TODO is it used or not ? Please explain or remove it */
#define DEFAULT_SCILAB_CLASSPATH "/.libs/" /* SCI/.libs */
#define DEFAULT_SCILAB_LIBRARYPATH "/.libs/" /* SCI/.libs */
#endif
/*--------------------------------------------------------------------------*/ 
#define USER_CLASSPATH "."
#define JVM_CLASSPATH "/modules/jvm/jar/org.scilab.modules.jvm.jar"
/*--------------------------------------------------------------------------*/ 
#endif /*  __JVM_COMMONS_H__ */
/*--------------------------------------------------------------------------*/ 
