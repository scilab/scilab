// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14218 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14218

A     = [-3 -1;1 -1;2 -1];
b     = [-3;0;2];
lb    = [-%inf;0];
ub    = [%inf;%inf];
c     = [0;1];

xopt  = karmarkar([],[],c,[],[],[],[],[],A,b,lb,ub)

assert_checkalmostequal(xopt, [0.75000162298698814; 0.7500048689416393]);
