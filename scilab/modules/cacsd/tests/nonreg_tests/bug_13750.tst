// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 13750 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13750
//
// <-- Short Description -->
//    Calling ss2ss with flag = 2 returned an error. The variable p was missing.
// =============================================================================


in=1;m=2;n=2;
sys=ssrand(in,m,n);
F1=rand(m,n);
G=rand(n,in);
[sys1,right,left]=ss2ss(sys,rand(n,n),F1,G,2);
res=clean(ss2tf(sys1) - ss2tf(left*sysdiag(sys*right,eye(1,1))));
assert_checkalmostequal(coeff(res.num), zeros(size(res,"r"), size(res, "c")), [], %eps);
