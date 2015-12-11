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

function [x,y,typ]=SUBMAT(job,arg1,arg2)
    //
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        graphics=arg1.graphics;
        label=graphics.exprs
        model=arg1.model;
        //  if size(label,'*')==14 then label(9)=[],end
        if size(label,"*")==5 then
            label(6)=sci2exp([1 1]),
        end //compatiblity
        while %t do
            [ok,typ,a,b,c,d,inp,exprs]=scicos_getvalue("Set SUBMAT Block",..
            ["Datatype (1=real double  2=Complex)";"Starting Row Index";"Ending Row Index";"Starting Column Index";"Ending Column Index";"Input Dimensions"],list("vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",2),label)
            if ~ok then
                break,
            end
            if (typ==1) then
                function_name="submat";
                ot=1;
                it=1;
            elseif (typ==2) then
                function_name="submatz";
                ot=2;
                it=2;
            else
                message("Datatype is not supported");
                ok=%f;
            end
            if (a<=0) | (b<=0) | (c<=0) | (d<=0) then
                message("invalid index");
                ok=%f;
            end
            if b<a then
                message("ending row must be greater than starting row");
                ok=%f;
            end
            if d<c then
                message("ending column must be greater than starting column");
                ok=%f;
            end
            if b>inp(1) then
                message ("index of ending row is out of range");
                ok=%f;
            end
            if d>inp(2) then
                message ("index of ending column is out of range");
                ok=%f;
            end
            model.ipar=[a;b;c;d];
            in=[inp(1) inp(2)];
            out=[(b-a)+1 (d-c)+1];
            funtyp=4;
            label=exprs;
            if ok then

                [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
                model.sim=list(function_name,funtyp);
                graphics.exprs=label;
                arg1.graphics=graphics;
                arg1.model=model;
                x=arg1
                break
            end
        end
    case "define" then
        model=scicos_model()
        function_name="submat";
        funtyp=4;
        model.sim=list(function_name,funtyp)

        model.in=-1
        model.in2=-2
        model.intyp=1
        model.out=-1
        model.out2=-2
        model.outtyp=1
        model.evtin=[]
        model.evtout=[]
        model.state=[]
        model.dstate=[]
        model.rpar=[]
        model.ipar=[1;1;1;1]
        model.blocktype="c"
        model.firing=[]
        model.dep_ut=[%t %f]
        label=[sci2exp(1);sci2exp(1);sci2exp(1);sci2exp(1);sci2exp(1)];
        gr_i=[]
        x=standard_define([2.5 2],model,label,gr_i)
    end
endfunction

