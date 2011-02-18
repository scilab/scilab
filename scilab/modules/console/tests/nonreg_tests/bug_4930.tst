// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH CONSOLE -->
//
// <-- Non-regression test for bug 4930 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/4930
//
// <-- Short Description -->
// When the console was resized from 0-height to x-height, lines()
// was not correctly updated.

a=lines();a(2)

// With the mouse reduce the height of the console to 0.
// Increase the height to something normal

a=lines();a(2)

// Should not be 0