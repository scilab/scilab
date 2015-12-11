// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 6390 -->
//
// <-- Bugzilla URL -->
//http://bugzilla.scilab.org/show_bug.cgi?id=6390
//
// <-- Short Description -->
// The external argument of odecd() was not documented well and not tested.

// Using one of the examples of odedc.tst, but with a C external.

ilib_verbose(0);

cd TMPDIR;
mkdir("odedc_test1");
cd("odedc_test1");
// The external
code = ["#include <math.h>"
"void f_name (int *iflag, int *nc, int *nd, double *t, double *y, double *ydp)"
"{int i = 0, j = 0;"
" double A[3][3]= {{-10,2,3},{4,-10,6},{7,8,-10}};"
" double B[3] = {1, 1, 1};"
" if (*iflag == 0) {"
"  //A = {{-10,2,3},{4,-10,6},{7,8,-10}};"
"  //B = {1, 1, 1};"
"  for (i=0; i<*nc; ++i)"
"    ydp[i] = 0;"
"  for (i=0; i<*nc; ++i)"
"   for (j=0;j<3; ++j)"
"    ydp[i] = ydp[i]+A[i][j]*y[j];"
"  for (i=0; i<*nc; ++i)"
"   ydp[i] = ydp[i]+B[i]*y[(*nc)];"
" } else"
"  ydp[0] = 1-y[(*nc)];"
"}"];
mputl(code,"f_name.c") ;
ilib_for_link(["f_name"], "f_name.c", "", "c");
exec("loader.sce");

expected_last_xcd = [ 1.2074118
2.0640519
2.5555553
1. ];

t0 = 0;
t = 0:0.1:30;
x0c = [0; 0; 0];
xcd = odedc([x0c; 0], 1, 1, t0, t, "f_name");

assert_checkalmostequal(xcd(:,$), expected_last_xcd, [], 1d-6);
