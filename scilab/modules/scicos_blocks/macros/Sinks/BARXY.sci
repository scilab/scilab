//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

function [x,y,typ]=BARXY(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;

        while %t do
            [ok,xmin,xmax,ymin,ymax,thickness,exprs]=scicos_getvalue("Set Scope parameters",..
            ["Xmin" ; "Xmax" ; "Ymin" ; "Ymax";"Segs Thickness"], ..
            list("vec",1,"vec",1,"vec",1,"vec",1, "vec", 1),exprs)
            if ~ok then
                break
            end //user cancel modification

            mess=[]
            if ymin>=ymax then
                mess=[mess;"Ymax must be greater than Ymin";" "]
                ok=%f
            end
            if xmin>=xmax then
                mess=[mess;"Xmax must be greater than Xmin";" "]
                ok=%f
            end
            if thickness <=0 then
                mess=[mess ; "Thickness must be strictly positive."]
                ok=%f
            end
            if ~ok then
                message(mess)
            else
                model.rpar=[xmin ; xmax ; ymin ; ymax]
                model.ipar=thickness
                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model;
                break
            end
        end
    case "define" then
        model=scicos_model();
        xmin = -15;
        xmax = 15;
        ymin = -15;
        ymax = 15;
        model.sim=list("BARXY_sim", 5)
        model.blocktype="d";
        model.dep_ut=[%t %f];
        model.in=[-1;-1];
        model.intyp=[1];
        model.out=[];
        model.evtin=[1]
        model.rpar=[xmin ; xmax ; ymin ; ymax]
        model.ipar=1;
        x=standard_define([2 2],model,[],[]);
        x.graphics.in_implicit=["E", "E"];
        x.graphics.out_implicit=[];
        x.graphics.exprs=["-15";"15";"-15";"15";"1"]
    end
endfunction
