// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16141 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16141
//
// <-- Short Description -->
// extraction on implicit list crashes Scilab

assert_checkequal((3:17)($), 17);
assert_checkequal((3:17)($-3:$), 14:17);