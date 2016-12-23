// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 14640 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14640
//
// <-- Short Description -->
// median(int8([10 60 80 100])) returned -58 instead of 70 due to (60+80)>128 overflow
x = int8([ 10    60   80  100
          -10   -60  -80 -100
          -120 -122 -124 -126
          -122 -122 -126 -126
         ]);
assert_checkequal(median(x), int8(-110));
assert_checkequal(median(x,"c"), int8([70 -70 -123 -124]'));
assert_checkequal(median(x,"r"), int8([-65 -91 -102 -113]));

// with hypermatrices
clear x
x(:,:,1) = int8([
    10    60   80  100
   -10   -60  -80 -100
   ]);
x(:,:,2) = int8([
   -120 -122 -124 -126
   -122 -122 -126 -126
    ]);
expected = int8(matrix([0 0 0 0 -121 -122 -125 -126],1,4,-1));
assert_checkequal(median(x,1), expected);
clear expected
expected = int8(matrix([70 -70 -123 -124],2,1,2));
assert_checkequal(median(x,2), expected);
clear expected
expected = int8([
 -55 -31 -22 -13
 -66 -91 -103 -113
 ]);
assert_checkequal(median(x,3), expected);

//
//see http://bugzilla.scilab.org/14647
//b = int64(2)^62;
//x = b + ..
//    int64([10    60   80  100
//          -10   -60  -80 -100
//          -120 -122 -124 -126
//          -122 -122 -126 -126
//         ]);
//assert_checkequal(median(x), b + int64(-110));
//assert_checkequal(median(x,"c"), b + int64([70 -70 -123 -124]'));
//assert_checkequal(median(x,"r"), b + int64([-65 -91 -102 -113]));
