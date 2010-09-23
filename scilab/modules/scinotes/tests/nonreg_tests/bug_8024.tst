// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8024 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8024
//
// <-- Short Description -->
// The key INSERT had no effect

edit foo;

// Hit the key "Insert", the caret should become a filled rectangle, a message
// is displayed in the infoBar and the entered text overwrites the previous one.
