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

function [x,y,typ]=CLR(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;exprs=graphics.exprs
        model=arg1.model;
        x0=model.state
        rpar=model.rpar
        ns=prod(size(x0));nin=1;nout=1
        %scicos_context=%scicos_context; //copy the semi-global variable locally
        %scicos_context.s=%s //add s definition to the context
        while %t do
            [ok,num,den,exprs]=scicos_getvalue("Set continuous SISO transfer parameters",..
            ["Numerator (s)";
            "Denominator (s)"],..
            list("pol",1,"pol",1),exprs)


            if ~ok then
                break,
            end
            if degree(num)>degree(den) then
                message("Transfer function must be proper or strictly proper.")
                ok=%f
            end
            if ok then
                H=cont_frm(num,den)
                [A,B,C,D]=H(2:5);
                graphics.exprs=exprs;
                [ns1,ns1]=size(A)
                rpar=[matrix(A,ns1*ns1,1);
                matrix(B,ns1,1);
                matrix(C,ns1,1);
                D]
                if norm(D,1)<>0 then
                    mmm=[%t %t];
                else
                    mmm=[%f %t];
                end
                if or(model.dep_ut<>mmm) then
                    model.dep_ut=mmm,
                end
                if ns1<=ns then
                    x0=x0(1:ns1)
                else
                    x0(ns1,1)=0
                end
                model.state=x0
                model.rpar=rpar
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        x0=0;
        A=-1;
        B=1;
        C=1;
        D=0;
        exprs=["1";"1+s"]
        model=scicos_model()
        model.sim=list("csslti4",4)
        model.in=1
        model.out=1
        model.state=x0
        model.rpar=[A(:);B(:);C(:);D(:)]
        model.blocktype="c"
        model.dep_ut=[%f %t]

        gr_i=[]

        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
