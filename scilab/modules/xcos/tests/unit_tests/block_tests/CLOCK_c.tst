// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->
//
// <-- Unit test for CLOCK_c -->
//
// <-- Short Description -->
// The block should have a valid sub-diagram
//

loadXcosLibs();

blk = CLOCK_c("define");
d = blk.model.rpar;

assert_checkequal(d.objs(1).gui, "CLKOUT_f");
assert_checkequal(d.objs(2).gui, "EVTDLY_c");
assert_checkequal(typeof(d.objs(3)), "Link");
assert_checkequal(d.objs(4).gui, "CLKSPLIT_f");
assert_checkequal(typeof(d.objs(5)), "Link");
assert_checkequal(typeof(d.objs(6)), "Link");

assert_checkequal(d.objs(3).from, [2 1 0]);
assert_checkequal(d.objs(3).to,   [4 1 1]);

assert_checkequal(d.objs(5).from, [4 1 0]);
assert_checkequal(d.objs(5).to,   [1 1 1]);

assert_checkequal(d.objs(6).from, [4 2 0]);
assert_checkequal(d.objs(6).to,   [2 1 1]);

