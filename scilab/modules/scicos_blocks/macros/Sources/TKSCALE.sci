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

function [x,y,typ]=TKSCALE(job,arg1,arg2)
    //Source block; output defined by tk widget scale
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        [ok,a,b,f,exprs]=scicos_getvalue("Set scale block parameters",..
        ["Min value";"Max value";"Normalization"],..
        list("vec",1,"vec",1,"vec",1),exprs)
        // tk widget returns a scalar, the value is divided by
        // Normalization factor
        if ok then
            graphics.exprs=exprs
            model.rpar=[a;b;f]
            x.graphics=graphics;
            x.model=model
        end
    case "define" then
        a=-10;
        b=10;
        f=1;// default parameter values
        model=scicos_model()
        model.sim=list("tkscaleblk",5)
        model.out=1
        model.evtin=1
        model.rpar=[a;b;f]
        model.blocktype="d"
        model.dep_ut=[%f %f]
        exprs=[sci2exp(a);sci2exp(b);sci2exp(f)]
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction




