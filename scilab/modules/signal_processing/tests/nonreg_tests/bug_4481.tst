// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 4481 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4481
//
// <-- Short Description -->
// iir should be able to handle a scalar for frq

hz_ref = iir(4, 'lp' , 'ellip' , [0.25 0.25], [0.05  0.02]);
hz = iir(4, 'lp' , 'ellip' , 0.25, [0.05  0.02]);
assert_checkequal(hz,hz_ref);
