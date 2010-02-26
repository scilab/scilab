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

function [x,y,typ] = DERIV(job,arg1,arg2)
x=[];y=[];typ=[]
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
  model.sim=list('deriv',4)
  model.in=-1
  model.out=-1
  model.blocktype='x'
  model.dep_ut=[%t %f]
  
  exprs=[]
  gr_i=['xstringb(orig(1),orig(2),'' du/dt   '',sz(1),sz(2),''fill'');'
        'txt=''s'';'
        'style=5;'
        'rectstr=stringbox(txt,orig(1),orig(2),0,style,1);'
        'if ~exists(''%zoom'') then %zoom=1, end;'
        'w=(rectstr(1,3)-rectstr(1,2))*%zoom;'
        'h=(rectstr(2,2)-rectstr(2,4))*%zoom;'
        'xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');'
        'e=gce();'
        'e.font_style=style;']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction


