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

function [x,y,typ] = CURV_f(job,arg1,arg2)

    x=[];
    y=[];
    typ=[];

    select job
    case "set" then
        x = arg1;
        model    = arg1.model;
        graphics = arg1.graphics;
        rpar = model.rpar;
        ipar = model.ipar;
        n    = ipar(1);
        xx = rpar(1:n);
        yy = rpar(n+1:2*n);
        gc = list(rpar(2*n+1:2*n+4),ipar(2:5))

        while %t do
            [ln,fun] = where();
            if ~or(fun == "do_eval") then // cas standard
                [xx, yy, ok, gc] = edit_curv(xx, yy, "axy", [" "," "," "], gc);
            else
                ok = %t;
            end // no need anymore to overload edit_curv in do_eval

            if ~ok then
                break
            end

            n=size(xx,"*")

            if or(xx(2:n)-xx(1:n-1)<0) then
                message("You have not defined a function");
                ok=%f
            end

            if ok then
                model.sim = "intplt"
                model.firing = [] //compatibility
                rect = gc(1)
                model.rpar = [xx(:);yy(:);rect(:)]
                axisdata = gc(2);
                model.ipar = [size(xx,"*");axisdata(:)]
                x.graphics = graphics;
                x.model = model
                break
            end
        end

    case "define" then
        xx = [0;1;2];
        yy = [-5;5;0]
        rect = [0,-5,2,5];
        axisdata = [2;10;2;10]
        ipar = [size(xx,1);axisdata(:)]
        rpar = [xx;yy;rect(:)]
        model = scicos_model()
        model.sim = "intplt"
        model.in = []
        model.out = 1
        model.rpar = [xx;yy;rect(:)]
        model.ipar = [size(xx,1);axisdata(:)]
        model.blocktype = "c"
        model.dep_ut    = [%f %t]

        gr_i = []
        x = standard_define([2 2],model,[],gr_i)
    end

endfunction
