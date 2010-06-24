// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


// <-- TEST WITH SCINOTES -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 5083 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5083
//
// <-- Short Description -->
// If lines are selected and user call the "Find/replace" dialog box, the radiobox "Selected lines" in
// the frame "Scope" should be checked by default.

editor SCI/modules/atoms/macros/atomsSearch.sci
// Select several lines
// CTRL + F






