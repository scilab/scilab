// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Sylvestre Koumar
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2914 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2914
//
// <-- Short Description -->
// Its not possible to paint the background of axes if box property is set to off

fig = scf();
fig.background = 3;

axes = gca();
axes.filled = "on"; // force background drawing
axes.box = "off"; // disable box drawing
axes.background = 2;

// axes background should be drawn
// could be check with some image processing







