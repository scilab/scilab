// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 12651 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12651
//
// <-- Short Description -->
// The scicos_simulate's 'nw' flag did not works while re-using an Info
// simulation state.

// this demo contains 2 scopes
assert_checktrue(importXcosDiagram("SCI/modules/xcos/demos/Simple_Demo.zcos"));

// clear any simulation state
Info = list();

// first call perform a full-compilation, then ignore some blocks
Info = scicos_simulate(scs_m, Info, "nw");
blocks=list2vec(Info(2).sim.funs);
assert_checkfalse(or(blocks == "cscope"));
assert_checktrue(or(blocks == "trash"));

// second call perform a partial-compilation, and should continue to ignore
// some blocks
Info = scicos_simulate(scs_m, Info, "nw");
blocks=list2vec(Info(2).sim.funs);
assert_checkfalse(or(blocks == "cscope"));
assert_checktrue(or(blocks == "trash"));

