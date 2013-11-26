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

function [x,y,typ]=PerteDP(job,arg1,arg2)
    // exemple d'un bloc implicit,
    //   -  sans entree ni sortie de conditionnement
    //   -  avec une entree et une sortie de type implicit et de dimension 1
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
        while %t do
            [ok,L,D,lambda,z1,z2,p_rho,exprs]=scicos_getvalue("Parametres du tuyau", ..
            ["Longueur du tube : L (m)";..
            "Diamètre interne du tube : D (m)";..
            "Coefficient de perte de charge-frottement(S.U) : lambda";..
            "Altitude entrée tuyauterie : z1 (m)";..
            "Altitude sortie tuyauterie : z2 (m)";..
            "Si >0, masse volumique imposée fu fluide : p_rho (kg/m3)"],..
            list("vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1),exprs)
            if ~ok then
                break,
            end
            model.rpar=[L;D;lambda;z1;z2;p_rho]
            model.equations.parameters(2)=list(L,D,lambda,z1,z2,p_rho)
            graphics.exprs=exprs
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        model=scicos_model()
        model.in=[1];
        model.out=[1];
        L=10
        D=0.2
        lambda=0.03
        z1=0
        z2=0
        p_rho=0
        model.rpar=[L;D;lambda;z1;z2;p_rho]
        model.sim="PerteDP"
        model.blocktype="c"
        model.dep_ut=[%t %f]
        mo=modelica()
        mo.model="PerteDP"
        mo.inputs="C1";
        mo.outputs="C2";
        mo.parameters=list(["L";"D";"lambda";"z1";"z2";"p_rho"],[L;D;lambda;z1;z2;p_rho])
        model.equations=mo
        model.in=ones(size(mo.inputs,"*"),1)
        model.out=ones(size(mo.outputs,"*"),1)
        exprs=[string(L);string(D);string(lambda);string(z1);string(z2);string(p_rho)]
        gr_i=[];
        x=standard_define([2 1],model,exprs,list(gr_i,0))
        x.graphics.in_implicit=["I"]
        x.graphics.out_implicit=["I"]
    end
endfunction
