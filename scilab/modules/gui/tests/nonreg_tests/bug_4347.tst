// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- WINDOWS ONLY -->

//
// <-- Non-regression test for bug 4347 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4347
//
// <-- Short Description -->
// messagebox() : buttons are displayed in reverse order with respect to their order in the vector specifying them: Last specified at the left, first specified at the right, what is quite puzzling.

messagebox("Message","Title","",["button 1" "button 2" "button 3"],"modal")

// Check that buttons are displayed in the same order than in the matrix ["button 1" "button 2" "button 3"]