// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4868 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4868
//
// <-- Short Description -->
// editor() deletes blanks lines when commenting region

editor()

//Type :
//  "comment 1"
//  ""
//  "comment 2"
//
//
// Select the previous text
//
// <CTRL+D>
