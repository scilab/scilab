/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013-2013 - S/E - Sylvestre Ledru
 *  Copyright (C) 2013-2013 - S/E - Antoine Elias
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "double.hxx"
#include "types_addition.hxx"

/* This is a configuration file which will be transformed by llvm + clang
for the JIT
*/
int foo()
{
    types::Double* pdbl1 = types::Double::Empty();
    int res = AddDoubleToDouble(pdbl1, pdbl1, &pdbl1);

    return res;
}


