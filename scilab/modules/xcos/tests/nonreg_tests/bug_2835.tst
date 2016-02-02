// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Paul Bignier
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
//
// <-- TEST WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 2835 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2835
//
// <-- Short Description -->
// On negative "initial event", EVTDLY_c took no notice of the input.
//

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_2835.zcos"));
xcos_simulate(scs_m, 4);
