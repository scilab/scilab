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

function [x,y,typ]=MEMORY_f(job,arg1,arg2)
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
            [ok,a,inh,exprs]=scicos_getvalue("Set memory block parameters",..
            ["initial condition";"Inherit (1: no, 0: yes)"],list("vec",-1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if inh==0 then
                inh=[];
            else
                inh=1;
            end
            [model,graphics,ok]=check_io(model,graphics,-1,-1,inh,[])
            out=size(a,"*");
            if out==0 then
                ok=%f,
                messagebox("Initial condition empty","modal","error");
            end
            in=out
            if ok then
                graphics.exprs=exprs;
                model.rpar=a;
                model.in=in;
                model.out=out
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        z=0
        in=1
        exprs=[string(z);string(1)]
        model=scicos_model()
        model.sim="memo"
        model.in=in
        model.out=in
        model.evtin=1
        model.dstate=0
        model.rpar=z
        model.blocktype="m"
        model.dep_ut=[%f %f]

        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
