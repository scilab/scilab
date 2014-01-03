// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH Ga rAPHIC -->

assert_checkerror("coolcolormap(0,1,2,3)", "%s: Wrong number of input argument(s): %d expected.\n", 10000, "coolcolormap", 1);

assert_checkerror("coolcolormap(%t)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "coolcolormap", 1);

assert_checkerror("coolcolormap(%i)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "coolcolormap", 1);

assert_checkerror("coolcolormap([0 1 2 3])", "%s: Wrong size for input argument #%d: a real scalar expected.\n", 10000, "coolcolormap", 1);

assert_checkequal(coolcolormap(0), []);

assert_checkequal(coolcolormap(1), [0 1 1]);

assert_checkequal(coolcolormap(2), [0 1 1;1 0 1]);

assert_checkequal(coolcolormap(3), [0 1 1;0.5 0.5 1;1 0 1]);
