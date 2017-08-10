// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 15137 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15137
//
// <-- Short Description -->
// ilib_build cannot build a library with more than 999 function entries in Scilab 6

cd(TMPDIR);

funcs = ["foo" + string(1:1000)', "sci_foo" + string(1:1000)'];
code = "int sci_foo" + string(1:1000)' + "(char *fname, void * pvApiCtx) {};";
filename = "foo.c";
mputl(code, fullfile(TMPDIR, filename));
libname = "foo";

ierr = execstr("ilib_build(libname, funcs, filename, []);", "errcatch");
assert_checkequal(ierr, 0);