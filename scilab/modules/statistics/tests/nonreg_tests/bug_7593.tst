// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7593 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7593
//
// <-- Short Description -->
// The function st_deviation was merged into stdev (they were duplicates),
// and msd is now accessible through the third input argument of stdev

x = matrix([5 2:8], 2, 4);

assert_checkalmostequal(stdev(x,"*",mean(x)), 1.870828693387);
assert_checkalmostequal(norm(stdev(x,1,mean(x,"r"))-[3 1 1 1]/2),   0);
assert_checkalmostequal(norm(stdev(x,"r",mean(x,"r"))-[3 1 1 1]/2), 0);
assert_checkalmostequal(norm(stdev(x,2,mean(x,"c"))-sqrt([2;5])),   0);
assert_checkalmostequal(norm(stdev(x,"c",mean(x,"c"))-sqrt([2;5])), 0);

// Biased deviation
assert_checkalmostequal(stdev(x,"*",%nan), 1.870828693387);
assert_checkalmostequal(norm(stdev(x,1,%nan)-[3 1 1 1]/2),   0);
assert_checkalmostequal(norm(stdev(x,"r",%nan)-[3 1 1 1]/2), 0);
assert_checkalmostequal(norm(stdev(x,2,%nan)-sqrt([2;5])),   0);
assert_checkalmostequal(norm(stdev(x,"c",%nan)-sqrt([2;5])), 0);

// With scalar means
rand("seed", 0);
x = rand(3, 10) + 0.5;
refR = [
0.364541128374
0.155589477734
0.316156843180
0.268553719068
0.219155131832
0.273506665569
0.262844435686
0.318334990307
0.149138182829
0.111372370623 ]';
refC = [
0.299608392744
0.216862008097
0.245620889607 ];
assert_checkalmostequal(stdev(x, "*", 1), 0.256335678990);
assert_checkalmostequal(stdev(x, 1, 1),   refR);
assert_checkalmostequal(stdev(x, "r", 1), refR);
assert_checkalmostequal(stdev(x, 2, 1),   refC);
assert_checkalmostequal(stdev(x, "c", 1), refC);

// Scalar a priori mean for hypermatrix
grand("setsd", 0);
x = grand(4, 3, 2, "unf", 0, 10);
refHM = [2.09659394107671737 2.55579559873818374 1.1805640530346788];
refHM(:, :, 2) = [3.14196221819646615 2.64930672717197169 2.1435288607041616];
assert_checkalmostequal(stdev(x, "r", 5), refHM);

// Error checks
refMsg = msprintf(_("%s: Wrong size for input argument #%d.\n"),"stdev",3);
assert_checkerror("stdev(x, ""*"", [1 1])",   refMsg);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"),"stdev",3);
assert_checkerror("stdev(x, ""*"", ""msd"")", refMsg);
