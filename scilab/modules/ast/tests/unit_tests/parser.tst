// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - DIGITEO - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->


// line break
colvect(1,1) = 1;
colvect(2,1) = -2;
linevect = colvect';


a=[1; ...
-2];
assert_checkequal(a,  colvect);
a=[1;...
-2];
assert_checkequal(a,  colvect);
a=[1; ...
 -2];
assert_checkequal(a,  colvect);
a=[1;...
 -2];
assert_checkequal(a,  colvect);
a=[1; ...
- 2];
assert_checkequal(a,  colvect);
a=[1;...
- 2];
assert_checkequal(a,  colvect);

a=[1 ...
-2];
assert_checkequal(a,  linevect);
a=[1...
-2];
assert_checkequal(a,  -1);
a=[1 ...
 -2];
assert_checkequal(a,  linevect);
a=[1...
 -2];
assert_checkequal(a,  linevect);
a=[1 ...
- 2];
assert_checkequal(a,  -1);
a=[1...
- 2];
assert_checkequal(a,  -1);

a=[1; ...//comment
-2];
assert_checkequal(a,  colvect);
a=[1;...//comment
-2];
assert_checkequal(a,  colvect);
a=[1; ...//comment
 -2];
assert_checkequal(a,  colvect);
a=[1;...//comment
 -2];
assert_checkequal(a,  colvect);
a=[1; ...//comment
- 2];
assert_checkequal(a,  colvect);
a=[1;...//comment
- 2];
assert_checkequal(a,  colvect);

a=[1 ...//comment
-2];
assert_checkequal(a,  linevect);
a=[1...//comment
-2];
assert_checkequal(a,  -1);
a=[1 ...//comment
 -2];
assert_checkequal(a,  linevect);
a=[1...//comment
 -2];
assert_checkequal(a,  linevect);
a=[1 ...//comment
- 2];
assert_checkequal(a,  -1);
a=[1...//comment
- 2];
assert_checkequal(a,  -1);

a=[1 ...
-2 ];
assert_checkequal(a,  linevect);
a=[1...
-2 ];
assert_checkequal(a,  -1);
a=[1 ...
 -2 ];
assert_checkequal(a,  linevect);
a=[1...
 -2 ];
assert_checkequal(a,  linevect);
a=[1 ...
- 2 ];
assert_checkequal(a,  -1);
a=[1...
- 2 ];
assert_checkequal(a,  -1);

// Operators within matrices
// DOTTIMES
a = [[2 3] .* -3];
expected = [-6 -9];
assert_checkequal(a, expected);

a = [[2 3] .* ...
-3];
assert_checkequal(a, expected);

a = [[2 3] .* ... // a comment
-3];
assert_checkequal(a, expected);

//DOTRDIVIDE
a = [[2 3] ./ -3];
expected = [-2/3 -1];
assert_checkequal(a, expected);

a = [[2 3] ./ ...
-3];
assert_checkequal(a, expected);

a = [[2 3] ./ ... // a comment
-3];
assert_checkequal(a, expected);

//DOTLDIVIDE
a = [[2 3] .\ -3];
expected = [-3/2 -1];
assert_checkequal(a, expected);

a = [[2 3] .\ ...
-3];
assert_checkequal(a, expected);

a = [[2 3] .\ ... // a comment
-3];
assert_checkequal(a, expected);

//DOTPOWER
a = [[2 3] .^ -3];
expected = [(2^(-3)), (3^(-3))];
assert_checkequal(a, expected);

a = [[2 3] .^ ...
-3];
assert_checkequal(a, expected);

a = [[2 3] .^ ... // a comment
-3];
assert_checkequal(a, expected);

//MINUS
a = [[2 3] - -3];
expected = [(2-(-3)), (3-(-3))];
assert_checkequal(a, expected);

a = [[2 3] - ...
-3];
assert_checkequal(a, expected);

a = [[2 3] - ... // a comment
-3];
assert_checkequal(a, expected);

//PLUS
a = [[2 3] + -3];
expected = [(2+(-3)), (3+(-3))];
assert_checkequal(a, expected);

a = [[2 3] + ...
-3];
assert_checkequal(a, expected);

a = [[2 3] + ... // a comment
-3];
assert_checkequal(a, expected);

//TIMES
a = [2 * -3];
expected = [(2*(-3))];
assert_checkequal(a, expected);

a = [2 * ...
 -3];
assert_checkequal(a, expected);

a = [2  * ... // a comment
 -3];
assert_checkequal(a, expected);

//RDIVIDE
a = [2 / -3];
expected = [(2/(-3))];
assert_checkequal(a, expected);

a = [2 / ...
 -3];
assert_checkequal(a, expected);

a = [2  / ... // a comment
 -3];
assert_checkequal(a, expected);

//LDIVIDE
a = [2 \ -3];
expected = [(2\(-3))];
assert_checkequal(a, expected);

a = [2 \ ...
 -3];
assert_checkequal(a, expected);

a = [2  \ ... // a comment
 -3];
assert_checkequal(a, expected);

//POWER
a = [2 ^ -3];
expected = [(2^(-3))];
assert_checkequal(a, expected);

a = [2 ^ ...
 -3];
assert_checkequal(a, expected);

a = [2  ^ ... // a comment
 -3];
assert_checkequal(a, expected);

//KRONTIMES
a = [[2 3] .*. -3];
expected = [([2, 3].*.(-3))];
assert_checkequal(a, expected);

a = [[2 3] .*. ...
 -3];
assert_checkequal(a, expected);

a = [[2 3] .*. ... // a comment
 -3];
assert_checkequal(a, expected);

//KRONRDIVIDE
a = [[2 3] ./. -3];
expected = [([2, 3]./.(-3))];
assert_checkequal(a, expected);

a = [[2 3] ./. ...
 -3];
assert_checkequal(a, expected);

a = [[2 3] ./. ... // a comment
 -3];
assert_checkequal(a, expected);

//KRONLDIVIDE
a = [[2 3] .\. -3];
expected = [([2, 3].\.(-3))];
assert_checkequal(a, expected);

a = [[2 3] .\. ...
 -3];
assert_checkequal(a, expected);

a = [[2 3] .\. ... // a comment
 -3];
assert_checkequal(a, expected);

//EQ
a = [[2 3] == -3];
expected = [([2, 3]==(-3))];
assert_checkequal(a, expected);

a = [[2 3] == ...
 -3];
assert_checkequal(a, expected);

a = [[2 3] == ... // a comment
 -3];
assert_checkequal(a, expected);

//NE
a = [[2 3] ~= -3];
expected = [([2, 3]~=(-3))];
assert_checkequal(a, expected);

a = [[2 3] ~= ...
 -3];
assert_checkequal(a, expected);

a = [[2 3]  ~= ... // a comment
 -3];
assert_checkequal(a, expected);

//LT
a = [[2 3] < -3];
expected = [([2, 3]<(-3))];
assert_checkequal(a, expected);

a = [[2 3] < ...
 -3];
assert_checkequal(a, expected);

a = [[2 3]  < ... // a comment
 -3];
assert_checkequal(a, expected);

//GT
a = [[2 3] > -3];
expected = [([2, 3]>(-3))];
assert_checkequal(a, expected);

a = [[2 3] > ...
 -3];
assert_checkequal(a, expected);

a = [[2 3]  > ... // a comment
 -3];
assert_checkequal(a, expected);

//LE
a = [[2 3] <= -3];
expected = [([2, 3]<=(-3))];
assert_checkequal(a, expected);

a = [[2 3] <= ...
 -3];
assert_checkequal(a, expected);

a = [[2 3]  <= ... // a comment
 -3];
assert_checkequal(a, expected);

//GE
a = [[2 3] >= -3];
expected = [([2, 3]>=(-3))];
assert_checkequal(a, expected);

a = [[2 3] >= ...
 -3];
assert_checkequal(a, expected);

a = [[2 3]  >= ... // a comment
 -3];
assert_checkequal(a, expected);


