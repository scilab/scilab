// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 9635 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9635
//
// <-- Short Description -->
// some temp. variables were not removed when we used test_run


// Interactive test since we can test test_run with test_run

// exec this line:
test_run("elementary_functions","repmat");

assert_checkequal(isdef('test_examples'), %F);
assert_checkequal(isdef('splitModule'), %F);
assert_checkequal(isdef('getDirectories'), %F);
assert_checkequal(isdef('comparethefiles'), %F);
assert_checkequal(isdef('createthefile'), %F);
assert_checkequal(isdef('remove_headers'), %F);
assert_checkequal(isdef('launchthecommand'), %F);
assert_checkequal(isdef('checkthefile'), %F);
assert_checkequal(isdef('test_single'), %F);
assert_checkequal(isdef('test_module'), %F);

