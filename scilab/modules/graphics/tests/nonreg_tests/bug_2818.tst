// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2171 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2171
//
// <-- Short Description -->
// incompatibility between Scilab-4.1.2 and Scilab-5.0.alpha
// when an empty graphic window is opened, the axes box is drawn in scilab5

clf();
axes = gca();
// no box should be present
if (axes.box == "on") then pause; end;

