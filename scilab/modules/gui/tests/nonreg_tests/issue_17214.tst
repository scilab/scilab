// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 17214 -->
//
// <-- Bugzilla URL -->
// https://gitlab.com/scilab/scilab/-/issues/17214
//
// <-- Short Description -->
// Statement findobj without arguments returns [] instead of list of all components.

u = uicontrol("style","text", ...
    "string", "This is a figure", ...
    "position", [50 70 100 100], ...
    "fontsize", 15, ...
    "tag", "Alabel");

// Find the object which "tag" value is "Alabel"
lab = findobj("tag", "Alabel");
assert_checktrue(lab == u);

all = findobj();
assert_checktrue(size(all) == [8 1]);
