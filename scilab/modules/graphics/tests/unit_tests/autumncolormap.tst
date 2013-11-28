// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH Ga rAPHIC -->

assert_checkerror("autumncolormap(0,1,2,3)", "%s: Wrong number of input argument(s): %d expected.\n", 10000, "autumncolormap", 1);

assert_checkerror("autumncolormap(%t)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "autumncolormap", 1);

assert_checkerror("autumncolormap(%i)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "autumncolormap", 1);

assert_checkerror("autumncolormap([0 1 2 3])", "%s: Wrong size for input argument #%d: a real scalar expected.\n", 10000, "autumncolormap", 1);

assert_checkequal(autumncolormap(0), []);

assert_checkequal(autumncolormap(1), [1 0 0]);

assert_checkequal(autumncolormap(2), [1 0 0;1 1 0]);

assert_checkequal(autumncolormap(3), [1 0 0;1 0.5 0;1 1 0]);
