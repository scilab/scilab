//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vladislav TRUBKIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 13608 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13608
//
// <-- Short Description -->
// There was an error with complex values.

value = [0.1+0*%i, 0+0*%i; 0+0*%i, 0.2+0*%i];
refValue = [-2.3025851, 0; 0, -1.6094379];
assert_checkalmostequal(refValue, logm(value));

value = [-0.1-0*%i, 1-0*%i; 0+0*%i, -0.2+0*%i];
refValue = [-2.3025851 + 3.1415927*%i, -6.9314718; 0, -1.6094379 + 3.1415927*%i];
assert_checkalmostequal(refValue, logm(value));

a = [-0.9+%i 0+0*%i; 0+0*%i -0.1-%i];
assert_checktrue(and(clean(expm(logm(a))-a)==0));
