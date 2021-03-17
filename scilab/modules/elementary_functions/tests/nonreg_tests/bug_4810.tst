// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 4810 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/4810
//
// <-- Short Description -->
// gsort does not manage sparse vector as sort

sp = sparse([1,2,4,5,3,10]);
ref = sparse([10,5,4,3,2,1]);
A = gsort(sp);
assert_checkequal(A, ref);
