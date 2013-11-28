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

function [x,y,typ]=REGISTER(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        if size(exprs,1)==1 then
            exprs=[exprs;sci2exp(1)];
        end
        while %t do
            [ok,z0,it,exprs]=scicos_getvalue("Set delay parameters",..
            ["Register initial condition";..
            "Datatype (1=double 3=int32 ...)"],..
            list("vec",-1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if prod(size(z0))<1 then
                message("Register length must be at least 1")
                ok=%f
            end
            if it==1 then
                model.sim=list("delay4",4);
                z0=double(z0);
                model.dstate=z0;
                model.odstate=list();
            else
                if it==3 then
                    model.sim=list("delay4_i32",4)
                    z0=int32(z0)
                elseif it==4 then
                    model.sim=list("delay4_i16",4)
                    z0=int16(z0)
                elseif it==5 then
                    model.sim=list("delay4_i8",4)
                    z0=int8(z0)
                elseif it==6 then
                    model.sim=list("delay4_ui32",4)
                    z0=uint32(z0)
                elseif it==7 then
                    model.sim=list("delay4_ui16",4)
                    z0=uint16(z0)
                elseif it==8 then
                    model.sim=list("delay4_ui8",4)
                    z0=uint8(z0)
                else
                    message("Datatype is not supported");
                    ok=%f
                end
                model.odstate=list(z0);
                model.dstate=[]
            end
            if ok then
                in=[1 1]
                [model,graphics,ok]=set_io(model,graphics,list(in,it),list(in,it),1,[])
            end
            if ok then
                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model
                break
            end
        end

    case "define" then
        z0=zeros(10,1)
        model=scicos_model()
        model.sim=list("delay4",4)
        model.in=1
        model.out=1
        model.evtin=1
        model.dstate=z0
        model.blocktype="d"
        model.dep_ut=[%f %f]

        exprs=strcat(string(z0), ";")
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
