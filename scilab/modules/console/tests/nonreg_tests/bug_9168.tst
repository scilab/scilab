// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
// <-- TEST WITH CONSOLE -->
//
// <-- Non-regression test for bug 9168 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9168
//
// <-- Short Description -->
// Bad command line width when completion window was shew at the console bottom.

// Go to the bottom of the console in hiting <RETURN> several times.
// Hit A, <TAB> and complete with abcd, <RETURN>.
// Press A until more than the console width is gained...