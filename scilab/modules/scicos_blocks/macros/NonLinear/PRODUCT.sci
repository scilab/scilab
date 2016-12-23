//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) 2016-2016 - Scilab Enterprises - Clement David
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

function [x,y,typ] = PRODUCT(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics
        model=arg1.model
        exprs=graphics.exprs

        // Compatibility before 6.0.0, there was no divideByZero parameter
        if size(exprs) == [1 1] then
            exprs($+1) = "1";
        end

        while %t do
            [ok,sgn,divideByZero,exprs]=scicos_getvalue(["Set block parameters"],...
            ["<html>Number of inputs or sign vector<br>(multiplication is set with + 1, division with -1)</html>"; "Error on divide by zero<br>(1: yes) (0: no)"],...
            list("vec",-1, "vec", 1),exprs)
            if ~ok then
                break,
            end
            sgn=sgn(:);
            if size(sgn,1)==1 then
                if sgn<1 then
                    message("Number of inputs must be > 0")
                    ok=%f
                elseif sgn==1 then
                    in=-1
                    sgn=[]
                    nout=1
                else
                    in=-ones(sgn,1)
                    sgn=ones(sgn,1)
                    nout=-1
                end
            else
                if ~and(abs(sgn)==1) then
                    message("Signs can only be +1 or -1")
                    ok=%f
                else
                    in=-ones(size(sgn,1),1)
                    nout=-1
                end
            end
            if ok then
                [model,graphics,ok]=check_io(model,graphics,in,nout,[],[])
            end

            if divideByZero==0 then
                model.rpar=%eps;
            else
                model.rpar=0;
            end

            if ok then
                model.ipar=sgn
                graphics.exprs=exprs
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        sgn=[1;-1]
        model=scicos_model()
        model.sim=list("product",4)
        model.in=[-1;-1]
        model.out=-1
        model.ipar=sgn
        model.rpar=0
        model.blocktype="c"
        model.dep_ut=[%t %f]


        exprs=[sci2exp(sgn) ; "1"]
        gr_i=[]
        x=standard_define([2 3],model, exprs,gr_i)
    end
endfunction
