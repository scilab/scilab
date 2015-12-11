//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2388 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2388
//
// <-- Short Description -->
//graphic handles can be appended to real vectors
if execstr('[123,gca()]','errcatch')==0 then pause,end
if execstr('[123;gca()]','errcatch')==0 then pause,end
