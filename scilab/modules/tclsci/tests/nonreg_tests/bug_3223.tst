// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3223 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3223
//

if getos() == 'Windows' then
  ierr = execstr("TCL_EvalStr(''package require registry'')",'errcatch');
  assert_checkequal(ierr, 999);
  err_msg = lasterror();
  tab_str = ascii(9) +"can''t find package registry"; 
  ref_err = ["TCL_EvalStr,  at line 1" ; ..
             tab_str; ..
             "    while executing" ; ..
             """package require registry"""];
  // current version of assert_checkerror does not manage multiline error (bug 9572)
  assert_checkequal(err_msg, ref_err);
end
