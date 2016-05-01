// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

d = scicos_diagram();
d.objs(1) = BIGSOM_f("define");
d.objs(2) = BIGSOM_f("define");
d.objs(3) = scicos_link(from=[2 1 0],to=[2 1 1]);
dClone = d;
dClone.objs(2) = scicos_block(); // Perform actual cloning

assert_checkequal(d.objs(2).graphics.pin , [3; 0]);
assert_checkequal(d.objs(2).graphics.pout,  3);
assert_checkequal(d.objs(3).from, [2 1 0]);
assert_checkequal(d.objs(3).to  , [2 1 1]);

assert_checkequal(dClone.objs(2).graphics.pin , 3);
assert_checkequal(dClone.objs(2).graphics.pout, 3);
assert_checkequal(dClone.objs(3).from, [2 1 0]);
assert_checkequal(dClone.objs(3).to  , [2 1 1]);


// Check that all the model items are freed
clear
