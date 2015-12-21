// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

res = buildouttb([2 2], 1); // Request a list containing a 2x2 matrix of doubles

resRef = list([0 0; 0 0]);
assert_checkequal(res, resRef);


refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected."), "buildouttb", 2);
assert_checkerror("buildouttb()", refMsg);
