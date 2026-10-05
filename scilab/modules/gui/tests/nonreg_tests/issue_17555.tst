// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for issue 17555 -->
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Gitlab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17555
//
// <-- Short Description -->
// figure("menubar", "none", ) failed with "figure: No more memory.".
// figure([id,] "prop", value, ...) must check its property/value pairs and
// report a meaningful error before creating or modifying any figure.

// Checks that instr fails with msg (any error when msg is []) and that no
// figure has been created or deleted.
function checkFigureError(instr, msg)
    w0 = winsid();
    if msg == [] then
        assert_checktrue(execstr(instr, "errcatch") <> 0);
    else
        assert_checkerror(instr, msg);
    end
    assert_checkequal(winsid(), w0);
endfunction

delete(winsid());

msgPairs = msprintf(_("%s: Wrong number of input arguments: property/value pairs expected.\n"), "figure");
msgId = msprintf(_("%s: Wrong type for input argument #%d: An integer value expected.\n"), "figure", 1);

// Trailing comma (the reported case): property without value
checkFigureError("figure(""menubar"", ""none"", )", msgPairs);
checkFigureError("figure(""menubar"", ""none"", ""visible"", ""off"", )", msgPairs);

// Same with a leading figure id
f = 17555;
checkFigureError("figure(f, ""menubar"", ""none"", )", msgPairs);
checkFigureError("figure(17555, ""menubar"", ""none"", )", msgPairs);

// Property name without value: creation-time property or not
checkFigureError("figure(""menubar"")", msgPairs);
checkFigureError("figure(""menubar"", ""none"", ""visible"")", msgPairs);
checkFigureError("figure(""menubar"", ""none"", ""backgroundcolor"")", msgPairs);
checkFigureError("figure(""menubar"", ""none"", ""figure_name"")", msgPairs);
checkFigureError("figure(17555, ""menubar"")", msgPairs);
checkFigureError("figure(17555, ""backgroundcolor"")", msgPairs);
checkFigureError("figure(17555, )", msgPairs);

// Empty argument instead of a property name
msg = msprintf(_("%s: Wrong type for input argument #%d: A single string expected.\n"), "figure", 3);
checkFigureError("figure(""menubar"", ""none"", , )", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: A single string expected.\n"), "figure", 4);
checkFigureError("figure(17555, ""menubar"", ""none"", , )", msg);

// Empty argument instead of a property value
checkFigureError("figure(""menubar"", , ""visible"", ""off"")", []);
checkFigureError("figure(""figure_name"", , ""visible"", ""off"")", []);

// Wrong first argument: neither a figure id nor a property name
checkFigureError("figure(, ""menubar"", ""none"")", msgId);
checkFigureError("figure(, )", msgId);
checkFigureError("figure([1 2], ""menubar"", ""none"")", msgId);
checkFigureError("figure(%t, ""menubar"", ""none"")", msgId);
checkFigureError("figure(1, 2, 3)", msprintf(_("%s: Wrong type for input argument #%d: A single string expected.\n"), "figure", 2));
h = figure("visible", "off");
checkFigureError("figure(h, ""menubar"", ""none"")", msgId);
delete(h);

// An existing figure must not be modified by an invalid call
h = figure(17555, "visible", "off", "figure_name", "issue 17555");
checkFigureError("figure(17555, ""figure_name"", ""modified"", )", msgPairs);
checkFigureError("figure(17555, ""figure_name"", ""modified"", ""backgroundcolor"")", msgPairs);
assert_checkequal(h.figure_name, "issue 17555");
delete(h);

// Valid calls still work
h = figure("visible", "off", "menubar", "none");
assert_checkequal(h.menubar, "none");
delete(h);

h = figure(17555, "visible", "off", "menubar", "none");
assert_checkequal(h.figure_id, 17555);
assert_checkequal(h.menubar, "none");
h2 = figure(17555, "figure_name", "issue 17555");
assert_checkequal(h2, h);
assert_checkequal(h.figure_name, "issue 17555");
delete(h);

assert_checkequal(winsid(), []);
