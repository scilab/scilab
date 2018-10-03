// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15753 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15753
//
// <-- Short Description -->
// issparse(sparseBoolean) returned 0 instead of 1

assert_checkequal(issparse(sparse([1 2 3])<2), 1);
