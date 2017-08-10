// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

loadXcosLibs
scicos_log("TRACE");

b = scicos_block();
l = scicos_link();
super = SUPER_f("define");
super.model.rpar.objs($+1) = b;
super.model.rpar.objs($+1) = l;
assert_checkequal(length(super.model.rpar.objs), 4);
super.model.rpar.objs($).from = [1 1 0];
super.model.rpar.objs($).to = [3 1 1];
assert_checkequal(super.model.rpar.objs(1).graphics.pout, 4);
assert_checkequal(super.model.rpar.objs(3).graphics.pin, 4);
assert_checkequal(super.model.rpar.objs(4).from, [1 1 0]);
assert_checkequal(super.model.rpar.objs(4).to, [3 1 1]);


// Check that all the model items are freed
clear
