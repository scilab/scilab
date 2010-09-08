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

function [x,y,typ]=IMPSPLIT_f(job,arg1,arg2)
x=[];y=[],typ=[];

select job
case 'plot' then
   //**--- This is the function that DRAW the object
   //pause ; //** debug
   orig = arg1.graphics.orig ;
   xarc(orig(1), orig(2)+1.0 , 1.0 , 1.0 , 0, 360*64)

case 'getinputs' then
  graphics=arg1.graphics;orig=graphics.orig;
  x=orig(1)
  y=orig(2)
  typ=ones(x)*2

case 'getoutputs' then
  graphics=arg1.graphics;orig=graphics.orig;
  x=[1 1]*orig(1)
  y=[1 1]*orig(2)
  typ=ones(x)*2

case 'getorigin' then
  [x,y]=standard_origin(arg1)

case 'set' then
  x=arg1;
  case 'define' then
  model=scicos_model();
  model.sim       = 'limpsplit';
  mo=modelica()
    mo.model='limpsplit'
    mo.inputs='n';
    mo.outputs=['n';'n']
  model.equations=mo
  model.in=ones(size(mo.inputs,'*'),1)
  model.out=ones(size(mo.outputs,'*'),1)
  x=standard_define([1 1]/3,model,[],[])
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I' 'I']
end

endfunction
