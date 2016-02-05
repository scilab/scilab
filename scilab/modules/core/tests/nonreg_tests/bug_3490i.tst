// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3490 (interactive part) -->
//
// <-- CLI SHELL MODE -->
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3490
//
// <-- INTERACTIVE TEST -->
// <-- Short Description -->
// "resume" inside an exec crashes Scilab + "resume" restriction of use removed

  //the following instructions has to be copied/pasted in the Scilab console
  clear aa bb  foo
  pause
  [aa,bb] = resume(1.1,2.2);
  if aa<>1.1|bb<>2.2 then pause,end


  deff("y=foo()","y=0;pause")
  if foo()<>3 then pause,end
  y=resume(3);

  clear foo
  deff("y=foo()","y=0;pause")
  if foo()<>3 then pause,end
  y=resume(3);

