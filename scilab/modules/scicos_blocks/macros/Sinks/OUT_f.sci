//  Xcos
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

function [x,y,typ]=OUT_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        model=arg1.model;
        exprs=graphics.exprs;
        if size(exprs,"*")==2 then
            exprs=exprs(1),
        end //compatibility
        while %t do
            [ok,prt,exprs]=scicos_getvalue([msprintf(gettext("Set %s block parameters"),"OUT_f");" ";gettext("Regular output port")],..
            gettext("Port number"),list("vec",1),exprs)
            if ~ok then
                break,
            end
            prt=int(prt)
            if prt<=0 then
                block_parameter_error(msprintf(gettext("Wrong value for ''Port Number'' parameter: %d."),prt), ..
                gettext("Strictly positive integer expected."));
            else
                model.ipar=prt
                graphics.exprs=exprs
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        n=-1
        prt=1
        model=scicos_model()
        model.sim="output"
        model.in=-1
        model.in2=-2
        model.intyp=-1
        model.ipar=prt
        model.blocktype="c"
        model.dep_ut=[%f %f]

        exprs=string(prt)
        gr_i=[]
        x=standard_define([1 1],model,exprs,gr_i)
    end
endfunction
