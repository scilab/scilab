// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16215 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16215
//
// <-- Short Description -->
// `mfile2sci` badly converted the colon `:` into `mtlb(:)`
m = [
"a = rand(3,5);"
"a(1,:) = 6"
"b = a(:,3)"
"c = a(:)"
"d(:) = 2"
"e = 1:4"
"f = 1:-2:10"
"g = a(3:end)"
];
mfile = TMPDIR+"/bug_16215.m";
mputl(m, mfile);
mfile2sci(mfile, TMPDIR);
ref = [
""
"// Display mode"
"mode(0);"
""
"// Display warning for floating point exception"
"ieee(1);"
""
"a = rand(3,5);"
"a(1,:) = 6"
"b = a(:,3)"
"c = a(:)"
"d(1,:) = 2"
"e = 1:4"
"f = 1:-2:10"
"g = a(3:$)"
]
assert_checkequal(mgetl(TMPDIR+"/bug_16215.sci"), ref);
