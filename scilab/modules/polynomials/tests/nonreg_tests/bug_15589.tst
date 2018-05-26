// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15589 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15589
//
// <-- Short Description -->
// inv([%s+0*%i 0 ; 0 1]) failed

pm = [%s+0*%i 0 ; 0 1];
assert_checkequal(pm*inv(pm), [1/(1+%s*0) 0 ; 0 1]);
