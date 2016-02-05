//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// 
// <-- JAVA NOT MANDATORY -->
//
// <-- Non-regression test for bug 11991 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11991
//
// <-- Short Description -->
// csvRead does not ignore blank lines
//
M=[1:10]*0.1;
filename=TMPDIR+"/data.csv";
csvWrite(M',filename);

content=mgetl(filename);
assert_checkequal(size(content,"*"), 10);
