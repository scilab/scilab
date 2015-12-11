// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12927 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12927
//
// <-- Short Description -->
// fix integer input in ones, rand, eye, zeros


//check errors
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "ones", 1);
assert_checkerror("ones(int32([5 5]), 4)", refMsg);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "ones", 2);
assert_checkerror("ones(4, int32([5 5]))", refMsg);

refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "zeros", 1);
assert_checkerror("zeros(int32([5 5]), 4)", refMsg);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "zeros", 2);
assert_checkerror("zeros(4, int32([5 5]))", refMsg);

refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "rand", 1);
assert_checkerror("rand(int32([5 5]), 4)", refMsg);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "rand", 2);
assert_checkerror("rand(4, int32([5 5]))", refMsg);

refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "eye", 1);
assert_checkerror("eye(int32([5 5]), 4)", refMsg);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "eye", 2);
assert_checkerror("eye(4, int32([5 5]))", refMsg);

//return []
assert_checkequal(ones(4, -int32(5)), []);
assert_checkequal(ones(-4, int32(5)), []);
assert_checkequal(ones(-4, -int32(5)), []);
assert_checkequal(ones(int32(5), -4), []);
assert_checkequal(ones(-int32(5), 4), []);
assert_checkequal(ones(-int32(5), -4), []);
assert_checkequal(ones(-int32(5), -int32(5)), []);

assert_checkequal(zeros(4, -int32(5)), []);
assert_checkequal(zeros(-4, int32(5)), []);
assert_checkequal(zeros(-4, -int32(5)), []);
assert_checkequal(zeros(int32(5), -4), []);
assert_checkequal(zeros(-int32(5), 4), []);
assert_checkequal(zeros(-int32(5), -4), []);
assert_checkequal(zeros(-int32(5), -int32(5)), []);

assert_checkequal(rand(4, -int32(5)), []);
assert_checkequal(rand(-4, int32(5)), []);
assert_checkequal(rand(-4, -int32(5)), []);
assert_checkequal(rand(int32(5), -4), []);
assert_checkequal(rand(-int32(5), 4), []);
assert_checkequal(rand(-int32(5), -4), []);
assert_checkequal(rand(-int32(5), -int32(5)), []);

assert_checkequal(eye(4, -int32(5)), []);
assert_checkequal(eye(-4, int32(5)), []);
assert_checkequal(eye(-4, -int32(5)), []);
assert_checkequal(eye(int32(5), -4), []);
assert_checkequal(eye(-int32(5), 4), []);
assert_checkequal(eye(-int32(5), -4), []);
assert_checkequal(eye(-int32(5), -int32(5)), []);

//
dblX = 5;
dblY = 5;
intX = int32(dblX);
intY = int32(dblY);

assert_checkequal(ones(intX, dblY), ones(dblX, dblY));
assert_checkequal(ones(dblX, intY), ones(dblX, dblY));
assert_checkequal(ones(intX, intY), ones(dblX, dblY));

assert_checkequal(zeros(intX, dblY), zeros(dblX, dblY));
assert_checkequal(zeros(dblX, intY), zeros(dblX, dblY));
assert_checkequal(zeros(intX, intY), zeros(dblX, dblY));

rand("seed", 0)
randInt1 = rand(intX, dblY);
randInt2 = rand(dblX, intY);
randInt3 = rand(intX, intY);
rand("seed", 0)
randDbl1 = rand(dblX, dblY);
randDbl2 = rand(dblX, dblY);
randDbl3 = rand(dblX, dblY);

assert_checkequal(randInt1, randDbl1);
assert_checkequal(randInt2, randDbl2);
assert_checkequal(randInt3, randDbl3);


assert_checkequal(eye(intX, dblY), eye(dblX, dblY));
assert_checkequal(eye(dblX, intY), eye(dblX, dblY));
assert_checkequal(eye(intX, intY), eye(dblX, dblY));


