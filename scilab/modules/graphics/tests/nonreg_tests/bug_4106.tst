// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4106 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4106
//
// <-- Short Description -->
// bar and barh override drawlater/drawnow mode.
// 

x = 1:10;
curFig = gcf();
// drawlater
curFig.immediate_drawing = "off";
bar(x,x);
// check that we are still in drawlater mode
if (curFig.immediate_drawing <> "off") then pause; end;

barh(x,x);
// check that we are still in drawlater mode
if (curFig.immediate_drawing <> "off") then pause; end;

