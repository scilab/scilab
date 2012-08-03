/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
