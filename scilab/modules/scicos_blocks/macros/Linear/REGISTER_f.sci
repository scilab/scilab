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

function [x,y,typ]=REGISTER_f(job,arg1,arg2)
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
            [ok,z0,exprs]=scicos_getvalue("Set delay parameters",..
            "Register initial condition",list("vec",-1),exprs)
            if ~ok then
                break,
            end
            if prod(size(z0))<1 then
                message("Register length must be at least 1")
                ok=%f
            end
            if ok then
                graphics.exprs=exprs;
                model.dstate=z0
                x.graphics=graphics;
                x.model=model
                break
            end
        end

    case "define" then
        z0=zeros(10,1)
        model=scicos_model()
        model.sim="delay"
        model.in=1
        model.out=1
        model.evtin=1
        model.dstate=z0
        model.blocktype="d"
        model.dep_ut=[%f %f]

        exprs=strcat(string(z0),";")
        gr_i=[]
        x=standard_define([2.5 2.5],model,exprs,gr_i)
    end
endfunction
