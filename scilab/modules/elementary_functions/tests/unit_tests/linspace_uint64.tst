// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// ------
// uint64
// ------
// Intervals with relative widths < %eps
// .....................................
// increasing
a = linspace(uint64(2)^63, uint64(2)^63+5, 6)';
assert_checkequal(diff(a,1,1), uint64(ones(5,1)));
a = linspace(uint64(2)^63, uint64(2)^63+5, 10)';
assert_checkequal(diff(a,1,1)', uint64([0 1 0 1 0 1 0 1 1]));

// decreasing
a = linspace(uint64(2)^63, uint64(2)^63-5, 6)'
d = a(1:$-1)-a(2:$)
assert_checkequal(d, uint64(ones(5,1)));
a = linspace(uint64(2)^63, uint64(2)^63-5,10)'
assert_checkequal(diff(a($:-1:1),1,1)', uint64([1 1 0 1 0 1 0 1 0]));

// Big intervals with widths > 1/%eps
// ..................................
// increasing - scalar
n = 3;
a = linspace(uint64(1), uint64(2)^63, n)'
d = diff(a,1,1);
assert_checkequal((d(1)-d)', uint64([0 1]));
// increasing - column-wise
a = linspace(uint64([1 10]'), (uint64(2)^63)+[1 10]', n)'
d = diff(a,1,1);
assert_checkequal((d(:,1)*ones(1,n-1)-d)', uint64([0 0;0 0]));
n = 11;
a = linspace(uint64([1 17]'), (uint64(2)^63)+[1 56]', n)'      // OK
d = diff(a,1,1);
//assert_checkequal(ones(n-1,1)*d(1,:)-d, uint64([0 0;0 0]));  // http://bugzilla.scilab.org/15836

// decreasing
n = 3;
a = linspace(uint64(2)^63, uint64(1), n)'
d = diff(a($:-1:1),1,1);
assert_checkequal((d-d(1))', uint64([0 1]));
n = 11;
a = linspace(uint64(2)^63, uint64(1), n)';        // http://bugzilla.scilab.org/15836
d = diff(a($:-1:1),1,1);
//assert_checkequal((d-d(1))', uint64([0 1]));

// -----
// int64
// -----
// Intervals with relative widths < %eps
// .....................................
// increasing
n = 3;
a = linspace(-(int64(2)^63), int64(2)^63-1, n)
d = diff(a,1,2);
assert_checkequal(d-d(1), int64([0 -1]));
n = 4;
a = linspace(-(int64(2)^63), int64(2)^63-1, n);
d = diff(a,1,2)
assert_checkequal(d-d(1), int64([0 0 0]));
n = 11;
a = linspace(-(int64(2)^63), int64(2)^63-1, n);
d = diff(a,1,2)
assert_checkequal(d-d(1), int64([0 -1  0 -1  0 -1  0 -1  0 -1]));

// decreasing
a = linspace(int64(2)^62, int64(2)^62-5, 6)'
d = a(1:$-1)-a(2:$)
assert_checkequal(d, int64(ones(5,1)));
a = linspace(int64(2)^62, int64(2)^62-5,10)'
assert_checkequal(diff(a($:-1:1),1,1)', int64([1 1 0 1 0 1 0 1 0]));

// Big intervals with widths > 1/%eps
// ..................................
// increasing - scalar
n = 3;
a = linspace(int64(1), int64(2)^62, n)'
d = diff(a,1,1);
assert_checkequal((d(1)-d)', int64([0 1]));

// increasing - column-wise
a = linspace(int64([1 10]'), (int64(2)^62)+[1 10]', n)'
d = diff(a,1,1);
assert_checkequal((d(:,1)*ones(1,n-1)-d)', int64([0 0;0 0]));
n = 11;
a = linspace(int64([1 17]'), (int64(2)^62)+[1 56]', n)'       // OK
d = diff(a,1,1);
//assert_checkequal(ones(n-1,1)*d(1,:)-d, int64([0 0;0 0]));  // http://bugzilla.scilab.org/15836

// decreasing
n = 3;
a = linspace(int64(2)^62, int64(1), n)'
d = diff(a($:-1:1),1,1);
assert_checkequal((d-d(1))', int64([0 1]));
n = 11;
a = linspace(int64(2)^62-1, int64(1), n)'
d = diff(a($:-1:1),1,1);
assert_checkequal((d-d(1))', int64([0 0 0 1 0 0 1 0 0 1]));
n = 137;
a = linspace(int64(2)^62-1, int64(1), n)';
d = diff(a($:-1:1),1,1);
assert_checkequal(max(abs(d-d(1))), int64(1));
