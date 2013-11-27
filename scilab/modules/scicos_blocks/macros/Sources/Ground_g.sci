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

function [x,y,typ]=Ground_g(job,arg1,arg2)
    // Copyright INRIA
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
    case "define" then
        C=[0]
        model=scicos_model()
        model.sim=list("cstblk4_m",4)
        model.in=[]
        model.out=1
        model.in2=[]
        model.out2=1
        model.outtyp=-1
        model.rpar=[]
        model.opar=list(C);
        model.blocktype="d"
        model.dep_ut=[%f %f]
        exprs=[]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
