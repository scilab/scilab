// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3800 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3800
//
// <-- Short Description -->
// After using surf giving a stacksize error, the figure saty in drawlater mode.
// 

fig = gcf();
fig.immediate_drawing = "on";

// will require too much stack
execstr("x = linspace(-%pi,%pi,1000)''; surf(x,x,sin(x)*cos(x)'')",'errcatch','n');

// check that immediate drawing is still on
if (fig.immediate_drawing <> "on") then pause; end



