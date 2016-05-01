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

function [x,y,typ]=SOM_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    p=1 //pixel sizes ratio
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        model=arg1.model;
        exprs=graphics.exprs
        if size(exprs,"*")==2 then
            exprs=exprs(2),
        end
        if size(exprs,"*")<>3 then
            exprs=string(model.rpar),
        end

        orient=%t;
        style=graphics.style;
        subStr=strstr(style,"flip=");
        if subStr<>"" then
            orient=%f;
        end
        if orient then
            labs=["down","left","up"]
        else
            labs=["down","right","up"]
        end

        str = ["This sum block is obsolete"
        "parameters cannot be modified. Please replace it with new sum block";
        "and gain blocks in the linear palette"
        " "
        "Input ports are located at up, side and  down positions."
        "Current gains are:"];

        str = [str ; (part(labs(:),1:7)+exprs(:))];
        message(str);

    case "define" then
        sgn=[1;1;1]

        model=scicos_model()
        model.sim=list("sum",2)
        model.in=[-1;-1;-1]
        model.out=-1
        model.rpar=sgn
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[sci2exp(1);sci2exp(sgn)]

        gr_i=[]

        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
