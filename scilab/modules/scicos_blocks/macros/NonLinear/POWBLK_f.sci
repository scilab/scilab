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

function [x,y,typ]=POWBLK_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        if size(exprs,"*")==2 then
            exprs=exprs(2),
        end //compatibility
        while %t do
            [ok,a,exprs]=scicos_getvalue("u^a: raise each element of the input",..
            "to the power of",list("vec",1),exprs)
            if ~ok then
                break,
            end
            graphics.exprs=exprs
            if a==int(a) then
                model.ipar=a;
                model.rpar=[]
            else
                model.rpar=a;
                model.ipar=[]
            end
            model.firing=[] //compatibility
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        in=1
        a=1.5
        model=scicos_model()
        model.sim="powblk"
        model.in=-1
        model.out=-1
        model.rpar=a
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=string(a)
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
