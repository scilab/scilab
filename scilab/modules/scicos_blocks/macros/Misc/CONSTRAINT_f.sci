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

function [x,y,typ]=CONSTRAINT_f(job,arg1,arg2)
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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,num,exprs]=getvalue('Set Constraint block parameters',..
	'Set number of constraints',list('vec',1),exprs)
    if ~ok then break,end
    if num<=0 then
      message('number of constraints must be > 0 ')
    else
      [model,graphics,ok]=check_io(model,graphics,num,num,[],[])
      if ok then
	graphics.exprs=exprs
	model.state=zeros(4*num,1)
	model.out=num
	model.in=num
//	model.rpar=[num]
	x.graphics=graphics;x.model=model
	break
      end
    end
  end
  
  
 case 'define' then
  x0=[0;0;0;0]
  model=scicos_model()
  model.sim=list('constraint',10001)
  model.in=1
  model.out=1
  model.state=x0
  model.blocktype='c'
  model.dep_ut=[%f %t]
  exprs='1'
  gr_i=['xstringb(orig(1),orig(2),''=0    ?'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
