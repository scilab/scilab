// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2200 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2200
//
// <-- Short Description -->
// When is resized in "drawlater mode", then the window is cleared.
warning("off");
f=gcf();
drawlater();
xrect([0.2 0.7 0.5 0.5]);
drawnow();
warning("on");
// Resize the figure, it should not be cleared...


