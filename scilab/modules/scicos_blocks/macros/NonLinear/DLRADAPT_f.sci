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

function [x,y,typ]=DLRADAPT_f(job,arg1,arg2)
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
            [ok,p,rn,rd,g,last_u,last_y,exprs]=scicos_getvalue("Set block parameters",..
            ["Vector of p mesh points";
            "Numerator roots (one line for each mesh)";
            "Denominator roots (one line for each mesh)";
            "Vector of gain at mesh points" ;
            "past inputs (Num degree values)";
            "past outputs (Den degree values)"],..
            list("vec",-1,"mat",[-1,-1],"mat",..
            ["size(%1,''*'')","-1"],"vec","size(%1,''*'')",..
            "vec","size(%2,2)","vec","size(%3,2)"),exprs)


            if ~ok then
                break,
            end
            m=size(rn,2)
            [npt,n]=size(rd)
            if m>=n then
                message("Transfer must be strictly proper"),
            elseif size(rn,1)<>0&size(rn,1)<>size(p,"*") then
                message("Numerator roots matrix row size''s is incorrect")
            else
                rpar=[p(:);real(rn(:));imag(rn(:));real(rd(:));imag(rd(:));g(:)]
                ipar=[m;n;npt]
                model.dstate=[last_u(:);last_y(:)]
                model.rpar=rpar
                model.ipar=ipar
                graphics.exprs=exprs
                x.graphics=graphics;
                x.model=model
                break;
            end
        end
    case "define" then
        p=[0;1]
        rn=[]
        rd=[0.2+0.8*%i,0.2-0.8*%i;0.3+0.7*%i,0.3-0.7*%i]
        g=[1;1]
        last_u=[]
        last_y=[0;0]

        model=scicos_model()
        model.sim="dlradp"
        model.in=[1;1]
        model.out=1
        model.evtin=1

        model.dstate=[last_u;last_y]
        model.rpar=[p(:);real(rn(:));imag(rn(:));real(rd(:));imag(rd(:));g(:)]
        model.ipar=[0;2;2]
        model.blocktype="d"
        model.firing=[]
        model.dep_ut=[%t %f]

        exprs=[sci2exp(p);
        sci2exp(rn);
        sci2exp(rd,0);
        sci2exp(g);
        sci2exp(last_u);
        sci2exp(last_y)]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
