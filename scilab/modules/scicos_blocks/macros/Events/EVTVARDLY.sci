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

function [x,y,typ]=EVTVARDLY(job,arg1,arg2)
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
            [ok,fir,exprs]=scicos_getvalue("Set parameter of variable event delay",..
            "Initial event firing time (<0 if absent)",..
            list("vec",1),exprs)
            if ~ok then
                break,
            end
            graphics.exprs=exprs
            model.firing=fir
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        model=scicos_model()
        model.sim=list("evtvardly",4)
        model.in=1
        model.evtin=1
        model.evtout=1
        model.blocktype="d"
        model.firing=-1
        model.blocktype="c"
        model.dep_ut=[%t %f]
        exprs=string(model.firing)
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
