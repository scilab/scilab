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

function [x,y,typ]=DOLLAR_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        //backward compatibility
        if size(exprs,"*")<2 then
            exprs(2)="0";
        end
        while %t do
            [ok,a,inh,exprs]=scicos_getvalue("Set 1/z block parameters",..
            ["initial condition";"Inherit (no:0, yes:1)"],...
            list("vec",-1,"vec",-1),exprs)
            if ~ok then
                break,
            end
            out=size(a,"*");
            if out==0 then
                out=[],
            end
            in=out

            if ok then
                [model,graphics,ok]=check_io(model,graphics,-1,-1,ones(1-inh,1),[])
            end

            if ok then
                graphics.exprs=exprs;
                model.dstate=a;
                model.in=in;
                model.out=out
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        z=0
        inh=0
        in=1
        exprs=string([z;inh])
        model=scicos_model()
        model.sim="dollar"
        model.in=in
        model.out=in
        model.evtin=1-inh
        model.dstate=z
        model.blocktype="d"
        model.dep_ut=[%f %f]

        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
