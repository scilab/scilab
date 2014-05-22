// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vladislav TRUBKIN
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
//<-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 13396 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13396
//
// <-- Short Description -->
// MBLOCK did not work with an external file containing the modelica class
//

global toggle;
toggle = %T;

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_13396.zcos"));

prot = funcprot();
funcprot(0);
level = ilib_verbose();
ilib_verbose(0);

// create external file
fd = mopen(TMPDIR+"/bug_13396.mo", "wt");
txt = ["class bug_13396"; ..
"    parameter Real R = 1.000000e-01;"; ..
"    Pin p,n;"; ..
"    Real i,v;"; ..
"equation"; ..
"    i = p.i;"; ..
"    n.i = -i;"; ..
"    v = p.v - n.v;"; ..
"    R * i = v;"; ..
"end bug_13396;"];
mputl(txt, fd);
mclose(fd);

// overload of scicos_getvalue and msg box
function str = x_mdialog(desc, labels, ini)
    global toggle;
    if toggle then
        ini($) = TMPDIR + "/bug_13396.mo";
        str = ini;
        toggle = ~toggle;
    else
        str = []; // cancelling
    end
endfunction
function messagebox(msg, msg_title)
endfunction

// set the path for external file
for i = 1:size(scs_m.objs)
    if typeof(scs_m.objs(i)) == "Block" & scs_m.objs(i).gui == "MBLOCK" then
        scs_m.objs(i).graphics.exprs(9) = TMPDIR + "/bug_13396.mo";
        scs_m.objs(i).model.sim(1) = TMPDIR + "/bug_13396.mo";
        MBLOCK("set", scs_m.objs(i));
        break
    end
end
// compile the diagram
[cpr, ok] = xcos_compile(scs_m);
assert_checktrue(ok);
// simulate the diagram
xcos_simulate(scs_m, 4);

funcprot(prot);
ilib_verbose(level);

mdelete(TMPDIR+"/bug_13396.mo");

