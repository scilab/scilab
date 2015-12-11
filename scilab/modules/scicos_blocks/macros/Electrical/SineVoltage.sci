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

function [x,y,typ]= SineVoltage(job,arg1,arg2)
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
            [ok,V,ph,frq,offset,start,exprs]=scicos_getvalue("Set voltage source parameter",..
            ["Amplitude (Volt)";"phase (rad)";"Frequency (Hz)";"Voltageoffset (V)";"Timeoffset (s)"],..
            list("vec",1,"vec",1,"vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            model.rpar=[V;ph;frq;offset;start]
            model.equations.parameters(2)=list(V,ph,frq,offset,start)
            graphics.exprs=exprs
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        model=scicos_model()
        model.in=[1];
        model.out=[1];
        V=1
        ph=0
        frq=1
        offset=0
        start=0
        model.rpar=[V;ph;frq;offset;start]
        model.sim="SineVoltage"
        model.blocktype="c"
        model.dep_ut=[%t %f]
        mo=modelica()
        mo.model="SineVoltage"
        mo.inputs="p";
        mo.outputs="n";
        mo.parameters=list(["V";"phase";"freqHz";"offset";"startTime"],list(V,ph,frq,offset,start))
        model.equations=mo

        exprs=[string(V);string(ph);string(frq);string(offset);string(start)]

        gr_i=[]


        x=standard_define([2 2],model,exprs,gr_i)
        x.graphics.in_implicit=["I"]
        x.graphics.out_implicit=["I"]
    end
endfunction
