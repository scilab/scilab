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
// <-- Non-regression test for bug 8654 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8654
//
// <-- Short Description -->
// Use the standard separator for csv.
//
A = [0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1];
filename=fullfile(TMPDIR,'data.csv');
write_csv(A,filename);
B = evstr(read_csv(filename));
assert_checkequal(A,B);

A = [1:10] * 0.1;
filename=fullfile(TMPDIR,'data2.csv');
write_csv(A,filename);
B = evstr(read_csv(filename));
assert_checkequal(A,B);
