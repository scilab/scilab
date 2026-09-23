// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Syntax and symbol-shadowing diagnostics.
//
// Keep this file unexecuted:
// 1. factorParameter and factorLocal must each report one
//    "shadowed-scilab-symbol" warning on the first declaration of factor.
// 2. Later assignments and usages must not produce duplicate warnings.
// 3. factor must retain the parameter/local-variable color, not the macro color.
// 4. Uncomment the intentionally invalid line at the bottom: a syntax error
//    must appear alongside the shadowing warnings without clearing them.

function result = factorParameter(factor)
    result = factor + 1;
endfunction

function result = factorLocal(value)
    factor = value;
    factor = factor + 1;
    result = factor;
endfunction

parameterResult = factorParameter(2);
localResult = factorLocal(3);

//invalidAssignment =
