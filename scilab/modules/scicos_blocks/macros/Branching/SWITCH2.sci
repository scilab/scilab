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

function [x,y,typ]=SWITCH2(job,arg1,arg2)
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
            [ok,rule,thra,nzz,exprs]=scicos_getvalue("Set parameters",..
            ["pass first input if: u2>=a (0), u2>a (1), u2~=a (2)";..
            "threshold a";"use zero crossing: yes (1), no (0)"],..
            list("vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            rule=int(rule);
            if (rule<0) then
                rule=0,
            end
            if (rule>2) then
                rule=2,
            end
            graphics.exprs=exprs;
            model.ipar=rule
            model.rpar=thra
            if nzz<>0 then
                model.nmode=1
                model.nzcross=1
            else
                model.nmode=0
                model.nzcross=0
            end
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        in=[-1;1;-1]
        ipar=[0] // rule
        nzz=1
        rpar=0

        model=scicos_model()
        model.sim=list("switch2",4)
        model.in=in
        model.out=-1
        model.ipar=ipar
        model.rpar=rpar
        model.nzcross=nzz
        model.nmode=1
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[string(ipar);string(rpar);string(nzz)]

        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
