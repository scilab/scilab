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

function [x,y,typ]=ABS_VALUE(job,arg1,arg2)
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
            [ok,zcr,exprs]=..
            scicos_getvalue("Set block parameters",..
            ["use zero_crossing (1: yes) (0:no)"],..
            list("vec",1),exprs)
            if ~ok then
                break,
            end
            graphics.exprs=exprs
            if ok then
                if zcr<>0 then
                    model.nmode=-1;
                    model.nzcross=-1;
                else
                    model.nmode=0;
                    model.nzcross=0;
                end
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        nu=-1
        model=scicos_model()
        model.sim=list("absolute_value",4)
        model.in=nu
        model.out=nu
        model.nzcross=nu
        model.nmode=nu
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[string([1])]
        gr_i=[]

        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
