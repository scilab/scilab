// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ASI Group - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 7675 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7675
//
// <-- Short Description -->
// sparse() got a wrong result, when creation with many values in same position

assert_checkequal(sparse([1 1; 2 2; 1 1],[1 2 3]), sparse([1 1;2 2], [4 2]))
