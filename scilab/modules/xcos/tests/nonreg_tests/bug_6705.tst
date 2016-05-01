// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 6705 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6705
//
// <-- Short Description -->
// When drag-and-dropping, port alignment is not right

// xcos();
// Put a BIGSOM_f block (#1) by right click and the "Add to" action.
// Put another BIGSOM_f (#2) by a single drag-and-drop (with an input port next
// to #1 output port)
//
// Linking #1 output port to the #2 input port should be a straight horizontal
// line.
