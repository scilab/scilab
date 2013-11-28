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

function [x,y,typ]=IFTHEL_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        if exprs==[] then
            exprs=string(1);
        end
        if size(exprs,"*")==1 then
            exprs(2)=string(1);
        end
        while %t do
            [ok,inh,nmod,exprs]=scicos_getvalue("Set parameters",..
            ["Inherit (1: no, 0: yes)";"zero-crossing (0: no, 1: yes)"],..
            list("vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            model.dep_ut=[%t %f];  //compatibility
            if nmod<>0 then
                nmod=1,
            end
            if inh<>1 then
                inh=[];
            end
            [model,graphics,ok]=check_io(model,graphics,1,[],inh,[1;1])
            if ok then
                graphics.exprs=exprs;
                model.evtin=inh;
                model.sim(2)=-1
                model.nmode=nmod
                model.nzcross=nmod
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        model=scicos_model()
        model.sim=list("ifthel",-1)
        model.in=1
        model.in2=1
        model.intyp=-1
        model.evtin=1
        model.evtout=[1;1]
        model.blocktype="l"
        model.firing=[-1 -1]
        model.dep_ut=[%t %f]
        model.nmode=1
        model.nzcross=1

        gr_i=[]
        exprs=[string(model.in);string(model.nmode)];
        x=standard_define([3 3],model,exprs,gr_i)
    end
endfunction
