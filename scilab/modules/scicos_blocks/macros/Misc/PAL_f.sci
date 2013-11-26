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

function [x,y,typ]=PAL_f(job,arg1,arg2)
    // Copyright INRIA
    x=[];
    y=[];
    typ=[];

    select job
    case "set" then
        [x,newparameters,needcompile,edited]=scicos(arg1.model.rpar)
        arg1.graphics.id=x.props.title(1);
        arg1.model.rpar=x;
        x=arg1
        y=[]
        typ=[]
        %exit=resume(%f)

    case "define" then
        scs=scicos_diagram();
        scs.props.title="Palette";
        model=scicos_model();
        model.sim="palette";
        model.in=[];
        model.out=[];
        model.rpar=scs;
        model.blocktype="h";
        model.dep_ut=[%f %f];

        gr_i=[]

        x=standard_define([2 2],model,[],gr_i)
        x.graphics.id=scs.props.title(1);
    end

endfunction
