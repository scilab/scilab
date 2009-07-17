// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
  if expected==0.0 then
    shift = norm(computed-expected);
  else
    shift = norm(computed-expected)/norm(expected);
  end
  if shift < epsilon then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

// unit tests for gamma() function
// =============================================================================

assert_equal ( isnan(gamma(-1.e308)) , %T );
assert_close ( gamma(-170.6) ,  -2.082900836150297478e-308 , 1.e2 * %eps );
assert_close ( gamma(-1.5) , 2.36327180120735480529 , %eps );
assert_equal ( isnan(gamma(-2.)) , %T );
assert_equal ( isnan(gamma(-1.)) , %T );
assert_close ( gamma(-0.5) , -3.5449077018110317638389 , %eps );
assert_equal ( gamma(0.0) , %inf );
assert_close ( gamma(1.e-307) , 1.e307 , %eps );
assert_close ( gamma(1.e-100) , 1.e100 , %eps );
assert_close ( gamma(1.e-30) , 1.e30 , %eps );
assert_close ( gamma(0.5) , 1.7724538509055161039640 , %eps );
assert_equal ( gamma(1.0) , 1.0 );
assert_equal ( gamma(2.0) , 1.0 );
assert_equal ( gamma(3.0) , 2.0 );
assert_equal ( gamma(4.0) , 6.0 );
assert_equal ( gamma(5.0) , 24.0 );
assert_equal ( gamma(10.0) , 362880.0 );
assert_close ( gamma(13.0) , 479001600.0 , 1000 * %eps );
assert_close ( gamma(171) , 7.257415615308056415e+306 , 1.e3 * %eps );
assert_equal ( gamma(172) , %inf );
assert_equal ( gamma(%inf) , %inf );
assert_equal ( gamma(-%inf) , %inf );

