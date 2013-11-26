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

function [x,y,typ]=fortran_block(job,arg1,arg2)
    //
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        model=arg1.model;
        graphics=arg1.graphics;
        label=graphics.exprs;
        while %t do
            [ok,i,o,rpar,funam,lab]=..
            scicos_getvalue("Set fortran_block parameters",..
            ["input ports sizes";
            "output port sizes";
            "System parameters vector";
            "function name"],..
            list("vec",-1,"vec",-1,"vec",-1,"str",-1),label(1))
            if ~ok then
                break,
            end
            if funam==" " then
                break,
            end
            label(1)=lab
            rpar=rpar(:)
            i=int(i(:));
            ni=size(i,1);
            o=int(o(:));
            no=size(o,1);
            tt=label(2);
            if model.sim(1)<>funam|size(model.in,"*")<>size(i,"*")..
                |size(model.out,"*")<>size(o,"*") then
                tt=[]
            end
            [ok,tt]=FORTR(funam,tt,i,o)
            if ~ok then
                break,
            end
            [model,graphics,ok]=check_io(model,graphics,i,o,[],[])
            if ok then
                model.sim(1)=funam
                model.rpar=rpar
                label(2)=tt
                x.model=model
                graphics.exprs=label
                x.graphics=graphics
                break
            end
        end
    case "define" then
        model=scicos_model()
        model.sim=list(" ",1001)
        model.in=1
        model.out=1
        model.evtin=[]
        model.evtout=[]
        model.state=[]
        model.dstate=[]
        model.rpar=[]
        model.ipar=0
        model.blocktype="c"
        model.firing=[]
        model.dep_ut=[%t %f]
        funam="forty"
        label=list([sci2exp(model.in);sci2exp(model.out);..
        strcat(sci2exp(model.rpar));funam],list([]))
        gr_i=[]
        x=standard_define([4 2],model,label,gr_i)
    end
endfunction
