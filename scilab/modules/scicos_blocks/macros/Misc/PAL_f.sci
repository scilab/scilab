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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=PAL_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[],typ=[]

select job
  case 'plot' then
    standard_draw(arg1)

  case 'getinputs' then
    x=[];y=[];typ=[];

  case 'getoutputs' then
    x=[];y=[];typ=[];

  case 'getorigin' then
    [x,y]=standard_origin(arg1)

  case 'set' then
    [x,newparameters,needcompile,edited]=scicos(arg1.model.rpar)
    arg1.graphics.id=x.props.title(1);
    arg1.model.rpar=x;
    x=arg1
    y=[]
    typ=[]
    %exit=resume(%f)

  case 'define' then
    scs=scicos_diagram();
    scs.props.title='Palette';
    model=scicos_model();
    model.sim='palette';
    model.in=[];
    model.out=[];
    model.rpar=scs;
    model.blocktype='h';
    model.dep_ut=[%f %f];

    gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
          'xx=orig(1)+      [1 3 5 1 3 5 1 3 5]*(sz(1)/7);';
          'yy=orig(2)+sz(2)-[1 1 1 4 4 4 7 7 7]*(sz(2)/10);';
          'xrects([xx;yy;[sz(1)/7;sz(2)/5]*ones(1,9)]);';
          'xset(''thickness'',thick)']

    x=standard_define([2 2],model,[],gr_i)
    x.graphics.id=scs.props.title(1);
end

endfunction
