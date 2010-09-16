// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8027 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8027
//
// <-- Short Description -->
// In a splitted view, a frame was not informed of a change in the other frame.

edit foo;

// Split the view (Window > Split View > Horizontally)
// Make a modif in the left frame and save (CTRL + S) and go to the right one
// No warning about an external modif !
