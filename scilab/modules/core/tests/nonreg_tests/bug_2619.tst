// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 2619 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2619
//
// <-- Short Description -->
//
// errcatch with the "stop" action is broken.
//
// man page : "errcatch(-1,'stop')" : interrupts the current Scilab session (useful
// when Scilab is called from an external program).
//
// => It works well with scilab-4.1.2 but with the trunk version, scilab is not
// closed and the error not displayed. After that, scilab is broken and doesn't
// display results anymore
//
//
// Steps to reproduce the bug:
//
// errcatch(-1,'stop');
// error(10000);

errcatch(-1,'stop');
error(10000);
