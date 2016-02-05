// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4675 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4675
//
// <-- Short Description -->
// An interface cannot have more than 99 functions. If you create the functions fun1, ...,fun100, then fun1, ...,fun99 are
// callable but fun100 make a crash

ilib_verbose(0)
cd TMPDIR;
//creating the interface file
c_code = ["#include <api_scilab.h>"
"int sci_funxxx(char *fname, void * pvApiCtx)"
"{"
"  static double dOut = 0;"
"  dOut ++;"
"  int m_out = 1;"
"  int n_out = 1;"
"  createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, &dOut);"
"  LhsVar(1) = 1;"
"  PutLhsVar();"
"  return 0;"
"}"];
mputl(c_code,"bug_4675.c");

test_table = [];
for i=1:999,
    test_table = [ test_table; "fun"+string(i), "sci_funxxx"];
end

files =["bug_4675.c"];
ilib_build("test_bug_4675",test_table,files,[]);

// load the shared library
exec("loader.sce");

//using the new primitive
for i=1:999,
    execstr("res = fun"+string(i)+"();");
    if res <> i then pause,end
end

ulink();

// check with more than 999 functions in a interface

test_table = [];
for i=1:1024,
    test_table = [ test_table; "fun"+string(i), "sci_funxxx"];
end

ierr = execstr("ilib_build(''test_bug_4675'',test_table,files,[]);","errcatch");
if ierr <> 999 then pause,end

