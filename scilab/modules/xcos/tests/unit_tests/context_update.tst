// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->

// test with context modification


assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/context_update.zcos"));

// checking without context re-definition
Context=struct();
scicos_simulate(scs_m,list(),Context,'nw');
assert_checkequal(out.values, 1.0);

// checking with context re-definition
Context.in1=1;
Context.in2=1;
scicos_simulate(scs_m,list(),Context,'nw');
assert_checkequal(out.values, 0.0);

// checking with context re-definition
Context.in1=0;
Context.in2=1;
scicos_simulate(scs_m,list(),Context,'nw');
assert_checkequal(out.values, -1.0);
