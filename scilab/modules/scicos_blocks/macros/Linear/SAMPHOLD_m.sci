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

function [x,y,typ]=SAMPHOLD_m(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        x.model.firing=[] //compatibility
        graphics=arg1.graphics;
        label=graphics.exprs
        model=arg1.model;
        while %t do
            [ok,it,exprs]=scicos_getvalue("Set parameters Block",..
            ["Datatype(1=real double 2=Complex 3=int32 ...)"],list("vec",1),label)
            if ~ok then
                break,
            end
            if ((it<1)|(it>8)) then
                message ("Datatype is not supported");
                ok=%f;
            end
            if ok then
                in=[model.in model.in2];
                [model,graphics,ok]=set_io(model,graphics,list(in,it),list(in,it),1,[])
                if ok then
                    graphics.exprs=exprs;
                    arg1.graphics=graphics;
                    arg1.model=model;
                    x=arg1
                    break
                end
            end
        end
    case "define" then
        model=scicos_model()
        model.sim=list("samphold4_m",4)
        model.in=-1
        model.in2=-2
        model.intyp=1
        model.outtyp=1
        model.out=-1
        model.out2=-2
        model.evtin=1
        model.blocktype="d"
        model.dep_ut=[%t %f]
        label=[sci2exp(1)];
        gr_i=[]
        x=standard_define([2 2],model,label,gr_i)
    end
endfunction
