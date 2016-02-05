// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2012 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/fromws_c.zcos"));

V = struct("time", (1:100)', "values", sin((0:0.1:9.9)'));
scicos_simulate(scs_m, list());

assert_checkequal(scs_m.objs(4).gui, "GAINBLK_f");

scs_m.objs(4).graphics.exprs = "[1 1 1]";
scs_m.objs(4).model.rpar = [1 1 1];

V.values = [V.values V.values V.values];
scicos_simulate(scs_m, list());
