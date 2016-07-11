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

// bug_14361

a=[1; .../* a comment */
-2];
assert_checkequal(a,  colvect);
a=[1;.../* a comment */
-2];
assert_checkequal(a,  colvect);
a=[1; .../* a comment */
 -2];
assert_checkequal(a,  colvect);
a=[1;.../* a comment */
 -2];
assert_checkequal(a,  colvect);
a=[1; .../* a comment */
- 2];
assert_checkequal(a,  colvect);
a=[1;.../* a comment */
- 2];
assert_checkequal(a,  colvect);

a=[1 .../* a comment */
-2];
assert_checkequal(a,  linevect);
a=[1.../* a comment */
-2];
assert_checkequal(a,  -1);
a=[1 .../* a comment */
 -2];
assert_checkequal(a,  linevect);
a=[1.../* a comment */
 -2];
assert_checkequal(a,  linevect);
a=[1 .../* a comment */
- 2];
assert_checkequal(a,  -1);
a=[1.../* a comment */
- 2];
assert_checkequal(a,  -1);

// Operators within matrices
// DOTTIMES
a = [[2 3] .* ... /* a comment */
-3];
expected = [-6 -9];
assert_checkequal(a, expected);

//DOTRDIVIDE

a = [[2 3] ./ ... /* a comment */
-3];
expected = [-2/3 -1];
assert_checkequal(a, expected);

//DOTLDIVIDE
a = [[2 3] .\ ... /* a comment */
-3];
expected = [-3/2 -1];
assert_checkequal(a, expected);

//DOTPOWER
a = [[2 3] .^ ... /* a comment */
-3];
expected = [(2^(-3)), (3^(-3))];
assert_checkequal(a, expected);

//MINUS
a = [[2 3] - ... /* a comment */
-3];
expected = [(2-(-3)), (3-(-3))];
assert_checkequal(a, expected);

//PLUS
a = [[2 3] + ... /* a comment */
-3];
expected = [(2+(-3)), (3+(-3))];
assert_checkequal(a, expected);

//TIMES
a = [2  * ... /* a comment */
 -3];
expected = [(2*(-3))];
assert_checkequal(a, expected);

//RDIVIDE
a = [2  / ... /* a comment */
 -3];
expected = [(2/(-3))];
assert_checkequal(a, expected);

//LDIVIDE
a = [2  \ ... /* a comment */
 -3];
expected = [(2\(-3))];
assert_checkequal(a, expected);

//POWER
a = [2  ^ ... /* a comment */
 -3];
expected = [(2^(-3))];
assert_checkequal(a, expected);

//KRONTIMES
a = [[2 3] .*. ... /* a comment */
 -3];
expected = [([2, 3].*.(-3))];
assert_checkequal(a, expected);

//kronrdivide
a = [[2 3] ./. ... /* a comment */
 -3];
expected = [([2, 3]./.(-3))];
assert_checkequal(a, expected);

//KRONLDIVIDE
a = [[2 3] .\. ... /* a comment */
 -3];
expected = [([2, 3].\.(-3))];
assert_checkequal(a, expected);

//EQ
a = [[2 3] == ... /* a comment */
 -3];
expected = [([2, 3]==(-3))];
assert_checkequal(a, expected);

//NE
a = [[2 3]  ~= ... /* a comment */
 -3];
expected = [([2, 3]~=(-3))];
assert_checkequal(a, expected);

//LT
a = [[2 3]  < ... /* a comment */
 -3];
expected = [([2, 3]<(-3))];
assert_checkequal(a, expected);

//GT
a = [[2 3]  > ... /* a comment */
 -3];
expected = [([2, 3]>(-3))];
assert_checkequal(a, expected);

//LE
a = [[2 3]  <= ... /* a comment */
 -3];
expected = [([2, 3]<=(-3))];
assert_checkequal(a, expected);

//GE
a = [[2 3]  >= ... /* a comment */
 -3];
expected = [([2, 3]>=(-3))];
assert_checkequal(a, expected);

// Comments on several lines
a=[1; .../* a comment 
 on several lines */
-2];
assert_checkequal(a,  colvect);
a=[1;.../* a comment 
 on several lines */
-2];
assert_checkequal(a,  colvect);
a=[1; .../* a comment 
 on several lines */
 -2];
assert_checkequal(a,  colvect);
a=[1;.../* a comment 
 on several lines */
 -2];
assert_checkequal(a,  colvect);
a=[1; .../* a comment 
 on several lines */
- 2];
assert_checkequal(a,  colvect);
a=[1;.../* a comment 
 on several lines */
- 2];
assert_checkequal(a,  colvect);

a=[1 .../* a comment 
 on several lines */
-2];
assert_checkequal(a,  linevect);
a=[1.../* a comment 
 on several lines */
-2];
assert_checkequal(a,  -1);
a=[1 .../* a comment 
 on several lines */
 -2];
assert_checkequal(a,  linevect);
a=[1.../* a comment 
 on several lines */
 -2];
assert_checkequal(a,  linevect);
a=[1 .../* a comment 
 on several lines */
- 2];
assert_checkequal(a,  -1);
a=[1.../* a comment 
 on several lines */
- 2];
assert_checkequal(a,  -1);

// Operators within matrices
// DOTTIMES
a = [[2 3] .* ... /* a comment 
 on several lines */
-3];
expected = [-6 -9];
assert_checkequal(a, expected);

//DOTRDIVIDE
a = [[2 3] ./ ... /* a comment 
 on several lines */
-3];
expected = [-2/3 -1];
assert_checkequal(a, expected);

//DOTLDIVIDE
a = [[2 3] .\ ... /* a comment 
 on several lines */
-3];
expected = [-3/2 -1];
assert_checkequal(a, expected);

//DOTPOWER
a = [[2 3] .^ ... /* a comment 
 on several lines */
-3];
expected = [(2^(-3)), (3^(-3))];
assert_checkequal(a, expected);

//MINUS
a = [[2 3] - ... /* a comment 
 on several lines */
-3];
expected = [(2-(-3)), (3-(-3))];
assert_checkequal(a, expected);

//PLUS
a = [[2 3] + ... /* a comment 
 on several lines */
-3];
expected = [(2+(-3)), (3+(-3))];
assert_checkequal(a, expected);

//TIMES
a = [2  * ... /* a comment 
 on several lines */
 -3];
expected = [(2*(-3))];
assert_checkequal(a, expected);

//RDIVIDE
a = [2  / ... /* a comment 
 on several lines */
 -3];
expected = [(2/(-3))];
assert_checkequal(a, expected);

//LDIVIDE
a = [2  \ ... /* a comment 
 on several lines */
 -3];
expected = [(2\(-3))];
assert_checkequal(a, expected);

//POWER
a = [2  ^ ... /* a comment 
 on several lines */
 -3];
expected = [(2^(-3))];
assert_checkequal(a, expected);

//KRONTIMES
a = [[2 3] .*. ... /* a comment 
 on several lines */
 -3];
expected = [([2, 3].*.(-3))];
assert_checkequal(a, expected);

//KRONRDIVIDE
a = [[2 3] ./. ... /* a comment 
 on several lines */
 -3];
expected = [([2, 3]./.(-3))];
assert_checkequal(a, expected);

//KRONLDIVIDE
a = [[2 3] .\. ... /* a comment 
 on several lines */
 -3];
expected = [([2, 3].\.(-3))];
assert_checkequal(a, expected);

//EQ
a = [[2 3] == ... /* a comment 
 on several lines */
 -3];
expected = [([2, 3]==(-3))];
assert_checkequal(a, expected);

//NE
a = [[2 3]  ~= ... /* a comment 
 on several lines */
 -3];
expected = [([2, 3]~=(-3))];
assert_checkequal(a, expected);

//LT
a = [[2 3]  < ... /* a comment 
 on several lines */
 -3];
expected = [([2, 3]<(-3))];
assert_checkequal(a, expected);

//GT
a = [[2 3]  > ... /* a comment 
 on several lines */
 -3];
expected = [([2, 3]>(-3))];
assert_checkequal(a, expected);

//LE
a = [[2 3]  <= ... /* a comment 
 on several lines */
 -3];
expected = [([2, 3]<=(-3))];
assert_checkequal(a, expected);

//GE
a = [[2 3]  >= ... /* a comment 
 on several lines */
 -3];
expected = [([2, 3]>=(-3))];
assert_checkequal(a, expected);

// Special behaviour of ... with + and -
a = [1 +...
2]
expected = 3;
assert_checkequal(a, expected);

a = [1 +...
 2]
expected = 3;
assert_checkequal(a, expected);

a = [1 +...
 +2]
expected = 3;
assert_checkequal(a, expected);

a = [1 -...
2]
expected = -1;
assert_checkequal(a, expected);

a = [1 -...
 2]
expected = -1;
assert_checkequal(a, expected);

a = [1 -...
 -2]
expected = 3;
assert_checkequal(a, expected);

// Case with multiline comment
str_cmd = ["a = [1 -... /* the start of a";
          "multiline comment */ 2 3]"]; // this currently generates an error
assert_checktrue(execstr(str_cmd, "errcatch") <> 0);

// bug_14374 comments in SHELLMODE
// Testing if shellmode execution with command
// convstr a u // a comment
// convstr a /* a comment */
// convstr a u /* some text
//                ending comment here */
exec(SCI + "/modules/ast/tests/nonreg_tests/bug_14374.tst");

// Bug 7646: matrix transpose extraction
exec(SCI + "/modules/ast/tests/nonreg_tests/bug_7646.tst");

// Bug 14662: matrix concatenation with simple quote and space led to a parser error
exec(SCI + "/modules/ast/tests/nonreg_tests/bug_14662.tst");

// Bug 14667: multi line string in matrix generated a non terminal parser state
exec(SCI + "/modules/ast/tests/nonreg_tests/bug_14667.tst");
