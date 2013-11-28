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

function [x,y,typ]=CLKOUT_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        model=arg1.model;

        exprs=graphics.exprs;
        exprs=exprs(1) //for compatibility
        while %t do
            [ok,prt,exprs]=scicos_getvalue("Set Event Output block parameters",..
            "Port number",list("vec",1),exprs)
            if ~ok then
                break,
            end
            prt=int(prt)
            if prt<=0 then
                message("Port number must be a positive integer")
            else
                model.ipar=prt
                model.evtin=1
                model.firing=[]//compatibility
                graphics.exprs=exprs
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        prt=1

        model=scicos_model()
        model.sim="output"
        model.evtin=1
        model.ipar=prt
        model.blocktype="d"
        model.firing=[]
        model.dep_ut=[%f %f]

        exprs=string(prt)
        x=standard_define([1 1],model,exprs," ")
    end
endfunction
