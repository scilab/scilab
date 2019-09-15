// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 16181 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16181
//
if ~isdef("m2scikernellib") then
    load("SCI/modules/m2sci/macros/kernel/lib")
end

txt = mgetl("SCI/modules/m2sci/tests/nonreg_tests/bug_16181.m");
printf("%s\n", replace_end_dollar(txt));
