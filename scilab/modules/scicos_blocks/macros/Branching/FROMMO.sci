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

function [x,y,typ]=FROMMO(job,arg1,arg2)
    x=[];
    y=[];
    typ=[]
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        while %t do
            [ok,tag,exprs]=scicos_getvalue("Set parameters",..
            ["Tag"],..
            list("str",-1),exprs)
            if ~ok then
                break,
            end
            if ok then
                if model.opar<>list(tag) then
                    needcompile=4;
                    y=needcompile,
                end
                graphics.exprs=exprs;
                model.opar=list(tag)
                x.model=model
                x.graphics=graphics
                break
            end
        end
        needcompile=resume(needcompile)
    case "define" then
        model=scicos_model()
        model.sim="frommo"
        model.in=[]
        model.in2=[]
        model.intyp=1
        model.out=-1
        model.out2=-2
        model.outtyp=-1
        model.ipar=[]
        model.opar=list("A")
        model.blocktype="c"
        model.dep_ut=[%f %f]
        mo=modelica()
        mo.model="frommo"
        mo.outputs="n"
        exprs=["A"]

        gr_i=[];
        x=standard_define([2 1],model,exprs,gr_i)
        x.graphics.out_implicit=["I"]
    end
endfunction
