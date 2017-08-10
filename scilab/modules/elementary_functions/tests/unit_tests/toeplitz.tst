// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
r = 1:5;
c = [1 6:9];
toe = [1 6 7 8 9;
2 1 6 7 8;
3 2 1 6 7;
4 3 2 1 6;
5 4 3 2 1];
assert_checkequal(toeplitz(r, c), toe);