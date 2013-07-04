// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2012 - S/E - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
a=bool2s(sparse([1 2 5 0 3 0 6 9]));
aref=sparse([1,1;1,2;1,3;1,5;1,7;1,8],[1;1;1;1;1;1],[1,8]);
assert_checkequal(a, aref);
b=bool2s(sparse([%t %f %t %t %f %t]));
bref=sparse([1,1;1,3;1,4;1,6],[1;1;1;1],[1,6]);
assert_checkequal(b, bref);

a=bool2s([%t %t %f %t]);
assert_checkequal(a, [1,1,0,1]);
b=bool2s([2.3 0 10 -1]);
assert_checkequal(b, [1,0,1,1]);
