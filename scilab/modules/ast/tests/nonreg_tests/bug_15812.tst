// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15812 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15812
//
// <-- Short Description -->
// On assigning variables the source variable becomes corrupted (e.g. x=[3 4 5];y(1,:)=x;)
u=[3 4 5];
clear v
v(1,:)=u;
assert_checkequal(size(u),[1,3])

