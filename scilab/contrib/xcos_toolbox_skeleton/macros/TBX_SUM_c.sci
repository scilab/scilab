//
// This file is part of the Xcos skeleton toolbox
//
// see license.txt for more licensing information

function [x,y,typ]=TBX_SUM_c(job,arg1,arg2)
    x=[];y=[];typ=[]
    select job
    case "set" then
        x=arg1;
        // no parameters yet
    case "define" then
        model=scicos_model()
        model.sim=list("block_sum",4)
        // two inputs with a single "double" element
        model.in=[1;1]
        model.intyp=[1;1]
        // one output with a single "double" element
        model.out=1
        model.outtyp=1

        model.blocktype="c"
        model.dep_ut=[%f %f]

        exprs=string([]);
        gr_i=[]

        x=standard_define([4 4],model,exprs,gr_i)
        x.graphics.style="blockWithLabel;displayedLabel=TBX_SUM_c"
    end
endfunction

