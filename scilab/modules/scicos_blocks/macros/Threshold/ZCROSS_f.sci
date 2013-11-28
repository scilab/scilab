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

function [x,y,typ]=ZCROSS_f(job,arg1,arg2)
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
            [ok,in,exprs]=scicos_getvalue(["Set Zero-Crossing parameters";..
            "All surfaces must cross together"],..
            "Input size",list("vec",1),exprs)
            if ~ok then
                break,
            end
            in=int(in)
            if in<=0 then
                message("Block must have at least one input")
            else
                kk=0
                for jj=1:in
                    kk=kk+2^(in+jj-1)
                end
                model.rpar=[-ones(kk,1);zeros(2^(2*in)-kk,1)]
                graphics.exprs=exprs
                model.in=in
                model.nzcross=in
                model.firing=-1 //compatibility
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        rpar=[-1;-1;0;0]
        in=1

        model=scicos_model()
        model.sim=list("zcross",1)
        model.in=in
        model.nzcross=in
        model.evtout=1
        model.rpar=[-1;-1;0;0]
        model.blocktype="z"
        model.firing=-1
        model.dep_ut=[%t %f]

        exprs=strcat(sci2exp(in))
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
