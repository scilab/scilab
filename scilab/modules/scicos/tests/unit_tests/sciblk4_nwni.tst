// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//

loadXcosLibs();

// simplest sciblk4 test
function [x,y,typ]=SCIBLK4_bench(job,arg1,arg2)
    x=[];y=[];typ=[]
    select job
    case "set" then
        x=arg1;
    case "define" then
        model=scicos_model()
        model.sim=list("SCIBLK4_bench_sim",5)

        model.in = 1
        model.in2 = 1
        model.out = 1
        model.out2 = 1

        model.blocktype="d"
        model.dep_ut=[%t %f]

        exprs=[];
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
        x.graphics.style=["blockWithLabel;displayedLabel=SCIBLK4_bench"]
    end
endfunction

function blk = SCIBLK4_bench_sim(blk, flag)
    blk.outptr(1) = blk.inptr(1) + 1;
endfunction

scs_m = scicos_diagram();
scs_m.props.tf = 0.5;

scs_m.objs(1) = GENSIN_f("define");
scs_m.objs(1).graphics.pout = 5;
scs_m.objs(2) = SCIBLK4_bench("define");
scs_m.objs(2).graphics.pin = 5;
scs_m.objs(2).graphics.pout = 6;
scs_m.objs(3) = TRASH_f("define");
scs_m.objs(3).graphics.pin = 6;
scs_m.objs(3).graphics.pein = 7;
scs_m.objs(4) = CLOCK_c("define");
scs_m.objs(4).graphics.peout = 7;

scs_m.objs(5) = scicos_link(from=[1 1 0], to=[2 1 1]);
scs_m.objs(6) = scicos_link(from=[2 1 0], to=[3 1 1]);
scs_m.objs(7) = scicos_link(from=[4 1 0], to=[3 1 1], ct=[1 -1]);

Info = scicos_simulate(scs_m, list());
assert_checkequal(Info(list(2,2,9)), list(sin(0.5), 1+sin(0.5)));
