// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4877 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4877
//
// <-- Short Description -->
// When commenting several lines :
// Even if the first one isn't completely selected, the comment should be added at the begginning of it.

editor SCI/modules/atoms/macros/atomsInstall.sci
// Put the cursor in the middle of a line
// Select several lines
// <Ctlr+D>
