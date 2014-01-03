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

function [x,y,typ]=CMATVIEW(job,arg1,arg2)
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
            [ok,colormap,cmin,cmax,exprs]=scicos_getvalue(..
            "Set Scope parameters",..
            ["ColorMap";
            "Minimum level range";
            "Maximum level range"],..
            list("vec",-1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end //user cancel modification
            mess=[]
            if cmax<=cmin then
                mess=[mess;"Error with minimum and maximum value";" "]
                ok=%f
            end
            if ~ok then
                message(["Some specified values are inconsistent:";" ";mess])
            end
            if ok then
                size_c=size(colormap(:),1);
                sol=inv([cmin 1;cmax 1])*[1;size_c/3];
                alpha_c = sol(1);
                beta_c = sol(2);
                ipar=[cmin;cmax;size_c];
                rpar=[alpha_c;beta_c;colormap(:)];
                model.ipar=ipar;
                model.rpar=rpar;
                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        cmin = 0;
        cmax = 100;
        size_c = 25;
        colormap = jetcolormap(size_c);
        alpha_c = 0.24
        beta_c = 1

        model=scicos_model()
        model.sim=list("cmatview",4)
        model.in=-1
        model.in2=-2
        model.intyp=1
        model.evtin=1
        model.ipar=[cmin;cmax;size_c]
        model.rpar=[alpha_c;beta_c;colormap(:)]
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[string("jetcolormap(25)");
        string(cmin);
        string(cmax)];
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
