//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2014 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// <-- NO CHECK REF -->
// Issue 1461: there was no unit test for code generation (fmu_do_compile)

loadXcosLibs();
path = fmigetPath();
blk = [];

assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "helpPageSuper.zcos")));
blk = scs_m.objs(1); // only one block is on the diagram

// getvalue overload 
function str = x_mdialog(desc, labels, ini)
    ini(1) = "issue_1461";
    ini(2) = TMPDIR+"/issue_1461/";
    str = ini;
endfunction

// msg box overloaded by the test harness

// mprintf overload
function mprintf(varargin)
endfunction

// generate an FMU for ME
assert_checkequal(execstr("do_compile_fmu()","errcatch"), 0);

// getvalue overload 
function str = x_mdialog(desc, labels, ini)
    ini(1) = "issue_1461";
    ini(2) = TMPDIR+"/issue_1461/";
    ini(3) = "cs"
    str = ini;
endfunction

// generate an FMU for CS
assert_checkequal(execstr("do_compile_fmu()","errcatch"), 0);
mdelete(TMPDIR+"/issue_1461/");
