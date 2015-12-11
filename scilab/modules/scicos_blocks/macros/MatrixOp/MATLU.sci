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

function [x,y,typ]=MATLU(job,arg1,arg2)
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
            [ok,typ,lab]=..
            scicos_getvalue("Set MATLU block parameters",["Datatype(1=real double  2=Complex)"],list("vec",1),label);
            if ~ok then
                break,
            end
            if (typ==1) then
                function_name="mat_lu";
                ot=[1 1];
                it=1;
            elseif (typ==2) then
                function_name="matz_lu";
                ot=[2 2];
                it=2;
            else
                message("Datatype is not supported");
                ok=%f;
            end
            if ok then
                [model,graphics,ok]=set_io(model,graphics,list([model.in model.in2],it),list([model.out model.out2],ot),[],[])
            end
            if ok then
                funtyp=4;
                model.sim=list(function_name,funtyp)
                graphics.exprs=lab;
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        model=scicos_model()
        function_name="mat_lu";
        funtyp=4;
        model.sim=list(function_name,funtyp)

        model.in=-1
        model.in2=-1
        model.intyp=1
        //model.out=[siz(1);min(siz)]
        //model.out2=[min(siz);siz(2)]
        model.out=[-1;-1]
        model.out2=[-1;-1]
        model.outtyp=[1 1]
        model.evtin=[]
        model.evtout=[]
        model.state=[]
        model.dstate=[]
        model.rpar=[]
        model.ipar=[]
        model.blocktype="c"
        model.firing=[]
        model.dep_ut=[%t %f]
        label=sci2exp(1);
        gr_i=[]
        x=standard_define([2 2],model,label,gr_i)
    end
endfunction

