//  Scicos
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

function [x,y,typ]=INTMUL(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics
        model=arg1.model
        exprs=graphics.exprs
        while %t do
            [ok,Datatype,np,exprs]=scicos_getvalue( ..
            [msprintf(gettext("Set %s block parameters"),"INTMUL"); " "; gettext("Integer matrix multiplication");" ";],..
            [msprintf(gettext("Data Type %s"), "(3:int32, 4:int16, 5:int8, ...)"); gettext("Do on Overflow (0:Nothing, 1:Saturate, 2:Error)")],..
            list("vec",1,"vec",1), exprs)
            if ~ok then
                break,
            end

            it=Datatype*ones(1,2);
            ot=Datatype;
            if (np~=0 & np~=1 & np~=2) then
                block_parameter_error(msprintf( gettext("Wrong value for ''%s'' parameter: %d."), gettext("Do on Overflow"), np), ..
                msprintf( gettext("Must be in the interval %s."), "[0, 2]"));
                ok=%f;
            elseif Datatype==3 then
                if np==0 then
                    model.sim=list("matmul_i32n",4)
                elseif np==1 then
                    model.sim=list("matmul_i32s",4)
                else
                    model.sim=list("matmul_i32e",4)
                end
            elseif Datatype==4 then
                if np==0 then
                    model.sim=list("matmul_i16n",4)
                elseif np==1 then
                    model.sim=list("matmul_i16s",4)
                else
                    model.sim=list("matmul_i16e",4)
                end
            elseif Datatype==5 then
                if np==0 then
                    model.sim=list("matmul_i8n",4)
                elseif np==1 then
                    model.sim=list("matmul_i8s",4)
                else
                    model.sim=list("matmul_i8e",4)
                end
            elseif Datatype==6 then
                if np==0 then
                    model.sim=list("matmul_ui32n",4)
                elseif np==1 then
                    model.sim=list("matmul_ui32s",4)
                else
                    model.sim=list("matmul_ui32e",4)
                end
            elseif Datatype==7 then
                if np==0 then
                    model.sim=list("matmul_ui16n",4)
                elseif np==1 then
                    model.sim=list("matmul_ui16s",4)
                else
                    model.sim=list("matmul_ui16e",4)
                end
            elseif Datatype==8 then
                if np==0 then
                    model.sim=list("matmul_ui8n",4)
                elseif np==1 then
                    model.sim=list("matmul_ui8s",4)
                else
                    model.sim=list("matmul_ui8e",4)
                end
            else
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Data Type"), ot), ..
                msprintf(gettext("Must be in the interval %s."), "[3, 8]"));
                ok=%f;

            end
            in=[model.in model.in2]
            out=[model.out model.out2]
            if ok then
                [model,graphics,ok]=set_io(model,graphics,...
                list(in,it),...
                list(out,ot),[],[])
            end
            if ok then
                model.ipar=np
                graphics.exprs=exprs
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        sgn=0
        model=scicos_model()
        model.sim=list("matmul_i32",4)
        model.in=[-1;-2]
        model.out=-1
        model.in2=[-2;-3]
        model.out2=-3
        model.intyp=[3 3]
        model.outtyp=3
        model.rpar=[]
        model.ipar=sgn
        model.blocktype="c"
        model.dep_ut=[%t %f]


        exprs=[sci2exp(3);sci2exp(0)]
        gr_i=[]
        x=standard_define([2 2],model, exprs,gr_i)
    end
endfunction
