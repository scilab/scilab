// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- LONG TIME EXECUTION -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16181
//
// <-- Short Description -->
// Unit tests for replacing the "end" index

if ~isdef("m2scikernellib") then
    load("SCI/modules/m2sci/macros/kernel/lib")
end

txt = mgetl("SCI/modules/m2sci/tests/unit_tests/conversion/END_index_syntax.m");
printf("%s\n", replace_end_dollar(txt));
