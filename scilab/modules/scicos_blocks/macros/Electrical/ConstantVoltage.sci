//  Scicos
//
// Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
// Copyright (C) 2018 - Samuel GOUGEON
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

function [x,y,typ]=ConstantVoltage(job,arg1,arg2)
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
            [ok,V,exprs]=scicos_getvalue("Set ConstantVoltage block parameter",..
            "V (volt)",list("vec",1),exprs)
            if ~ok then
                break,
            end
            model.rpar=V
            model.equations.parameters(2)=list(V)
            graphics.exprs=exprs

            // Updating the icon according to the Voltage value:
            v = evstr(exprs(1));
            if v>=1e-8 & v<1e-5
                sv = msprintf("%d\\:n\n",v*1e9)
            elseif v>=1e-5 & v<1e-2
                sv = msprintf("%d\\:{\\mu}\n",v*1e6)
            elseif v>=1e-2 & v<10
                sv = msprintf("%d\\:m\n",v*1000)
            elseif v>=10 & v<10000
                sv = msprintf("%d\\:\n",v)
            elseif v>=1e4 & v<1e7
                sv = msprintf("%d\\:k\n",v/1000)
            else
                p = floor(log10(v));
                v = v/(10^p);
                sv = msprintf("%3.1f\\,10^{%d}\\,\n",v,p)
            end
            lab = "ConstantVoltage;displayedLabel=" + ..
            "$\mathsf{\,\\\,\\\,\\\,\,\\\tiny{\!"+sv+"V}}$"
            graphics.style = lab;
            x.graphics=graphics;

            x.model=model
            break
        end

    case "define" then
        V=0.01

        model=scicos_model()
        model.rpar=V
        model.in=1;model.out=1;
        model.sim="ConstantVoltage";
        model.blocktype="c"
        model.dep_ut=[%f %f]
        mo=modelica()
        mo.model="ConstantVoltage";
        mo.inputs="p";
        mo.outputs="n";
        mo.parameters=list("V",list(V))
        model.equations=mo
        exprs=string(V)

        gr_i=[]

        x=standard_define([2 2],model,exprs,list(gr_i,0))
        x.graphics.in_implicit=["I"]
        x.graphics.out_implicit=["I"]
    end
endfunction
