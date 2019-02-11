// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Unit tests for size(cblock)

T = mlist('cblock');
assert_checkequal(size(T), [0 0]);
assert_checkequal(size(T,1), 0);
assert_checkequal(size(T,2), 0);
assert_checkequal(size(T,"r"), 0);
assert_checkequal(size(T,"c"), 0);
assert_checkequal(size(T,"*"), 0);
[m, n] = size(T);
assert_checkequal(m, 0);
assert_checkequal(n, 0);

T = msscanf(-1,["Alain 19";"Pierre 15";"Tom 12"],'%s %d');
assert_checkequal(size(T), [3 2]);
assert_checkequal(size(T,1), 3);
assert_checkequal(size(T,2), 2);
assert_checkequal(size(T,"r"), 3);
assert_checkequal(size(T,"c"), 2);
assert_checkequal(size(T,"*"), 6);
[m, n] = size(T);
assert_checkequal(m, 3);
assert_checkequal(n, 2);
