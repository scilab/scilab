// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->


// <-- Non-regression test for bug 14330 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=
//
// <-- Short Description -->
//    luget was really slow.

[A,descr,ref,mtype] = ReadHBSparse(SCI+"/modules/umfpack/demos/bcsstk24.rsa");
[hand,rk] = lufact(A);
[P,L,U,Q] = luget(hand);
assert_checktrue(norm(P*L*U*Q-A)<1d-2);
ludel(hand);
