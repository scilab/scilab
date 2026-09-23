// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// AST-aware hover versus runtime context.
//
// Before execution:
// 1. sin inside localHover is a parameter, never the global Scilab macro.
// 2. localValue is a local variable on its declaration and use.
// 3. undocumentedHover is recognized from the AST and shows its signature.
// 4. externalBefore and externalAfter have the same semantic color inside
//    their functions and neither hover describes them as local variables.
// 5. Hover result and sin in localHover: they must be described respectively
//    as an output argument and an input argument.
//
// After execution:
// 6. globalValue hover shows its current Scilab value.
// 7. handlers.callback resolves the complete field expression and displays
//    acoshm help rather than generic information about a field.

globalValue = 42;
handlers.callback = acoshm;
externalBefore = 12;

function result = localHover(sin)
    localValue = sin + 1;
    result = localValue;
endfunction

function value = undocumentedHover(value)
    value = value + 1;
endfunction

function scopeBefore()
    disp(externalBefore);
endfunction

function scopeAfter()
    disp(externalAfter);
endfunction

externalAfter = 12;

handlers.callback(1)
undocumentedHover(globalValue);
