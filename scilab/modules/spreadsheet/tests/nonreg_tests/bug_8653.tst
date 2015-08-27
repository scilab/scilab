//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - S/E - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// 
// <-- JAVA NOT MANDATORY -->
//
// <-- Non-regression test for bug 8653 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8653
//
// <-- Short Description -->
// csvRead returns double by default


A = 1:50;
mputl(strcat(string(A),ascii(9)), TMPDIR + '/foo.csv');
B = csvRead(TMPDIR + '/foo.csv',ascii(9));

assert_checkequal(A,B);

// Check previous behavior
B = read_csv(TMPDIR + '/foo.csv',ascii(9));
assert_checkequal(A,evstr(B));