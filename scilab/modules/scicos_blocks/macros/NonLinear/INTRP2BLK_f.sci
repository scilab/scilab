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

function [x,y,typ]=INTRP2BLK_f(job,arg1,arg2)
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
            [ok,a,b,c,exprs]=scicos_getvalue("Set Interpolation block parameters",..
            ["X coord.";"Y coord.";"Z values"],list("vec",-1,"vec",-1,"mat",[-1,-1]),exprs)
            if ~ok then
                break,
            end
            if size(a,"*") <> size(c,"c") | size(b,"*") <> size(c,"r") then
                message("incompatible dimension")
            elseif min(a(2:$)-a(1:$-1)) <=0 | min(b(2:$)-b(1:$-1)) <=0 then
                message("X and Y must be strictly increasing")
            else
                if ok then
                    graphics.exprs=exprs
                    model.rpar=[a(:);b(:);c(:)]
                    model.ipar=[size(a,"*");size(b,"*")]
                    x.graphics=graphics;
                    x.model=model
                    break
                end
            end
        end
    case "define" then
        a=[0;1];
        b=[0;1];
        c=[0 1;1 2]
        model=scicos_model()
        model.sim=list("intrp2",1)
        model.in=[1;1]
        model.out=1
        model.rpar=[a;b;c(:)]
        model.ipar=[2;2]
        model.blocktype="c"
        model.dep_ut=[%t %f]
        exprs=[strcat(sci2exp(a));strcat(sci2exp(b));strcat(sci2exp(c,0))]
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
