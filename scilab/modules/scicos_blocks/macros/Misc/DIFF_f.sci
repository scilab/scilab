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

function [x,y,typ]=DIFF_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        while %t do
            [ok,x0,xd0,exprs]=scicos_getvalue("Set continuous linear system parameters",..
            ["Initial state";"Initial Derivative"],list("vec",1,"vec",1),exprs)


            if ~ok then
                break,
            end
            graphics.exprs=exprs;
            model.state=[x0(:);xd0(:)];
            x.graphics=graphics;
            x.model=model
            break
        end
        x.model.firing=[] //compatibility
    case "define" then
        x0=[0;0]
        model=scicos_model()
        model.sim=list("diffblk",10001)
        model.in=1
        model.out=1
        model.state=x0
        model.blocktype="c"
        model.dep_ut=[%f %t]

        exprs=[strcat(sci2exp(x0(1)));strcat(sci2exp(x0(2)))]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
