// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================
//
// <-- Non-regression test for bug 13723 -->
//
// <-- JVM MANDATORY -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13723
//
// <-- Short Description -->
// No way to insert a single double in double[][]

b = jarray("double",3,1);
b(1) = [1 2 3];
b(2) = [4 5];
b(3) = 6;

assert_checkequal(b(1), [1 2 3]);
assert_checkequal(b(2), [4 5]);
assert_checkequal(b(3), 6);