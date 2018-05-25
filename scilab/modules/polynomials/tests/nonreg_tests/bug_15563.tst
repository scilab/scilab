// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15563 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15563
//
// <-- Short Description -->
// poly(A,v) is broken when A is a matrix (regression)

a=[0.9,0,0.6,-1.4,-4.2;
0.2,0.1,-0.2,0.5,0.6;
-4.3,0,2.2,0,2.4;
-3.7,-0.5,2.4,-0.6,2.7;
6.4,0.1,-4,-0.5,-4];
p=poly(a,'x');
assert_checktrue(isreal(p));