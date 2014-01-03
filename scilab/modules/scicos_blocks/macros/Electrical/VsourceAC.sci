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

function [x,y,typ]=VsourceAC(job,arg1,arg2)
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
            [ok,VA,FR,exprs]=scicos_getvalue("Set voltage source parameter",..
            ["Amplitude (Volt)";"Frequency (Hz)"],..
            list("vec",-1,"vec",-1),exprs)
            if ~ok then
                break,
            end
            model.rpar=[VA;FR]
            model.equations.parameters(2)=list(VA,FR)
            graphics.exprs=exprs
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        model=scicos_model()
        model.in=[1];
        model.out=[1];
        VA=220
        FR=50
        model.rpar=[VA;FR]
        model.sim="VsourceAC"
        model.blocktype="c"
        model.dep_ut=[%t %f]
        mo=modelica()
        mo.model="VsourceAC"
        mo.inputs="p";
        mo.outputs="n";
        mo.parameters=list(["VA";"f"],list(VA,FR))
        model.equations=mo

        exprs=[string(VA);string(FR)]
        gr_i=[]

        x=standard_define([2 2],model,exprs,list(gr_i,0))
        x.graphics.in_implicit=["I"]
        x.graphics.out_implicit=["I"]
    end
endfunction
