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

function [x,y,typ]=RAND_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        if size(exprs,"*")==5 then
            exprs=exprs(1:3),
        end //compatibility
        if size(exprs,"*")==3 then
            exprs=[exprs;string(model.dstate(1))],
        end //compatibility
        while %t do
            [ok,flag,a,b,seed_c,exprs]=scicos_getvalue([
            "Set Random generator block parameters";
            "flag = 0 : Uniform distribution A is min and A+B max";
            "flag = 1 : Normal distribution A is mean and B deviation";
            " ";
            "A and B must be vector with equal sizes";
            "seed is the seed of random number generator (integer<2**31)"],..
            ["flag";"A";"B";"seed"],..
            list("vec",1,"vec",-1,"vec","size(%2,''*'')","vec",1),exprs)
            if ~ok then
                break,
            end
            if flag<>0&flag<>1 then
                message("flag must be equal to 1 or 0")
            else
                nout=size(a,"*")
                graphics.exprs=exprs
                model.out=nout
                model.ipar=flag
                model.rpar=[a(:);b(:)]
                model.dstate=[seed_c;0*a(:)]
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        a=0
        b=1
        dt=0
        out=1
        flag=0
        model=scicos_model()
        model.sim="rndblk"
        model.out=out
        model.evtin=1
        model.dstate=[int(rand()*(10^7-1));0*a(:)]
        model.rpar=[a(:);b(:)]
        model.ipar=flag
        model.blocktype="d"
        model.dep_ut=[%f %f]

        exprs=[string(flag);sci2exp(a(:));sci2exp(b(:));string(model.dstate(1))]
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
