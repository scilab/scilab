// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8885 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8885
//
// <-- Short Description -->
// Anchors were removed on a Replace All action.

edit test_run;

// Add anchors (ctrl+M)
// Find action (ctrl+F)
// In Search field put a 'a'
// In Replace Field put a 'b'
// Replace all
// All the anchors should be here !