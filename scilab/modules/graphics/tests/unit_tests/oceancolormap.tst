// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

assert_checkerror("oceancolormap(0,1,2,3)", "%s: Wrong number of input argument(s): %d expected.\n", 10000, "oceancolormap", 1);

assert_checkerror("oceancolormap(%t)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "oceancolormap", 1);

assert_checkerror("oceancolormap(%i)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "oceancolormap", 1);

assert_checkerror("oceancolormap([0 1 2 3])", "%s: Wrong size for input argument #%d: a real scalar expected.\n", 10000, "oceancolormap", 1);

assert_checkequal(oceancolormap(0), []);

assert_checkequal(oceancolormap(1), [0 0.25 0.5]);

assert_checkalmostequal(oceancolormap(2), [0 0 0.25;0.25 0.625 0.75]);

assert_checkalmostequal(oceancolormap(3), [0 0 1/6;0 0.25 0.5;0.5 0.75 5/6]);
