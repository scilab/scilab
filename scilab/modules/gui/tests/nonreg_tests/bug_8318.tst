*// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8318 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8318
//
// <-- Short Description -->
// help window was not brought up to front when already opened.

help;

// click in the console

help;

// the help browser should be brought up to front or the window button in
// the taskbar should flash (that depends of the OS).