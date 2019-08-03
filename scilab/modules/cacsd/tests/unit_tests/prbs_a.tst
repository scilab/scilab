// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// --------------------------
// Unitary tests for prbs_a()
// --------------------------

// Checking the output size
assert_checkequal(size(prbs_a(0, 7)), [0 0]);
assert_checkequal(size(prbs_a(100, 0)), [1 100]);
assert_checkequal(size(prbs_a(100,20)), [1 100]);
assert_checkequal(size(prbs_a(100,99)), [1 100]);

// Checking the number of switches
[?,k] = gsort(rand(1,98));
for i = [k(1:19) 99] // 20 random unique numbers in [1, 99], 99 mandatory
    tmp = prbs_a(100, i);
    assert_checkequal(sum(tmp(2:$)<>tmp(1:$-1)), i);
end
// Checking state values
assert_checkequal(prbs_a(0, 5), []);
for i = [k(1:19) 99]
    tmp = unique(prbs_a(100, i));
    assert_checkequal(tmp, [-1 1]);
end

// Checking the equiprobability of the initial state
tmp = []
for i = 1:100
    tmp = [tmp prbs_a(10,4)(1)];
end
assert_checktrue(abs(sum(tmp==1)/100-0.5)<0.15);


// Switches at given positions
// ---------------------------
for nc = [-7 0 5 15 30]    // In this case, nc is ignored
    tmp = prbs_a(20, nc, [6 2 13]);
    assert_checkequal(size(tmp), [1 20]);    // Checking size
    assert_checkequal(unique(tmp), [-1 1]);  // Checking state values
    tmp = tmp(2:$)<>tmp(1:$-1)
    assert_checkequal(sum(tmp), 3);          // Checking number of switches
    assert_checkequal(find(tmp), [2 6 13]);  // Checking the position of switches
end
