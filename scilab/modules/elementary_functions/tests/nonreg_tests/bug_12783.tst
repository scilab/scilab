// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12783 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12783
//
// <-- Short Description -->
// There are some inconsistent error messages in dsearch

assert_checktrue(execstr("dsearch(int8([ 1 5 7]), [3 4 7 10], ""d"")", "errcatch")==0);

assert_checktrue(execstr("dsearch([1 5 7], int8([3 4 7 10]), ""d"")", "errcatch")==0);

m=50000;
n=10;
X=grand(m,1,"def");
s=linspace(0,1,n)';
assert_checktrue(execstr("[ind, occ] = dsearch(X, s);", "errcatch")==0);
