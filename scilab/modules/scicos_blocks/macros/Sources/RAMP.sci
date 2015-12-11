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

function [x,y,typ]=RAMP(job,arg1,arg2)
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
            [ok,slope,stt,iout,exprs]=scicos_getvalue([msprintf(gettext("Set %s block parameters"), "RAMP"); " "; gettext("Ramp function");" "], ..
            [gettext("Slope"); gettext("Start Time"); gettext("Initial Value")], ..
            list("vec",1,"vec",1,"vec",1), exprs)
            if ~ok then
                break,
            end
            if stt<0  then
                block_parameter_error(msprintf(gettext("Wrong value for ''Start Time'' parameter: %e."), stt), ..
                gettext("Null or positive integer expected."));
            else
                model.rpar=[slope;stt;iout];
                graphics.exprs=exprs
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        slope=0;
        iout=0;
        stt=0;
        rpar=[slope;stt;iout];
        model=scicos_model()
        model.sim=list("ramp",4)
        model.in=[]
        model.out=1
        model.rpar=rpar
        model.blocktype="c"
        model.nmode=1
        model.nzcross=1
        model.dep_ut=[%f %t]

        exprs=[string(rpar)]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
