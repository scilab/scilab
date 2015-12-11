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

function [x,y,typ]= PotentialSensor(job,arg1,arg2)
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
    case "define" then
        model=scicos_model()
        model.in=[1];
        model.out=[1];
        model.rpar=[]
        model.sim="PotentialSensor"
        model.blocktype="c"
        model.dep_ut=[%t %f]
        mo=modelica()
        mo.model="PotentialSensor"
        mo.inputs="p";
        mo.outputs=["v"]
        model.equations=mo
        gr_i=[]

        x=standard_define([2 2],model,"",list(gr_i,0))
        x.graphics.in_implicit=["I"]
        x.graphics.out_implicit=["E"]
    end
endfunction
