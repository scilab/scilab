// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Incomplete AST and syntax diagnostics.
//
// This file is intentionally invalid. Do not fix it before testing:
// 1. A syntax diagnostic must be displayed on the incomplete assignment.
// 2. monacoIncompleteAnalysis must remain in the document outline.
// 3. Folding must remain available for the function and if/else block.
// 4. Parameters and local variables must keep their semantic colors.
// 5. Hover, completion, and Ctrl+click must not crash or clear the document.
// 6. The lambda must not crash document analysis despite the syntax error below.

lambdaMedian = #(values)->(median(values));

function result = monacoIncompleteAnalysis(value)
    if value > 0 then
        result = value;
    else
        result = // INTENTIONAL temporary syntax error
    end
endfunction

incompleteValue = monacoIncompleteAnalysis(4);
