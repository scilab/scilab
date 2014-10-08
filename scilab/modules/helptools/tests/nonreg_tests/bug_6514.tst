// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 6514 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6514
//
// <-- Short Description -->
// "help %k" pointed to the symbols help page.

help %k
// Check that the help page opened is the %k help page and not the sympbols help page