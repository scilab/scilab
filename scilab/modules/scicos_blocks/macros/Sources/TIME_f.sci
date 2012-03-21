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

function [x,y,typ]=TIME_f(job,arg1,arg2)
x=[];y=[],typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1
case 'define' then

  model=scicos_model()
  model.sim='timblk'
  model.out=1
  model.blocktype='c'
  model.dep_ut=[%f %t]
  
  gr_i=['wd=xget(''wdim'').*[1.016,1.12];';
    'thick=xget(''thickness'');xset(''thickness'',2);';
    'p=wd(2)/wd(1);p=1;';
    'rx=sz(1)*p/2;ry=sz(2)/2;';
    'xarc(orig(1)+0.05*sz(1),orig(2)+0.95*sz(2),0.9*sz(1)*p,0.9*sz(2),0,360*64);';
    'xset(''thickness'',1);';
    'xx=[orig(1)+rx    orig(1)+rx;';
    'orig(1)+rx    orig(1)+rx+0.6*rx*cos(%pi/6)];';
    'yy=[orig(2)+ry    orig(2)+ry ;';
    '	  orig(2)+1.8*ry  orig(2)+ry+0.6*ry*sin(%pi/6)];';
    'xsegs(xx,yy,0);';
    'xset(''thickness'',thick);']
   x=standard_define([2 2],model,[],gr_i)
end
endfunction
