// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - ESI-Group - Delamarre Cedric
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

a=1:10;
assert_checkequal(a(1+(1:2:$-1)), [2 4 6 8 10]);
assert_checkequal(a((1:2:$-1)+1), [2 4 6 8 10]);
assert_checkequal(a(11-(1:2:$)), [10 8 6 4 2]);
assert_checkequal(a((2:2:$)-1), [1 3 5 7 9]);
assert_checkequal(a(-(-$:2:-1)), [10 8 6 4 2]);