// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

o = Ground("define");
l = scicos_link(ct=[1 2]);
d = scicos_diagram(objs=list(o,l));
d.objs(2).to = [1 1 1];

assert_checkequal(d.objs(1).graphics.pin,  2);
assert_checkequal(d.objs(1).graphics.pout, []);

o = Ground("define");
l = scicos_link(ct=[1 2]);
d = scicos_diagram(objs=list(o,l));
d.objs(2).from = [1 1 1];

assert_checkequal(d.objs(1).graphics.pin,  2);
assert_checkequal(d.objs(1).graphics.pout, []);


// Check that all the model items are freed
clear
