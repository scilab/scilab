// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9599 -->
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9599
//
// <-- Short Description -->
// The error message "at line ... of function ... called by" was printed directly
// on the same line.

// interactive test because "at line ... of function ... called by" can not be catched
// copy paste these lines and check that you have the good display.

function y = f1(x)
  error(msprintf("%s: Ai!","f"))
endfunction

function y = f2(x)
  error(msprintf("%s: Ai!","f") + ascii(10))
endfunction

function y = f3(x)
  error(9999, 'blabla1')
endfunction

function y = f4(x)
  error('blabla2', 9999)
endfunction


// NOW do:
y = f1(1)
y = f2(1)
y = f3(1)
y = f4(1)

// 
assert_checkerror ( "y = f1(1)" , msprintf("%s: Ai!","f"));
assert_checkerror ( "y = f2(1)" , msgerror );
assert_checkerror ( "y = f3(1)" , "blabla1");
assert_checkerror ( "y = f4(1)" , "blabla2");
