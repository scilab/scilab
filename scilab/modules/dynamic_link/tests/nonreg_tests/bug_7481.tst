// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7481 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7481
// <-- Short Description -->
// 6th input argument of ilib_for_link did not allow to set library name
// on Windows
//
ilib_verbose(0);
cd  TMPDIR;
f1=["int ext1c(int *n, double *a, double *b, double *c)"
    "{int k;"
    "  for (k = 0; k < *n; ++k) "
    "      c[k] = a[k] + b[k];"
    "  return(0);}"];

mputl(f1,"fun1.c");
libname = ilib_for_link("ext1c", "fun1.c", [], "c", [], "loader.sce", "test");
refname = "libtest" + getdynlibext();
if refname <> libname then pause, end
exec('loader.sce');
// =============================================================================
