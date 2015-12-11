//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
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

function [x,y,typ]=SUPER_f(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];

    select job
    case "set" then
        // not used on Xcos, re-implemented for compatibility
        xcos(arg1.model.rpar);

    case "define" then
        // nested diagram settings (2 ports)
        scs=scicos_diagram();
        scs.props.title="Super Block";

        in = IN_f("define");
        in.graphics.orig = [40, 40];
        in.graphics.sz = [20, 20];
        out = OUT_f("define");
        out.graphics.orig = [240, 40];
        out.graphics.sz = [20, 20];

        scs.objs(1) = in;
        scs.objs(2) = out;

        // block settings
        model=scicos_model();
        model.sim="super";
        model.in=1;
        model.out=1;
        model.rpar=scs;
        model.blocktype="h";
        model.dep_ut=[%f %f];

        gr_i=[]
        x=standard_define([2 2],model,[],gr_i)
    end
endfunction
