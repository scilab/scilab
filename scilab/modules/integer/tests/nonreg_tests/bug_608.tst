//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 608 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=608
//
// <-- Short Description -->
//    the integer types (tested with uint8 and uint16) operators
//    do not work very well. The problem only occur when one of
//    the operands is a scalar (1 by 1 matrix). That can be
//    observed in SCI/routines/int/gencheck.c that when changing
//    some value on r (pointer to the output boolean matrix), this
//    affect the scalar element, changing the comparison.
//    This occur only if the form is
//    "scalar op matrix" or "matrix op scalar", where op stands
//    for <=,<,==,<>,>= or >
//    The function is called by SCI/modules/integer/src/fortran/i_logic.f

a = uint8(zeros(4,4));
b = (a<=uint8(0));
c = [ %T %T %T %T  ; %T %T %T %T ; %T %T %T %T ; %T %T %T %T  ];

if or(c <> b) then pause,end
