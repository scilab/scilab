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

function [x,y,typ]=ESELECT_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[]
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        if size(exprs,"*")==1 then
            exprs(2)=string(1);
        end
        if size(exprs,"*")==2 then
            exprs(3)=string(0);
        end
        model=arg1.model;
        while %t do
            [ok,out,inh,nmod,exprs]=scicos_getvalue("Set ESELECT block parameters",..
            ["number of output event ports";"Inherit (1: no, 0: yes)";
            "zero-crossing (0: no, 1: yes)"],..
            list("vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if nmod<>0 then
                nmod=1,
            end
            if inh==0 then
                inh=[];
            else
                inh=1;
            end
            out=int(out)
            if out<2 then
                message("Block must have at least two output ports")
            else
                [model,graphics,ok]=check_io(model,graphics,1,[],inh,[ones(out,1)])
                if ok then
                    graphics.exprs=exprs;
                    model.evtout=ones(out,1);
                    model.firing=-ones(out,1)
                    x.graphics=graphics;
                    model.nmode=nmod
                    model.nzcross=nmod
                    x.model=model
                    break
                end
            end
        end
    case "define" then
        out=2
        model=scicos_model()
        model.sim=list("eselect",-2)
        model.in=1
        model.in2=1
        model.intyp=-1
        model.evtin=1
        model.evtout=ones(out,1);
        model.blocktype="l"
        model.firing=-ones(out,1);
        model.dep_ut=[%t %f]
        model.nmode=0
        model.nzcross=0

        gr_i=[]
        exprs=[string(out);string(1);string(model.nmode)]
        x=standard_define([4 2],model,exprs,gr_i)
    end
endfunction
