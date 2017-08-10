// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for meanf function
// =============================================================================

assert_checkfalse(execstr("meanf()"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument: %d to %d expected.\n"), "meanf", 2, 3);
assert_checkerror("meanf()", refMsg);

assert_checkequal(meanf([], 1), %nan);

assert_checkequal(meanf(%nan, 1), %nan);

assert_checkequal(meanf(%nan, %nan), %nan);

// Vector
x = 1:10;
assert_checkequal(meanf(x, 5), 55);
assert_checkequal(meanf(x', 5), 55);

assert_checkfalse(execstr("meanf(x, ''r'')"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A full or sparse matrix, or an integer matrix, or an hypermat expected.\n"), "meanf", 2);
assert_checkerror("meanf(x, ''r'')", refMsg);

assert_checkequal(meanf(x, x), 7);
assert_checkequal(meanf(x', x'), 7);

assert_checkequal(meanf(x, []), %nan);
assert_checkequal(meanf(x', []), %nan);

assert_checkequal(meanf(x, x, "c"), 7);
assert_checkequal(meanf(x, x, 2), 7);
assert_checkequal(meanf(x, x, "*"), 7);
assert_checkequal(meanf(x', x', "r"), 7);
assert_checkequal(meanf(x', x', 1), 7);
assert_checkequal(meanf(x', x', "*"), 7);

assert_checkfalse(execstr("meanf(x, x, ''t'')"   ,"errcatch") == 0);
refMsg = msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', %d or %d expected.\n"),"meanf",3,"r","c",1,2);
assert_checkerror("meanf(x, x, ''t'')", refMsg);

assert_checkfalse(execstr("meanf(x, x, 4)"   ,"errcatch") == 0);
refMsg = msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', %d or %d expected.\n"),"meanf",3,"r","c",1,2);
assert_checkerror("meanf(x, x, 4)", refMsg);

// Matrix
A=[1,2,10;7,7.1,7.01];

assert_checkfalse(execstr("meanf(A, A'')"   ,"errcatch") == 0);
refMsg = msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: Same dimensions expected.\n"),"meanf",1,2);
assert_checkerror("meanf(A, A'')", refMsg);

assert_checkequal(meanf(A, 3), 34.11);
assert_checkequal(meanf(A, 3, "r"), [8 9.1 17.01]);
assert_checkequal(meanf(A, 3, 1), [8 9.1 17.01]);
assert_checkequal(meanf(A, 3, "c"), [13;21.11]);
assert_checkequal(meanf(A, 3, 2), [13;21.11]);
assert_checkequal(meanf(A, 3, "*"), 34.11);

assert_checkalmostequal(meanf(A, A), 7.4333069, [], %eps);
assert_checkalmostequal(meanf(A, A, "*"), 7.4333069, [], %eps);
assert_checkalmostequal(meanf(A, A, "r"), [6.25 5.9791209 8.7677895], [], %eps);
assert_checkalmostequal(meanf(A, A, 1), [6.25 5.9791209 8.7677895], [], %eps);
assert_checkalmostequal(meanf(A, A, "c"), [8.0769231; 7.0369541], [], %eps);
assert_checkalmostequal(meanf(A, A, 2), [8.0769231; 7.0369541], [], %eps);

assert_checkequal(meanf(sparse(A), 3), 34.11);
assert_checkequal(meanf(sparse(A), 3, "*"), 34.11);
assert_checkalmostequal(meanf(sparse(A), 3, "r"), sparse([8 9.1 17.01]), [], %eps);
assert_checkalmostequal(meanf(sparse(A), 3, 1), sparse([8 9.1 17.01]), [], %eps);
assert_checkequal(meanf(sparse(A), 3, "c"), sparse([13;21.11]));
assert_checkequal(meanf(sparse(A), 3, 2), sparse([13;21.11]));

assert_checkalmostequal(meanf(sparse(A), A), 7.4333069, [], %eps);
assert_checkalmostequal(meanf(sparse(A), A, "*"), 7.4333069, [], %eps);
assert_checkalmostequal(meanf(sparse(A), A, "r"), sparse([6.25 5.9791209 8.7677895]), [], %eps);
assert_checkalmostequal(meanf(sparse(A), A, 1), sparse([6.25 5.9791209 8.7677895]), [], %eps);
assert_checkalmostequal(meanf(sparse(A), A, "c"), sparse([8.0769231; 7.0369541]), [], %eps);
assert_checkalmostequal(meanf(sparse(A), A, 2), sparse([8.0769231; 7.0369541]), [], %eps);

assert_checkalmostequal(meanf(A, sparse(A)), 7.4333069, [], %eps);
assert_checkalmostequal(meanf(A, sparse(A), "*"), 7.4333069, [], %eps);
assert_checkalmostequal(meanf(A, sparse(A), "r"), sparse( [6.25 5.9791209 8.7677895]), [], %eps);
assert_checkalmostequal(meanf(A, sparse(A), 1),  sparse([6.25 5.9791209 8.7677895]), [], %eps);
assert_checkalmostequal(meanf(A, sparse(A), "c"),  sparse([8.0769231; 7.0369541]), [], %eps);
assert_checkalmostequal(meanf(A, sparse(A), 2),  sparse([8.0769231; 7.0369541]), [], %eps);

assert_checkequal(meanf(int8(A), 3), int8(34));
assert_checkequal(meanf(int8(A), 3, "*"), int8(34));
assert_checkequal(meanf(int8(A), 3, "r"), int8([8 9 17]));
assert_checkequal(meanf(int8(A), 3, 1), int8([8 9 17]));
assert_checkequal(meanf(int8(A), 3, "c"), int8([13;21]));
assert_checkequal(meanf(int8(A), 3, 2), int8([13;21]));

assert_checkequal(meanf(int8(A), A), int8(0));
assert_checkequal(meanf(A, int8(A)), int8(0));
assert_checkequal(meanf(int8(A), A, "*"), int8(0));
assert_checkequal(meanf(int8(A), A, "r"), int8([6 5 -6]));
assert_checkequal(meanf(int8(A), A, 1), int8([6 5 -6]));
assert_checkequal(meanf(int8(A), A, "c"), int8([8; -5]));
assert_checkequal(meanf(int8(A), A, 2), int8([8; -5]));

assert_checkequal(meanf(A, int8(3)), int8(34));
assert_checkequal(meanf(A, int8(3), "*"), int8(34));
assert_checkequal(meanf(A, int8(3), "r"), int8([8 9 17]));
assert_checkequal(meanf(A, int8(3), 1), int8([8 9 17]));
assert_checkequal(meanf(A, int8(3), "c"), int8([13;21]));
assert_checkequal(meanf(A, int8(3), 2), int8([13;21]));


assert_checkequal(meanf(uint8(A), 3), uint8(34));
assert_checkequal(meanf(A, uint8(3)), uint8(34));
assert_checkequal(meanf(uint8(A), 3, "r"), uint8([8 9 17]));
assert_checkequal(meanf(uint8(A), 3, 1), uint8([8 9 17]));
assert_checkequal(meanf(uint8(A), 3, "c"), uint8([13; 21]));
assert_checkequal(meanf(uint8(A), 3, 2), uint8([13; 21]));

assert_checkequal(meanf(uint8(A), A), uint8(7));
assert_checkequal(meanf(A, uint8(A)), uint8(7));
assert_checkequal(meanf(uint8(A), A, "r"), uint8([6 5 8]));
assert_checkequal(meanf(uint8(A), A, 1), uint8([6 5 8]));
assert_checkequal(meanf(uint8(A), A, "c"), uint8([8;7]));
assert_checkequal(meanf(uint8(A), A, 2), uint8([8;7]));

assert_checkequal(meanf(int16(A), 3), int16(34));
assert_checkequal(meanf(int16(A), A), int16(7));
assert_checkequal(meanf(A, int16(3)), int16(34));
assert_checkequal(meanf(A, int16(A)), int16(7));

assert_checkequal(meanf(uint16(A), 3), uint16(34));
assert_checkequal(meanf(A, uint16(3)), uint16(34));

assert_checkequal(meanf(uint16(A), A), uint16(7));
assert_checkequal(meanf(A, uint16(A)), uint16(7));

assert_checkequal(meanf(int32(A), 3), int32(34));
assert_checkequal(meanf(A, int32(3)), int32(34));

assert_checkequal(meanf(int32(A), A), int32(7));
assert_checkequal(meanf(A, int32(A)), int32(7));

assert_checkequal(meanf(uint32(A), 3), uint32(34));
assert_checkequal(meanf(A, uint32(3)), uint32(34));

assert_checkequal(meanf(uint32(A), A), uint32(7));
assert_checkequal(meanf(A, uint32(A)), uint32(7));

M = matrix( A(:), [1 2 3]);
assert_checkequal(meanf(M, 3), 34.11);
assert_checkequal(meanf(M, 3, "*"), 34.11);
assert_checkalmostequal(meanf(M, 3, "r"), M, [], %eps);
assert_checkalmostequal(meanf(M, 3, 1), M, [], %eps);
assert_checkequal(meanf(M, 3, "c"), matrix( [8 9.1 17.01], [1 1 3]));
assert_checkequal(meanf(M, 3, 2), matrix( [8 9.1 17.01], [1 1 3]));

assert_checkalmostequal(meanf(M, M), 7.4333069, [], %eps);
assert_checkalmostequal(meanf(M, M, "*"), 7.4333069, [], %eps);
assert_checkalmostequal(meanf(M, M, "r"), M, [], %eps);
assert_checkalmostequal(meanf(M, M, 1), M, [], %eps);
assert_checkalmostequal(meanf(M, M, "c"), matrix( [6.25 5.9791209 8.7677895], [1 1 3]), [], %eps);
assert_checkalmostequal(meanf(M, M, 2), matrix( [6.25 5.9791209 8.7677895], [1 1 3]), [], %eps);
