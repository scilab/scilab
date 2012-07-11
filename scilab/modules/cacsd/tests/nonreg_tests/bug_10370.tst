// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10370 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10370
//
// <-- Short Description -->
// ss2tf returned a wrong value on Windows 64 bit


A2 = [0 1 0
      1 0 1
      0 0 -1];
B2 = [0;0;10.523];
C2 = [1 0 0];

sl2 = syslin('c', A2,B2,C2);
G2 = clean(ss2tf(sl2));
REF =  10.523/ (-1 -%s + (%s*%s) + (%s*%s*%s));
assert_checkalmostequal(coeff(G2(2)), coeff(REF(2)));
assert_checkalmostequal(coeff(G2(3)), coeff(REF(3)));
