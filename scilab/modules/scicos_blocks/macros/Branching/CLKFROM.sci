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

function [x,y,typ]=CLKFROM(job,arg1,arg2)
    x=[];y=[];typ=[]
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        model=arg1.model;
        exprs=graphics.exprs
        while %t do
            [ok,tag,exprs]=scicos_getvalue("Set block parameters",..
            "Tag",list("str",-1),exprs)
            if ~ok then
                break,
            end
            if model.opar<>list(tag) then
                needcompile=4;
                y=needcompile,
            end
            model.opar=list(tag)
            model.evtout=1
            model.firing=-1//compatibility
            graphics.exprs=exprs
            x.graphics=graphics
            x.model=model
            break
        end
        needcompile=resume(needcompile)
    case "define" then
        model=scicos_model()
        model.sim="clkfrom"
        model.evtout=1
        model.opar=list("A")
        model.blocktype="d"
        model.firing=-1
        model.dep_ut=[%f %f]

        exprs="A"
        x=standard_define([2 1],model,exprs," ")
        x.graphics.id="From"
    end
endfunction
