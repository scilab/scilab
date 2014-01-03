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

function [x,y,typ]=IdealTransformer(job,arg1,arg2)
    // the automatically generated interface block for Modelica Transformer.mo model
    //   - avec un dialogue de saisie de parametre
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        x=arg1
        exprs=x.graphics.exprs
        while %t do
            [ok,N,exprs]=scicos_getvalue(["Set Transformer block parameters:";"";"N:"+...
            " Turn ratio (N1/N2)"],["N"],list("vec",1),exprs)
            if ~ok then
                break,
            end
            x.model.equations.parameters(2)=list(N)
            x.graphics.exprs=exprs
            break
        end
    case "define" then
        ModelName="IdealTransformer"
        PrametersValue=[1]
        ParametersName=["N"]
        model=scicos_model()
        Typein=[];
        Typeout=[];
        MI=[];
        MO=[]
        P=[2.5,90,2,0;2.5,10,2,0;97.5,90,-2,0;97.5,10,-2,0]
        PortName=["p1";"n1";"p2";"n2"]
        for i=1:size(P,"r")
            if P(i,3)==1  then
                Typein= [Typein; "E"];
                MI=[MI;PortName(i)];
            end
            if P(i,3)==2  then
                Typein= [Typein; "I"];
                MI=[MI;PortName(i)];
            end
            if P(i,3)==-1 then
                Typeout=[Typeout;"E"];
                MO=[MO;PortName(i)];
            end
            if P(i,3)==-2 then
                Typeout=[Typeout;"I"];
                MO=[MO;PortName(i)];
            end
        end
        model=scicos_model()
        mo=modelica()
        model.sim=ModelName;
        mo.inputs=MI;
        mo.outputs=MO;
        model.rpar=PrametersValue;
        mo.parameters=list(ParametersName,PrametersValue,zeros(ParametersName));
        exprs=["1"]
        gr_i=[]
        model.blocktype="c"
        model.dep_ut=[%f %t]
        mo.model=ModelName
        model.equations=mo
        model.in=ones(size(MI,"*"),1)
        model.out=ones(size(MO,"*"),1)
        x=standard_define([2,2],model,exprs,list(gr_i,0))
        x.graphics.in_implicit=Typein;
        x.graphics.out_implicit=Typeout;
    end
endfunction
