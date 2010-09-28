// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- JVM NOT MANDATORY -->
//
//
// <-- Non-regression test for bug 8032 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8032
//
// <-- Short Description -->
// cdfnor was able to fail silently

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


Mean = 0;
Std = 1;
P = 0.5;
Q = 0.5;
X = cdfnor("X",Mean,Std,P,Q);
assert_equal ( X , 0 );

Mean = 1;
X = 1;
P = 0.5;
Q = 0.5;
Std = cdfnor("Std",P,Q,X,Mean);
assert_equal ( isnan(Std) , %t );

