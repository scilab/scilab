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
// <-- Non-regression test for bug 15770 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15770
//
// <-- Short Description -->
// test_run cacsd zp2ss and zp2tf fail 

sz=zp2ss([%i %i -%i -%i],1,1);
[a,b,c,d]=abcd(sz);
assert_checkalmostequal([a b c d],[1 1 1 1+%z]);
assert_checkequal(zp2tf([%i %i -%i -%i],1,1),(1+2*%z^2+%z^4)/(-1+%z));
