// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4611 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4611
//
// <-- Short Description -->
// what function doesn't returns a complete list of functions.

[a,b]=what();

if with_module("arnoldi") then
    assert_checkfalse(isempty(grep(a, "zneupd")));
end

if with_module("graphics") then
    assert_checkfalse(isempty(grep(a,"zoom_rect")));
end

assert_checkequal(part(a(1),1), "!");
assert_checkequal(part(a(2),1), "%");
