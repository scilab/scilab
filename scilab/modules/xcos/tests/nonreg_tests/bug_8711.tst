// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8711 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8711
// http://www.jgraph.org/bugzilla/show_bug.cgi?id=57
//
// <-- Short Description -->
// The creation of a link lead to a translated target point when the origin is
// not (0,0)

xcos(SCI + "/modules/xcos/demos/Cont.Disc-Observer.zcos");
// Delete the link from "Random Generator" to "DEMUX" block.
// Try to recreate it from the "Random Generator" block.
