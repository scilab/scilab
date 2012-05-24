// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bernard Hugueney
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 7140 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7140
//
// <-- Short Description -->
// stacksize() and gstacksize() used to crash when called from parallel_run.
// They are now forbidden with an error message.

function f(n)
  stacksize('min');
  gstacksize('min');
endfunction

f(1);
if execstr("parallel_run(1:2,f);","errcatch")==0 then pause,end
f(1);