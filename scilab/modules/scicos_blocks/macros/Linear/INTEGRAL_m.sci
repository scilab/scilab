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

function [x,y,typ] = INTEGRAL_m(job,arg1,arg2)
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
            [ok,x0,reinit,satur,maxp,lowp,exprs]=scicos_getvalue("Set Integral block parameters",..
            ["Initial Condition";
            "With re-initialization (1:yes, 0:no)";"With saturation (1:yes, 0:no)";
            "Upper limit";"Lower limit"],..
            list("mat",[-1 -1],"vec",1,"vec",1,"mat",[-1 -1],"mat",[-1 -1]),exprs)
            if ~ok then
                break,
            end
            if isreal(x0) then
                Datatype=1;
            else
                Datatype=2;
            end;
            if reinit<>0 then
                reinit=1;
            end
            if satur<>0 then
                satur=1;
                if Datatype==1 then
                    if size(maxp,"*")==1 then
                        maxp=maxp*ones(x0),
                    end
                    if size(lowp,"*")==1 then
                        lowp=lowp*ones(x0),
                    end
                    if (size(x0)<>size(maxp) | size(x0)<>size(lowp)) then
                        message("x0 and Upper limit and Lower limit must have same size")
                        ok=%f
                    elseif or(maxp<=lowp)  then
                        message("Upper limits must be > Lower limits")
                        ok=%f
                    elseif or(x0>maxp)|or(x0<lowp) then
                        message("Initial condition x0 should be inside the limits")
                        ok=%f
                    else
                        rpar=[real(maxp(:));real(lowp(:))]
                        model.nzcross=size(x0,"*")
                        model.nmode=size(x0,"*")
                    end
                elseif (Datatype==2) then
                    if size(maxp,"*")==1 then
                        maxp=maxp*ones(x0)+%i*(maxp*ones(x0)),
                    end
                    if size(lowp,"*")==1 then
                        lowp=lowp*ones(x0)+%i*(lowp*ones(x0)),
                    end
                    if (size(x0)<>size(maxp) | size(x0)<>size(lowp)) then
                        message("x0 and Upper limit and Lower limit must have same size")
                        ok=%f
                    elseif or(real(maxp)<=real(lowp))| or(imag(maxp)<=imag(lowp)) then
                        message("Upper limits must be > Lower limits")
                        ok=%f
                    elseif or(real(x0)>real(maxp))|or(real(x0)<real(lowp))| or(imag(x0)>imag(maxp))|or(imag(x0)<imag(lowp)) then
                        message("Initial condition x0 should be inside the limits")
                        ok=%f
                    else
                        rpar=[real(maxp(:));real(lowp(:));imag(maxp(:));imag(lowp(:))]
                        model.nzcross=2*size(x0,"*")
                        model.nmode=2*size(x0,"*")
                    end
                end
            else
                rpar=[]
                model.nzcross=0
                model.nmode=0
            end
            if ok then
                model.rpar=rpar
                if (Datatype==1) then
                    model.state=real(x0(:))
                    model.sim=list("integral_func",4)
                    it=[1;ones(reinit,1)]
                    ot=1;
                elseif (Datatype==2) then
                    model.state=[real(x0(:));imag(x0(:))];
                    model.sim=list("integralz_func",4)
                    it=[2;2*ones(reinit,1)]
                    ot=2;
                else
                    message("Datatype is not supported");
                    ok=%f;
                end
                if ok then
                    in=[size(x0,1)*[1;ones(reinit,1)],size(x0,2)*[1;ones(reinit,1)]]
                    out=size(x0)
                    [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),ones(reinit,1),[])
                end
            end
            if ok then
                graphics.exprs=exprs
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        maxp=1;
        minp=-1;
        rpar=[]
        model=scicos_model()
        model.state=0
        model.sim=list("integral_func",4)
        model.in=1
        model.out=1
        model.in2=1
        model.out2=1
        model.rpar=rpar
        model.blocktype="c"
        model.dep_ut=[%f %t]

        exprs=string([0;0;0;maxp;minp])
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
