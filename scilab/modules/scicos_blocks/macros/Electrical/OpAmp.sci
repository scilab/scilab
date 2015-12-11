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

function [x,y,typ]=OpAmp(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        while %f do
            [ok,OLGain,SatH,SatL,exprs]=scicos_getvalue("Set the Operational Amplifier parameters",..
            ["Open Loop Gain";"Positive saturation voltage";"Negative saturation voltage"],..
            list("vec",1,"vec",1,"vec",1),exprs);
            if ~ok then
                break,
            end
            model.equations.parameters(2)=list(OLGain,SatH,SatL)
            graphics.exprs=exprs
            x.graphics=graphics;
            x.model=model
            break
        end
    case "define" then
        //  OLGain=1000;
        //  SatH=10;
        // SatL=-10;
        // S=['OLGain';'SatH';'SatL'];
        // Z=eval(S);
        S=[];
        Z=[];
        model=scicos_model();
        model.sim="OpAmp";
        model.blocktype="c";
        model.dep_ut=[%t %f];
        mo=modelica();
        mo.model=model.sim;
        mo.inputs=["in_p";"in_n"];
        mo.outputs=["out"];
        mo.parameters=list(S,Z);
        model.equations=mo;
        model.in=ones(size(mo.inputs,"*"),1);
        model.out=ones(size(mo.outputs,"*"),1);
        model.rpar=Z;
        exprs=string(Z);
        gr_i=[]
        x=standard_define([3 5],model,exprs,gr_i)
        x.graphics.in_implicit=["I";"I"]
        x.graphics.out_implicit=["I"]
    end
endfunction
// OpAmp

