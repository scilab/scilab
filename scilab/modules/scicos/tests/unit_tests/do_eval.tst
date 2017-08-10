// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

loadXcosLibs();

// check contained blocks interfaces
o = CONST_m("define");
assert_checkequal(o.graphics.pin, []);
assert_checkequal(o.graphics.pout, 0);
assert_checkequal(o.graphics.pein, []);
assert_checkequal(o.graphics.peout, []);
clear o

o = TRASH_f("define");
assert_checkequal(o.graphics.pin, 0);
assert_checkequal(o.graphics.pout, []);
assert_checkequal(o.graphics.pein, 0);
assert_checkequal(o.graphics.peout, []);
clear o

o = CLOCK_f("define");
assert_checkequal(o.graphics.pin, []);
assert_checkequal(o.graphics.pout, []);
assert_checkequal(o.graphics.pein, []);
assert_checkequal(o.graphics.peout, 0);

assert_checkequal(o.model.rpar.objs(1).graphics.pin, []);
assert_checkequal(o.model.rpar.objs(1).graphics.pout, []);
assert_checkequal(o.model.rpar.objs(1).graphics.pein, 5);
assert_checkequal(o.model.rpar.objs(1).graphics.peout, []);
assert_checkequal(o.model.rpar.objs(4).graphics.pin, []);
assert_checkequal(o.model.rpar.objs(4).graphics.pout, []);
assert_checkequal(o.model.rpar.objs(4).graphics.pein, 3);
assert_checkequal(o.model.rpar.objs(4).graphics.peout, [5;6]);
assert_checkequal(o.model.rpar.objs(2).graphics.pin, []);
assert_checkequal(o.model.rpar.objs(2).graphics.pout, []);
assert_checkequal(o.model.rpar.objs(2).graphics.pein, 6);
assert_checkequal(o.model.rpar.objs(2).graphics.peout, 3);
clear o

// create a block and connect it
function o = create_and_connect(fname, pin, pout, pein, peout)
    o = fname("define");
    if exists("pin","local") then o.graphics.pin = pin; end
    if exists("pout","local") then o.graphics.pout = pout; end
    if exists("pein","local") then o.graphics.pein = pein; end
    if exists("peout","local") then o.graphics.peout = peout; end
endfunction

// create a valid diagram
scs_m = scicos_diagram(objs=list( ..
CONST_m("define"), ..
TRASH_f("define"), ..
CLOCK_c("define"), ..
scicos_link(from=[1 1 0],to=[2 1 1]), ..
scicos_link(from=[3 1 0],to=[2 1 1],ct=[5,-1])));

assert_checkequal(scs_m.objs(1).graphics.pout, 4);
assert_checkequal(scs_m.objs(2).graphics.pin, 4);
assert_checkequal(scs_m.objs(2).graphics.pein, 5);
assert_checkequal(scs_m.objs(3).graphics.peout, 5);

assert_checkequal(scs_m.objs(3).model.rpar.objs(1).graphics.pin, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(1).graphics.pout, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(1).graphics.pein, 5);
assert_checkequal(scs_m.objs(3).model.rpar.objs(1).graphics.peout, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(4).graphics.pin, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(4).graphics.pout, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(4).graphics.pein, 3);
assert_checkequal(scs_m.objs(3).model.rpar.objs(4).graphics.peout, [5;6]);
assert_checkequal(scs_m.objs(3).model.rpar.objs(2).graphics.pin, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(2).graphics.pout, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(2).graphics.pein, 6);
assert_checkequal(scs_m.objs(3).model.rpar.objs(2).graphics.peout, 3);

needcompile = 4;
[scs_m, %cpr, needcompile, ok] = do_eval(scs_m, list(),struct());

assert_checkequal(scs_m.objs(1).graphics.pout, 4);
assert_checkequal(scs_m.objs(2).graphics.pin, 4);
assert_checkequal(scs_m.objs(2).graphics.pein, 5);
assert_checkequal(scs_m.objs(3).graphics.peout, 5);

assert_checkequal(scs_m.objs(3).model.rpar.objs(1).graphics.pin, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(1).graphics.pout, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(1).graphics.pein, 5);
assert_checkequal(scs_m.objs(3).model.rpar.objs(1).graphics.peout, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(4).graphics.pin, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(4).graphics.pout, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(4).graphics.pein, 3);
assert_checkequal(scs_m.objs(3).model.rpar.objs(4).graphics.peout, [5;6]);
assert_checkequal(scs_m.objs(3).model.rpar.objs(2).graphics.pin, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(2).graphics.pout, []);
assert_checkequal(scs_m.objs(3).model.rpar.objs(2).graphics.pein, 6);
assert_checkequal(scs_m.objs(3).model.rpar.objs(2).graphics.peout, 3);


