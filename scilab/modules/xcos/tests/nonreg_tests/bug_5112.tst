/ =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 5112 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5112
//
// <-- Short Description -->
// When opening several diagrams at the same time :
// → Some of them are marked as modified
// → Title of some of them is "Untitled"
// When opening the same diagrams separately, this behavior doesn't happen.

A = ls(SCI+'/modules/xcos/demos/*.zcos');
xcos(A);

// Check that no title is set to Untitled
// Check that no diagram is set as modified