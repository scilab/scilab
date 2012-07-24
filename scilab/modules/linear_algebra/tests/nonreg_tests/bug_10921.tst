// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 10921 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10921
//
// <-- Short Description -->
//    For ill-conditioned matrix, inv() prints a wrong value of rcond

M = [1 0; 0 10^(-8)];
inv(M);

A=diag([1:10]);
A(1,1) = 10^-8;
inv(A);



