// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3527 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3527
//
// <-- Short Description -->
// Graphic windows created by figure are unable to display plots.
// 

h1 = figure(1);
plot([1,2,5,4,5,2,1]);

// check that the plot is visible
axes = gca();
cmp = gce();
curve = cmp.children(1);
if (h1.visible <> "on") then pause; end
if (axes.visible <> "on") then pause; end
if (cmp.visible <> "on") then pause; end
if (curve.visible <> "on") then pause; end
