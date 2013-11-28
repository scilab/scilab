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

function [x,y,typ]=HYSTHERESIS(job,arg1,arg2)
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
            [ok,high_lim,low_lim,out_high,out_low,nzz,exprs]=scicos_getvalue("Set parameters",..
            ["switch on at";"switch off at";"output when on";
            "output when off";"use zero crossing: yes (1), no (0)"],..
            list("vec",1,"vec",1,"vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if low_lim>high_lim then
                message("switch on value must be larger than switch off value")
            else
                graphics.exprs=exprs;
                model.rpar=[high_lim,low_lim,out_high,out_low]'
                if nzz>0 then
                    nzz=2,
                end
                model.nzcross=nzz
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        in=1
        ipar=[0] // rule
        nzz=2
        rpar=[1;0;1;0]

        model=scicos_model()
        model.sim=list("hystheresis",4)
        model.in=in
        model.out=1
        model.rpar=rpar
        model.nzcross=nzz
        model.nmode=1
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[string(rpar);string(sign(nzz))]


        gr_i=[]

        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
