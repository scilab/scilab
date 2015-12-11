
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __JVM_COMMONS_H__
#define __JVM_COMMONS_H__
/*--------------------------------------------------------------------------*/
#include <jni.h>
#include "JVM_functions.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define JRE_PATH "/java/jre"
#else
/* Always this value when we embedded JRE */
#define JRE_PATH "/java/jre"
#endif
/*--------------------------------------------------------------------------*/
#endif /*  __JVM_COMMONS_H__ */
/*--------------------------------------------------------------------------*/
