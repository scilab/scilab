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

function [x,y,typ]=GENERAL_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        rpar=model.rpar
        in=model.in;
        out=model.evtout
        nin=sum(in)
        nout=sum(out)
        [ok,in,out,exprs]=scicos_getvalue("Set General Zero-Crossing parameters",..
        ["Input size";
        "Number of event output"],..
        list("vec",1,"vec",1),exprs)
        if ok then
            [model,graphics,ok]=check_io(model,graphics,in,[],[],ones(out,1))
            if ok then
                nout1=out
                nin1=in
                if nout==nout1 & nin==nin1 then
                    rp=matrix(rpar,nout,2^(2*nin));
                else
                    rp=-1*ones(nout1,2^(2*nin1))
                end
                n=size(rp,2)/2
                result=x_mdialog("routing matrix",string(1:nout1),..
                string(1:2^(2*nin1)),string(rp(:,:)))
                if result<>[] then
                    rp(1:nout1,1:2*n)=evstr(result)
                    model.nzcross=in
                    model.rpar=rp(:)
                    model.firing=-ones(out,1)
                    graphics.exprs=exprs
                    x.graphics=graphics;
                    x.model=model
                end
            end
        end
    case "define" then
        rpar=[0;0;0;0]
        in=1;
        out=1;
        model=scicos_model()
        model.sim=list("zcross",1)
        model.nzcross=in
        model.in=in
        model.evtout=ones(out,1)
        model.rpar=[0;0;0;0]
        model.blocktype="z"
        model.firing=-ones(out,1),
        model.dep_ut=[%t %f]

        exprs=[strcat(sci2exp(in));strcat(sci2exp(out))]
        gr_i=[]
        x=standard_define([3 2],model,exprs, gr_i)
    end
endfunction
