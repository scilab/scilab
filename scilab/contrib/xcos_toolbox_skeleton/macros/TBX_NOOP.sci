//
// This file is part of the Xcos skeleton toolbox
//
// see license.txt for more licensing information

function [x,y,typ]=TBX_NOOP(job,arg1,arg2)
    x=[];y=[];typ=[]
    select job
    case "set" then
        x=arg1;
        model=arg1.model;
        graphics=arg1.graphics;
        exprs=graphics.exprs;

        while %t do
            [ok,sim,in1,in2,intyp,out1,out2,outyp,exprs]=scicos_getvalue(..
            "Set no-operation parameters",..
            ["(0=C, 1=Scilab)";
            "Input rows sizes";
            "Input columns sizes";
            "Input types";
            "Output rows sizes";
            "Output columns sizes";
            "Output types"],..
            list("vec",1,"vec",-1,"vec",-1,"vec",-1,"vec",-2,"vec",-2,"vec",-2),..
            exprs)
            if ~ok then
                break,
            end //user cancel modification

            if or(size(in1) <> size(in2)) | or(size(in1) <> size(intyp)) then
                message(["Some specified values are inconsistent:";
                " ";"All input parameters should have the same length"])
                ok = %f;
            end
            if or(size(out1) <> size(out2)) | or(size(out1) <> size(outyp)) then
                message(["Some specified values are inconsistent:";
                " ";"All output parameters should have the same length"])
                ok = %f;
            end

            if ok then
                [model,graphics,ok]=set_io(model,graphics,list([in1,in2],intyp),list([out1,out2],outyp),1,[])
            end

            if ok then
                if sim == 0 then
                    model.sim=list("tbx_block_noop",4);
                    graphics.style=["blockWithLabel;displayedLabel=""NOOP C"""]
                else
                    model.sim=list("TBX_NOOP_sim",5);
                    graphics.style=["blockWithLabel;displayedLabel=""NOOP Scilab"""]
                end

                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model;
                break;
            end
        end

    case "define" then
        model=scicos_model()
        model.sim=list("tbx_block_noop",4)

        model.evtin=1
        model.blocktype="d"
        model.dep_ut=[%f %f]

        exprs=["0";"";"";"";"";"";""];
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
        x.graphics.style=["blockWithLabel;displayedLabel=""NOOP C"""]
    end
endfunction

