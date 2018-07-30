//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 4216 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/4216
//
// <-- Short Description -->
//  csim returned error "!--error 27  Division by zero..."

D = 1. + 2*%s;
H = syslin('c',1./D);
t = (0:0.01:20)';
assert_checktrue(execstr("csim(''step'',t,H)", "errcatch")==0);
