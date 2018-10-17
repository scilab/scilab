// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15814 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15814
//
// <-- Short Description -->
// Selecting graphic children with booleans failed

clf
t = 0:0.01:2*%pi;
polarplot(t, -1 + sin(t));
elements = gca().children;
angFrame = elements.type=="Arc";
elements(angFrame).foreground = 2;
