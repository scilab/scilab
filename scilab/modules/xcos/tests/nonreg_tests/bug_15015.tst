// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15015 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15015
//
// <-- Short Description -->
// ascii standard was not respected for some Xcos blocks

assert_checktrue(importXcosDiagram("SCI/modules/xcos/examples/sources_pal/en_US/rfile_f_en_US.zcos"));
xcos_simulate(scs_m, 4);
