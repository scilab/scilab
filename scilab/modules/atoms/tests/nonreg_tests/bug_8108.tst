// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8108 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8108
//
// <-- Short Description -->
// The number module cannot be removed from current ATOMS installation.

atomsInstall("number");
atomsRemove("number");

// => number should be correctly removed
