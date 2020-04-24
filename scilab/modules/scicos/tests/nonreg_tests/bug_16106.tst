// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16106 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16106
//
// <-- Short Description -->
// block.opar and block.oz could not be used for block implemented in Scilab
// with the scicos api 5.
//

loadXcosLibs

function [x,y,typ]=DEMO_BLK4(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];

    select job
    case "set" then
        x=arg1;

    case "define" then
        model=scicos_model();
        model.sim=list("demo_blk4_sim", 5)
        model.blocktype="d";
        model.dep_ut=[%f %t];
        model.in=[1];
        model.intyp=[1];
        model.out=[1];
        model.outtyp=[1];

        r = rand(); // same shared random value for checking
        model.opar=list("FOO", r)
        model.odstate=list("BAR", r)

        x=standard_define([2 2],model,[],[]);
    end

endfunction

function [blk] = demo_blk4_sim(blk, flag)
    if flag == 4 | flag == 6 // INIT or REINIT
    elseif flag ==  1 // OUTPUT UPDATE
        assert_checkequal(blk.opar(1), "FOO");
        assert_checkequal(floor(blk.opar(2)), 0);
        assert_checkequal(blk.oz(1), "BAR");
        assert_checkequal(blk.oz(2) - floor(blk.oz(2)), blk.opar(2));
    elseif flag ==  2 // STATE UPDATE
        blk.odstate(2) = blk.odstate(2) + 1;
        assert_checkequal(floor(blk.oz(2)) - blk.opar(2), 0);
    end
endfunction

// For user specific testing
// pal = xcosPal("demo");
// pal = xcosPalAddBlock(pal, "DEMO_BLK4");
// xcosPalAdd(pal);

// Create a diagram with a DEMO_BLK4 linked to itself
blk = DEMO_BLK4("define");
blk.graphics.pin = 2;
blk.graphics.pout = 2;

lnk = scicos_link();
lnk.from = [1 1 0];
lnk.to = [1 1 1];

scs_m = scicos_diagram(objs=list(blk, lnk));
Info = scicos_simulate(scs_m, list());

