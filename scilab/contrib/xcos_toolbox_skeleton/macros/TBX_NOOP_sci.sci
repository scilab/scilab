//
// This file is part of the Xcos skeleton toolbox
//
// see license.txt for more licensing information

function [x,y,typ]=TBX_NOOP_sci(job,arg1,arg2)
    x=[];y=[];typ=[]
    select job
    case "set" then
        x=arg1;
        model=arg1.model;
        graphics=arg1.graphics;
        exprs=graphics.exprs;

        while %t do
            [ok,in1,in2,intyp,exprs]=scicos_getvalue(..
            "Set no-operation parameters",..
            ["Input rows sizes";
            "Input columns sizes";
            "Input types"],..
            list("vec",-1,"vec",-1,"vec",-1),..
            exprs)
            if ~ok then
                break,
            end //user cancel modification

            if or(size(in1) <> size(in2)) | or(size(in1) <> size(in2)) then
                message(["Some specified values are inconsistent:";
                " ";"All parameters should have the same length"])
                ok = %f;
            end

            if ok then
                model.in1=in1;
                model.in2=in2;
                model.intyp=intyp;

                graphics.pin=zeros(in1, 1);

                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model;
                break;
            end
        end

    case "define" then
        model=scicos_model()
        model.sim=list("TBX_NOOP_sim",5)

        model.evtin=1
        model.blocktype="d"
        model.dep_ut=[%f %f]

        exprs=["[]";"[]";"[]"];
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
        x.graphics.style=["blockWithLabel;displayedLabel=NOOP_sci"]
    end
endfunction

