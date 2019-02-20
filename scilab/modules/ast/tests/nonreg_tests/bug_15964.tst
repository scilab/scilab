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
// <-- Non-regression test for bug 15964 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15964
//
// <-- Short Description -->
// The complex empty sparse matrix must resume to the real one.

s = sprand(10,10,0.3)*%i;
s(:)=[];
assert_checktrue(issparse(s))
assert_checktrue(isreal(s))
s = sprand(1,10,0.3)*%i;
s(1,:)=[];
assert_checktrue(issparse(s))
assert_checktrue(isreal(s))
