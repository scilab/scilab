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

function [x,y,typ]=TrigFun(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        %FF=["sin";"cos";"tan";
        "asin";"acos";"atan";
        "sinh";"cosh";"tanh";
        "asinh";"acosh";"atanh"];
        %GG=["Choose among "+strcat(%FF(1:4),", ");strcat(%FF(5:$),", ")]
        while %t do
            [ok,fun,exprs]=scicos_getvalue(%GG,"Function",list("str",1),exprs)
            if ~ok then
                break,
            end
            if find(%FF==fun)==[] then
                message("Sorry but "+fun +" is not in the list!")
            else
                graphics.exprs=exprs
                execstr("model.sim=list(''"+fun+"_blk'',4)")
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        model=scicos_model()
        model.sim=list("sin_blk",4)
        model.in=-1
        model.out=-1
        model.blocktype="c"
        model.dep_ut=[%t %f]
        exprs="sin"
        gr_i=[]
        x=standard_define([4 2],model,exprs,gr_i)
    end
endfunction
