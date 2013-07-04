// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8441 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8441
//
// <-- Short Description -->
// Block with an empty gui string are translated to xcos_block one. These blocks 
// must be removed from the diagram before any simulation.

status = importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_8441.zcos");
if ~status then pause, end

// compile and simulate
xcos_simulate(scs_m, 4);

