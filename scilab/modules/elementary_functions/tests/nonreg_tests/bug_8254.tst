// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 8254 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8254
//
// <-- Short Description -->
// The nearfloat function does not work properly for near denormalized numbers.

// <-- JVM NOT MANDATORY -->

//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if ( and ( computed == expected ) ) then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

//
// Test at vmin, the smallest normal
// x = 2.2250738585072013D-308;
x = 1 * 2^-1022; 
computed = nearfloat("succ",x);
assert_equal ( computed , 1.0000000000000002 * 2^-1022 );
computed = nearfloat("pred",x);
assert_equal ( computed , 1.9999999999999995 * 2^-1023 );

