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

function [x,y,typ]=STEP(job,arg1,arg2)
x=[];y=[];typ=[];
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
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,temps,in,fi,exprs]=getvalue('Set Saturation parameters',..
	['Step time';'Initial value';'Final value'],list('vec',1,'vec',-1,'vec',-1),exprs)
    if ~ok then break,end
    in=in(:);fi=fi(:);
    if size(in,'*')<>size(fi,'*')  then
      if size(in,'*')==1 then 
	in=in*ones(fi)
      elseif size(fi,'*')==1 then 
	fi=fi*ones(in)
      else
	message('Incompatible initial and final values')
	ok=%f
      end
    end
    if ok then
      [model,graphics,ok]=check_io(model,graphics,[],size(fi,'*'),1,1)
    end
    if ok then
      rpar=[in;fi]
      model.rpar=rpar
      model.firing=temps
      graphics.exprs=exprs
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  rpar=[0;1]
  model=scicos_model()
  model.sim=list('step_func',4)
  model.evtin=1
  model.evtout=1
  model.out=1
  model.firing=1
  model.rpar=rpar
  model.blocktype='c'
  model.dep_ut=[%f %t]
  
  exprs=[string(1);string(rpar)]
  gr_i=['txt=[''Step''];';
        'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
