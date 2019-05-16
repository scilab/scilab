// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 15374 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15374
//
// <-- Short Description -->
// Trivial infinite loop cannot be interrupted

// following loop should be interruptible by CTRL-C on the command line,
// Control/Interrupt and Control/Abort menu items

while 1;end