//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 8607 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8607
//
// <-- Short Description -->
// Some error messages in modules/overloading/macros are not standard and not localized

b=cell(3,1);
assert_checkequal(size(b,3), 1);

A(:,:,2)=[1;1;1];
B(:,:,2)=[1,1,1];
errmsg2=msprintf(_("Inconsistent row/column dimensions.\n"));
assert_checkerror("A<B", errmsg2);
assert_checkerror("[1 1]<A", errmsg2);

s=poly(0,"s");
P=[2*s+1;s];
Q=[3*s+2,s,s];
errmsg3=msprintf(_("%s: Wrong size for input arguments.\n"),"%p_v_p");
assert_checkerror("P/.Q", errmsg3);

errmsg4=msprintf(_("%s: Wrong value for input argument #%d: %d or %s expected.\n"),"%r_norm",2,2,"inf");
assert_checkerror("norm(P(1)/Q(1),1)", errmsg4);

sys=tf2ss(P(1)/Q(1));
errmsg5=msprintf(_("%s: Wrong value for input argument #%d: %d or %s expected.\n"),"%lss_norm",2,2,"inf");
assert_checkerror("norm(sys,1)",errmsg5);
