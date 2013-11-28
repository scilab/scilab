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

function [x,y,typ]=BACKLASH(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        rpar=model.rpar
        while %t do
            [ok,ini,gap,zcr,exprs]=scicos_getvalue("Set backlash parameters",..
            ["initial output";
            "gap";"use zero-crossing (0:no, 1:yes)"],..
            list("vec",1,"vec",1,"vec",1),exprs)


            if ~ok then
                break,
            end
            if ok then
                graphics.exprs=exprs;
                rpar(1)=ini;
                rpar(2)=gap;
                if zcr<>0 then
                    model.nzcross=2
                else
                    model.nzcross=0
                end
                model.rpar=rpar
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        exprs=["0";"1";"1"]
        model=scicos_model()
        model.sim=list("backlash",4)
        model.in=1
        model.out=1
        model.rpar=[0;1]
        model.nzcross=2;
        model.blocktype="c"
        model.dep_ut=[%t %f]

        gr_i=[]

        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
