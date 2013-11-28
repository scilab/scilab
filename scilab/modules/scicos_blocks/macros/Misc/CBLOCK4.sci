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
function [x,y,typ]=CBLOCK4(job,arg1,arg2)
    //
    // Copyright INRIA
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
            [ok,function_name,impli,in,it,out,ot,ci,co,xx,z,oz,...
            rpar,ipar,opar,nmode,nzcr,auto0,depu,dept,lab]=..
            scicos_getvalue("Set C-Block4 block parameters",..
            ["Simulation function";
            "Is block implicit? (y,n)";
            "Input ports sizes";
            "Input ports type";
            "Output port sizes";
            "Output ports type";
            "Input event ports sizes";
            "Output events ports sizes";
            "Initial continuous state";
            "Initial discrete state";
            "Initial object state";
            "Real parameters vector";
            "Integer parameters vector";
            "Object parameters list";
            "Number of modes";
            "Number of zero crossings";
            "Initial firing vector (<0 for no firing)";
            "Direct feedthrough (y or n)";
            "Time dependence (y or n)"],..
            list("str",1,"str",1,"mat",[-1 2],"vec",-1,"mat",[-1 2],"vec",-1,"vec",-1,"vec",-1,..
            "vec",-1,"vec",-1,"lis",-1,"vec",-1,"vec",-1,"lis",-1,"vec",1,"vec",1,"vec","sum(%8)",..
            "str",1,"str",1),label(1))
            if ~ok then
                break
            end
            label(1)=lab
            funam=stripblanks(function_name)
            xx=xx(:);
            z=z(:);
            rpar=rpar(:);
            ipar=int(ipar(:));
            nx=size(xx,1);
            nz=size(z,1);

            ci=int(ci(:));
            nevin=size(ci,1);
            co=int(co(:));
            nevout=size(co,1);
            if part(impli,1)=="y" then
                funtyp=12004
            else
                funtyp=2004
            end
            if [ci;co]<>[] then
                if max([ci;co])>1 then
                    message("vector event links not supported");
                    ok=%f;
                end
            end

            if ok then
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
                    break
                end

                //cross checking
                if model.sim(1)<>funam|sign(size(model.state,"*"))<>sign(nx)|..
                    sign(size(model.dstate,"*"))<>sign(nz)|model.nzcross<>nzcr|..
                    sign(size(model.evtout,"*"))<>sign(nevout) then
                    tt=[]
                end

                tt=label(2);

                [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),ci,co)
            end

            if ok then
                while %t
                    [ok,tt,cancel]=CC4(funam,tt)
                    if ~ok then
                        if cancel then
                            break,
                        end
                    else
                        model.sim=list(funam,funtyp)
                        model.state=xx
                        model.dstate=z
                        model.odstate=oz
                        model.rpar=rpar
                        model.ipar=ipar
                        model.opar=opar
                        model.firing=auto0
                        model.nzcross=nzcr
                        model.nmode=nmode
                        model.dep_ut=dep_ut
                        label(2)=tt
                        x.model=model
                        graphics.exprs=label
                        x.graphics=graphics
                        break
                    end
                end

                if ok|cancel then
                    break
                end
            end
        end

    case "define" then

        funam="toto"

        model=scicos_model()
        model.sim=list(" ",2004)

        model.in=1
        model.in2=1
        model.intyp=1
        model.out=1
        model.out2=1
        model.outtyp=1
        model.dep_ut=[%t %f]
        label=list([funam;
        "n";
        sci2exp([model.in model.in2]);
        sci2exp(model.intyp);
        sci2exp([model.out model.out2]);
        sci2exp(model.outtyp);
        sci2exp(model.evtin);
        sci2exp(model.evtout);
        sci2exp(model.state);
        sci2exp(model.dstate);
        sci2exp(model.odstate);
        sci2exp(model.rpar);
        sci2exp(model.ipar);
        sci2exp(model.opar);
        sci2exp(model.nmode);
        sci2exp(model.nzcross);
        sci2exp(model.firing);
        "y";
        "n"],...
        []);

        gr_i=[]
        x=standard_define([4 2],model,label,gr_i)
    end
endfunction





