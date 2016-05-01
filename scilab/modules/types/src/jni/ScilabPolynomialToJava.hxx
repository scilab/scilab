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

#ifndef __SCILABPOLYNOMIALTOJAVA_HXX__
#define __SCILABPOLYNOMIALTOJAVA_HXX__

#include "ScilabVariables.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_types;

namespace org_modules_types
{

class ScilabPolynomialToJava
{

    static jobjectArray getJavaArrayFromPolynom(JNIEnv * curEnv, double *** data, int rows, int cols, int ** nbCoeff);

public :

    static void sendPolynomial(JavaVM * jvm_, char * varName, int * indexes, int indexesSize, char * polyVarName, double *** data, int rows, int cols, int ** nbCoeff, bool swaped, int handlerId);

    static void sendPolynomial(JavaVM * jvm_, char * varName, int * indexes, int indexesSize, char * polyVarName, double *** real, double *** img, int rows, int cols, int ** nbCoef, bool swaped, int handlerId);
};
}

#endif
