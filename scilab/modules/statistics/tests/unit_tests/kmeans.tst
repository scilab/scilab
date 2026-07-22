// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for kmeans function
// =============================================================================

grand("setsd", 0)
x = [2 3; 4 4; 1 4; 2 1; 4 3; 4 2; 5 2; 1 2; 5 1; 3 4];
[index, c] = kmeans(x, 2);

assert_checkequal(index, [2 2 2 1 1 1 1 2 1 2]');
assert_checkequal(c, [4 1.8; 2.2 3.4]);

grand("setsd", 0)
n = 10;
x1 = rand(n, 2, "normal") + 3 * ones(n, 2);
x2 = rand(n, 2, "normal") - 3 * ones(n, 2);
x3 = rand(n, 2, "normal") + [3 -3].*.ones(n, 1);
x4 = rand(n, 2, "normal") + [-3 3].*.ones(n, 1);
x5 = rand(n, 2, "normal") + [1 -1].*.ones(n, 1);
x6 = rand(n, 2, "normal") + [-1 1].*.ones(n, 1);
x = [x1; x2; x3; x4; x5; x6];
[index, c] = kmeans(x, 2);

assert_checkequal(size(index), [60 1]);
assert_checkalmostequal(c, [2.5908718 -0.5057625; -2.9320205 0.4066662], [], 10^-7);

// checkerror
msg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "kmeans", 2);
assert_checkerror("kmeans()", msg);

msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in ""%s"".\n"), "kmeans", 1, "double");
assert_checkerror("kmeans(""1"", 2)", msg);

msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in ""%s"".\n"), "kmeans", 2, "double");
assert_checkerror("kmeans(1, ""2"")", msg);

msg = msprintf(_("%s: Wrong value for input argument #%d: Integer numbers expected.\n"), "kmeans", 2);
assert_checkerror("kmeans(x, 1.5)", msg);

msg = msprintf(_("%s: Wrong value for input argument #%d: Positive numbers expected.\n"), "kmeans", 2);
assert_checkerror("kmeans(x, -2)", msg);

msg = msprintf(_("%s: Wrong value for input argument #%d: Must be less than or equal to %d.\n"), "kmeans", 2, 10);
assert_checkerror("kmeans(rand(10, 2), 11)", msg);
