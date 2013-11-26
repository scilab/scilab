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

function [x,y,typ]=SCALAR2VECTOR(job,arg1,arg2)
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
            [ok,nout,exprs]=..
            scicos_getvalue("Set block parameters",..
            ["size of output (-1: if don''t know)"],..
            list("vec",1),exprs)
            if ~ok then
                break,
            end
            nout=int(nout)
            if(nout<>-1 & (nout<=0)) then
                message("size of output must be -1 or >0")
                ok=%f
            end
            if ok then
                [model,graphics,ok]=check_io(model,graphics,[1],nout,[],[])
            end
            if ok then
                graphics.exprs=exprs;// Correction ED le 24/11/04
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        nout=-1
        model=scicos_model()
        model.sim=list("scalar2vector",4)
        model.out=nout
        model.in=1
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[string([nout])]
        gr_i=[]

        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
