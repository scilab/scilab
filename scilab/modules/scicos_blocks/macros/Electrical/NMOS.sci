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

function [x,y,typ]=NMOS(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        while %t do
            [ok,W,L,Beta,Vt,K2,K5,dW,dL,RDS,exprs]=scicos_getvalue("Set NMOS Transistor block parameters",..
            ["Width [m]";..
            "Length [m]";..
            "Transconductance parameter [A/(V*V)]";..
            "Zero bias threshold voltage [V]";..
            "Bulk threshold parameter";..
            "Reduction of pinch-off region";..
            "Narrowing of channel [m]";..
            "Shortening of channel [m]";..
            "Drain-Source-Resistance [Ohm]"],..
            list("vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1),exprs)

            if ~ok then
                break,
            end
            model.equations.parameters(2)=list(W,L,Beta,Vt,K2,K5,dW,dL,RDS)
            graphics.exprs=exprs
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        model=scicos_model()
        W=20.e-6;
        L=6.e-6;
        Beta=0.041e-3;
        Vt=0.8;
        K2=1.144;
        K5= 0.7311;
        dW=-2.5e-6;
        dL= -1.5e-6;
        RDS=1.e+7;

        model.sim="NMOS"
        model.blocktype="c"
        model.dep_ut=[%t %f]
        mo=modelica()
        mo.model="NMOS";
        mo.outputs=["D";"B";"S"]
        mo.inputs="G";
        mo.parameters=list(["W";"L";"Beta";"Vt";"K2";"K5";"dW";"dL";"RDS"],[W;L;Beta;Vt;K2;K5;dW;dL;RDS])
        model.equations=mo
        model.in=ones(size(mo.inputs,"*"),1)
        model.out=ones(size(mo.outputs,"*"),1)
        exprs=[string(W);string(L);string(Beta);string(Vt);string(K2);string(K5);string(dW);string(dL);string(RDS)]
        gr_i=[]

        x=standard_define([2 2],model,exprs,gr_i)
        x.graphics.in_implicit=["I"]
        x.graphics.out_implicit=["I";"I";"I"]
    end
endfunction


