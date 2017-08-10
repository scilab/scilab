// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 4405 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4405
//
// <-- Short Description -->
// Syntax [Q,R,rk,E]=qr(A,"e") should return an error

A=[1 2; 3 4; 5 6];
expectedMsg = msprintf(_("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "qr", 2);
assert_checkerror("[Q,R,rk,E]=qr(A,""e"")",expectedMsg);
