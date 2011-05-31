// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 9527 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9527
//
// <-- Short Description -->
// Problem when text was exported in SVG in using the gui

xtitle('abcde');

// ctrl+E to export
// choose svg file format
// export