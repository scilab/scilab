// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 9186 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9186
//
// <-- Short Description -->
// In code navigator, an exception was thrown when the textfield got the focus.

edit ilib_compile;

// CTRL+G and click in the textfield at the bottom.