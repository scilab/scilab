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

function [x,y,typ] = RELATIONALOP(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        if size(exprs,1)==2 then
            exprs=[exprs;sci2exp(1)];
        end
        while %t do
            [ok,rule,zcr,Datatype,exprs]=scicos_getvalue("Set parameters",..
            ["Operator: == (0), ~= (1), < (2), <= (3), > (4), >= (5)";..
            "Use zero crossing (no: 0), (yes: 1)"
            "Datatype (1=double 3=int32 ...)"],..
            list("vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            rule=int(rule);
            if zcr<>0 then
                zcr=1,
            end
            if (rule<0)|(rule>5) then
                message("Incorrect operator "+string(rule)+" ; must be 0 to 5.")
                ok=%f;
            end
            if (Datatype==1) then
                model.sim=list("relational_op",4)
            elseif (Datatype==3|Datatype==9) then
                model.sim=list("relational_op_i32",4)
            elseif(Datatype==4) then
                model.sim=list("relational_op_i16",4)
            elseif(Datatype==5) then
                model.sim=list("relational_op_i8",4)
            elseif(Datatype==6) then
                model.sim=list("relational_op_ui32",4)
            elseif(Datatype==7) then
                model.sim=list("relational_op_ui16",4)
            elseif(Datatype==8) then
                model.sim=list("relational_op_ui8",4)
            else
                message("Datatype is not supported");
                ok=%f;
            end
            if ok then
                it=Datatype*ones(1,2)
                ot=Datatype
                in=[-1 -2;-1 -2]
                out=[-1 -2]
                [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
            end
            if ok then
                if rule == 0 then
                    label = "==";
                elseif rule == 1 then
                    label = "~=";
                elseif rule == 2 then
                    label = "&#60;"; // <
                elseif rule == 3 then
                    label = "&#8804;"; // <=
                elseif rule == 4 then
                    label = "&#62;"; // >
                elseif rule == 5 then
                    label = "&#8805;"; // >=
                end
                graphics.exprs=exprs;
                graphics.style=["fontSize=13;fontStyle=1;displayedLabel="+label];
                model.ipar=[rule],
                model.nzcross=zcr,
                model.nmode=zcr,
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        ipar=[2]
        label="&lt";
        model=scicos_model()
        model.sim=list("relationalop",4)
        model.in=[1;1]
        model.out=1
        model.ipar=ipar
        model.blocktype="c"
        model.dep_ut=[%t %f]
        exprs=[string(ipar);string(0)]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
        x.graphics.style=["fontSize=13;fontStyle=1;displayedLabel="+label];
    end
endfunction
