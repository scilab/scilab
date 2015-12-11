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

function [x,y,typ]=LOGIC(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        while %t do
            [ok,mat,herit,exprs]=scicos_getvalue([msprintf(gettext("Set %s block parameters"), "LOGIC"); " ";gettext("Combinatorial logic");" ";
            gettext("&nbsp; Rows of the matrix are the output values"); gettext("&nbsp; Number of rows must be a power of two."); ..
            gettext("&nbsp; Number of columns gives the number of outputs.");" "], ..
            [gettext("Truth Table (matrix of outputs)"); gettext("Accepts Inherited Events (0:No, 1:Yes)")], ..
            list("mat",[-1,-2],"vec",1), exprs);

            if ~ok then
                break,
            end
            nout=size(mat,2)
            nin=(log(size(mat,1))/log(2))
            u1=floor(nin)
            //if (herit<>0) then herit=1;end; The test below verify the value of parameter
            if (u1 <> nin) then
                block_parameter_error(msprintf(gettext("Wrong size for ''%s'' parameter: %d."), gettext("Truth Table"), size(mat,1)), ..
                gettext("Number of rows must be a power of two."));
                ok=%f;
            elseif (find(mat(:) <> 0 & mat(:) <> 1) <> []) then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter."), gettext("Truth Table")), ..
                msprintf(gettext( "Elements must be in the interval %s."),"[0, 1]"));
                ok=%f;
            elseif herit <0 | herit > 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Accepts Inherited Events"), herit), ..
                msprintf(gettext( "Must be in the interval %s."),"[0, 1]"));
                ok=%f;
            end
            if ok then
                in=[ones(nin,1) ones(nin,1)]
                out=[ones(nout,1) ones(nout,1)]
                it=5*ones(1,nin)
                ot=5*ones(1,nout)
                [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),ones(1-herit,1),[])
            end
            if ok then
                graphics.exprs=exprs;
                mat=int8(mat);
                model.opar=list(mat);
                x.graphics=graphics;
                x.model=model;
                break
            end
        end
    case "define" then
        mat=[0;0;0;1]
        model=scicos_model()
        model.sim=list("logic",4)
        model.in=[1;1]
        model.in2=[1;1]
        model.out=1
        model.out2=1
        model.evtin=1
        model.intyp=[5 5]
        model.outtyp=5
        model.opar=list(int8(mat));
        model.blocktype="c"
        model.firing=%f
        model.dep_ut=[%t %f]
        exprs=[sci2exp(mat);sci2exp(0)]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction

