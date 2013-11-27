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

function [x,y,typ]=NEGTOPOS_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        x.model.firing=-1 //compatibility
    case "define" then
        model=scicos_model()
        model.sim=list("zcross",1)
        model.nzcross=1
        model.in=1
        model.evtout=1
        model.rpar=[-1;-1;0;-1]
        model.blocktype="z"
        model.firing=-1
        model.dep_ut=[%t %f]

        gr_i=[]
        x=standard_define([2 2],model,[],gr_i)
    end
endfunction
