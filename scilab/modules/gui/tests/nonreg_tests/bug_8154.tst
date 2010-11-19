*// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8154 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8154
//
// <-- Short Description -->
// It was not possible to cancel a messagebox with ESC key.

messagebox("hello tester !");

// and press the ESCAPE key, the messagebox must be closed.
