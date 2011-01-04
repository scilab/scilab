// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 8719 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8719
//
// <-- Short Description -->
// The karmarkar function prints unwanted messages.
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

c = [-20 -24 0 0]';
a = [
3 6 1 0
4 2 0 1
];
b = [60 32]';
expected = [4 8 0 0]';
x0 = [
4.1128205
7.7333333
1.2615385
0.0820513
];
xopt=karmarkar(a,b,c,x0);
assert_close ( xopt , expected , 1.e-3 );

