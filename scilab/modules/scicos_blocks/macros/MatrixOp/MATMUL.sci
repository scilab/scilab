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

function [x,y,typ] = MATMUL(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        graphics=x.graphics;
        label=graphics.exprs
        model=x.model;
        if model.ipar==[] then
            model.ipar=1;
        end
        if size(label,"*")==1 then
            label(2)=sci2exp(1),
        end //compatiblity
        if size(label,"*")==2 then
            label(3)=sci2exp(1);
        end //compatibility
        while %t do
            [ok,dtype,rule,np,exprs]=scicos_getvalue(["Set MATMUL parameter";
            "For the Multipication rule:";
            "    1= Matrix by Matrix";
            "    2= Matrix by Matrix element wise ";
            "    3= Matrix by Scalar";
            "In the third case the second input will be the scalar"],..
            ["Datatype(1=real double 2=Complex 3=int32 ...)";
            "Multiplication rule";
            "Do on Overflow(0=Nothing 1=Saturate 2=Error)"],list("vec",1,"vec",1,"vec",1),label)
            if ~ok then
                break,
            end
            rule=int(rule)
            if (dtype<1|dtype>8) then
                message("type is not supported");
                ok=%f;
            end
            if (rule<1|rule>3) then
                message("Multiplication rule must be only 1,2 or 3");
                ok=%f;
            end
            if (dtype==1|dtype==2) then
                np=0;
            end
            TABMIN=[0;0;-(2^31);-(2^15);-(2^7);0;0;0]
            TABMAX=[0;0;(2^31)-1;(2^15)-1;(2^7)-1;(2^32)-1;(2^16)-1;(2^8)-1]
            if rule==2 then
                if np==0 then
                    model.sim=list("matmul2_m",4)
                elseif np==1 then
                    model.sim=list("matmul2_s",4)
                else
                    model.sim=list("matmul2_e",4)
                end
            elseif rule==3 then
                if np==0 then
                    model.sim=list("matbyscal",4)
                elseif np==1 then
                    model.sim=list("matbyscal_s",4)
                else
                    model.sim=list("matbyscal_e",4)
                end
            else
                if (dtype==1) then
                    model.sim=list("matmul_m",4);
                elseif (dtype==2) then
                    model.sim=list("matzmul_m",4);
                elseif dtype==3 then
                    if np==0 then
                        model.sim=list("matmul_i32n",4)
                    elseif np==1 then
                        model.sim=list("matmul_i32s",4)
                    else
                        model.sim=list("matmul_i32e",4)
                    end
                elseif dtype==4 then
                    if np==0 then
                        model.sim=list("matmul_i16n",4)
                    elseif np==1 then
                        model.sim=list("matmul_i16s",4)
                    else
                        model.sim=list("matmul_i16e",4)
                    end
                elseif dtype==5 then
                    if np==0 then
                        model.sim=list("matmul_i8n",4)
                    elseif np==1 then
                        model.sim=list("matmul_i8s",4)
                    else
                        model.sim=list("matmul_i8e",4)
                    end
                elseif dtype==6 then
                    if np==0 then
                        model.sim=list("matmul_ui32n",4)
                    elseif np==1 then
                        model.sim=list("matmul_ui32s",4)
                    else
                        model.sim=list("matmul_ui32e",4)
                    end
                elseif dtype==7 then
                    if np==0 then
                        model.sim=list("matmul_ui16n",4)
                    elseif np==1 then
                        model.sim=list("matmul_ui16s",4)
                    else
                        model.sim=list("matmul_ui16e",4)
                    end
                elseif dtype==8 then
                    if np==0 then
                        model.sim=list("matmul_ui8n",4)
                    elseif np==1 then
                        model.sim=list("matmul_ui8s",4)
                    else
                        model.sim=list("matmul_ui8e",4)
                    end
                end
            end
            kmin=TABMIN(dtype);kmax=TABMAX(dtype)
            it=dtype*ones(1,2);
            ot=dtype
            if rule==1 then
                in=[-1 -2;-2 -3];
                out=[-1 -3];
            elseif rule==2 then
                in=[-1 -2;-1 -2]
                out=[-1 -2]
            else
                in=[-1 -2;1 1]
                out=[-1 -2]
            end
            [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
            if ok then
                //if model.ipar<>np then needcompile=1;end
                label=exprs;
                model.ipar=rule
                model.rpar=[kmin;kmax]
                graphics.exprs=label;
                x.graphics=graphics;
                x.model=model;
                arg1=x
                break;
            end
        end
        //needcompile=resume(needcompile)
    case "define" then
        model=scicos_model()
        model.sim=list("matmul_m",4)

        model.in=[-1;-2]
        model.in2=[-2;-3]
        model.out=-1
        model.out2=-3
        model.dep_ut=[%t %f]

        model.ipar=1
        label=[sci2exp(model.ipar)]
        gr_i=[]
        x=standard_define([3 2],model,label,gr_i)
    end
endfunction

