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

function [x,y,typ]=SELECT_m(job,arg1,arg2)
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
            [ok,typ,nin,z0,exprs]=scicos_getvalue("Set parameters",..
            ["Datatype(1= real double  2=Complex 3=int32 ..)";"number of inputs";"initial connected input"],..
            list("vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            if z0>nin|z0<=0 then
                message("initial connected input is not a valid input port number")
            elseif ((typ<1)|(typ>8))& (typ<>-1) then
                message("Datatype is not supported");
                ok=%f;
            else
                it=typ*ones(1,nin)
                ot=typ
                if ok then
                    in=[-ones(nin,1) -2*ones(nin,1)]
                    out=[-1 -2]
                    [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),ones(nin,1),[])
                    if ok then
                        graphics.exprs=exprs;
                        model.dstate=z0,
                        x.graphics=graphics;
                        x.model=model
                        break
                    end
                end
            end
        end
    case "define" then
        z0=1
        nin=2

        model=scicos_model()
        model.sim=list("selector_m",4)
        model.in=[-1;-1]
        model.in2=[-2;-2]
        model.intyp=1
        model.out=-1
        model.out2=-2
        model.outtyp=1
        model.evtout=[]
        model.state=[]
        model.rpar=[]
        model.ipar=[]
        model.firing=[]
        model.evtin=ones(nin,1)
        model.dstate=z0
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[sci2exp(1);sci2exp(nin);sci2exp(z0)]
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
