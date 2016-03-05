
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
