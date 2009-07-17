// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 4426 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4426
//
// <-- Short Description -->
// when we choose a file from the *root* directory of a drive the first character of the filename is cut off.

// Try this line

[FILE,PATH]=uigetfile('*.txt','/','Choose file')


