//
// This file is part of the Xcos skeleton toolbox
//
// see license.txt for more licensing information

function [x,y,typ]=TBX_SUM_sci(job,arg1,arg2)
    x=[];y=[];typ=[]
    select job
    case "set" then
        x=arg1;
        // no parameters yet
    case "define" then
        model=scicos_model()
        model.sim=list("TBX_SUM_sim",5)
        // one inputs with a variable size "double" element
        model.in=[-1]
        model.intyp=[1]
        // one output with a single "double" element
        model.out=1
        model.outtyp=1

        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=string([]);
        gr_i=[]
        x=standard_define([4 4],model,exprs,gr_i)
        x.graphics.style=["blockWithLabel;displayedLabel=TBX_SUM_sci"]
    end
endfunction

