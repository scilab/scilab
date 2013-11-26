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

function [x,y,typ]=MATCATH(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        model=arg1.model;
        graphics=arg1.graphics;
        label=graphics.exprs
        if size(label,"*")>1 then //compatibility
            label="size(evstr("+label(2)+"),''*'')";
        end
        while %t do
            [ok,nin,lab]=..
            scicos_getvalue("Set MATCATH block parameters",..
            ["Number of input"],..
            list("vec",1),label);
            if ~ok then
                break,
            end
            label=lab
            in=[-1*(ones(nin,1)) -([2:nin+1]')];
            out=[-1 0]
            it=-1*(ones(nin,1));
            ot=-1;
            [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
            if ok then
                funtyp=4;
                model.sim=list("mat_cath",funtyp)
                graphics.exprs=label
                arg1.graphics=graphics
                arg1.model=model
                x=arg1
                break
            end
        end
    case "define" then
        model=scicos_model()
        function_name="mat_cath";
        funtyp=4;
        model.sim=list(function_name,funtyp)
        model.in=[-1;-1]
        model.in2=[-2;-3]
        model.intyp=[1 1]
        model.out=-1
        model.out2=0
        model.outtyp=-1
        model.evtin=[]
        model.evtout=[]
        model.state=[]
        model.dstate=[]
        model.rpar=[]
        model.ipar=[]
        model.blocktype="c"
        model.firing=[]
        model.dep_ut=[%t %f]
        label=[sci2exp(2)];
        gr_i=[]
        x=standard_define([2 3],model,label,gr_i)
    end
endfunction
