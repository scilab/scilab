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

function [x,y,typ]=NPN(job,arg1,arg2)
    // the automatically generated interface block for Modelica NPN.mo model
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
            [ok,Bf,Br,Is,Vak,Tauf,Taur,Ccs,Cje,Cjc,Phie,Me,Phic,Mc,Gbc,Gbe,Vt,EMinMax,exprs]=scicos_getvalue(["Set NPN block parameters:";""],["Bf  : Forward beta";"Br  : Reverse beta";"Is  : Transport saturation current";"Vak : Early voltage (inverse), 1/Volt";"Tauf: Ideal forward transit time";"Taur: Ideal reverse transit time";"Ccs : Collector-substrat(ground) cap.";"Cje : Base-emitter zero bias depletion cap.";"Cjc : Base-coll. zero bias depletion cap.";"Phie: Base-emitter diffusion voltage";"Me  : Base-emitter gradation exponent";"Phic: Base-collector diffusion voltage";"Mc  : Base-collector gradation exponent";"Gbc : Base-collector conductance";"Gbe : Base-emitter conductance";"Vt  : Voltage equivalent of temperature";"EMinmax: if x > EMinMax, the exp(x) is linearized"],list("vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end
            x.model.equations.parameters(2)=list(Bf,Br,Is,Vak,Tauf,Taur,Ccs,Cje,Cjc,Phie,Me,Phic,Mc,Gbc,Gbe,Vt,EMinMax)
            x.graphics.exprs=exprs
            break
        end
    case "define" then
        ModelName="NPN"
        PrametersValue=[50;0.1;0;0.02;1.200D-10;5.000D-09;1.000D-12;4.000D-13;5.000D-13;0.8;0.4;0.8;0.333;1.000D-15;1.000D-15;0.02585;40]
        ParametersName=["Bf";"Br";"Is";"Vak";"Tauf";"Taur";"Ccs";"Cje";"Cjc";"Phie";"Me";"Phic";"Mc";"Gbc";"Gbe";"Vt";"EMinMax"]
        model=scicos_model()
        Typein=[];
        Typeout=[];
        MI=[];
        MO=[]
        P=[100,90,-2,0; 0,50,2,0; 100,10,-2,0]
        PortName=["C";"B";"E"]
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
        exprs=["50";"0.1";"1.e-16";"0.02";"0.12e-9";"5e-9";"1e-12";"0.4e-12";"0.5e-12";"0.8";"0.4";"0.8";"0.333";"1e-15";"1e-15";"0.02585";"40"]
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
