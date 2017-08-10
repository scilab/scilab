// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13810 -->
//
// <-- Bugzilla URL -->
//     http://bugzilla.scilab.org/13810
//
// <-- Short Description -->
// householder(v, k*v) returned [%nan %nan %nan]'
assert_checkequal(householder([%e ; 0 ; 0]), [0 ; 1 ; 0]);
v = [ 2 3 1]';
ref = [ 0.8320503  -0.5547002  0 ]';
assert_checkalmostequal(householder(v,v), ref, 1e-6);
assert_checkalmostequal(householder(v,2*v), ref, 1e-6);
ref = [  0.5345225  0.8017837  0.2672612 ]';
assert_checkalmostequal(householder(v,-2*v), ref, 1e-6);
v = [ 2 -%i 1+%i].';
ref = [0.7559289 -0.3779645*%i  0.3779645 + 0.3779645*%i].';
assert_checkalmostequal(householder(v,v), ref, 1e-6);
