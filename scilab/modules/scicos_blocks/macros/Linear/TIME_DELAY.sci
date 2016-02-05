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

function [x,y,typ]=TIME_DELAY(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        nin=model.in(1)

        while %t do
            [ok,T,init,N,exprs]=scicos_getvalue("Set delay parameters",..
            ["Delay";"initial input";"Buffer size"],..
            list("vec",1,"vec",1,"vec",1),..
            exprs);
            if ~ok then
                break,
            end
            if N<2 then
                message("Buffer must be larger than 2")
                ok=%f
            end
            if T<=0 then
                message("Delay must be positive")
                ok=%f
            end

            if ok then
                [model,graphics,ok]=check_io(model,graphics,[-1],-1,[],[])
            end
            if ok then
                graphics.exprs=exprs;
                model.rpar=[T;init];
                model.ipar=N

                model.dep_ut=[%f %t];
                // dp_t = %t
                // updated for Scilab 5.5.1
                // see http://bugzilla.scilab.org/show_bug.cgi?id=13436
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        nin=1
        T=1
        init=0
        N=1024
        model=scicos_model()
        model.sim=list("time_delay",4)
        model.in=[nin]
        model.out=nin
        model.rpar=[T,init];
        model.ipar=N
        model.blocktype="x"
        model.dep_ut=[%f %t]

        exprs=[string(T);string(init);string(N)];
        gr_i=[]
        x=standard_define([3.5 2],model,exprs,gr_i)
    end
endfunction
