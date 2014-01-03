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

function [x,y,typ] = CLKSPLIT_f(job,arg1,arg2)
    //** 22 Jun 2006: I found a problem here from scicos_new
    //** 23           ... but it was not here :(
    //** 26 June 2006: eliniating the /scicos_blocks/Events/<duplicate> !

    x=[];
    y=[];
    typ=[];

    select job

    case "set" then
        x=arg1;

    case "define" then
        model=scicos_model()
        model.sim="split"
        model.evtin=1
        model.evtout=[1;1]
        model.blocktype="d"
        model.firing=[%f,%f,%f] //????
        model.dep_ut=[%f %f]
        x = standard_define([1 1]/3,model,[],[])
    end //** ...select job

endfunction
//**----------------------------------------------------------

//** 26 Jun 2006: The original source code
//function [x,y,typ]=CLKSPLIT_f(job,arg1,arg2)
//// Copyright INRIA
//x=[];y=[],typ=[];
//select job
//case 'plot' then
//case 'getinputs' then
//  orig=arg1.graphics.orig;
//  x=orig(1)
//  y=orig(2)
//  typ=-ones(x)
//case 'getoutputs' then
//  orig=arg1.graphics.orig;
//  x=[1,1]*orig(1)
//  y=[1,1]*orig(2)
//  typ=-ones(x)
//case 'getorigin' then
//  [x,y]=standard_origin(arg1)
//case 'set' then
//  x=arg1;
//case 'define' then
//  model=scicos_model()
//  model.sim='split'
//  model.evtin=1
//  model.evtout=[1;1]
//  model.blocktype='d'
//  model.firing=[%f,%f,%f] //????
//  model.dep_ut=[%f %f]
//
//  x=standard_define([1 1]/3,model,[],[])
//end
//endfunction
