// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 9673 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9673
//
// <-- Short Description -->
// Priority of colon : operator is too low

assert_checkequal(1 == 2 : 3,[%f %f])

