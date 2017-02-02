// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//===============================
// unit tests emptystr
//===============================
assert_checkequal(emptystr(), "");
//===============================
assert_checkequal(emptystr([]), []);
//===============================
assert_checkequal(size(emptystr("Scilab")), [1 1]);
assert_checkequal(emptystr("Scilab"), "");
//===============================
s = emptystr(["Scilab";"String"]);
assert_checkequal(size(s), [2 1]);
assert_checktrue(and(s == ""));
//===============================
M = 5;
N = 3;
s = emptystr(M,N);
assert_checkequal(size(s), [M N]);
assert_checktrue(and(s == ""));
//===============================
M = 200;
N = 300;
s = emptystr(M,N);
assert_checkequal(size(s), [M N]);
assert_checktrue(and(s == ""));
//===============================
assert_checkequal(emptystr(0,0), []);
//===============================
assert_checkequal(emptystr(0), "");
//===============================
M = 200;
s = emptystr(M);
assert_checkequal(size(s), [1 1]);
assert_checkequal(s, "");
//===============================
assert_checkequal(size(emptystr(0,1)), [0 0]);
//===============================
