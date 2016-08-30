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

function [x,y,typ]=CONSTRAINT2_c(job,arg1,arg2)
    // Copyright INRIA
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        while %t do
            ask_again=%f

            [ok,x0,xd0,id,exprs]=scicos_getvalue("Set Constraint block parameters",["Initial guess values of states x";"Initial guess values of derivative x''";"Id(i)=1: if x''(i) is present in the feedback, else Id(i)=0"],list("vec",-1,"vec",-1,"vec",-1),exprs)
            if ~ok then
                break,
            end
            x0=x0(:);
            N=size(x0,"*");
            xd0=xd0(:);
            Nxd=size(xd0,"*");
            id=id(:);
            Nid=size(id,"*");

            if (N~=Nxd)|(N~=Nid) then
                messagebox("Incompatible sizes, states, their derivatives, and ID should be the same size")
                ask_again=%t
            end

            if (N<=0 & ~ask_again) then
                messagebox("Number of states (constraints) must be > 0")
                ask_again=%t
            end

            if (~ask_again) then
                for i=1:N,
                    if ~((id(i)==0) | (id(i)==1)) then
                        ask_again=%t
                        messagebox(["Id(i) must be either:";"0 when x''(i) is not present in the feedback";"1: when x''(i) is present in the feedback"])
                        break
                    end
                    if (id(i)==0) then
                        id(i)=-1;
                    end;
                end
            end

            if ~ask_again  then
                graphics.exprs=exprs
                model.state=[x0;xd0];
                model.out=[N;N]
                model.in=N
                model.ipar=id
                x.graphics=graphics;
                x.model=model
                break
            end
        end

    case "define" then
        x0=[0];
        xd0=[0];
        id=[0];
        model=scicos_model()
        model.sim=list("constraint_c",10004)
        model.in=1
        model.out=[1;1]
        model.state=[x0;xd0]
        model.ipar=id
        model.blocktype="c"
        model.dep_ut=[%f %t]
        exprs=list(strcat(sci2exp(x0)),strcat(sci2exp(xd0)),strcat(sci2exp(id)))

        gr_i=[]

        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
