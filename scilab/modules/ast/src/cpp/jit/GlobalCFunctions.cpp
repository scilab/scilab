/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "allexp.hxx"
#include "allvar.hxx"

#include "ScilabJITTraits.hxx"
#include "types_addition.hxx"
#include "types_substraction.hxx"
#include "types_multiplication.hxx"
#include "dynlib_ast.h"

extern "C"
{
    EXTERN_AST void putInContext_S_D_d(symbol::Context * ctxt, symbol::Variable * var, double x)
    {
        jit::putInContext_S<Double, double>(ctxt, var, x);
    }

    EXTERN_AST void putInContext_M_D_ds(symbol::Context * ctxt, symbol::Variable * var, double * x, int r, int c)
    {
        jit::putInContext_M<Double, double>(ctxt, var, x, r, c);
    }

    EXTERN_AST void add_M_M_d_d(double * l, long long size, double * r, double * o)
    {
        add<double, double>(l, size, r, o);
    }

    EXTERN_AST void sub_M_M_d_d(double * l, long long size, double * r, double * o)
    {
        sub<double, double>(l, size, r, o);
    }

    EXTERN_AST void dotmul_M_M_d_d(double * l, long long size, double * r, double * o)
    {
        dotmul<double, double>(l, size, r, o);
    }
}
