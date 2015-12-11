// Xcos
//
// Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
// Copyright (C) 2011 - Bernard DUJARDIN <bernard.dujardin@contrib.scilab.org>
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

function [x,y,typ]=Modulo_Count(job,arg1,arg2)
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
            [ok,ini_c,base,exprs] = scicos_getvalue([msprintf(gettext("Set %s block parameters"), "Modulo_Count");" "; gettext("Modulo counter (0 to N counter)");" "], ..
            [gettext("Initial State (zero or positive number)"); gettext("Upper Limit (positive number)")], ..
            list("vec",1,"vec",1), exprs);

            ini_c = int(ini_c);
            base = int(base);
            if ~ok then
                break,
            end
            if ini_c <0 then
                block_parameter_error(msprintf(gettext("Wrong value for ''Initial State'' parameter: %d."), ini_c), ..
                gettext("Null or positive integer expected."));
            elseif base <= 0 then
                block_parameter_error(msprintf(gettext("Wrong values for ''Upper Limit'' parameter: %d."), base),..
                gettext("Strictly positive integer expected."));
            else
                graphics.exprs=exprs
                model.ipar=base;
                model.dstate=ini_c;
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        ini_c=0
        base=3
        model=scicos_model()
        model.sim=list("modulo_count",4)
        model.evtin=1
        model.out=1
        model.dstate=ini_c
        model.ipar=base
        model.blocktype="c"
        model.dep_ut=[%f %f]

        exprs=[string(ini_c);string(base)]
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
