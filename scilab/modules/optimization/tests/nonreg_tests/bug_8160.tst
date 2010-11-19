// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 8160 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8160
//
// <-- Short Description -->
// The derivative function did not check for input argument #1, if it is a list.
//

function flag = MY_assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction


x=[1;2;3];
instr="[J,H]=derivative(list(),x)";
ierr=execstr(instr,"errcatch");
laserr = lasterror();
MY_assert_equal ( ierr , 10000 );
MY_assert_equal ( laserr , "derivative: Wrong number of elements in input argument #1: At least 2 elements expected, but current number is 0." );
//
x=[1;2;3];
instr="[J,H]=derivative(list(""foo""),x)";
err=execstr(instr,"errcatch");
laserr = lasterror();
MY_assert_equal ( ierr , 10000 );
MY_assert_equal ( laserr , "derivative: Wrong number of elements in input argument #1: At least 2 elements expected, but current number is 1." );
//
x=[1;2;3];
instr="[J,H]=derivative(list(""foo"",""aa""),x)";
err=execstr(instr,"errcatch");
laserr = lasterror();
MY_assert_equal ( ierr , 10000 );
MY_assert_equal ( laserr , "derivative: Wrong type for element #1 in input argument #1: A function is expected, but current type is string." );



