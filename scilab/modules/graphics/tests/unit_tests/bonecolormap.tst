// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH Ga rAPHIC -->

assert_checkerror("bonecolormap(0,1,2,3)", "%s: Wrong number of input argument(s): %d expected.\n", 10000, "bonecolormap", 1);

assert_checkerror("bonecolormap(%t)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "bonecolormap", 1);

assert_checkerror("bonecolormap(%i)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "bonecolormap", 1);

assert_checkerror("bonecolormap([0 1 2 3])", "%s: Wrong size for input argument #%d: a real scalar expected.\n", 10000, "bonecolormap", 1);

assert_checkequal(bonecolormap(0), []);

assert_checkequal(bonecolormap(1), [0.125 0.125 0.125]);

assert_checkequal(bonecolormap(2), [0.0625 0.125 0.125;1 1 1]);

assert_checkequal(bonecolormap(3), [0 0 0.125;0.4375 0.5625 0.5625;1 1 1]);
