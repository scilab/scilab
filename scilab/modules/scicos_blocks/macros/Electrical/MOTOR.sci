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

function [x,y,typ]=MOTOR(job,arg1,arg2)
    // exemple d'un bloc implicit,
    //   -  sans entree ni sortie de conditionnement
    //   -  avec deux entrees et une sortie de type implicit et de dimension 1
    //   -  avec des ports disposes en des positions non standard

    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
    case "define" then
        model=scicos_model()
        model.out=[1;1]
        model.in=[1];
        model.sim="motor"
        model.blocktype="c"
        model.dep_ut=[%t %f]
        gr_i=[];


        exprs=""
        x=standard_define([2 2],model,exprs,gr_i)
        x.graphics.out_implicit=["I";"I"]
        x.graphics.in_implicit=["I"]
    end
endfunction
