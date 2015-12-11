// This file is released under the 3-clause BSD license. See COPYING-BSD.
//=======================================================================

loadXcosLibs();
loadScicos();

assert_checktrue(importXcosDiagram("tests/benchmark/noop_scalar.zcos"))

context = struct();
context.NOOP_mode = 1; // using a Scilab simulation function

// <-- BENCH START -->
timer();
scicos_simulate(scs_m, context);
disp(timer());
// <-- BENCH END -->

