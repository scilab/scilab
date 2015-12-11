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

function [x,y,typ]=Switch(job,arg1,arg2)
    // exemple d'un bloc implicit,
    //   -  sans entree ni sortie de conditionnement
    //   -  avec une entree et une sortie de type implicit et de dimension 1
    //   - avec un dialogue de saisie de parametre
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
            [ok,Ron,Roff,exprs]=scicos_getvalue("Set non-ideal electrical switch parameters",..
            ["Resistance in On state (Ohm)";"Resistance in Off state (Ohm)"],list("vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            model.equations.parameters(2)=list(Ron,Roff)
            graphics.exprs=exprs
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        model=scicos_model()
        Ron=0.01;
        Roff=1e5;
        S=["Ron";"Roff"];
        Z=eval(S);
        model.sim="Switch"
        model.blocktype="c"
        model.dep_ut=[%t %f]
        mo=modelica()
        mo.model=model.sim
        mo.inputs=["p";"inp"];
        mo.outputs="n";
        mo.parameters=list(S,Z);
        model.equations=mo
        model.in=ones(size(mo.inputs,"*"),1)
        model.out=ones(size(mo.outputs,"*"),1)
        model.rpar=Z;
        exprs=string(Z);
        gr_i=[];
        x=standard_define([2 2],model,exprs,list(gr_i,0))
        x.graphics.in_implicit=["I";"E"]
        x.graphics.out_implicit=["I"]
    end
endfunction
