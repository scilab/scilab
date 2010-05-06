// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4837 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4837
//
// <-- Short Description -->
// uigetfile()/uigetfile() shall accept relative paths for the dir argument.

// Test for uigetfile
cd SCI
uigetfile("*.*","./tools")
// Check that the directory opened by default is SCI/tools

// Test for uigetdir
cd SCI
uigetdir("./tools")
// Check that the directory opened by default is SCI/tools
