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

function [x,y,typ]=Inductor(job,arg1,arg2)
    // Copyright INRIA
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
            [ok,L,exprs]=scicos_getvalue("Set Inductor block parameter",..
            "L (H)",list("vec",1),exprs)
            if ~ok then
                break,
            end
            model.rpar=L
            model.equations.parameters(2)=list(L)
            graphics.exprs=exprs
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        model=scicos_model()
        model.in=[1];
        model.out=[1];
        L=1.d-5
        model.rpar=L
        model.sim="Inductor"
        model.blocktype="c"
        model.dep_ut=[%t %f]
        mo=modelica();
        mo.model="Inductor"
        mo.inputs="p";
        mo.outputs="n";
        mo.parameters=list("L",list(L))
        model.equations=mo;
        exprs=string(L)

        gr_i=[]

        x=standard_define([2 0.9],model,exprs,list(gr_i,0))
        x.graphics.in_implicit=["I"]
        x.graphics.out_implicit=["I"]
    end
endfunction
