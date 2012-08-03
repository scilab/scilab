// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
//
// <-- Non-regression test for bug 8032 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8032
//
// <-- Short Description -->
// cdfnor was able to fail silently


Mean = 0;
Std = 1;
P = 0.5;
Q = 0.5;
X = cdfnor("X",Mean,Std,P,Q);
assert_checkequal ( X , 0 );

Mean = 1;
X = 1;
P = 0.5;
Q = 0.5;
Std = cdfnor("Std",P,Q,X,Mean);
assert_checkequal ( isnan(Std) , %t );

