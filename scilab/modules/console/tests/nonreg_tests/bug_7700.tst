// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
// <-- TEST WITH CONSOLE -->
//
// <-- Non-regression test for bug 7700 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7700
//
// <-- Short Description -->
// Bad proposition in history

abcde=1;
aaaaa=2;

// Enter a 'a' and hit the UP key then the two previous expressions should be proposed
// Hit the DOWN key to come back to 'a' and add a 'b', then hit the UP key
// only abcde=1 must be proposed.