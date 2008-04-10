// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 187 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=187
//
// <-- Short Description -->
//     

// <-- INTERACTIVE TEST -->

ew=evstr(x_mdialog("Matrix to edit",["a" "b"],["c","d"],string(rand(2,2))))