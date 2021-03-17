// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 13593 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13593
//
// <-- Short Description -->
// csvRead() did not use the given range when the header input is provided.
// [] could be rejected as default range.


File = tempname();
m = matrix(1:20, 5, -1);
csvWrite(m, File);

r = csvRead(File,[],[],"double",[],[],[3 2 4 3]);
assert_checkequal(r, [8 13 ; 9 14]);

r = csvRead(File,[],[],"double",[],[],[], 3);
assert_checkequal(r, [4 9 14 19 ; 5 10 15 20]);

r = csvRead(File,[],[],"double",[],[],[]);
assert_checkequal(r, m);

r = csvRead(File,[],[],"double",[],[],[2 2 3 3],1);
assert_checkequal(r, [8 13 ; 9 14]);
