//  Xcos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright 2011 - Bernard DUJARDIN <bernard.dujardin@contrib.scilab.org>
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

function [x,y,typ]=CONST_m(job,arg1,arg2)
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
            [ok, C, exprs] = scicos_getvalue([msprintf(gettext("Set %s block parameters"), "CONST_m");" "; ..
            gettext("Constant value generator");" "], gettext("Constant Value"), list("vec", -1), exprs)

            if ~ok then
                break,
            end
            nout=size(C)
            if find(nout==0)<>[] then
                block_parameter_error(msprintf(gettext("Wrong size for ''%s'' parameter"), gettext("Constant Value")), gettext("Constant value must have at least one element."));
            else
                model.sim=list("cstblk4_m",4)
                model.opar=list(C)
                if (type(C)==1) then
                    if isreal(C) then
                        ot=1
                    else
                        ot=2
                    end
                elseif (typeof(C)=="int32") then
                    ot=3
                elseif (typeof(C)=="int16") then
                    ot=4
                elseif (typeof(C)=="int8") then
                    ot=5
                elseif (typeof(C)=="uint32") then
                    ot=6
                elseif (typeof(C)=="uint16") then
                    ot=7
                elseif (typeof(C)=="uint8") then
                    ot=8
                else
                    block_parameter_error(msprintf(gettext("Wrong type for ''%s'' parameter"), gettext("Constant Value")), ..
                    gettext("Value type must be a numeric type (double, complex, int, int8, ...)."));
                    ok=%f;
                end

                if ok then
                    model.rpar=[]
                    [model,graphics,ok]=set_io(model,graphics,list(),list(nout,ot),[],[])
                    graphics.exprs=exprs;
                    x.graphics=graphics;
                    x.model=model
                    break;
                end
            end
        end
    case "define" then
        C=[1]

        model=scicos_model()
        model.sim=list("cstblk4",4)
        model.in=[]
        model.out=size(C,1)
        model.in2=[]
        model.out2=size(C,2)
        model.rpar=C
        model.opar=list()
        model.blocktype="d"
        model.dep_ut=[%f %f]
        exprs=sci2exp(C)
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
