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

function [x,y,typ]=MFCLCK_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        [ok,dt,nn,exprs]=scicos_getvalue("Set Multifrequency clock parameters",..
        ["basic period (1/f)";"multiply by (n)"],list("vec",1,"vec",1),exprs)
        if ok then
            model.ipar=nn
            model.rpar=dt;
            hh=model.firing;
            hh(2)=0;
            model.firing=hh //compatibility
            graphics.exprs=exprs
            x.graphics=graphics;
            x.model=model
        end
    case "define" then
        nn=2
        dt=0.1
        model=scicos_model()
        model.sim="mfclck"
        model.evtin=1
        model.evtout=[1;1]
        model.dstate=0
        model.rpar=dt
        model.ipar=nn
        model.blocktype="d"
        model.firing=[-1 0]
        model.dep_ut=[%f %f]

        exprs=[string(dt);string(nn)]
        gr_i=[]
        x=standard_define([3 2],model,exprs,gr_i)
    end
endfunction
