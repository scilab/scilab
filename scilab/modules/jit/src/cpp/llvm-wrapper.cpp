/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013-2013 - S/E - Sylvestre Ledru
 *  Copyright (C) 2013-2013 - S/E - Antoine ELIAS
 *  Copyright (C) 2013-2013 - S/E - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_comparison_lt_le_gt_ge.hxx"
#include "double.hxx"
#include "types_addition.hxx"
#include "types_multiplication.hxx"
#include "types_substraction.hxx"
#include "context.hxx"

/* This is a configuration file which will be transformed by llvm + clang
for the JIT
*/
int foo()
{

    types::Double* pdbl = types::Double::Empty();
    types::Bool* pb = 0;
    /* Addition */
    int res = AddDoubleToDouble(pdbl, pdbl, &pdbl);
    types::Double* pdbl1 = types::Double::Empty();
    /* Multiplication */
    res = MultiplyDoubleByDouble(pdbl1, pdbl1, &pdbl1);
    res = SubstractDoubleToDouble(pdbl1, pdbl1, &pdbl1);

    /* Substract */
    res = DoubleLessDouble(pdbl, pdbl, 0);

    symbol::Symbol * sym = 0;// = symbol::Symbol(L"a"); => This declaration makes llc fails

    symbol::Context::getInstance()->put(*sym, *pdbl);
    symbol::Context::getInstance()->get(*sym);

    pb->get(0);

    return res;
}


