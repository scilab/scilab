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

function [x,y,typ]=STEP(job,arg1,arg2)
    // Copyright INRIA
    x=[];y=[];typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;exprs=graphics.exprs
        model=arg1.model;
        while %t do
            [ok,temps,in,fi,exprs] = scicos_getvalue([msprintf(gettext("Set %s block parameters"), "STEP_FUNCTION");" "; ..
            gettext("Step Function");" "], ..
            [gettext("Step Time"); gettext("Initial Value"); gettext("Final Value")], ..
            list("vec",1,"vec",-1,"vec",-1), exprs);
            if ~ok then
                break,
            end
            in=in(:);fi=fi(:);
            if size(in,"*")<>size(fi,"*")  then
                if size(in,"*")==1 then
                    in=in*ones(fi)
                elseif size(fi,"*")==1 then
                    fi=fi*ones(in)
                else
                    block_parameter_error(msprintf(gettext("''Initial Value'' and ''Final Value'': incompatible sizes: %d and %d."), size(in,"*"), size(fi,"*")), gettext("Same sizes expected."));
                    ok=%f
                end
            end
            if ok then
                model.out2=1;
                model.outtyp=1;
                [model,graphics,ok]=check_io(model,graphics,[],size(fi,"*"),1,1)
            end
            if ok then
                model.firing=temps
                if temps==0 then
                    rpar=[fi;fi]
                else
                    rpar=[in;fi]
                end
                model.rpar=rpar
                graphics.exprs=exprs
                x.graphics=graphics;x.model=model
                break
            end
        end
    case "define" then
        rpar=[0;1]
        model=scicos_model()
        model.sim=list("step_func",4)
        model.evtin=1
        model.evtout=1
        model.out=1
        model.out2=1;
        model.outtyp=1;
        model.firing=1
        model.rpar=rpar
        model.blocktype="c"
        model.dep_ut=[%f %f]

        exprs=[string(1);string(rpar)]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
