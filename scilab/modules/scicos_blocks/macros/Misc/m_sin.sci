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

function [out1,out2]=m_sin(t,x,z,up,uc,clock,flag,rpar,ipar)
    //if flag>0 then write(0,'m_sin t='+string(t)+' flag ='+string(flag)),end
    out1=[];
    out2=[];
    select flag
    case 1 then
        out1=sin(rpar(1)*t+rpar(2))
    case 2 then
        message("?")
    case 3 then
        message("?")
    case -1 then //initialisation
        exprs="Sin"

        model=scicos_model()
        model.sim=t(1)
        model.out=1

        model.rpar=[1;0]
        model.blocktype="c"
        model.firing=-1
        model.dep_ut=[%f %t]

        out1=list(model,exprs)
    case -2 then //update
        model=t
        exprs=x
        if exprs==[] then
            [ok,gain,phase,exprs1]=scicos_getvalue("Set Sin block parameters",..
            ["Frequency";"Phase"],list("vec",1,"vec",1))
        else

            [ok,gain,phase,exprs1]=scicos_getvalue("Set Sin block parameters",..
            ["Frequency";"Phase"],list("vec",1,"vec",1),exprs)
        end
        if ok then
            model.rpar=[gain;phase]
            exprs=exprs1
        end
        out1=list(model,exprs)
    end
endfunction
