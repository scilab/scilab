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

function [x,y,typ]=DELAYV_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        nin=model.in(1)
        z0=model.dstate;
        zz0=z0(1:$-1);
        told=z0($);

        while %t do
            [ok,nin,zz0,T,exprs]=scicos_getvalue("Set delay parameters",..
            ["Number of inputs";
            "Register initial condition";
            "Max delay"],..
            list("vec",1,"vec",-1,"vec",1),..
            exprs);
            if ~ok then
                break,
            end
            if size(zz0,"*")<2 then
                message("Register length must be at least 2")
                ok=%f
            end
            if T<=0 then
                message("Delay must be positive")
                ok=%f
            end

            if ok then
                [model,graphics,ok]=check_io(model,graphics,[nin;1],nin,1,[1;1])
            end
            if ok then
                graphics.exprs=exprs;
                model.dstate=[zz0(:);told];
                model.rpar=T/(size(zz0,"*"));
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        nin=1
        z0=zeros(11,1);
        zz0=z0(1:$-1)
        T=1

        model=scicos_model()
        model.sim=list("delayv",1)
        model.in=[nin;1]
        model.out=nin
        model.evtin=1
        model.evtout=[1;1]
        model.dstate=z0
        model.rpar=T/(size(zz0,"*"))
        model.blocktype="d"
        model.firing=[0 -1]
        model.dep_ut=[%t %f]

        exprs=[string(nin);strcat(string(z0(1:$-1)),";");string(T)];
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
