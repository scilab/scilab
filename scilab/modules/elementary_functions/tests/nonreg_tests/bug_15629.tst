// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15629 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15629
//
// <-- Short Description -->
// int(1e20) yields -9.223D+18  (regression)

assert_checkequal(int(1e20),1e20);
assert_checkequal(int(1e20*(1+%i)),1e20*(1+%i));
assert_checkequal(int(1e20*(1+%s)),1e20*(1+%s));
assert_checkequal(int(1e20*[1+%s;1-%s]),1e20*[1+%s;1-%s]);
a=sparse([0 1e20]);
assert_checkequal(int(a),a);
assert_checkequal(int(a*(1+%i)),a*(1+%i));
