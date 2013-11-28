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

function [x,y,typ]=SAT_f(job,arg1,arg2)
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
            [ok,minp,maxp,pente,exprs]=scicos_getvalue("Set Saturation parameters",..
            ["Min";"Max";"Slope"],list("vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if maxp<=0  then
                message("Max must be strictly positive")
            elseif pente<=0 then
                message("Slope must be strictly positive")
            else
                rpar=[minp/pente;maxp/pente;pente]
                model.rpar=rpar
                model.firing=[] //compatibility
                graphics.exprs=exprs
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        minp=-1;
        maxp=1;
        slope=1;
        rpar=[minp;maxp;slope]

        model=scicos_model()
        model.sim=list("lusat",1)
        model.in=1
        model.nzcross=2
        model.out=1
        model.rpar=[minp;maxp;slope]
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[string(minp);string(maxp);string(slope)]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
