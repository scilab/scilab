// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1391 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1391
//
// exec SCI/modules/core/tests/nonreg_tests/bug_1391.tst

// <-- INTERACTIVE TEST -->

function ok = test ();
  ok = %F;
  abort;  // crashes Scilab 3.1 but stops running Scilab 
  // never go here
  3.0
  ok = %T;
endfunction;  

test()

disp ("OK");
