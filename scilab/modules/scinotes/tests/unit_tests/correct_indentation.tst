// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Correct indentation (Ctrl+I)
//
// 1. Click the line number of "y = 1;" below and press Ctrl+I: only that
//    line moves, to 8 spaces. Ctrl+Z restores it.
// 2. Select from "if x > 0" to its "end" and press Ctrl+I: only the selected
//    lines move; "function" and the lines after the "end" stay as they are.
// 3. Click in the editor without selecting anything and press Ctrl+I: the
//    whole file is indented by one level per block (4 spaces by default),
//    "elseif", "else", "case", "otherwise", "catch" and "end" stay at the
//    level of their block.
// 4. Only the start of lines may change: "a(end)", "1e3", 'single', the
//    continuation "...", the matrix rows and the inside of the block comment
//    are kept as written; the continuation and the second matrix row move
//    with their first line, and the closing "*/" of the block comment is
//    aligned with its opening "/*".
// 5. Press Ctrl+I again without selection: nothing changes.
// 6. Change the tab preferences (tabs instead of spaces), redo step 3: the
//    indentation uses tabs.
// 7. Delete the "end" of the "if", press Ctrl+I: nothing changes and the info
//    bar says to correct the syntax errors first. Restore the "end".

function y = indentation_sample(x)
if x > 0 then
y = 1;
elseif x < 0 then
y = -1;
else
      // comment before the end of the else body
y = a(end) + 1e3;
end
select x
case 1 then
disp('single')
otherwise
disp("double")
end
try
s = x + ...
    2;
M = [1 2 3
     4 5 6];
catch
/* block comment
      kept as written
*/
disp(lasterror())
end
endfunction
