// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

function flag = MY_assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

// Compare real values
order = assert_comparecomplex ( 1 , -1 );
MY_assert_equal ( order , 1 );
//
order = assert_comparecomplex ( -1 , 1 );
MY_assert_equal ( order , -1 );
//
order = assert_comparecomplex ( 1 , 1 );
MY_assert_equal ( order , 0 );
//
order = assert_comparecomplex ( 1 , -1 , %eps , 0 );
MY_assert_equal ( order , 1 );
//
order = assert_comparecomplex ( -1 , 1 , %eps , 0 );
MY_assert_equal ( order , -1 );
//
order = assert_comparecomplex ( 1 , 1 , %eps , 0 );
MY_assert_equal ( order , 0 );
//

// Compare complex values #1
order = assert_comparecomplex ( 1+2*%i , 1+3*%i , %eps , 0 );
MY_assert_equal ( order , -1 );
//
order = assert_comparecomplex ( 1+3*%i , 1+2*%i , %eps , 0 );
MY_assert_equal ( order , 1 );
//
order = assert_comparecomplex ( 1+2*%i , 1+2*%i , %eps , 0 );
MY_assert_equal ( order , 0 );
//
order = assert_comparecomplex ( 1+2*%i , 1+3*%i  );
MY_assert_equal ( order , -1 );
//
order = assert_comparecomplex ( 1+3*%i , 1+2*%i  );
MY_assert_equal ( order , 1 );
//
order = assert_comparecomplex ( 1+2*%i , 1+2*%i  );
MY_assert_equal ( order , 0 );
//

// Compare complex values #2
order = assert_comparecomplex ( 1+%i , -1+%i );
MY_assert_equal ( order , 1 );
//
order = assert_comparecomplex ( -1+%i , 1+%i );
MY_assert_equal ( order , -1 );
//
order = assert_comparecomplex ( 1+%i , 1+%i );
MY_assert_equal ( order , 0 );
//
order = assert_comparecomplex ( 1+%i , -1+%i , %eps , 0 );
MY_assert_equal ( order , 1 );
//
order = assert_comparecomplex ( -1+%i , 1+%i , %eps , 0 );
MY_assert_equal ( order , -1 );
//
order = assert_comparecomplex ( 1+%i , 1+%i , %eps , 0 );
MY_assert_equal ( order , 0 );
//

// Compare with tolerances : equality cases
order = assert_comparecomplex ( 1.2345+%i , 1.2346+%i , %eps , 1.e-3 );
MY_assert_equal ( order , 0 );
//
order = assert_comparecomplex ( 1.2345+%i , 1.2346+%i , 1.e12*%eps , 0 );
MY_assert_equal ( order , 0 );
//
order = assert_comparecomplex ( 1+1.2345*%i , 1+1.2347*%i , %eps , 1.e-3 );
MY_assert_equal ( order , 0 );
//
order = assert_comparecomplex ( 1+1.2345*%i , 1+1.2347*%i , 1.e12*%eps , 0 );
MY_assert_equal ( order , 0 );
//

// Compare more realistic data
x = [
-0.123452 - 0.123454 * %i
-0.123451 + 0.123453 * %i
0.123458 - 0.123459 * %i
0.123456 + 0.123457 * %i
];
// Consider less than 4 significant digits
for i = 1 : size(x,"*")-1
  order(i) = assert_comparecomplex ( x(i) , x(i+1) , 1.e-4 );
end
MY_assert_equal ( order , -ones(3,1) );
clear order;

// Compare data from bug #415
x = [
-1.9914145
-1.895889
-1.6923826
-1.4815461
-1.1302576
-0.5652256 - 0.0655080 * %i
-0.5652256 + 0.0655080 * %i
0.3354023 - 0.1602902 * %i
0.3354023 + 0.1602902 * %i
1.3468911
1.5040136
1.846668
1.9736772
1.9798866
];
// Consider less than 4 significant digits
for i = 1 : size(x,"*")-1
  order(i) = assert_comparecomplex ( x(i) , x(i+1) , 1.e-5 );
end
MY_assert_equal ( order , -ones(13,1) );
clear order;
//
order = assert_comparecomplex ( 1 , 1 );
MY_assert_equal ( order , 0 );
//
order = assert_comparecomplex ( 1+%eps , 1 );
MY_assert_equal ( order , 0 );
//
order = assert_comparecomplex ( 0 , 0 , 0 , 1.e-5 );
MY_assert_equal ( order , 0 );
//
order = assert_comparecomplex ( 1 , 2 );
MY_assert_equal ( order , -1 );
//
order = assert_comparecomplex ( 1.e-4 , 0 , 0 , 1.e-5 );
MY_assert_equal ( order , 1 );
//
// Success: not obvious!
// The two values are equal, very small, but nonzero.
// The relative tolerance must be used here.
// If, instead, a bug in the assert function is so that the 
// absolute tolerance is used as 10^-16, then the output 
// of this comparison is wrong.
order = assert_comparecomplex ( 1.23456789123456789e-30 , 1.23456789123456789e-30 );
MY_assert_equal ( order , 0 );
//
// Failure : not obvious!
// There is only one significant digit here and we require the 
// maximum precision.
// The test must fail because the relative tolerance must be used here.
// If, instead, there is a bug in the comparison and 
// the absolute tolerance is used and set to
// 10^-16, the output of this test is wrong.
order = assert_comparecomplex ( 1.23456789123456789e-30 , 1.3e-30 );
MY_assert_equal ( order , -1 );
//
// Success : not obvious!
// The expected result is zero, so that the absolute tolerance is used.
order = assert_comparecomplex ( 1.23456789123456789e-30 , 0 , 0 , 1.e-10 );
MY_assert_equal ( order , 0 );
//
// Check that slightly different values cannot pass basic test
// Simple sequence with default settings
order = assert_comparecomplex ( 1 + 5 * %eps , 1 , %eps );
MY_assert_equal ( order , 1 );
//
// Customize the relative precision so that a test can pass
// Simple sequence with default settings
order = assert_comparecomplex ( 1 + 5 * %eps , 1 , 10*%eps , 0 );
MY_assert_equal ( order , 0 );
//
// Customize the absolute precision so that a test can pass
// Simple sequence with default settings
order = assert_comparecomplex ( 5 * %eps , 0 , 0 , 10*%eps );
MY_assert_equal ( order , 0 );
//
// An example where the relative error is used,
// with a customized tolerance.
order = assert_comparecomplex ( 1.23456 , 1.23457 , 1.e11*%eps , 0 );
MY_assert_equal ( order , 0 );
//
// Test all IEEE values
// We choose -%inf < 0 < %inf < %nan.
table = [
  %inf 0       1
  %inf  %inf   0
  %inf -%inf   1
  %inf  %nan  -1
 -%inf 0      -1
 -%inf  %inf  -1
 -%inf -%inf   0
 -%inf  %nan  -1
  %nan 0       1
  %nan  %inf   1
  %nan -%inf   1
  %nan  %nan   0
  0     0      0
  0     %inf  -1
  0    -%inf   1
  0     %nan  -1
];
ntests = size(table,"r");
for i = 1 : ntests
  a = table(i,1);
  b = table(i,2);
  expected = table(i,3);
  order = assert_comparecomplex ( a , b );
  MY_assert_equal ( order , expected );
  order = assert_comparecomplex ( b , a );
  MY_assert_equal ( order , -expected );
end

