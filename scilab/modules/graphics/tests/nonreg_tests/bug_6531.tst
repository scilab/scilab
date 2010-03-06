// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 6531 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6531
//
// <-- Short Description -->
// the caption's default background color changes when a color is added to the current colormap
// 

plot(sin(0:10));e=gce();
p=e.children(1);
c=captions(p,'foo');
c.background //-->-2 ok
p.foreground=color(200,128,33);

// The caption's background color should remain equal to -2
if (c.background <> -2) then pause; end

