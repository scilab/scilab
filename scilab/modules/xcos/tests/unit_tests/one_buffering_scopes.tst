// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->
// <-- TEST WITH GRAPHIC -->

// <-- NO CHECK REF -->

loadXcosLibs();

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/one_buffering_scopes.zcos"));
xcos_simulate(scs_m, 4);


