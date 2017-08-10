// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2012 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/fromws_c.zcos"));

V = struct("time", (1:100)', "values", sin((0:0.1:9.9)'));
scicos_simulate(scs_m, list());

found = %f;
index = 0;
for i=1:length(scs_m.objs)
    if typeof(scs_m.objs(i))=="Block" & scs_m.objs(i).gui=="GAINBLK_f" then
        found = %t;
        index = i;
        break;
    end
end

assert_checktrue(found);

scs_m.objs(index).graphics.exprs = "[1 1 1]";
scs_m.objs(index).model.rpar = [1 1 1];

V.values = [V.values V.values V.values];
scicos_simulate(scs_m, list());
