// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- INTERACTIVE TEST -->
// =============================================================================
// start scilab (STD and NW mode)
//
r1 = lines();
lines(0)
r2 = lines();
assert_checkequal(r2(2),int32(0));
lines(-1)
r3 = lines();
assert_checkequal(r1, r3);

// resize window (on STD)
// and do same commands

// =============================================================================
