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

function [x,y,typ]=DLSS_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        graphics=arg1.graphics;
        exprs=graphics.exprs
        if size(exprs,"*")==7 then
            exprs=exprs([1:4 7]),
        end //compatibility
        model=arg1.model;
        while %t do
            [ok,A,B,C,D,x0,exprs]=scicos_getvalue("Set discrete linear system parameters",..
            ["A matrix";
            "B matrix";
            "C matrix";
            "D matrix";
            "Initial state"],..
            list("mat",[-1,-1],..
            "mat",["size(%1,2)","-1"],..
            "mat",["-1","size(%1,2)"],..
            "mat",[-1 -1],..
            "vec","size(%1,2)"),..
            exprs)
            if ~ok then
                break,
            end
            out=size(C,1);
            if out==0 then
                out=[],
            end
            in=size(B,2);
            if in==0 then
                in=[],
            end
            [ms,ns]=size(A)
            if ms<>ns then
                message("A matrix must be square")
            else
                [model,graphics,ok]=check_io(model,graphics,in,out,1,[])
                if ok then
                    graphics.exprs=exprs;
                    rpar=[A(:);B(:);C(:);D(:)];
                    if D<>[] then
                        if norm(D,1)<>0 then
                            mmm=[%t %f];
                        else
                            mmm=[%f %f];
                        end
                        if or(model.dep_ut<>mmm) then
                            model.dep_ut=mmm,
                        end
                    else
                        model.dep_ut=[%f %f];
                    end
                    model.dstate=x0(:);
                    model.rpar=rpar
                    x.graphics=graphics;
                    x.model=model
                    break
                end
            end
        end
    case "define" then
        x0=0;A=-1;B=1;C=1;D=0;
        model=scicos_model()
        model.sim=list("dsslti",1)
        model.in=1
        model.out=1
        model.evtin=1
        model.dstate=x0(:)
        model.rpar=[A(:);B(:);C(:);D(:)]
        model.blocktype="d"
        model.dep_ut=[%f %f]

        exprs=[strcat(sci2exp(A));
        strcat(sci2exp(B));
        strcat(sci2exp(C));
        strcat(sci2exp(D));
        strcat(sci2exp(x0))]
        gr_i=[]
        x=standard_define([4 2],model,exprs,gr_i)
    end
endfunction
