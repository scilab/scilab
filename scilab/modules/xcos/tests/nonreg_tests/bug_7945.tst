// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 7945 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7945
//
// <-- Short Description -->
// Fix the Xcos title to be coherent with the Scilab plateform

xcos();
// check that the diagram title is "Untitled - Xcos"
// check that the palette browser title is "Palette browser - Xcos"

xcos("SCI/modules/xcos/demos/Electrical/Boost_Converter.xcos");
// check that the title is "Boost_Converter (SCI/modules/xcos/demos/Electrical/Boost_Converter.xcos) - Xcos"

xcos("SCI/modules/scicos/demos/Bouncing_ball.cosf");
// check that the title is "*Bouncing_ball - Xcos"

