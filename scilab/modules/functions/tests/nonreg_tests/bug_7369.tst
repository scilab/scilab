// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 7619 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7369
//
// <-- Short Description -->
//fun2string may produce invalid output code

function y=foo(ntimes)
  a=33;b=7;
  y  = sqrt(a/ntimes - (b/ntimes)^2);
endfunction

execstr(fun2string(foo,"foo1"));
if foo(4)<>foo1(4) then pause,end
