// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15497 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15497
//
// <-- Short Description -->
// Structures containing lists with 1 element : wrong type with syntax: structure("field")

s.a = list(3.1415);
assert_checkequal(type(s), 17);
assert_checkequal(type(s.a), 15);
assert_checkequal(type(s.a(1)), 1);

assert_checkequal(type(s), 17);
assert_checkequal(type(s("a")), 15);
assert_checkequal(type(s("a")(1)), 1);
