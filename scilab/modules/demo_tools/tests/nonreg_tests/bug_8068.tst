// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 8068 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8068
//
// <-- Short Description -->
// When I run a demo via the demo_gui interface, some times several windows are opened (10001, 10002, etc ...). When I start a new demo, only the window 10001 is closed. The demo_gui script should look after other windows and, if they exist, it closes them.

// Launch Scilab Demonstration GUI: Menu ?/Scilab Démonstrations

// Launch demo CACSD/PID => Discrete Time => Time response => Impulse response => Cancel all message boxes

// Click on Graphics category

// Check that all figures related to CACSD/PID have been closed

