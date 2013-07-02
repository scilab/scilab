// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->

// CUMSUM block test

loadXcosLibs();

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/cumsum.zcos"));
scicos_simulate(scs_m, list());

in = ones(4,2);

v = cumsum_m.values(:,:,1);
assert_checkequal(v, cumsum(in, "*"));

v = cumsum_r.values(:,:,1);
assert_checkequal(v, cumsum(in, "r"));

v = cumsum_c.values(:,:,1);
assert_checkequal(v, cumsum(in, "c"));

// FIXME: to workspace does not handle complex numbers
//in = complex(ones(4,2), (1:4)' * (1:2));
//v = cumsumz_m.values(:,:,1);
//assert_checkequal(v, cumsum(in, "*"));
//
//v = cumsumz_r.values(:,:,1);
//assert_checkequal(v, cumsum(in, "r"));
//
//v = cumsumz_c.values(:,:,1);
//assert_checkequal(v, cumsum(in, "c"));

