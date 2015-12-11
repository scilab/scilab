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

function [x,y,typ]=M_SWITCH(job,arg1,arg2)
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
            [ok,nin,base,rule,exprs]=scicos_getvalue("Set parameters",..
            ["number of inputs";"zero base indexing (0), otherwise 1";..
            "rounding rule: int (0), round (1), ceil (2), floor (3)"],..
            list("vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            nin=int(nin);
            base=int(base);
            if nin<1 then
                message("Number of inputs must be >=1 ")
            elseif ~((base==1)|(base==0)) then
                message("base indexing must be 1 or 0")
            elseif ~((rule==1)|(rule==0)|(rule==2)|(rule==3)) then
                message("incorrect rounding rule")
            else
                if nin==1 then
                    in=[1 1;-1 1];
                    out=[1 1];
                else
                    in1=[1;-ones(nin,1)];
                    in2=[1;-2*ones(nin,1)];
                    in=[in1 in2];
                    out=[-1 -2];
                end
                it=[-1;-2*ones(nin,1)];
                ot=-2;
                [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
                //[model,graphics,ok]=check_io(model,graphics,[1;-ones(nin,1)],-1,[],[])
                if ok then
                    graphics.exprs=exprs;
                    model.ipar=[base;rule],
                    x.graphics=graphics;
                    x.model=model
                    break
                end
            end
        end
    case "define" then
        in=[1;-1;-1]
        ipar=[1;3]
        nin=2

        model=scicos_model()
        model.sim=list("mswitch",4)
        model.in=in
        model.out=-1
        model.ipar=ipar
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[string(nin);string(ipar)]

        gr_i=[]
        x=standard_define([2.5 2],model,exprs,gr_i)
    end
endfunction
