// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 8502 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8502
//
// <-- Short Description -->
// -Axis labels use number with 3digits in the exponent like 8e+005 instead of 8e+05

f=scf();
plot(logspace(5,10,100));
assert_checkequal(f.children.y_ticks.labels,["0e00";"1e09";"2e09";"3e09";"4e09";"5e09";"6e09";"7e09";"8e09";"9e09";"1e10"]);
