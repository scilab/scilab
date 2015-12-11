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

function [x,y,typ]=RICC(job,arg1,arg2)
    //
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        graphics=arg1.graphics;
        label=graphics.exprs
        model=arg1.model;
        if size(label,"*")==14 then
            label(9)=[],
        end //compatiblity
        while %t do
            [ok,tpe,mod,exprs]=scicos_getvalue("Set RICC Block",["Type (1=Cont  2=Disc)";"Model(1=Schr  2=sign(cont) inv(disc))"],list("vec",1,"vec",1),label)
            if ~ok then
                break,
            end
            in=[model.in model.in2];
            out=[model.out model.out2];
            it=[1 1 1]
            ot=1
            label=exprs;
            [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
            if ok then
                model.ipar=[tpe;mod]
                graphics.exprs=label;
                arg1.graphics=graphics;
                arg1.model=model;
                x=arg1
                break
            end
        end
    case "define" then
        model=scicos_model()
        function_name="ricc_m";
        funtyp=4;
        model.sim=list(function_name,funtyp)
        model.in=[-1;-1;-1]
        model.in2=[-1;-1;-1]
        model.intyp=[1 1 1]
        model.out=-1
        model.out2=-1
        model.outtyp=1
        model.evtin=[]
        model.evtout=[]
        model.state=[]
        model.dstate=[]
        model.rpar=[]
        model.ipar=[1;1]
        model.blocktype="c"
        model.firing=[]
        model.dep_ut=[%t %f]
        label=[sci2exp(1);sci2exp(1)];
        gr_i=[]
        x=standard_define([2 2],model,label,gr_i)
    end
endfunction
