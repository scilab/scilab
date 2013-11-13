// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

assert_checkerror("graycolormap(0,1,2,3)", "%s: Wrong number of input argument(s): %d expected.\n", 10000, "graycolormap", 1);

assert_checkerror("graycolormap(%t)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "graycolormap", 1);

assert_checkerror("graycolormap(%i)", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "graycolormap", 1);

assert_checkerror("graycolormap([0 1 2 3])", "%s: Wrong size for input argument #%d: a real scalar expected.\n", 10000, "graycolormap", 1);

assert_checkequal(graycolormap(0), []);

assert_checkequal(graycolormap(1), [0 0 0]);

assert_checkequal(graycolormap(2), [0 0 0;1 1 1]);

assert_checkequal(graycolormap(3), [0 0 0;0.5 0.5 0.5;1 1 1]);
