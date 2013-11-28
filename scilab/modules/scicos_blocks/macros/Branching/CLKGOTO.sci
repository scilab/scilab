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

function [x,y,typ]=CLKGOTO(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        model=arg1.model;
        exprs=graphics.exprs
        while %t do
            [ok,tag,tagvis,exprs]=scicos_getvalue("Set block parameters",..
            ["Tag";"Tag Visibility (1=Local 2=Scoped 3=Global)"], list("str",-1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if ((tagvis<1)|(tagvis>3)) then
                message("Tag Visibility must be between 1 and 3");ok=%f;
            end
            tagvis=int(tagvis);
            if ok then
                if ((model.opar<>list(tag))| (model.ipar<>tagvis)) then
                    needcompile=4;y=needcompile
                end
                model.opar=list(tag)
                model.ipar=tagvis
                model.evtin=1
                model.firing=-1//compatibility
                graphics.exprs=exprs
                x.graphics=graphics
                x.model=model
                break
            end
        end
        needcompile=resume(needcompile)
    case "define" then
        model=scicos_model()
        model.sim="clkgoto"
        model.evtin=1
        model.opar=list("A")
        model.ipar=int(1)
        model.blocktype="d"
        model.firing=-1
        model.dep_ut=[%f %f]

        exprs=["A";sci2exp(1)]
        x=standard_define([2 1],model,exprs," ")
        x.graphics.id="Goto"
    end
endfunction
