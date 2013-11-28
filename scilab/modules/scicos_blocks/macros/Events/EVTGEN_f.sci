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

function [x,y,typ]=EVTGEN_f(job,arg1,arg2)
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
            [ok,tt,exprs]=scicos_getvalue("Set Event time",..
            ["Event Time"],list("vec",1),exprs)
            if ~ok then
                break,
            end
            graphics.exprs=exprs
            if model.firing<>tt then
                model.firing=tt
            end
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        tt=0
        model=scicos_model()
        model.sim="trash"
        model.evtout=1
        model.blocktype="d"
        model.firing=tt
        model.dep_ut=[%f %f]

        exprs=string(tt)
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
