// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2022 - UTC - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- BENCH NB RUN : 1 -->

// <-- BENCH START -->
exec(fullfile("SCI", "modules", "differential_equations", "tests", "unit_tests", "sundials_band.tst"));

// Dense reference runs
timer();
[t,v] = cvode(HEAT,tspan,v0,method="BDF");
t1cvode=timer();

timer();
[t,v] = arkode(HEAT,tspan,v0,method="DIRK",atol=1e-9);
t1arkode=timer();

assert_checktrue(t1cvode/t2cvode>10);
assert_checktrue(t1cvode/t3cvode>10);
assert_checktrue(t1cvode/t4cvode>10);

assert_checktrue(t1arkode/t2arkode>10);
assert_checktrue(t1arkode/t3arkode>10);
assert_checktrue(t1arkode/t4arkode>10);
// <-- BENCH END -->
