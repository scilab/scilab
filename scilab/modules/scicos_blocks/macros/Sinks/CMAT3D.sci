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

function [x,y,typ]=CMAT3D(job,arg1,arg2)
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
            [ok,vec_x,vec_y,colormap,cmin,cmax,exprs]=scicos_getvalue(..
            "Set Scope parameters",..
            ["Bounds Vector X (-1 for standard)";
            "Bounds Vector Y (-1 for standard)";
            "ColorMap";
            "Zmin";
            "Zmax"],..
            list("vec",-1,"vec",-1,"vec",-1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end //user cancel modification
            mess=[]
            if size(vec_x,"*")<>size(vec_y,"*") then
                mess=[mess;"Vector X and Vector Y must have the same size";" "]
                ok=%f
            end
            if cmax<=cmin then
                mess=[mess;"Error with minimum and maximum value";" "]
                ok=%f
            end
            if ~ok then
                message(["Some specified values are inconsistent:";" ";mess])
            end
            if ok then
                size_x = size(vec_x,"*");
                size_c=size(colormap(:),1);
                ipar=[cmin;cmax;size_c;size_x];
                rpar=[colormap(:);vec_x(:);vec_y(:)];
                model.ipar=ipar;
                model.rpar=rpar;
                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model;
                break;
            end
        end
    case "define" then
        cmin = 0;
        cmax = 100;
        colormap = jetcolormap(25);
        size_c = 25;
        x=-1;
        y=-1;
        size_x = 1;
        size_y = 1;

        model=scicos_model()
        model.sim=list("cmat3d",4)
        model.in=-1
        model.in2=-2
        model.intyp=1
        model.evtin=1
        model.ipar=[cmin;cmax;size_c;size_x;size_y]
        model.rpar=[colormap(:);x;y]
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[strcat(string(x)," ");
        strcat(string(y)," ");
        string("jetcolormap(25)");
        string(cmin);
        string(cmax)];
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
