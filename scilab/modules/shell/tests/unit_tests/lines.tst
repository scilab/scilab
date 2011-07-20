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

// Since Scilab 5.4.0 lines(0) by default
r1 = lines(); 
assert_checkequal(r1(2),0);

lines(0); // Should noyt modify anything
r2 = lines();
assert_checkequal(r2(2),0);
assert_checkequal(r1,r2);

lines(-1)
r3 = lines();
assert_checkequal(r1(1), r3(1));

// resize window (on STD)
// and do same commands

// =============================================================================
