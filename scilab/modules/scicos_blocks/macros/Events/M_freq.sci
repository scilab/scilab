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

function [x,y,typ]=M_freq(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        model=arg1.model;
        exprs=graphics.exprs
        while %t do
            [ok,frequ,offset,exprs]=scicos_getvalue("Set block parameters",..
            ["Sample time";"Offset"],..
            list("vec",-1,"vec",-1),exprs)
            if ~ok then
                break,
            end
            offset=offset(:);
            frequ=frequ(:);
            if (size(frequ,"*"))<>(size(offset,"*")) then
                message("offset and frequency must have the same size");
                ok=%f;
            elseif or(frequ<0) then
                message("Frequency must be a positif number");
                ok=%f;
            elseif or(abs(offset) > frequ) then
                message("The |Offset| must be less than the Frequency");
                ok=%f
            end
            if ok then
                [m,den,off,count,m1,fir,frequ,offset,ok]=mfrequ_clk(frequ,offset);
            end
            if ok then
                model.opar=list(m,double(den),off,count)
                mn=(2**size(m1,"*"))-1;
                [model,graphics,ok]=set_io(model,graphics,list(),list(),1,ones(mn,1))
                if mn>3 then
                    graphics.sz=[40+(mn-3)*10 40]
                else
                    graphics.sz=[50 40]
                end
                model.firing=fir;
                graphics.exprs=exprs
                x.graphics=graphics
                x.model=model
                break
            end
        end
    case "define" then
        model=scicos_model()
        model.sim=list("m_frequ",4)
        model.evtout=[1;1;1]
        model.evtin=1
        model.rpar=[]
        model.opar=list([1 1 0;1 1 1;1 3 2],1,0,0);
        model.blocktype="d"
        model.firing=[0 -1 -1]
        model.dep_ut=[%f %f]
        exprs=[sci2exp([1;2]);sci2exp([0;0])]
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction

function [m,k]=uni(fr,of)
    k=[];
    m=[];
    ot=[];
    for i=1:size(fr,"*")
        istreated=%f;
        ind=find(m==fr(i));
        if ind==[] then
            m=[m;fr(i)];
            ot=[ot;of(i)];
            k=[k;i];
        else
            for j=ind
                if of(i)==ot(j) then
                    istreated=%t
                end
            end
            if ~istreated then
                m=[m;fr(i)];
                ot=[ot;of(i)]
                k=[k;i];
            end
        end
    end
endfunction
