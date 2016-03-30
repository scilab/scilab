// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14097 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14097
//
// <-- Short Description -->
//
// genlib no more adds a separator at the end of the lib path if it is not given in the directory path.

createdir(TMPDIR + "/macros");
macro_dir = pathconvert("TMPDIR/macros");
macro_foo = ["function [] = foo()"; ...
"endfunction"];
mputl(macro_foo, macro_dir + "foo.sci");

ref = pathconvert("TMPDIR/macros/");
genlib("foolib", TMPDIR + "/macros");
s1=string(foolib)(1);
assert_checkequal(s1, ref);
clear foolib;

genlib("foolib", TMPDIR + "/macros/");
s2=string(foolib)(1);
assert_checkequal(s2, ref);
clear foolib;

