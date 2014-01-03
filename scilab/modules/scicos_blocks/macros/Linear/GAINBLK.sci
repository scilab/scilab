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

function [x,y,typ] = GAINBLK(job,arg1,arg2)

    x=[];
    y=[];
    typ=[];

    select job

    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        if size(exprs,"*")==1 then
            exprs=[exprs;sci2exp(0)];
        end // compatibility
        while %t do
            [ok,gain,over,exprs]=scicos_getvalue("Set gain block parameters",..
            ["Gain";..
            "Do On Overflow(0=Nothing 1=Saturate 2=Error)"],..
            list("mat",[-1,-1],"vec",1),exprs)
            if ~ok then
                break,
            end
            if gain==[] then
                message("Gain must have at least one element")
            else
                if typeof(gain)=="constant" then
                    if isreal(gain) then
                        it =1;
                        ot=1;
                        model.sim=list("gainblk",4);
                        model.rpar=gain(:);
                        model.opar=list();
                    else
                        message("type is not supported");
                        ok=%f;
                    end
                else
                    if (over==0) then
                        if (typeof(gain)=="int32") then
                            ot=3
                            model.sim=list("gainblk_i32n",4)
                        elseif (typeof(gain)=="int16") then
                            ot=4
                            model.sim=list("gainblk_i16n",4)
                        elseif (typeof(gain)=="int8") then
                            ot=5
                            model.sim=list("gainblk_i8n",4)
                        elseif (typeof(gain)=="uint32") then
                            ot=6
                            model.sim=list("gainblk_ui32n",4)
                        elseif (typeof(gain)=="uint16") then
                            ot=7
                            model.sim=list("gainblk_ui16n",4)
                        elseif (typeof(gain)=="uint8") then
                            ot=8
                            model.sim=list("gainblk_ui8n",4)
                        else
                            message("type is not supported.");
                            ok=%f;
                        end
                    elseif (over==1) then
                        if (typeof(gain)=="int32") then
                            ot=3
                            model.sim=list("gainblk_i32s",4)
                        elseif (typeof(gain)=="int16") then
                            ot=4
                            model.sim=list("gainblk_i16s",4)
                        elseif (typeof(gain)=="int8") then
                            ot=5
                            model.sim=list("gainblk_i8s",4)
                        elseif (typeof(gain)=="uint32") then
                            ot=6
                            model.sim=list("gainblk_ui32s",4)
                        elseif (typeof(gain)=="uint16") then
                            ot=7
                            model.sim=list("gainblk_ui16s",4)
                        elseif (typeof(gain)=="uint8") then
                            ot=8
                            model.sim=list("gainblk_ui8s",4)
                        else
                            message("type is not supported.");
                            ok=%f;
                        end
                    elseif (over==2) then
                        if (typeof(gain)=="int32") then
                            ot=3
                            model.sim=list("gainblk_i32e",4)
                        elseif (typeof(gain)=="int16") then
                            ot=4
                            model.sim=list("gainblk_i16e",4)
                        elseif (typeof(gain)=="int8") then
                            ot=5
                            model.sim=list("gainblk_i8e",4)
                        elseif (typeof(gain)=="uint32") then
                            ot=6
                            model.sim=list("gainblk_ui32e",4)
                        elseif (typeof(gain)=="uint16") then
                            ot=7
                            model.sim=list("gainblk_ui16e",4)
                        elseif (typeof(gain)=="uint8") then
                            ot=8
                            model.sim=list("gainblk_ui8e",4)
                        else
                            message("type is not an integer.");
                            ok=%f;
                        end
                    else
                        message("Do on Overflow must be 0,1,2");
                        ok=%f;
                    end
                    model.rpar=[];
                    model.opar=list(gain(:));
                end
                if ok then
                    [out,in]=size(gain)
                    if out*in<>1 then
                        [model,graphics,ok]=set_io(model,graphics,...
                        list([in,-1],ot),...
                        list([out,-1],ot),[],[])
                    else
                        [model,graphics,ok]=set_io(model,graphics,...
                        list([-1,-2],ot),...
                        list([-1,-2],ot),[],[])
                    end
                end
                if ok then
                    graphics.exprs=exprs
                    x.graphics=graphics;
                    x.model=model
                    break
                end
            end
        end
    case "define" then
        gain=1
        in=-1;out=-1
        in2=-2;out2=-2
        model=scicos_model()
        model.sim=list("gainblk",4)
        model.in=in
        model.out=out
        model.in2=in2
        model.out2=out2
        model.rpar=gain
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[strcat(sci2exp(gain))]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
