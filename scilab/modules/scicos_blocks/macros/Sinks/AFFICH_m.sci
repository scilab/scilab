//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [x,y,typ] = AFFICH_m(job,arg1,arg2)
    //** 22 Aug 2006: looking for a nasty "font" bugs
    //**
    //** 07 Aug 2008: this function has been update for Scilab 5.0 by
    //**              Simone Mannori and Jean-Baptiste Silvy

    x=[];
    y=[];
    typ=[];

    select job

    case "set" then
        x = arg1 ;
        graphics = arg1.graphics ;
        exprs = graphics.exprs ;
        model = arg1.model ;

        while %t do
            [ok,in,font,fontsize,colr,nt,nd,herit,exprs]=scicos_getvalue(..
            "Set  parameters",..
            ["Input Size",
            "Font number";
            "Font size";
            "Color";
            "Total number of digits";
            "Number of rational part digits";
            "Block inherits (1) or not (0)"],..
            list("mat",[1 2],"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1),exprs)

            if ~ok then
                break,
            end //user cancel modification

            mess = [] ; //** no message

            if font<=0 then
                mess=[mess;"Font number must be positive";" "]
                ok=%f
            end

            if fontsize<=0 then
                mess=[mess;"Font size must be positive";" "]
                ok=%f
            end

            if nt<=3 then
                mess=[mess;"Total number of digits must be greater than 3";" "]
                ok=%f
            end

            if nd<0 then
                mess=[mess;"Number of rational part digits must be "
                "greater or equal 0";" "]
                ok=%f
            end

            if ~ok then
                message(["Some specified values are inconsistent:";
                " ";mess]);
            end

            if ~or(herit==[0 1]) then
                mess=[mess;"Accept inherited values are 0 and 1";" "]
                ok=%f
            end

            if ~ok then
                message(["Some specified values are inconsistent:";
                " ";mess])
            end

            //** Positive case ->
            if ok then
                //[model,graphics,ok]=check_io(model,graphics,1,[],ones(1-herit,1),[])
                [model,graphics,ok] = set_io(model, graphics, list(in,1), list(), ones(1-herit,1), [])
            end

            if ok then
                model.ipar = [font;fontsize;colr;nt;nd;in(1,1)];
                model.dstate = [-1;0;0;1;1;0;zeros(in(1,1)*in(1,2),1)]
                model.evtin = ones(1-herit,1)
                graphics.exprs = exprs;
                x.graphics = graphics;
                x.model = model ;
                break
            end

        end


    case "define" then
        font = 1
        fontsize = 1
        colr = 1
        nt = 5
        nd = 1
        in = [1 1]

        model = scicos_model();
        model.sim = list("affich2",4) ;
        model.in = in(1,1);
        model.in2 = in(1,2);
        model.evtin  = 1 ;
        model.dstate = [-1;0;0;1;1;0;zeros(in(1,1)*in(1,2),1)]
        model.ipar   = [font;fontsize;colr;1000;nt;nd;in(1,1)]
        model.blocktype = "c" ;
        model.firing = []     ;
        model.dep_ut = [%t %f]
        model.label = "" ;

        exprs = [ sci2exp([model.in model.in2]);
        string(font);
        string(fontsize);
        string(colr);
        string(nt);
        string(nd);
        string(0) ]

        gr_i=[]

        x = standard_define([3 2],model,exprs,gr_i)

    end

endfunction
