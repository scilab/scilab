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

function [x,y,typ]=CVS(job,arg1,arg2)
    // the automatically generated interface block for Modelica CVS.mo model
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
    case "define" then
        ModelName="CVS"
        PrametersValue=[]
        ParametersName=[]
        model=scicos_model()
        Typein=[];
        Typeout=[];
        MI=[];
        MO=[]
        P=[2,50,1,0; 70,98,2,0;70,2,-2,0]

        PortName=["vin";"p";"n"]
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
        exprs=[]
        gr_i=[]
        model.blocktype="c"
        model.dep_ut=[%f %t]
        mo.model=ModelName
        model.equations=mo
        model.in=ones(size(MI,"*"),1)
        model.out=ones(size(MO,"*"),1)
        x=standard_define([2.1,3],model,exprs,list(gr_i,0))
        x.graphics.in_implicit=Typein;
        x.graphics.out_implicit=Typeout;
    end
endfunction
