// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// =============================================================================
// <-- Non-regression test for bug 5292 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5292
//
// <-- Short Description -->
// There is a conflict between bool and scilab header mex.h

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include <mex.h>"
"#include <list>"
"#include <memory.h>"
""
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])"
"{"
"    if(nrhs < 2)"
"        mexErrMsgTxt(""Two input arguments required."");"
"    if (nlhs > 1)"
"        mexErrMsgTxt(""Too many output arguments."");"
"}"],"mexfunction_5292.cpp");
ilib_mex_build("libmextest", ["mex_5292", "mexfunction_5292", "cmex"], "mexfunction_5292.cpp", []);
exec("loader.sce");

assert_checkerror("mex_5292()", [], 999);
ierr = execstr("mex_5292(2,2)","errcatch");
assert_checktrue(ierr == 0);
