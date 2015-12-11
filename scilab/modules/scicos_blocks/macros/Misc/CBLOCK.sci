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

function [x,y,typ]=CBLOCK(job,arg1,arg2)
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
            [ok,function_name,impli,i,o,ci,co,xx,ng,z,rpar,ipar,auto0,depu,dept,lab]=..
            scicos_getvalue("Set C-Block2 block parameters",..
            ["simulation function";
            "is block implicit? (y,n)";
            "input ports sizes";
            "output ports sizes";
            "input event ports sizes";
            "output events ports sizes";
            "initial continuous state";
            "number of zero crossing surfaces";
            "initial discrete state";
            "Real parameters vector";
            "Integer parameters vector";
            "initial firing vector (<0 for no firing)";
            "direct feedthrough (y or n)";
            "time dependence (y or n)"],..
            list("str",1,"str",1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,..
            "vec",-1,"vec",1,"vec",-1,"vec",-1,"vec",-1,"vec","sum(%6)",..
            "str",1,"str",1),label(1))
            if ~ok then
                break,
            end
            label(1)=lab
            funam=stripblanks(function_name)
            xx=xx(:);
            z=z(:);
            rpar=rpar(:);
            ipar=int(ipar(:));
            nx=size(xx,1);
            nz=size(z,1);
            i=int(i(:));
            o=int(o(:));
            nout=size(o,1);
            ci=int(ci(:));
            nevin=size(ci,1);
            co=int(co(:));
            nevout=size(co,1);
            if part(impli,1)=="y" then
                funtyp=12004,
            else
                funtyp=2004,
            end
            if [ci;co]<>[] then
                if max([ci;co])>1 then
                    message("vector event links not supported");
                    ok=%f;
                end
            end
            depu=stripblanks(depu);
            if part(depu,1)=="y" then
                depu=%t;
            else
                depu=%f;
            end
            dept=stripblanks(dept);
            if part(dept,1)=="y" then
                dept=%t;
            else
                dept=%f;
            end
            dep_ut=[depu dept];


            if funam==" " then
                break,
            end

            if model.sim(1)<>funam|sign(size(model.state,"*"))<>sign(nx)|..
                sign(size(model.dstate,"*"))<>sign(nz)|model.nzcross<>ng|..
                sign(size(model.evtout,"*"))<>sign(nevout) then
                tt=[]
            end

            tt=label(2);
            while %t

                [ok,tt,cancel]=CFORTR2(funam,tt)
                if ~ok then
                    if cancel then
                        break,
                    end
                else
                    [model,graphics,ok]=check_io(model,graphics,i,o,ci,co)
                    if ok then
                        model.sim=list(funam,funtyp)
                        model.in=i
                        model.out=o
                        model.evtin=ci
                        model.evtout=co
                        model.state=xx
                        model.dstate=z
                        model.rpar=rpar
                        model.ipar=ipar
                        model.firing=auto0
                        model.dep_ut=dep_ut
                        model.nzcross=ng
                        label(2)=tt
                        x.model=model
                        graphics.exprs=label
                        x.graphics=graphics
                        break
                    end
                end
            end
            if ok|cancel then
                break,
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
        ipar=[]
        funam="toto"
        ng=0

        model=scicos_model()
        model.sim=list(" ",2004)
        model.in=in
        model.out=out
        model.evtin=clkin
        model.evtout=clkout
        model.state=x0
        model.dstate=z0
        model.rpar=rpar
        model.ipar=ipar
        model.blocktype=typ
        model.firing=auto
        model.dep_ut=[%t %f]
        model.nzcross=ng

        label=list([funam,"n",sci2exp(in),sci2exp(out),sci2exp(clkin),sci2exp(clkout),..
        sci2exp(x0),sci2exp(0),sci2exp(z0),sci2exp(rpar),sci2exp(ipar),..
        sci2exp(auto),"y","n"]',[])

        gr_i=[]
        x=standard_define([4 2],model,label,gr_i)
    end
endfunction


