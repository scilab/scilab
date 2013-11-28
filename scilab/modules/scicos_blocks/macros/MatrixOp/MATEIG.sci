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

function [x,y,typ]=MATEIG(job,arg1,arg2)
    //
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        model=arg1.model;
        graphics=arg1.graphics;
        label=graphics.exprs
        if size(label,"*")==14 then
            label(9)=[],
        end //compatiblity
        while %t do
            [ok,typ,decomptyp,lab]=scicos_getvalue("Set MATEIG block parameters",["Datatype(1=real double  2=Complex)";"decomposition type (1=eig values  2=eig values+eig vectors"],list("vec",1,"vec",1),label)
            if ~ok then
                break,
            end
            label=lab
            if (typ==1) then
                if (decomptyp==1) then
                    function_name="mat_vps";
                    out=[-1 1];
                    ot=2;
                elseif (decomptyp==2) then
                    function_name="mat_vpv";
                    out=[-1 -1;-1 -1];
                    ot=[2 2];
                else
                    message("decomposition type is not supported");
                    ok=%f;
                end
                it=1;
            elseif (typ==2) then
                if (decomptyp==1) then
                    function_name="matz_vps";
                    out=[-1 1];
                    ot=2;
                elseif (decomptyp==2) then
                    function_name="matz_vpv";
                    out=[-1 -1;-1 -1];
                    ot=[2 2];
                else
                    message("decomposition type is not supported");
                    ok=%f;
                end
                it=2;
            else
                message("Datatype is not supported");
                ok=%f;
            end
            in=[-1 -1];
            funtyp=4;
            if ok then
                [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
            end
            if ok then
                model.sim=list(function_name,funtyp);
                arg1.model=model
                graphics.exprs=label
                arg1.graphics=graphics
                x=arg1
                break
            end
        end
    case "define" then
        model=scicos_model()
        function_name="mat_vps";
        funtyp=4;
        model.sim=list(function_name,funtyp)

        model.in=-1
        model.in2=-1
        model.intyp=1
        model.out=-1
        model.out2=1
        model.outtyp=2
        model.evtin=[]
        model.evtout=[]
        model.state=[]
        model.dstate=[]
        model.rpar=[]
        model.ipar=[]
        model.blocktype="c"
        model.firing=[]
        model.dep_ut=[%t %f]
        label=[sci2exp(1);sci2exp(1)];
        gr_i=[]
        x=standard_define([2 2],model,label,gr_i)
    end
endfunction

