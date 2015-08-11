// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8368 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8368
//
// <-- Short Description -->
// Incremental search did not highlight correctly the pattern under Windows.

edit numderivative

// F3 to make an incremental search, put f in the text field and hit 2 times
// the ENTER key, a 'f' must be highlighted rather than a 'u'.
