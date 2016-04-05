// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9960 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9960
//
// <-- Short Description -->
// There was no simplified assert_checkerror syntax.

function flag = MY_assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

function y=f(varargin)
  [lhs,rhs]=argn()
  if ( rhs <> 1 ) then
    lclmsg = gettext("%s: Wrong number of input argument: %d expected.\n")
    errmsg = sprintf ( lclmsg  , "f" , 1 )
    error(errmsg,1234)
  end
  x = varargin(1)
  y = x
endfunction

// Just check the error number: 
// do not check the error message.
assert_checkerror("f(1,2)",[],1234);

// This is an impossible syntax of assert_checkerror.
// Either the message or the error number must be set to 
// a non empty value.
instr = "f(1,2)";
statement = "assert_checkerror(instr,[],[])";
ierr=execstr(statement,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
ferrmsg = msprintf(gettext("%s: Wrong size for input argument #%d: Non-empty matrix expected.\n"),"assert_checkerror",3);
MY_assert_equal ( lerr , ferrmsg );

