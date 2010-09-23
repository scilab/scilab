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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=SUPER_f(job,arg1,arg2)
x=[];y=[],typ=[]

select job
case 'plot' then
  standard_draw(arg1)
  s_port_names(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  // not used on Xcos, re-implemented for compatibility
  xcos(arg1.model.rpar);
  
case 'define' then
  // nested diagram settings (2 ports)
  scs=scicos_diagram();
  scs.props.title='Super Block';
  
  in = IN_f('define');
  in.graphics.orig = [40, 40];
  in.graphics.sz = [40, 40];
  out = OUT_f('define');
  out.graphics.orig = [240, 40];
  out.graphics.sz = [40, 40];

  scs.objs(1) = in;
  scs.objs(2) = out;
  
  // block settings
  model=scicos_model();
  model.sim='super';
  model.in=1;
  model.out=1;
  model.rpar=scs;
  model.blocktype='h';
  model.dep_ut=[%f %f];

  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=orig(1)+      [2 4 4]*(sz(1)/7);';
    'yy=orig(2)+sz(2)-[2 2 6]*(sz(2)/10);';
    'xrects([xx;yy;[sz(1)/7;sz(2)/5]*ones(1,3)]);';
    'xx=orig(1)+      [1 2 3 4 5 6 3.5 3.5 3.5 4 5 5.5 5.5 5.5]*sz(1)/7;';
    'yy=orig(2)+sz(2)-[3 3 3 3 3 3 3   7   7   7 7 7   7   3  ]*sz(2)/10;';
    'xsegs(xx,yy,0);';
    'xset(''thickness'',thick)']
  x=standard_define([2 2],model,[],gr_i)
end
endfunction
