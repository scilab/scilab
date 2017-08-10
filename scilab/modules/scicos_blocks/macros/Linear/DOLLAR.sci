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

function [x,y,typ]=DOLLAR(job,arg1,arg2)
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
            list("mat",[-1 -2],"vec",-1),exprs)
            if ~ok then
                break,
            end
            out=[size(a,1) size(a,2)];
            if out==0 then
                out=[],
            end
            in=out
            model.sim=list("dollar4_m",4)
            model.odstate=list(a);
            model.dstate=[];
            if (type(a)==1) then
                if isreal(a) then
                    it=1;
                    ot=1;
                    if (size(a,1)==1 | size(a,2)==1) then
                        model.sim=list("dollar4",4);
                        model.dstate=a(:);
                        model.odstate=list();
                    end
                else
                    it=2;
                    ot=2;
                end
            elseif (typeof(a)=="int32") then
                it=3;
                ot=3;
            elseif (typeof(a)=="int16") then
                it=4;
                ot=4;
            elseif (typeof(a)=="int8") then
                it=5;
                ot=5;
            elseif (typeof(a)=="uint32") then
                it=6;
                ot=6;
            elseif (typeof(a)=="uint16") then
                it=7;
                ot=7;
            elseif (typeof(a)=="uint8") then
                it=8;
                ot=8;
            else
                message ("type is not recognized");
                ok=%f;
            end
            if ok then
                [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),ones(1-inh,1),[])
            end
            if ok then
                graphics.exprs=exprs;
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
        model.sim=list("dollar4",4)
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

