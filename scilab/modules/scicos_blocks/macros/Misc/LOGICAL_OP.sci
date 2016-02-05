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

function [x,y,typ]=LOGICAL_OP(job,arg1,arg2)
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
            exprs=[exprs;sci2exp(1);sci2exp(0)];
        end
        while %t do
            [ok,nin,rule,Datatype,tp,exprs]=scicos_getvalue("Set parameters",..
            ["number of inputs";..
            "Operator: AND (0), OR (1), NAND (2), NOR (3), XOR (4), NOT (5)"
            "Datatype (1=double 3=int32 ...)";..
            "Bitwise Rule(0=No 1=yes)"],..
            list("vec",1,"vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            nin=int(nin);
            rule=int(rule);
            tp=int(tp)
            if nin<1 then
                message("Number of inputs must be >=1 ");
                ok=%f
            elseif (rule<0)|(rule>5) then
                message("Incorrect operator "+string(rule)+" ; must be 0 to 5.");
                ok=%f
            elseif (rule==5)&(nin>1) then
                message("Only one input allowed for NOT operation")
                nin=1
            elseif ((Datatype==1)&(tp~=0))
                message ("Bitwise Rule is only activated when Data type is integer");
                ok=%f
            end
            if ok then
                if (tp~=0) then
                    tp=1;
                end
                if Datatype==1 then
                    model.sim=list("logicalop",4)
                    model.ipar=[rule],
                else
                    if Datatype==3 then
                        model.sim=list("logicalop_i32",4)
                    elseif Datatype==4 then
                        model.sim=list("logicalop_i16",4)
                    elseif Datatype==5 then
                        model.sim=list("logicalop_i8",4)
                    elseif Datatype==6 then
                        model.sim=list("logicalop_ui32",4)
                    elseif Datatype==7 then
                        model.sim=list("logicalop_ui16",4)
                    elseif Datatype==8 then
                        model.sim=list("logicalop_ui8",4)
                    else
                        message ("Datatype is not supported");
                        ok=%f;
                    end
                    model.ipar=[rule;tp];
                end
                if ok then
                    it=Datatype*ones(nin,1);
                    ot=Datatype;
                    in=[-ones(nin,1) -2*ones(nin,1)]
                    if (rule<>5)&(nin==1) then
                        out=[1 1]
                        [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
                    else
                        out=[-1 -2]
                        [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
                    end
                end
                if ok then
                    if rule == 0 then
                        label = "AND";
                    elseif rule == 1 then
                        label = "OR";
                    elseif rule == 2 then
                        label = "NAND";
                    elseif rule == 3 then
                        label = "NOR";
                    elseif rule == 4 then
                        label = "XOR";
                    elseif rule == 5 then
                        label = "NOT";
                    end
                    graphics.exprs=exprs;
                    graphics.style = ["blockWithLabel;displayedLabel="+label];
                    x.graphics=graphics;
                    x.model=model
                    break
                end
            end
        end
    case "define" then
        in=[-1;-1]
        ipar=[0]
        nin=2

        model=scicos_model()
        model.sim=list("logicalop",4)
        model.in=in
        model.out=-1
        model.ipar=ipar
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[string(nin);string(ipar)]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
