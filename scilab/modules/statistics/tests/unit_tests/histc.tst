// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
//<-- CLI SHELL MODE -->
//<-- NO CHECK REF -->

rand("seed", 0);
d = rand(1, 10000, "normal");
[cfC, indC] = histc(20, d);

refCF = [
   0.0002572096
   0.00102883842
   0.00514419209
   0.00925954575
   0.02700700845
   0.07356194682
   0.12860480213
   0.20576768341
   0.30376454264
   0.36472321885
   0.39713162899
   0.36317996123
   0.28987522401
   0.19033510716
   0.11008571063
   0.06121588582
   0.02674979884
   0.01028838417
   0.00282930565
   0.00128604802
]';
assert_checkequal(size(indC), [1 10000]);
assert_checkalmostequal(cfC, refCF,1e-7);
[cfC, indC] = histc(int32(20), d);
assert_checkequal(size(indC), [1 10000]);
assert_checkalmostequal(cfC, refCF, 1e-7);

// Same test, without normalization
[cfC, indC] = histc(20, d, normalization=%f);

refCF = [
1
4
20
36
105
286
500
800
1181
1418
1544
1412
1127
740
428
238
104
40
11
5 ]';
assert_checkequal(size(indC), [1 10000]);
assert_checkequal(cfC, refCF);
[cfC, indC] = histc(int16(20), d, normalization=%f);
assert_checkequal(size(indC), [1 10000]);
assert_checkequal(cfC, refCF);


// With x instead of n as first argument
cfC = histc([-5 0 5], d);
refCF = [0.09982   0.10018];
assert_checkequal(cfC, refCF);
cfC = histc(int8([-5 0 5]), d);
assert_checkequal(cfC, refCF);
cfC = histc(int16([-5 0 5]), d);
assert_checkequal(cfC, refCF);
cfC = histc(int32([-5 0 5]), d);
assert_checkequal(cfC, refCF);
cfC = histc([-5 0 5], d, %f);
assert_checkequal(sum(cfC), 10000); // -5 > d > 5, and d has 10000 elements.



lambda = 2;
grand("setsd", 0);
D = grand(100000, 1, "exp", 1/lambda);
[cfC, indC] = histc(40, D);

refCF = [
   1.6987118D+00
   1.2440861D+00
   9.0843138D-01
   6.5661150D-01
   4.8963575D-01
   3.5287585D-01
   2.5718177D-01
   1.9447913D-01
   1.4149098D-01
   9.7271105D-02
   7.3615681D-02
   5.0401824D-02
   3.8416409D-02
   2.8512672D-02
   2.1447585D-02
   1.4634823D-02
   1.0660711D-02
   8.2005472D-03
   5.8665453D-03
   4.2895170D-03
   4.1633547D-03
   1.3877849D-03
   1.4508660D-03
   9.4621698D-04
   7.5697358D-04
   7.5697358D-04
   4.4156792D-04
   3.1540566D-04
   2.5232453D-04
   1.8924340D-04
   6.3081132D-05
   1.8924340D-04
   1.2616226D-04
   6.3081132D-05
   6.3081132D-05
   0.0000000D+00
   0.0000000D+00
   6.3081132D-05
   0.0000000D+00
   6.3081132D-05
 ]';
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF, 1e-7);
[cfC, indC] = histc(int8(40), D);
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF, 1e-7);

// Same test, without normalization
[cfC, indC] = histc(40, D, normalization=%f);

refCF = [
26929
19722
14401
10409
7762
5594
4077
3083
2243
1542
1167
799
609
452
340
232
169
130
93
68
66
22
23
15
12
12
7
5
4
3
1
3
2
1
1
0
0
1
0
1 ]';
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF);
[cfC, indC] = histc(int32(40), D, normalization=%f);
assert_checkequal(size(indC), [100000 1]);
assert_checkalmostequal(cfC, refCF);

// With x instead of n as first argument
cfC = histc([0 7], D);
refCF = 1/7;
assert_checkalmostequal(cfC, refCF);
cfC = histc(int8([0 7]), D);
assert_checkalmostequal(cfC, refCF);
cfC = histc(int16([0 7]), D);
assert_checkalmostequal(cfC, refCF);
cfC = histc(int32([0 7]), D);
assert_checkalmostequal(cfC, refCF);
cfC = histc([0 7], D, %f);
assert_checkequal(cfC, 100000); // 0 > D > 7, and D has 100000 elements.

// Error checks
refMsg = msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"), "histc", 2);
assert_checkerror("histc(1)", refMsg);
assert_checkerror("histc()", refMsg);

refMsg = msprintf(_("%s: Wrong type of input argument #%d: A single boolean expected.\n"), "histc", 3);
assert_checkerror("histc(1, 1, normalization=[%t %t])", refMsg);
assert_checkerror("histc(1, 1, [%t %t])", refMsg);
assert_checkerror("histc(1, 1, """")", refMsg);
assert_checkerror("histc(1, 1, 1)", refMsg);

refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the interval %s.\n"), "histc", 1, "[1, oo)");
assert_checkerror("histc(0, 1, %t)", refMsg);
assert_checkerror("histc(-1, 1)", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: Integer expected.\n"), "histc", 1);
assert_checkerror("histc(1.5, 1, %t)", refMsg);

refMsg = msprintf(_("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"), "histc", 1);
assert_checkerror("histc([2 1], 1, %t)", refMsg);
assert_checkerror("histc([1 1], 1, %t)", refMsg);

refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "histc", 1);
assert_checkerror("histc(list(), 1)", refMsg);
assert_checkerror("histc("""", 1)", refMsg);
assert_checkerror("histc(%t, 1)", refMsg);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "histc", 2);
assert_checkerror("histc(1, list())", refMsg);
assert_checkerror("histc(1, """")", refMsg);
assert_checkerror("histc(1, %t)", refMsg);
