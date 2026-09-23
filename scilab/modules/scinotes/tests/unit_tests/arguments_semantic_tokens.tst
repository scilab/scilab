// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Semantic tokens inside an arguments block.
//
// Keep this file unexecuted, then check:
// 1. values and enabled have the input/output-argument color in the function
//    declaration, in the arguments block, and in the function body.
// 2. result has the input/output-argument color in its declaration and uses.
// 3. double keep its callable colors.
// 4. mustBeFinite, and mustBeA keep their keywork colors.
// 5. %t and %f have the predefined-constant color used by %pi.
// 6. Temporarily introduce a syntax error on the line containing %f: its
//    predefined-constant color must be preserved without a complete AST.

function result = argumentsSemanticTokens(values, enabled)
    arguments
        values (1, :) double {mustBeFinite}
        enabled (1, 1) {mustBeA(enabled, "boolean")} = %t
    end

    if enabled == %f then
        result = [];
    else
        result = values;
    end
endfunction
