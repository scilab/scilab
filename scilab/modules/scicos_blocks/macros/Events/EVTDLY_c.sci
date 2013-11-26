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

function [x,y,typ]=EVTDLY_c(job,arg1,arg2)
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
            [ok,dt,ff,exprs]=scicos_getvalue(["Set Event Delay block parameters";
            "Delay  is the delay between an input event ";
            "       and the generated output event";
            "Block may initially generate an output event before ";
            "       any input event. ""Date of initial output event""";
            "       gives the date of this event. Set a negative value";
            "       to disable any output event."],..
            ["Delay";"Date of initial output event"],..
            list("vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if dt<=0 then
                message("Delay must be positive")
                ok=%f
            end
            if ok then
                graphics.exprs=exprs
                model.rpar=[dt;ff]
                model.firing=ff
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        dt=0.1
        ff=0.0
        model=scicos_model()
        model.sim=list("evtdly4",4)
        model.evtin=1
        model.evtout=1
        model.rpar=[dt;ff]
        model.blocktype="d"
        model.firing=ff
        model.dep_ut=[%f %f]

        exprs=[string(dt);sci2exp(ff)]
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
