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

function [x,y,typ]=CONST(job,arg1,arg2)
x=[];y=[];typ=[];
select job
 case 'plot' then
  C=arg1.graphics.exprs;
  standard_draw(arg1)
case 'getinputs' then
  x=[];y=[];typ=[];
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,C,exprs]=scicos_getvalue(['Set Contant Block'],..
	    'Constant',list('vec',-1),exprs)
    if ~ok then break,end
    nout=size(C,'*')
    if nout==0 then
      message('C must have at least one element')
    else
      model.rpar=C(:);model.out=nout
      graphics.exprs=exprs;
      x.graphics=graphics;x.model=model
      break;
    end
  end
case 'define' then
  C=1

  model=scicos_model()
  model.sim=list('cstblk4',4)
  model.in=[]
  model.out=1
  model.rpar=C
  model.blocktype='d'
  model.dep_ut=[%f %f]

  exprs=strcat(sci2exp(C))
  gr_i=['dx=sz(1)/5;dy=sz(2)/10;';
    'w=sz(1)-2*dx;h=sz(2)-2*dy;';
    'txt=C;'
    'xstringb(orig(1)+dx,orig(2)+dy,txt,w,h,''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
