// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

assert_checkerror("pinkcolormap(0,1,2,3)", "%s: Wrong number of input argument(s): %d expected.\n", 10000, "pinkcolormap", 1);

assert_checkerror("pinkcolormap(%t)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "pinkcolormap", 1);

assert_checkerror("pinkcolormap(%i)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "pinkcolormap", 1);

assert_checkerror("pinkcolormap([0 1 2 3])", "%s: Wrong size for input argument #%d: a real scalar expected.\n", 10000, "pinkcolormap", 1);

assert_checkequal(pinkcolormap(0), []);

assert_checkalmostequal(pinkcolormap(1), [0.5773503,0.5773503,0.5773503], 1e-7);

assert_checkalmostequal(pinkcolormap(2), [0.5773503,0.5773503,0.4082483;1,1,1], 1e-7);

assert_checkalmostequal(pinkcolormap(3), [0.5773503,0,0;0.8164966,0.8164966,0.5773503;1,1,1], 1e-7);
