//  Xcos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright 2011 - Bernard DUJARDIN <bernard.dujardin@contrib.scilab.org>
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

function [x,y,typ] = Counter(job,arg1,arg2)
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
            [ok,minim,maxim,rule,exprs]=scicos_getvalue([msprintf(gettext("Set %s block parameters"), "Counter"); " "; ..
            gettext("Integer counter generator");" "], ..
            [gettext("Minimum"); gettext("Maximum"); ..
            gettext("Rule (1:Increment, 2:Decrement)");], ..
            list("vec",1,"vec",1,"vec",1),exprs);

            if ~ok then
                break,
            end
            maxim=int(maxim);
            minim=int(minim);

            if maxim < minim then
                block_parameter_error(msprintf(gettext("Wrong values for ''Maximum'' and ''Minimum'' parameters: %d &lt; %d"), minim, maxim), ..
                msprintf(gettext("''Minimum'' must be less than ''Maximum''.")));
            elseif (rule <> 1 & rule <> 2) then
                block_parameter_error(msprintf(gettext("Wrong value for ''Rule'' parameter: %d"), rule), ..
                msprintf(gettext("Must be in the interval %s."), "[1,2]"));
            else
                graphics.exprs=exprs
                model.dstate=0
                model.ipar=[rule;maxim;minim]
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        minim=0
        maxim=2
        rule=1
        model=scicos_model()
        model.sim=list("counter",4)
        model.evtin=1
        model.out=1
        model.out2=1
        model.dstate=0
        model.ipar=[rule;maxim;minim]
        model.blocktype="c"
        model.dep_ut=[%f %f]

        exprs=[string(minim);string(maxim);string(rule)]
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
