// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Semantic tokens, document symbols, hover, and folding.
//
// Keep this file unexecuted, then check:
// 1. Open the document outline: both functions must be listed.
// 2. Fold each function, the for block, the if/else block, and the select block.
// 3. Ctrl+click each call at the bottom: it must open the local declaration.
// 4. Hover the function declarations and calls: the local signatures must appear.
// 5. Check colors for parameters, local variables, fields, $, and index-context end.
// 6. In the Kronecker product below, ones must retain its primitive color:
//    the last dot in the .*. operator is not a field separator.
// 7. endValue inside the function must not stop local line numbering: only a
//    standalone end closes a block.

function result = monacoAbsoluteSum(values)
    result = 0;
endValue = 12;

    for index = 1:size(values, "*")
        if values(index) >= 0 then
            result = result + values(index);
        else
            result = result - values(index);
        end
    end
endfunction

function selected = monacoSelectLast(values)
    // The field after the dot, $, and index-context "end" have special roles.
    metadata.label = "last value";
    selected = values($);
    selected = values(end);

    select sign(selected)
    case -1 then
        selected = -selected;
    case 0 then
        selected = 0;
    else
        selected = selected;
    end
end



// Do not execute the file before testing these calls.
previewSum = monacoAbsoluteSum([-2, 3, -4]);
previewLast = monacoSelectLast([-2, 3, -4]);
operatorResult = 1 .*. ones(2, 2);
