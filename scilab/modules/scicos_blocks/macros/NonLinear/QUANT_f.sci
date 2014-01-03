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

function [x,y,typ]=QUANT_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        while %t then
            [ok,pas,meth,exprs]=scicos_getvalue("Set parameters",..
            ["Step";"Quantization Type (1-4)"],..
            list("vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if meth<1|meth>4 then
                message("Quantization Type must be from 1 to 4")
            else
                rpar=pas
                model.rpar=rpar
                model.ipar=meth
                select meth
                case 1 then
                    model.sim="qzrnd"
                case 2 then
                    model.sim="qztrn"
                case 3 then
                    model.sim="qzflr"
                case 4  then
                    model.sim="qzcel"
                end
                graphics.exprs=exprs
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        pas=0.1;
        meth=1
        model=scicos_model()
        model.sim="qzrnd"
        model.in=-1
        model.out=-1
        model.rpar=pas
        model.ipar=meth
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[string(pas);string(meth)]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
