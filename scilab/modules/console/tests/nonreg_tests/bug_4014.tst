// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4014 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4014
//
// <-- Short Description -->
// ESC key clears the command line in the console. 

// <-- INTERACTIVE TEST -->

// 1] Type some commands into Console
// Invoke Preferences -> Clear Console action 
// checks prompt position

// 2] Type some commands into Console
// type : clc
// checks prompt position

// 3] Type some commands into Console
// F2 key
// checks prompt position

// prompt position must be same with these 3 cases