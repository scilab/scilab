// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->
//

function y=test1(a, b)
    y = a+b;
endfunction

a = 12;
b = ones(13, 17);
out = testAnalysis("test1", "a", "b");
assert_checkequal(out.type, "double");
assert_checkequal(out.rows, 13);
assert_checkequal(out.cols, 17);

a = ones(12, 11);
b = ones(13, 17);
out = testAnalysis("test1", "a", "b");
assert_checkequal(out.type, "double");
assert_checkequal(out.rows, %nan);
assert_checkequal(out.cols, %nan);

a = ones(12, 11);
b = ones(12, 11);
out = testAnalysis("test1", "a", "b");
assert_checkequal(out.type, "double");
assert_checkequal(out.rows, 12);
assert_checkequal(out.cols, 11);

a = int8(12);
b = ones(13, 17);
out = testAnalysis("test1", "a", "b");
assert_checkequal(out.type, "int8");
assert_checkequal(out.rows, 13);
assert_checkequal(out.cols, 17);

a = ones(12, 11);
b = uint32(ones(12, 11));
out = testAnalysis("test1", "a", "b");
assert_checkequal(out.type, "uint32");
assert_checkequal(out.rows, 12);
assert_checkequal(out.cols, 11);

function y = test2(a, b, c)
    y = [a b;c]
endfunction

a = 1:3;
b = 1:7;
c = matrix(1:20, 2, -1);
out = testAnalysis("test2", "a", "b", "c");
assert_checkequal(out.type, "double");
assert_checkequal(out.rows, 3);
assert_checkequal(out.cols, 10);

a = 1:3;
b = 1:7;
c = matrix(1:20, 2, -1);
// we "forget" c, so c is taken in this scope
out = testAnalysis("test2", "a", "b");
assert_checkequal(out.type, "double");
assert_checkequal(out.rows, 3);
assert_checkequal(out.cols, 10);

function y = test3(a, b, c)
    row = size(a, "r");
    col = size(a, "c");
    a = matrix(a, col, row);

    [row, col] = size(b);
    b = matrix(b, col, row);

    y = (a .*. b) * c'
endfunction

a = ones(2, 4);
b = ones(5, 9);
c = ones(321, 10);
out = testAnalysis("test3", "a", "b", "c");
assert_checkequal(out.type, "double");
assert_checkequal(out.rows, 36);
assert_checkequal(out.cols, 321);

a = int32(ones(2, 4));
b = uint8(ones(5, 9));
c = ones(321, 10);
out = testAnalysis("test3", "a", "b", "c");
assert_checkequal(out.type, "uint32");
assert_checkequal(out.rows, 36);
assert_checkequal(out.cols, 321);

a = int32(ones(2, 4));
b = uint8(ones(5, 9));
c = ones(2, 2);
out = testAnalysis("test3", "a", "b", "c");
assert_checkequal(out.type, "uint32");
assert_checkequal(out.rows, %nan);
assert_checkequal(out.cols, %nan);

function y = test4(a, b, c)
    y = test1(a, b) * test2(a, b, c)
endfunction

a = ones(5, 8);
b = ones(5, 8);
c = ones(3, 16);
out = testAnalysis("test4", "a", "b", "c");
assert_checkequal(out.type, "double");
assert_checkequal(out.rows, 5);
assert_checkequal(out.cols, 16);
