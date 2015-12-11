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

function [x,y,typ]=Bache(job,arg1,arg2)
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
            [ok,Patm,A,ze1,ze2,zs1,zs2,z0,T0,p_rho,exprs]=scicos_getvalue("Parametres de la bache", ..
            ["Pression dans le ciel de la bache : Patm (Pa)";..
            "Section de la bache : A (m2)";..
            "Altitude du piquage d entrée 1: ze1 (m)";..
            "Altitude du piquage d entrée 2: ze2 (m)";..
            "Altitude du piquage de sortie 1: zs1 (m)";..
            "Altitude du piquage de sortie 2: zs2 (m)";..
            "Altitude initiale du fluide : z0 (m)";..
            "Température initiale du fluide : T0 (K)";..
            "Si >0, masse volumique imposée du fluide : p_rho (kg/m3)"],..
            list("vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1),exprs)


            if ~ok then
                break,
            end
            model.rpar=[Patm;A;ze1;ze2;zs1;zs2;z0;T0;p_rho]
            model.equations.parameters(2)=list(Patm,A,ze1,ze2,zs1,zs2,z0,T0,p_rho)
            graphics.exprs=exprs
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        in=2
        out=3
        model=scicos_model()
        model.in=[-(1:in)'];
        model.out=[-(1:out)'];
        Patm=1.013E5
        A=1
        ze1=40
        ze2=0
        zs1=40
        zs2=0
        z0=30
        T0=290
        p_rho=0
        model.rpar=[Patm;A;ze1;ze2;zs1;zs2;z0;T0;p_rho]
        model.sim="Bache"
        model.blocktype="c"
        model.dep_ut=[%t %f]

        mo=modelica()
        mo.model="Bache"
        mo.inputs=["Ce1" "Ce2"];
        mo.outputs=["Cs1" "Cs2" "yNiveau"];
        mo.parameters=list(["Patm";"A";"ze1";"ze2";"zs1";"zs2";"z0";"T0";"p_rho"],[Patm;A;ze1;ze2;zs1;zs2;z0;T0;p_rho])
        model.equations=mo
        model.in=ones(size(mo.inputs,"*"),1)
        model.out=ones(size(mo.outputs,"*"),1)
        exprs=[string(Patm);string(A);string(ze1);string(ze2);string(zs1);string(zs2);string(z0);string(T0);string(p_rho)]
        gr_i=[];
        //'xfpolys(orig(1)+[0;5;7;3;5;10;10;0;0]*sz(1)/10,orig(2)+[4;2;7;7;2;0;4;0;4]*sz(2)/10,scs_color(15))'
        x=standard_define([2 2],model,exprs,list(gr_i,0))
        x.graphics.in_implicit=["I";"I"]
        x.graphics.out_implicit=["I";"I";"E"]
    end
endfunction
