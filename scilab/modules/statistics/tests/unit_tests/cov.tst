// Copyright (C) 2012-2013 - Michael Baudin
// Copyright (C) 2010 - INRIA - Michael Baudin
// Copyright (C) 2019 - Stéphane MOTTELET
//
// This file must be used under the terms of the GNU Lesser General Public License license :
// http://www.gnu.org/copyleft/lesser.html

// Run with test_run('statistics', 'cov', ['no_check_error_output']);

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Check error
assert_checkfalse(execstr("cov()", "errcatch") =  = 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d, %d or %d expected.\n"), "cov", 1, 2, 3);
assert_checkerror("cov()", refMsg);

assert_checkfalse(execstr("cov(""r"")", "errcatch") =  = 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: a real matrix expected.\n"), "cov", 1);
assert_checkerror("cov(""r"")", refMsg);

assert_checkfalse(execstr("cov([1;2], ""r"")", "errcatch") =  = 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: an integer or a real matrix expected.\n"), "cov", 2);
assert_checkerror("cov([1;2], ""r"")", refMsg);

assert_checkfalse(execstr("cov(""r"", [1;2])", "errcatch") =  = 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: a real matrix expected.\n"), "cov", 1);
assert_checkerror("cov(""r"", [1;2])", refMsg);

assert_checkfalse(execstr("cov(""r"", [1;2], 1)", "errcatch") =  = 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: a real matrix expected.\n"), "cov", 1);
assert_checkerror("cov(""r"", [1;2], 1)", refMsg);

assert_checkfalse(execstr("cov([1;2], ""r"", 1)", "errcatch") =  = 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: a real matrix expected.\n"), "cov", 2);
assert_checkerror("cov([1;2], ""r"", 1)", refMsg);

assert_checkfalse(execstr("cov([1;2], [3;4], ""r"")", "errcatch") =  = 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: an integer expected.\n"), "cov", 3);
assert_checkerror("cov([1;2], [3;4], ""r"")", refMsg);

x = [1;2];
y = [3;4];
computed = cov(x, y);
expected = [0.5, 0.5;0.5, 0.5];
assert_checkalmostequal(computed, expected);
//
// The same, with nrmlztn = 0
x = [1;2];
y = [3;4];
computed = cov(x, y, 0);
expected = [0.5, 0.5;0.5, 0.5];
assert_checkalmostequal(computed, expected);
//
x = [230;181;165;150;97;192;181;189;172;170];
y = [125;99;97;115;120;100;80;90;95;125];
expected = [103721, -4001;-4001, 3664]./[90, 45;45, 15];
computed = cov(x, y);
assert_checkalmostequal(computed, expected);
//
// The same, with nrmlztn = 0
x = [230;181;165;150;97;192;181;189;172;170];
y = [125;99;97;115;120;100;80;90;95;125];
expected = [103721, -4001;-4001, 3664]./[90, 45;45, 15];
computed = cov(x, y, 0);
assert_checkalmostequal(computed, expected);
//
x = [1;2;3;4;5];
computed = cov(x);
expected = 2.5;
assert_checkequal(computed, expected);
//
// The same, with nrmlztn = 0
x = [1;2;3;4;5];
computed = cov(x, 0);
expected = 2.5;
assert_checkequal(computed, expected);
//
A = [-1 1 2 ; -2 3 1 ; 4 0 3];
Cexpected =  [31, -25, 3;-25, 7, -3;3, -3, 1]./[3, 6, 1;6, 3, 2;1, 2, 1];
C = cov(A);
assert_checkalmostequal(Cexpected, C, [], [], "element");
//
// The same, with nrmlztn = 0
A = [-1 1 2 ; -2 3 1 ; 4 0 3];
C = cov(A, 0);
assert_checkalmostequal(Cexpected, C, [], [], "element");
//
// Complex matrix
A = complex([-1, 1, 2;-2, 3, 1;4, 0, 3], [2, -1, 1;1, -2, 3;3, 4, 0]);
C = cov(A);
// Diagonal has to be exactly real
assert_checkequal(imag(diag(C)), [0;0;0])
Cexpected = complex( [34, -7, 3;-7, 38, -17;3, -17, 10]./[3, 6, 2;6, 3, 3;2, 3, 3], ...
	                 [0, 71, -31;-71, 0, -2;31, 2, 0]./ [1, 6, 6;6, 1, 3;6, 3, 1])
assert_checkalmostequal(C, Cexpected)
//
// Reference
// 6.5.4.1. Mean Vector and Covariance Matrix
// http://www.itl.nist.gov/div898/handbook/pmc/section5/pmc541.htm
A = [
4.0 2.0 0.60
4.2 2.1 0.59
3.9 2.0 0.58
4.3 2.1 0.62
4.1 2.2 0.63
];
S = [
0.025 0.0075 0.00175
0.0075 0.007 0.00135
0.00175 0.00135 0.00043
];
C = cov(A);
assert_checkalmostequal(S, C, [], [], "element");
//
// The same, with nrmlztn = 0
A = [
4.0 2.0 0.60
4.2 2.1 0.59
3.9 2.0 0.58
4.3 2.1 0.62
4.1 2.2 0.63
];
S = [
0.025 0.0075 0.00175
0.0075 0.007 0.00135
0.00175 0.00135 0.00043
];
C = cov(A, 0);
assert_checkalmostequal(S, C, [], [], "element");
//
x = [1;2];
computed = cov(x, 1);
expected = 0.25;
assert_checkalmostequal(computed, expected);
//
x = [1;2];
computed = cov(x, 0);
expected = 0.5;
assert_checkalmostequal(computed, expected);
//
x = [1;2];
y = [3;4];
computed = cov(x, y, 1);
expected = [0.25, 0.25;0.25, 0.25];
assert_checkalmostequal(computed, expected);
//
// Case where x and y are matrices (cross-covariance)
//
x = [-1 1 2 ; -2 3 1 ; 4 0 3];
y = [2, -1, 1;1, -2, 3;3, 4, 0]
computed = cov(x, y);
// check that cross-covariance is an exact submatrix of cov([x y])
computed = cov(x, y);
expected = cov([x, y])(1:3,4:$);
assert_checkequal(computed, expected);
// check actual value
expected = [3,31,-25;-3,-25,7;1,3,-3]./[1,3,6;2,6,3;1,1,2];
assert_checkalmostequal(computed, expected);
computed = cov(x, y, 0);
assert_checkalmostequal(computed, expected);
computed = cov(x, y, 1);
expected = [2,62,-25;-1,-25,14;2,2,-1]./[1,9,9;1,9,9;3,1,1];
assert_checkalmostequal(computed, expected);
//
computed = cov(x, y);
cov([x, y])(1:3,4:$)==cov(x,y)
//
assert_checkalmostequal(cov(1:5, 0), 2.5);
assert_checkalmostequal(cov(1:5, 1), 2);
assert_checkequal(cov(1:5, 0), cov((1:5)', 0));
assert_checkequal(cov(1:5, 1), cov((1:5)', 1));
//
assert_checkequal(cov(2), 0);
assert_checkequal(cov(2, 1), 0);
assert_checkequal(cov(2, 2), zeros(2, 2));
//
