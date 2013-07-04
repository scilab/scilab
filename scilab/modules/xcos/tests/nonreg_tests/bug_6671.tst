// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan SIMON 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 6671 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6671
//
// <-- Short Description -->
// changing style of a block in a superBlockDiagram wasn't kept when reloading it
//
//
// start scilab
// start xcos
// open demo Cont.Disc-Observer.zcos
// double click on the second super block
// change the direction of the gain block
// close the superBlockDiagram
// save it
// reopen it
// reopen the second super block
// the gain block should have kept the new direction
