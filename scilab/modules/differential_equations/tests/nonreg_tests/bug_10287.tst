// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10287 -->
//
// <-- Bugzilla URL -->
//http://bugzilla.scilab.org/show_bug.cgi?id=10287
//
// <-- Short Description -->
// An error message added for complex expression with integrate function.

errmsg=msprintf(_("%s: Wrong value for input argument #%d: A real expected.\n"),"integrate",1);
assert_checkerror("integrate(''abs(exp(%i*t))*exp(%i*t)*%i*t'',''t'',-%pi,0)",errmsg);
