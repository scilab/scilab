// This file is released under the 3-clause BSD license. See COPYING-BSD.
//=======================================================================

loadXcosLibs();
loadScicos();

assert_checktrue(importXcosDiagram("tests/benchmark/noop_matrices.zcos"))

context = struct();
context.NOOP_mode = 0; // using a C simulation function

// <-- BENCH START -->
timer();
scicos_simulate(scs_m, context);
disp(timer());
// <-- BENCH END -->

