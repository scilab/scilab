// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2819 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2819
//
// <-- Short Description -->
// When setting the property "auto_ticks" of the Axes, it is not set properly.

a=gca();
a.auto_ticks=["off" "off" "off"];
if (a.auto_ticks <> ["off", "off", "off"]) then pause; end;

a.auto_ticks=["on" "on" "off"];
if (a.auto_ticks <> ["on", "on", "off"]) then pause; end;


