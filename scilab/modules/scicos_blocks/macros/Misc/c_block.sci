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

function [x,y,typ]=c_block(job,arg1,arg2)
    //
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        model=arg1.model;
        graphics=arg1.graphics;
        label=graphics.exprs;
        while %t do
            [ok,i,o,rpar,funam,lab]=..
            scicos_getvalue("Set C_block parameters",..
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
            [ok,tt]=CFORTR(funam,tt,i,o)
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
        in=1
        out=1
        clkin=[]
        clkout=[]
        x0=[]
        z0=[]
        typ="c"
        auto=[]
        rpar=[]
        funam="toto"
        //chek

        //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
        //model=list(list(' ',2001),in,out,clkin,clkout,x0,z0,rpar,0,typ,auto,[%t %f],..
        //      ' ',list());

        model=scicos_model()
        model.sim=list(" ",2001)
        model.in=in
        model.out=out
        model.evtin=clkin
        model.evtout=clkout
        model.state=x0
        model.dstate=z0
        model.rpar=rpar
        model.ipar=0
        model.blocktype=typ
        model.firing=auto
        model.dep_ut=[%t %f]
        //**********************************************************************************
        label=list([sci2exp(in);sci2exp(out);	strcat(sci2exp(rpar));funam],..
        list([]))
        gr_i=[]
        x=standard_define([3 2],model,label,gr_i)
    end
endfunction
