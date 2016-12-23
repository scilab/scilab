//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) 2016-2016 - Scilab Enterprises - Clement David
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

function [x,y,typ]=INVBLK(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;

        // Compatibility before 6.0.0, there was no divideByZero parameter
        if exprs == " " then
            exprs($+1) = "1";
        end

        while %t do
            [ok,divideByZero,exprs]=..
            scicos_getvalue("Set block parameters",..
            ["Error on divide by zero (1: yes) (0:no)"],..
            list("vec",1),exprs)
            if ~ok then
                break,
            end
            graphics.exprs=exprs
            if ok then
                if divideByZero==0 then
                    model.rpar=%eps;
                else
                    model.rpar=0;
                end
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        in=-1
        model=scicos_model()
        model.sim=list("invblk4",4)
        model.in=in
        model.out=in
        model.rpar=0;
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs="1"
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
