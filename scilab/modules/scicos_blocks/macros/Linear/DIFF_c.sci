//  Scicos
//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
// Copyright (C) 2009-2010 - DIGITEO - Clément DAVID
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

function [x,y,typ]=DIFF_c(job,arg1,arg2)
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
            [ok,x0,xd0,exprs]=scicos_getvalue("Set continuous linear system parameters",..
            ["Initial state";"Initial Derivative"],list("vec",-1,"vec",-1),exprs)
            if ~ok then
                break,
            end
            x0=x0(:);
            N=size(x0,"*");
            xd0=xd0(:);
            Nxd=size(xd0,"*");
            if (N~=Nxd) then
                messagebox("Incompatible sizes: states and their derivatives should have the same size")
                ask_again=%t
            end
            if (N<=0 & ~ask_again) then
                messagebox("Number of states must be > 0")
                ask_again=%t
            end

            if ~ask_again  then
                graphics.exprs=exprs
                model.state=[x0;xd0];
                model.out=[N]
                model.in=N
                x.graphics=graphics;
                x.model=model
                break
            end
        end
        x.model.firing=[] //compatibility
    case "define" then
        x0=[0;0]
        model=scicos_model()
        model.sim=list("diffblk_c",10004)
        model.in=1
        model.out=1
        model.state=x0
        model.blocktype="c"
        model.dep_ut=[%f %t]

        exprs=[strcat(sci2exp(x0(1)));strcat(sci2exp(x0(2)))]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
