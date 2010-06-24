// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 2623 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2623
//
// <-- Short Description -->
//
// If an error "n" is catch with an errcatch(-1,'pause'), the only way to quit
// the pause mode is abort. "resume" and "return" don't seem to 
// work

// <-- INTERACTIVE TEST -->

errcatch(-1,'pause');
error(1);
resume;

// enter
// enter
