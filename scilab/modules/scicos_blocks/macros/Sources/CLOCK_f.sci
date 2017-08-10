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

function [x,y,typ]=CLOCK_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        // look for the internal edge trigger block
        for i=1:length(arg1.model.rpar.objs) do
            o = arg1.model.rpar.objs(i);
            if typeof(o) == "Block" & o.gui == "EVTDLY_f" then
                path = i;
                break;
            end
        end

        newpar=list();
        xx=arg1.model.rpar.objs(path)// get the evtdly block
        exprs=xx.graphics.exprs
        model=xx.model;
        t0_old=model.firing
        dt_old= model.rpar
        model_n=model
        while %t do
            [ok,dt,t0,exprs0]=scicos_getvalue("Set Clock  block parameters",..
            ["Period";"Init time"],list("vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if dt<=0 then
                message("period must be positive")
                ok=%f
            end
            if ok then
                xx.graphics.exprs=exprs0
                model.rpar=dt
                model.firing=t0
                xx.model=model
                arg1.model.rpar.objs(path)=xx// Update
                break
            end
        end
        if ~and([t0_old dt_old]==[t0 dt])|~and(exprs0==exprs) then
            // parameter  changed
            newpar(size(newpar)+1)=path// Notify modification
        end
        if t0_old<>t0 then
            needcompile=2,
        else
            needcompile=0,
        end
        x=arg1
        y=needcompile
        typ=newpar
    case "define" then
        evtdly=EVTDLY_f("define")
        evtdly.graphics.orig=[320,232]
        evtdly.graphics.sz=[40,40]
        evtdly.graphics.exprs=["0.1";"0.1"]
        evtdly.graphics.pein=6
        evtdly.graphics.peout=3
        evtdly.model.rpar=0.1
        evtdly.model.firing=0.1

        output_port=CLKOUT_f("define")
        output_port.graphics.orig=[399,162]
        output_port.graphics.sz=[20,20]
        output_port.graphics.exprs="1"
        output_port.graphics.pein=5
        output_port.model.ipar=1

        split=CLKSPLIT_f("define")
        split.graphics.orig=[380.71066;172]
        split.graphics.pein=3,
        split.graphics.peout=[5;6]

        gr_i=[]
        diagram=scicos_diagram();
        diagram.objs(1)=output_port
        diagram.objs(2)=evtdly
        diagram.objs(3)=scicos_link(xx=[340;340;380.71],..
        yy=[226.29;172;172],..
        ct=[5,-1],from=[2,1],to=[4,1])
        diagram.objs(4)=split
        diagram.objs(5)=scicos_link(xx=[380.71;399],yy=[172;172],..
        ct=[5,-1],from=[4,1],to=[1,1])
        diagram.objs(6)=scicos_link(xx=[380.71;380.71;340;340],..
        yy=[172;302;302;277.71],..
        ct=[5,-1],from=[4,2],to=[2,1])
        x=scicos_block()
        x.gui="CLOCK_f"
        x.graphics.sz=[2,2]
        x.graphics.gr_i=gr_i
        x.graphics.peout=0
        x.model.sim="csuper"
        x.model.evtout=1
        x.model.blocktype="h"
        x.model.firing=%f
        x.model.dep_ut=[%f %f]
        x.model.rpar=diagram
    end
endfunction
