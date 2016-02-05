// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//

// <-- INTERACTIVE TEST -->

result = read(5,2,2);
1 2
3 4
assert_checkequal(result,[1, 2; 3, 4]);

result = read(5,1,1,"(a)");
a
assert_checkequal(result,"a");

result = read(5,4,1,"(a)");
t
e
s
t

assert_checkequal(result,["t";"e";"s";"t"]);

result = read(5,-1,2);
1 2
3 4
5 6
7 8
9 10

assert_checkequal(result,[1, 2; 3, 4; 5, 6; 7, 8; 9, 10]);
