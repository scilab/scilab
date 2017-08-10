//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14829 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14829
//
// <-- Short Description -->
// The product of a rational matrix by a polynomial vector has been broken
a=[1/%s 1/%s];b=[%s;%s];
assert_checktrue(a*b==rlist(2,1,[]));
